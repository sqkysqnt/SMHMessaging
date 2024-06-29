#ifndef SMHMESSAGING_H
#define SMHMESSAGING_H

// Define fixed-width integer types
typedef unsigned char uint8_t;
typedef unsigned int size_t; // Define size_t as needed

// Error codes
enum SMHMessagingError {
    MSG_NO_ERROR = 0,
    MSG_PACKET_TOO_SHORT,
    MSG_INVALID_PAYLOAD_LENGTH,
    MSG_UNKNOWN_MESSAGE_TYPE,
    MSG_INVALID_ADDRESS
};

class SMHMessaging {
public:
    // Constructor with human-readable deviceID and groupID (1 through 255)
    SMHMessaging(uint8_t deviceID, uint8_t groupID);
    
    // Method to create packets
    int createPacket(uint8_t userID, uint8_t groupID, uint8_t packetType, const uint8_t* payload);
    
    // Methods to parse and handle packets
    int parsePacket(const uint8_t* packet, size_t packet_length, uint8_t& dest_addr, uint8_t& src_addr, uint8_t& msg_type, uint8_t* payload, size_t& payload_length);
    bool isPacketForMe(const uint8_t* packet, size_t packet_length);
    int handlePacket(const uint8_t* packet, size_t packet_length, uint8_t* return_packet, size_t& return_packet_length);

    // Method to get the created packet
    void getCreatedPacket(uint8_t*& packet, size_t& packet_length);

protected:
    // Virtual methods for handling different types of messages
    virtual void handleCommand(uint8_t src_addr, const uint8_t* payload, size_t payload_length);
    virtual void handleData(uint8_t src_addr, const uint8_t* payload, size_t payload_length);
    virtual void handleAck(uint8_t src_addr, const uint8_t* payload, size_t payload_length);
    virtual void handleError(uint8_t src_addr, const uint8_t* payload, size_t payload_length);
    virtual void handleStatus(uint8_t src_addr, const uint8_t* payload, size_t payload_length);

private:
    uint8_t deviceID; // Device ID (1 through 255)
    uint8_t groupID;  // Group ID (1 through 255)
    uint8_t createdPacket[256]; // Buffer for created packet
    size_t createdPacketLength; // Length of created packet
};

#endif // SMHMESSAGING_H
