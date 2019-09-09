//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// 16550 UART Header

#ifndef _DEV_16550_UART_HEADER
#define _DEV_16550_UART_HEADER

#define UART_16550_REG_THR 0x00  /* Transmitter holding reg. */
#define UART_16550_REG_RDR 0x00  /* Receiver data reg.       */
#define UART_16550_REG_BRDL 0x00 /* Baud rate divisor (LSB)  */
#define UART_16550_REG_BRDH 0x01 /* Baud rate divisor (MSB)  */
#define UART_16550_REG_IER 0x01  /* Interrupt enable reg.    */
#define UART_16550_REG_IIR 0x02  /* Interrupt ID reg.        */
#define UART_16550_REG_FCR 0x02  /* FIFO control reg.        */
#define UART_16550_REG_LCR 0x03  /* Line control reg.        */
#define UART_16550_REG_MDC 0x04  /* Modem control reg.       */
#define UART_16550_REG_LSR 0x05  /* Line status reg.         */
#define UART_16550_REG_MSR 0x06  /* Modem status reg.        */

#define UART_16550_IER_RXRDY 0x01 /* receiver data ready */
#define UART_16550_IER_TBE 0x02   /* transmit bit enable */
#define UART_16550_IER_LSR 0x04   /* line status interrupts */
#define UART_16550_IER_MSI 0x08   /* modem status interrupts */

#define UART_16550_LSR_RXRDY 0x01 /* receiver data available */
#define UART_16550_LSR_OE 0x02    /* overrun error */
#define UART_16550_LSR_PE 0x04    /* parity error */
#define UART_16550_LSR_FE 0x08    /* framing error */
#define UART_16550_LSR_BI 0x10    /* break interrupt */
#define UART_16550_LSR_THRE 0x20  /* transmit holding register empty */
#define UART_16550_LSR_TEMT 0x40  /* transmitter empty */
#define UART_16550_LSR_BOTH_EMPTY (LSR_THRE|LSR_TEMT)

//---------------------------------------------------

charResult uart_16550_dev_write(char out);
char uart_16550_dev_read();
charResult uart_16550_dev_init();

//---------------------------------------------------

#endif
