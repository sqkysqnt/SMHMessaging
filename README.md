# SMH Messaging Library

SMH Messaging (Stage Messaging Handler) is a lightweight library for ESP32 devices that facilitates sending and receiving messages between multiple devices. The library supports up to 255 devices and 64 groups.

## Features

- Create and parse packets with minimal dependencies
- Support for device and group addressing
- Handle various message types (commands, data, acknowledgments, errors, status)
- Simple API for packet creation and handling

## Installation

To use the SMH Messaging library in your project, download the `SMHMessaging.h` and `SMHMessaging.cpp` files and include them in your project.

## Usage

### Include the Library

```cpp
#include "SMHMessaging.h"

### Create an Instance

Create an instance of the `SMHMessaging` class for the sending and receiving devices. Each device has a unique device ID and an optional group ID.

```cpp
// Simulate sending device (Device 9)
SMHMessaging senderDevice(9, 0); // Device ID 9, no group

// Simulate receiving device (Device 3 in group 11)
SMHMessaging receiverDevice(3, 11); // Device ID 3, Group ID 11


### Create a Packet

Use the `createPacket` method to create a packet with a specific user ID, group ID, packet type, and payload.

```cpp
uint32_t timestamp = millis();
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


### Handle a Packet

Use the `handlePacket` method to process an incoming packet. If the packet is intended for the device or its group, the method will return the packet and its length.

```cpp
uint8_t return_packet[256] = {0};
size_t return_packet_length = 0;

uint8_t* packet;
size_t packet_length;
senderDevice.getCreatedPacket(packet, packet_length);

int error = receiverDevice.handlePacket(packet, packet_length, return_packet, return_packet_length);
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

## API Reference

### Constructor

```cpp
SMHMessaging(uint8_t deviceID, uint8_t groupID);

- deviceID: Unique identifier for the device (1-255).
- groupID: Group identifier for the device (1-255).

### Methods

#### `int createPacket(uint8_t userID, uint8_t groupID, uint8_t packetType, const uint8_t* payload);`
- Creates a packet for a specific user or group.
- `userID`: Destination user ID (1-255).
- `groupID`: Destination group ID (1-255).
- `packetType`: Type of the packet (command, data, etc.).
- `payload`: Pointer to the payload data.
- Returns an error code (`MSG_NO_ERROR`, `MSG_INVALID_ADDRESS`, `MSG_INVALID_PAYLOAD_LENGTH`).

#### `void getCreatedPacket(uint8_t*& packet, size_t& packet_length);`
- Retrieves the created packet and its length.
- `packet`: Reference to the packet buffer.
- `packet_length`: Reference to the packet length.

#### `int handlePacket(const uint8_t* packet, size_t packet_length, uint8_t* return_packet, size_t& return_packet_length);`
- Handles an incoming packet and returns it if intended for the device or its group.
- `packet`: Pointer to the incoming packet.
- `packet_length`: Length of the incoming packet.
- `return_packet`: Buffer to store the returned packet.
- `return_packet_length`: Length of the returned packet.
- Returns an error code (`MSG_NO_ERROR`, `MSG_PACKET_TOO_SHORT`, `MSG_INVALID_PAYLOAD_LENGTH`, `MSG_UNKNOWN_MESSAGE_TYPE`).

## License

This library is licensed under the MIT License. See the LICENSE file for details.
