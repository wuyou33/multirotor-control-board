/*
 * xbee.h
 *
 * Created: 8.1.2016 10:42:34
 *  Author: Tomas Baca
 */ 

#ifndef XBEE_H_
#define XBEE_H_

#include "system.h"
#include "xbeeAdresses.h"

#define XBEE_BUFFER_SIZE	64

#define XBEE_INIT_BYTE	(uint8_t) 0x7E

#define XBEE_API_PACKET_RECEIVE (uint8_t) 0x90
#define XBEE_API_PACKET_TRANSMIT (uint8_t) 0x10

#define XBEE_SERIAL_TICKS_TIMEOUT 10

#define XBEE_16BIT_ADDRESS_UNKNOWN (uint16_t) (0xFFFE)

#define XBEE_RADIUS_MAX 0

typedef enum {
	
	XBEE_NOT_RECEIVING,				// waiting for the first byte
	XBEE_RECEIVING_PAYLOAD_SIZE_1,	// expecting to receive the first byte of the payload size
	XBEE_RECEIVING_PAYLOAD_SIZE_2,	// expecting to receive the second byte of the payload size
	XBEE_RECEIVING_API_TYPE,			// 0x90 for "received packet"
	XBEE_RECEIVING_PAYLOAD,			// the payload is being received
	XBEE_RECEIVING_CHECKSUM,			// expecting to receive the checksum byte
	XBEE_MESSAGE_RECEIVED,			// signalizes that the message is complete and ready to proceess
	
} xbee_receiver_states_t;

typedef struct {
	
	// storing checksum while the message is received
	uint8_t checksum;
	
	// state of the receiver states machine
	xbee_receiver_states_t receiverState;
	
	// the payload size of the packet
	uint16_t payloadSize;
	
	// how many bytes have been already received
	uint16_t bytesReceived;
	
	// buffer for received message
	uint8_t rxBuffer[XBEE_BUFFER_SIZE];
	
} xbeeReceiverStateMachine_t;

// this structure holds a message received from RPI
typedef struct {

	uint64_t address64;
	uint16_t address16;
	uint8_t messageId;
	uint8_t receiveOptions;
	uint8_t * payload;
	uint16_t messageLength;
	
} xbeeMessageHandler_t;

xbeeReceiverStateMachine_t xbeeReceiver;

uint8_t xbeeParseChar(uint8_t inChar, xbeeMessageHandler_t * messageHandler, xbeeReceiverStateMachine_t * receiver);

// sends message to a particular device
void xbeeSendMessage(uint8_t * message, uint16_t length, uint64_t address);

#endif /* XBEE_H_ */