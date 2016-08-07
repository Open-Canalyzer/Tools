#ifndef SLCAN_H
#define SLCAN_H

#define SLCAN_CMD_SETUP     'S' //   YES+      Sn[CR]               Setup with standard CAN bit-rates where n is 0-8.
                                //                                  S0 10Kbit          S4 125Kbit         S8 1Mbit
                                //                                  S1 20Kbit          S5 250Kbit         S9 83.3Kbit
                                //                                  S2 50Kbit          S6 500Kbit
                                //                                  S3 100Kbit         S7 800Kbit
#define SLCAN_CMD_SETUP_BTR 's' //    -        sxxyy[CR]            Setup with BTR0/BTR1 CAN bit-rates where xx and yy is a hex value.
#define SLCAN_CMD_OPEN      'O' //   YES       O[CR]                Open the CAN channel in normal mode (sending & receiving).
#define SLCAN_CMD_LISTEN    'L' //   YES       L[CR]                Open the CAN channel in listen only mode (receiving).
#define SLCAN_CMD_CLOSE     'C' //   YES       C[CR]                Close the CAN channel.
#define SLCAN_CMD_TX11      't' //   YES       tiiildd...[CR]       Transmit a standard (11bit) CAN frame.
#define SLCAN_CMD_TX29      'T' //   YES       Tiiiiiiiildd...[CR]  Transmit an extended (29bit) CAN frame
#define SLCAN_CMD_RTR11     'r' //   YES       riiil[CR]            Transmit an standard RTR (11bit) CAN frame.
#define SLCAN_CMD_RTR29     'R' //   YES       Riiiiiiiil[CR]       Transmit an extended RTR (29bit) CAN frame.
#define SLCAN_CMD_POLL_ONE  'P' //   YES       P[CR]                Poll incomming FIFO for CAN frames (single poll)
#define SLCAN_CMD_POLL_MANY 'A' //   YES       A[CR]                Polls incomming FIFO for CAN frames (all pending frames)
#define SLCAN_CMD_FLAGS     'F' //   YES+      F[CR]                Read Status Flags.
#define SLCAN_CMD_AUTOPOLL  'X' //   YES       Xn[CR]               Sets Auto Poll/Send ON/OFF for received frames.
#define SLCAN_CMD_FILTER    'W' //    -        Wn[CR]               Filter mode setting. By default CAN232 works in dual filter mode (0) and is backwards compatible with previous CAN232 versions.
#define SLCAN_CMD_ACC_CODE  'M' //    -        Mxxxxxxxx[CR]        Sets Acceptance Code Register (ACn Register of SJA1000). // we use MCP2515, not supported
#define SLCAN_CMD_ACC_MASK  'm' //    -        mxxxxxxxx[CR]        Sets Acceptance Mask Register (AMn Register of SJA1000). // we use MCP2515, not supported
#define SLCAN_CMD_UART      'U' //   YES       Un[CR]               Setup UART with a new baud rate where n is 0-6.
#define SLCAN_CMD_VERSION1  'V' //   YES       v[CR]                Get Version number of both CAN232 hardware and software
#define SLCAN_CMD_VERSION2  'v' //   YES       V[CR]                Get Version number of both CAN232 hardware and software
#define SLCAN_CMD_SERIAL    'N' //   YES       N[CR]                Get Serial number of the CAN232.
#define SLCAN_CMD_TIMESTAMP 'Z' //   YES+      Zn[CR]               Sets Time Stamp ON/OFF for received frames only. EXTENSION to LAWICEL: Z2 - millis() timestamp w/o standard 60000ms cycle
#define SLCAN_CMD_AUTOSTART 'Q' //   YES  todo     Qn[CR]               Auto Startup feature (from power on). 

#define SLCAN_OK                      0x00
#define SLCAN_OK_SMALL                0x01
#define SLCAN_OK_BIG                  0x02
#define SLCAN_ERR                     0x03
#define SLCAN_ERR_NOT_IMPLEMENTED     0x04
#define SLCAN_ERR_UNKNOWN_CMD         0x05

int RunStateMachine(const char* in, char* out, int* parsed);

#endif
