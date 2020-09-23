/* Taken from Wiki pages */
The UART test cases tries to cover all the below test cases.



Overrun error
An "overrun error" occurs when the receiver cannot process the character that just came in before the next one arrives. Various devices have different amounts of buffer space to hold received characters. The CPU or DMA controller must service the UART in order to remove characters from the input buffer. If the CPU or DMA controller does not service the UART quickly enough and the buffer becomes full, an Overrun Error will occur, and incoming characters will be lost.

Procedure: 
Tries to receive continuous data in polling mode and just try to read status register without reading receive fifo buffer.
As FIFO is not read to receive new data, overrun error will occur.

Underrun error
An "underrun error" occurs when the UART transmitter has completed sending a character and the transmit buffer is empty. In asynchronous modes this is treated as an indication that no data remains to be transmitted, rather than an error, since additional stop bits can be appended. This error indication is commonly found in USARTs, since an underrun is more serious in synchronous systems.

Procedure:

Configure UART and send only one byte.
Try to check the status register.
As none of the data except the first is transmitted, we will get underrun error.


Framing error
A UART will detect a framing error when it does not see a "stop" bit at the expected "stop" bit time. As the "start" bit is used to identify the beginning of an incoming character, its timing is a reference for the remaining bits. If the data line is not in the expected state (high) when the "stop" bit is expected (according to the number of data and parity bits for which the UART is set), the UART will signal a framing error. A "break" condition on the line is also signaled as a framing error.


Procedure:
1. Configure UART with two stop bits.
2. Configure miniterm for 1 stop bit.
3. Wait in the baseloop. Try to read the status register by sending some data from miniterm.
4. We will get an framing error bit set.

Parity error
A parity error occurs when the parity of the number of one-bits disagrees with that specified by the parity bit. Use of a parity bit is optional, so this error will only occur if parity-checking has been enabled.

SoC receives data.
1. Configure UART for odd partity.
2. Configure miniterm for odd parity.
3. Send data from miniterm to SoC.
4. Read data in the code. We will be getting right data. Also check the parity 
   error bit in status register. We won't get any error.
5. Configure miniterm for even parity.
6. Read data in the code. We won't be getting right data. Also check the parity 
   error bit in status register. We will get error.
5. Configure miniterm for no parity.
7. Read data in the code. We won't be getting right data. Also check the parity 
   error bit in status register. We will get error.

SoC transmits data.
1. Configure UART for odd partity.
2. Configure miniterm for odd parity.
3. Send data from SoC to miniterm.
4. Read data in the miniterm. We will be getting right data. 
5. Configure miniterm for even parity.
6. Read data in the SoC. We won't be getting right data. The data will be junk.
5. Configure miniterm for no parity.
7. Read data in the code. We won't be getting right data. The data will be junk.


Repeat the above two test cases for even parity (Replace odd with even and even with add in the above two cases.).


Break condition
A break condition occurs when the receiver input is at the "space" (logic low, i.e., '0') level for longer than some duration of time, typically, for more than a character time. This is not necessarily an error, but appears to the receiver as a character of all zero-bits with a framing error. The term "break" derives from current loop signaling, which was the traditional signaling used for teletypewriters. The "spacing" condition of a current loop line is indicated by no current flowing, and a very long period of no current flowing is often caused by a break or other fault in the line.

Some equipment will deliberately transmit the "space" level for longer than a character as an attention signal. When signaling rates are mismatched, no meaningful characters can be sent, but a long "break" signal can be a useful way to get the attention of a mismatched receiver to do something (such as resetting itself). Computer systems can use the long "break" level as a request to change the signaling rate, to support dial-in access at multiple signaling rates. The DMX512 protocol uses the break condition to signal the start of a new packet.


Procedure:
1. Configure UART and miniterm with same baudrate, no of bits to be transmitted and parity.
2. Continuously send 0x00 from miniterm.
3. Check the status register in code.
4. After some time, we will be getting break condition.


