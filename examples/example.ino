#include <SMHMessaging.h>

SMHMessaging device(1, 192); // Using human-readable deviceID and groupID

void setup() {
    Serial.begin(115200);

    uint8_t payload[] = {0xAA, 0xBB, 0xCC};
    uint8_t packet[256];
    size_t packet_length;

    int error = device.createPacket(1, 1, payload, sizeof(payload), packet, packet_length);
    if (error == MSG_NO_ERROR) {
        Serial.print("Packet: ");
        for (size_t i = 0; i < packet_length; ++i) {
            Serial.printf("%02X ", packet[i]);
        }
        Serial.println();

        device.handlePacket(packet, packet_length);
    } else {
        Serial.printf("Error creating packet: %d\n", error);
    }
}

void loop() {
    // Your code here
}
