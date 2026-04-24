#include <Arduino.h>

#define TXD2 17
#define RXD2 16

// -------- Control Variables --------
int8_t rx = 0;
int8_t ry = 0;
int8_t lx = 0;

// -------- Data Array --------
int8_t data[3];   // RY, RX, LX

// -------- Send Function --------
void datasend()
{
    uint8_t checksum = 0;

    // Fill data
    data[0] = ry;
    data[1] = rx;
    data[2] = lx;

    // Checksum
    for (int i = 0; i < 3; i++) {
        checksum += (uint8_t)data[i];
    }

    // Header
    Serial2.write(0x32);
    Serial2.write(0x32);

    // Data
    Serial2.write((uint8_t*)data, 3);

    // Checksum
    Serial2.write(checksum);
}

// -------- Debug Print --------
void printData()
{
    Serial.print("TX -> ");
    Serial.print("RY:"); Serial.print(ry);
    Serial.print(" RX:"); Serial.print(rx);
    Serial.print(" LX:"); Serial.print(lx);

    Serial.print(" | CHK:");

    uint8_t chk = ((uint8_t)ry + (uint8_t)rx + (uint8_t)lx);
    Serial.println(chk);
}

// -------- Setup --------
void setup()
{
    Serial.begin(115200);

    // IMPORTANT: Use UART2 (not GPIO 1,3)
    Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

    Serial.println("ESP32 Raw Packet Sender Started");
}

// -------- Loop --------
void loop()
{
    // -------- TEST 1: Forward --------
    ry = 100;
    rx = 0;
    lx = 0;

    datasend();
    printData();
    delay(500);

    // -------- TEST 2: Right --------
    ry = 0;
    rx = 100;
    lx = 0;

    datasend();
    printData();
    delay(500);

    // -------- TEST 3: Rotate --------
    ry = 0;
    rx = 0;
    lx = 100;

    datasend();
    printData();
    delay(500);

    // -------- TEST 4: Stop --------
    ry = 0;
    rx = 0;
    lx = 0;

    datasend();
    printData();
    delay(1000);
}