# Celestron AUX Bus Protocol

## Connection Details
The AUX Bus communicates over serial, 19200bps, no parity, 1 stop bit, and hardware (RTS/CTS) flow control.

## Device IDs
The following are some known Device IDs:
0x01    Main Board
0x04    Hand Control
0x10    Azimuth Motor Control
0x11    Altitude Motor Control
0xB0    GPS Board
0xB2    RTC

## Message Format
Data is sent over the AUX Bus in formatted messages. The message format is as follows:
Byte 0: 0x3B (preamble - all packets start with this byte)
Byte 1: <message bytes>
Byte 2: <source device ID>
Byte 3: <destination device ID>
Byte 4: <command>
Byte 5 .. Byte n-1: <data>
Byte n: <checksum>
 - <message bytes> is a count of the bytes in the message, starting with Byte 2 and ending with the byte just before the checksum. So in a non-data message <message bytes> would be 0x03 - <source ID>, <destination ID>, and <command>.

### Checksum Calculation
The checksum is calculated by doing the following:
1. Taking all bytes from (and including) Byte 1 to Byte n-1
2. Summing the result of Step 1
3. Taking the two's complement of the result of Step 2
4. Taking the least significant byte from the result of Step 3
Consider the following message from the HC to the Azimuth Motor Control requesting its version:
    <0x3B> <0x03> <0x04> <0x10> <0xFE> <checksum>
    Sum all the bytes: 0x03 + 0x04 + 0x10 + 0xFE = 0x115
    Take the two's complement of the LSB: 0x15 --> 0xEB
    <checksum> = <0xEB>

### Message Example
This is an example of a message exchange between the Hand Control and the Azimuth Motor Control requesting the Azimuth Motor Control's version number.
    <0x3B> <0x03> <0x04> <0x10> <0xFE> <0xEB>
    <0x3B> <0x05> <0x10> <0x04> <0xFE> <0x04> <0x03> <0xE2>
