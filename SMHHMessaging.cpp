#include "SMHMessaging.h"

// Constructor initializes deviceID and groupID with human-readable values
SMHMessaging::SMHMessaging(uint8_t deviceID, uint8_t groupID)
    : deviceID(deviceID), groupID(groupID), createdPacketLength(0) {}

int SMHMessaging::createPacket(uint8_t userID, uint8_t groupID, uint8_t packetType, const uint8_t* payload) {
    if (userID != 0 && groupID != 0) {
        return MSG_INVALID_ADDRESS; // Both userID and groupID are filled
    }
    
    uint8_t dest_addr = userID != 0 ? userID : groupID;
    size_t payload_length = 0;

    while (payload[payload_length] != '\0') { // Calculate payload length
        payload_length++;
    }
    
    if (payload_length > 255) return MSG_INVALID_PAYLOAD_LENGTH;
    
    createdPacket[0] = dest_addr;
    createdPacket[1] = deviceID;
    createdPacket[2] = packetType;
    createdPacket[3] = payload_length;

    for (size_t i = 0; i < payload_length; ++i) {
        createdPacket[4 + i] = payload[i];
    }

    createdPacketLength = 4 + payload_length;
    
    return MSG_NO_ERROR;
}

void SMHMessaging::getCreatedPacket(uint8_t*& packet, size_t& packet_length) {
    packet = createdPacket;
    packet_length = createdPacketLength;
}

int SMHMessaging::parsePacket(const uint8_t* packet, size_t packet_length, uint8_t& dest_addr, uint8_t& src_addr, uint8_t& msg_type, uint8_t* payload, size_t& payload_length) {
    if (packet_length < 4) return MSG_PACKET_TOO_SHORT;

    dest_addr = packet[0];
    src_addr = packet[1];
    msg_type = packet[2];
    payload_length = packet[3];

    if (packet_length < 4 + payload_length) return MSG_INVALID_PAYLOAD_LENGTH;

    for (size_t i = 0; i < payload_length; ++i) {
        payload[i] = packet[4 + i];
    }

    return MSG_NO_ERROR;
}

bool SMHMessaging::isPacketForMe(const uint8_t* packet, size_t packet_length) {
    if (packet_length < 1) return false;

    uint8_t dest_addr = packet[0];
    return dest_addr == deviceID || dest_addr == groupID;
}

int SMHMessaging::handlePacket(const uint8_t* packet, size_t packet_length, uint8_t* return_packet, size_t& return_packet_length) {
    if (!isPacketForMe(packet, packet_length)) return MSG_NO_ERROR;

    for (size_t i = 0; i < packet_length; ++i) {
        return_packet[i] = packet[i];
    }
    return_packet_length = packet_length;

    uint8_t dest_addr, src_addr, msg_type;
    uint8_t payload[256];
    size_t payload_length;

    int error = parsePacket(packet, packet_length, dest_addr, src_addr, msg_type, payload, payload_length);
    if (error != MSG_NO_ERROR) return error;

    switch (msg_type) {
        case 0x01:
            handleCommand(src_addr, payload, payload_length);
            break;
        case 0x02:
            handleData(src_addr, payload, payload_length);
            break;
        case 0x03:
            handleAck(src_addr, payload, payload_length);
            break;
        case 0x04:
            handleError(src_addr, payload, payload_length);
            break;
        case 0x05:
            handleStatus(src_addr, payload, payload_length);
            break;
        default:
            return MSG_UNKNOWN_MESSAGE_TYPE;
    }

    return MSG_NO_ERROR;
}

void SMHMessaging::handleCommand(uint8_t src_addr, const uint8_t* payload, size_t payload_length) {
    // Implement command handling logic
}

void SMHMessaging::handleData(uint8_t src_addr, const uint8_t* payload, size_t payload_length) {
    // Implement data handling logic
}

void SMHMessaging::handleAck(uint8_t src_addr, const uint8_t* payload, size_t payload_length) {
    // Implement acknowledgment handling logic
}

void SMHMessaging::handleError(uint8_t src_addr, const uint8_t* payload, size_t payload_length) {
    // Implement error handling logic
}

void SMHMessaging::handleStatus(uint8_t src_addr, const uint8_t* payload, size_t payload_length) {
    // Implement status handling logic
}
