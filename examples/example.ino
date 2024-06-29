#include <SMHMessaging.h>
#include <Arduino.h>

// Simulate sending device (Device 9)
SMHMessaging senderDevice(9, 0); // Device ID 9, no group

// Simulate receiving device (Device 3 in group 11)
SMHMessaging receiverDevice(3, 11); // Device ID 3, Group ID 11

unsigned long lastSendTime = 0;

void setup() {
    Serial.begin(115200);
}

void loop() {
    unsigned long currentTime = millis();

    // Send a packet every 2 seconds
    if (currentTime - lastSendTime >= 2000) {
        lastSendTime = currentTime;

        // Step 1: Create a packet from device 9 to group 11 with timestamp as payload
        uint32_t timestamp = currentTime;
        uint8_t payload[5];
        payload[0] = (timestamp >> 24) & 0xFF;
        payload[1] = (timestamp >> 16) & 0xFF;
        payload[2] = (timestamp >> 8) & 0xFF;
        payload[3] = timestamp & 0xFF;
        payload[4] = '\0'; // Null-terminate the payload

        int error = senderDevice.createPacket(0, 11, 0x01, payload); // Destination is Group 11
        if (error == MSG_NO_ERROR) {
            Serial.print("Created Packet: ");
            uint8_t* packet;
            size_t packet_length;
            senderDevice.getCreatedPacket(packet, packet_length);
            for (size_t i = 0; i < packet_length; ++i) {
                Serial.printf("%02X ", packet[i]);
            }
            Serial.println();
        } else if (error == MSG_INVALID_ADDRESS) {
            Serial.println("Error: Both userID and groupID are filled.");
        } else {
            Serial.printf("Error creating packet: %d\n", error);
        }

        // Step 2: Handle the packet on device 3
        uint8_t return_packet[256] = {0};
        size_t return_packet_length = 0;

        uint8_t* packet;
        size_t packet_length;
        senderDevice.getCreatedPacket(packet, packet_length);

        error = receiverDevice.handlePacket(packet, packet_length, return_packet, return_packet_length);
        if (error == MSG_NO_ERROR) {
            if (return_packet_length > 0) {
                Serial.print("Return Packet: ");
                for (size_t i = 0; i < return_packet_length; ++i) {
                    Serial.printf("%02X ", return_packet[i]);
                }
                Serial.println();

                // Extract and print the timestamp
                if (return_packet_length >= 8) { // Ensure there's enough data to extract the timestamp
                    uint32_t received_timestamp = (return_packet[4] << 24) | (return_packet[5] << 16) | (return_packet[6] << 8) | return_packet[7];
                    Serial.printf("Timestamp: %lu ms\n", received_timestamp);
                }
            } else {
                Serial.println("Packet not for this device or its group.");
            }
        } else {
            Serial.printf("Error handling packet: %d\n", error);
        }
    }
}
