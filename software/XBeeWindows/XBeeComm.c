#include <stdio.h>
#include "serialLink.h"
#include "defines.h"

HANDLE XBeeH;
unsigned char packet[60];

void openXBeeComm(int port){
    XBeeH=openSerialLine(port);
}

void closeXBeeComm(){
    closeSerialLine(XBeeH);
}


unsigned char* readPacket(){
    DWORD bytesReaded;
    int i;
    do{
        if(!ReadFile(XBeeH,packet,1,&bytesReaded,NULL)){
            packet[0]=0x00;
        }
    }while(packet[0]!=0x7E);

    while(!ReadFile(XBeeH,packet+1,2,&bytesReaded,NULL));
    ReadFile(XBeeH,packet+3,packet[2]+1,&bytesReaded,NULL);

    #ifdef DEBUG
     printf("packet received: ");
    for(i=0;i<*(packet+2)+4;i++){
        printf("%X ",packet[i]);
    }printf("\n");
    #endif // DEBUG


    return packet;
}


void sendPacket(unsigned char *packeto){
    DWORD bytesWrited;
    int i;

    WriteFile(XBeeH,packeto,*(packeto+2)+4,&bytesWrited,NULL);
    FlushFileBuffers(XBeeH);
    #ifdef DEBUG
     printf("packet send: ");
    for(i=0;i<*(packeto+2)+4;i++){
        printf("%X ",packeto[i]);
    }printf("\n");
    #endif // DEBUG

}