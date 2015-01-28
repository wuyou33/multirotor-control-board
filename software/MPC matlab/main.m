clear all;

%% Matice systemu

dt = 0.0114;

A = [1 dt 0;
     0 1 dt;
     0 0 dt*82.6135];
 
B = [0; 0; 0.0105*dt];

%% Kovariancni matice mereni

Cov = diag([0.0058, 0.2418, 0]);

% pocet stavu systemu 
n_states = size(A, 1);

% pocet vstupu do systemu
u_size = size(B, 2);

%% Pomocne promenne

% delka predikcniho horizontu
horizon_len = 200;

%% Matice U

n_variables = 38;

U = zeros(horizon_len, n_variables);
 
U(1:20, 1:20) = eye(20);

for i=1:18
    U((21+(i-1)*10):(20+i*10), 20+i) = ones(10, 1); 
end

% U(1:20, 1:20) = eye(20);
 
% U(21:30, 21) = 1;
% U(31:40, 22) = 1;
% U(41:50, 23) = 1;
% U(51:60, 24) = 1;

% n_variables = 21;
% 
% U = zeros(horizon_len, n_variables);
 
%% Matice A_roof
% n = delka predikcniho horizontu
% A_roof = [A;
%           A^2;
%           A^3;
%           ...;
%           A^n]; 

A_roof = zeros(horizon_len * n_states, n_states);

for i=1:horizon_len
   A_roof(((i-1)*n_states+1):(n_states*i), :) = A^i; 
end

%% Matice B_roof
% n = delka predikcniho horizontu
% B_roof = [B,        0,        0,   0;
%           AB,       B,        0,   0;
%           A^2B,     AB,       B,   0;
%           ...;
%           A^(n-2)B, A^(n-1)B, ..., 0;
%           A^(n-1)B, A^(n-2)B, ..., B;

B_roof = zeros(horizon_len * n_states, horizon_len);

for i=1:horizon_len
    for j=1:i
        B_roof((i-1)*n_states+1:i*n_states, ((j-1)*u_size+1):j*u_size) = (A^(i - j))*B;
    end
end

B_roof = B_roof*U;

%% Matice Q_roof
% n = pocet stavu
% Q = n*n
%     diagonala, penalizace odchylek stavu
% S = n*n
%     penalizace odchylky posledniho stavu
% Q_roof = [Q,   0,   ...,  0;
%           0,   Q,   ...,  0;
%           ..., ..., Q,    0;
%           0,   ..., ...,  S];

Q = [1, 0, 0;
     0, 2, 0;
     0, 0, 0];

S = [10, 0, 0;
     0, 0, 0;
     0, 0, 0];
 
Q_roof = zeros(n_states * horizon_len, n_states * horizon_len);

for i=1:horizon_len
    Q_roof((i-1)*n_states+1:i*n_states, (i-1)*n_states+1:i*n_states) = Q; 
end

Q_roof(end-n_states+1:end, end-n_states+1:end) = S;

%% Matice P_roof
% penalizace akcnich zasahu
% P_roof = [P,   0,   ...,  0;
%           0,   P,   ...,  0;
%           ..., ..., P,    0;
%           0,   ..., ...,  P];

P = 0.000005;

P_roof = zeros(n_variables, n_variables);

for i=1:n_variables
    P_roof(i, i) = P;
end

for i=21:38
    P_roof(i, i) = 10*P; 
end

%% Matice H
% matice kvadratickeho clenu kvadraticke formy QP

H = B_roof'*Q_roof*B_roof + P_roof;
H_inv = (0.5*H)^(-1);

%% ��dic� smy�ka s MPC

% delka simulace
simu_len = 2000;

time(1) = 0;

% pozadovana pozice
x_ref = zeros(n_states*simu_len*2, 1);

% x_ref((n_states*simu_len)/4+1:3:end, 1) = 1+0.2*sin(linspace(1, 100, length((n_states*simu_len)/4+1:3:length(x_ref))))';
    
% pocatecni podminky simulace
x(:, 1) = [1; 0; 0];
pid_x(:, 1) = [1; 0; 0];

noisy_hist = [0; 0; 0];

% vektor akcnich zasahu
u = zeros(horizon_len, u_size);

% saturace akcnich zasahu
saturace = 100;

u_sat = 0;

% kalman variables
kalmanCovariance = eye(3);
estimate(:, 1) = x;

xd_pure_integration(1) = x(1, 1);

state = 1;

for i=2:simu_len
    
    time(i) = time(i-1)+dt;
    
    measurement(:, i-1) = createMeasurement(x(:, i-1), Cov);
    
    xd_pure_integration(i) = xd_pure_integration(i-1) + measurement(2, i-1)*dt;

    [estimate(:, i), kalmanCovariance] = kalman(estimate(:, i-1), kalmanCovariance, measurement(2, i-1), u_sat, A, B); 

    if (mod(state, 1) == 0)
        
        state = 1;
        
        X_0 = A_roof*(estimate(:, i)) - x_ref(n_states*(i-1)+1:n_states*(i-1)+horizon_len*n_states, 1);
        c = (X_0'*Q_roof*B_roof)';

        u_cf = H_inv*(c./(-2));
        
        HMM = u_cf;
        
        u_cf = U*u_cf;
        
        % simulace predikce z aktualniho vektoru akcnich zasahu
        x_cf(:, 1) = estimate(:, i);
        for j=2:horizon_len
            x_cf(:, j) = A*x_cf(:, j-1) + B*u_cf(j-1);
        end
        
    else
        state = state + 1;
    end
        
    u_sat = u_cf(state);
    
    % saturuj akcni zasah     
    if (u_sat > saturace)
        u_sat = saturace;
    elseif (u_sat < -saturace)
        u_sat = -saturace;
    end

    % spocti nove stavy podle modelu     
    x(:, i) = A*x(:, i-1) + B*u_sat;
    
    if (mod(i, 4) == 0)
        
        figure(1);
    
        subplot(2, 2, 1);
        hold off
        plot(linspace(0, dt*simu_len, simu_len), x_ref(1:3:n_states*simu_len), 'r');
        hold on
        plot(time, estimate(1, 1:i), 'b');
        plot(linspace(i*dt, dt*(i+horizon_len), horizon_len), x_cf(1, :),'--r');
        title('Position in time');
        ylabel('Position [m]');
        axis([0, dt*simu_len, -0.25, 1.25]);

        subplot(2, 2, 2);
        plot(u_cf, 'r');
        title('Predicted action');
        ylabel('Desired angle [deg]');
        axis([0, horizon_len, -saturace, saturace]);
        
        subplot(2, 2, 3);
        hold off
        plot(time(1:end-1), estimate(1, 1:(i-1)), 'r');
        hold on
        plot(time, x(1, 1:i), 'b');
        plot(time, xd_pure_integration, 'g');
        axis([0, dt*simu_len, -0.25, 1.25]);

        subplot(2, 2, 4);
        hold off
        plot(time(1:end-1), measurement(2, 1:(i-1)), 'r');
        hold on
        plot(time(1:end-1), estimate(2, 1:(i-1)), 'b');
        axis([0, dt*simu_len, -1, 1]);

    end
    
clear     drawnow;
    
end