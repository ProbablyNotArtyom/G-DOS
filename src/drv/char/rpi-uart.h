//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Raspberry pi UART header

#ifndef _DEV_RPI_UART_HEADER
#define _DEV_RPI_UART_HEADER

	#include <platdep/rpi-gpio.h>
	#include <platdep/rpi-mailbox.h>

/* PL011 UART registers */
#define UART0_DR        ((MMIO_BASE+0x00201000))
#define UART0_FR        ((MMIO_BASE+0x00201018))
#define UART0_IBRD      ((MMIO_BASE+0x00201024))
#define UART0_FBRD      ((MMIO_BASE+0x00201028))
#define UART0_LCRH      ((MMIO_BASE+0x0020102C))
#define UART0_CR        ((MMIO_BASE+0x00201030))
#define UART0_IMSC      ((MMIO_BASE+0x00201038))
#define UART0_ICR       ((MMIO_BASE+0x00201044))

//---------------------------------------------------

#endif
