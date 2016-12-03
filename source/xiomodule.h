/* vim: set fileencoding=utf-8:
 *
 *                   GNU GENERAL PUBLIC LICENSE
 *                       Version 2, June 1991
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 *
 * 2016.11.29   created by buaa.byl@gmail.com
 *
 * @ref https://www.xilinx.com/support/documentation/ip_documentation/iomodule/v1_00_a/ds866_iomodule.pdf
 * @ref https://www.xilinx.com/support/documentation/sw_manuals/xilinx14_1/ds865_microblaze_mcs.pdf
 * @ref C:/Xilinx/14.7/ISE_DS/EDK/sw/lib/bsp/standalone_v3_11_a/src/microblaze
 * @ref C:/Xilinx/14.7/ISE_DS/EDK/sw/XilinxProcessorIPLib/drivers/iomodule_v1_04_a/src
 */
#ifndef XIOMODULE_H_48D0AB7A_B600_11E6_938E_93F2EE7A8987_INCLUDED_
#define XIOMODULE_H_48D0AB7A_B600_11E6_938E_93F2EE7A8987_INCLUDED_

////////////////////////////////////////////////////////////
#define NULL    0
typedef char            int8_t;
typedef short           int16_t;
typedef int             int32_t;
typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;

////////////////////////////////////////////////////////////
#define XIOMODULE_BASEADDR      0x80000000

////////////////////////////////////////////////////////////
#define XTC_DEVICE_TIMER_COUNT           4
#define XTC_TIMER_COUNTER_OFFSET        16
#define XTC_TIMER_COUNTER_SHIFT          4

#define XUL_RX_OFFSET           0x00000000  /**< UART Receive Register     - R */
#define XUL_TX_OFFSET           0x00000004  /**< UART Transmit Register    - W */
#define XUL_STATUS_REG_OFFSET   0x00000008  /**< UART Status Register      - R */
#define XUL_BAUDRATE_OFFSET     0x0000004C  /**< UART Baud Rate Register   - W */

#define XIN_IMR_OFFSET          0x0000000C  /**< Intr Mode Register        - W */

#define XGO_OUT_OFFSET          0x00000010  /**< General Purpose Output    - W */

#define XGI_IN_OFFSET           0x00000020  /**< General Purpose Input     - R */

#define XIN_ISR_OFFSET          0x00000030  /**< Intr Status Register      - R */
#define XIN_IPR_OFFSET          0x00000034  /**< Intr Pending Register     - R */
#define XIN_IER_OFFSET          0x00000038  /**< Intr Enable Register      - W */
#define XIN_IAR_OFFSET          0x0000003C  /**< Intr Acknowledge Register - W */

#define XTC_TLR_OFFSET          0x00000040  /**< Timer Load register       - W */
#define XTC_TCR_OFFSET          0x00000044  /**< Timer counter register    - R */
#define XTC_TCSR_OFFSET         0x00000048  /**< Timer Control register    - W */
#define XIN_IVAR_OFFSET         0x00000080  /**< Intr Vector Address Register,
                                                 Interrupt 0 offset, present
                                                 only for Fast Interrupt   - W */

////////////////////////////////////////////////////////////
// Peripheral: UART
#define XUL_SR_PARITY_ERROR           0x80
#define XUL_SR_FRAMING_ERROR          0x40
#define XUL_SR_OVERRUN_ERROR          0x20
#define XUL_SR_INTR_ENABLED           0x10  /**< UART Interrupt enabled     */
#define XUL_SR_TX_FIFO_FULL           0x08  /**< UART Transmit FIFO full    */
#define XUL_SR_RX_FIFO_VALID_DATA     0x01  /**< UART Data Register valid   */

#define XUL_STOP_BITS                    1

#define XUL_PARITY_NONE                  0
#define XUL_PARITY_ODD                   1
#define XUL_PARITY_EVEN                  2

// Peripheral: GPIO
#define XGPI_DEVICE_COUNT                4
#define XGPO_DEVICE_COUNT                4

#define XGPI_CHAN_OFFSET           0x00004
#define XGPI_DATA_OFFSET           0x00020

#define XGPO_CHAN_OFFSET           0x00004
#define XGPO_DATA_OFFSET           0x00010


////////////////////////////////////////////////////////////
// Interrupt offset for internal interrupts
#define XIN_IOMODULE_GPI_4_INTERRUPT_INTR      14
#define XIN_IOMODULE_GPI_3_INTERRUPT_INTR      13
#define XIN_IOMODULE_GPI_2_INTERRUPT_INTR      12
#define XIN_IOMODULE_GPI_1_INTERRUPT_INTR      11
#define XIN_IOMODULE_FIT_4_INTERRUPT_INTR      10
#define XIN_IOMODULE_FIT_3_INTERRUPT_INTR       9
#define XIN_IOMODULE_FIT_2_INTERRUPT_INTR       8
#define XIN_IOMODULE_FIT_1_INTERRUPT_INTR       7
#define XIN_IOMODULE_PIT_4_INTERRUPT_INTR       6
#define XIN_IOMODULE_PIT_3_INTERRUPT_INTR       5
#define XIN_IOMODULE_PIT_2_INTERRUPT_INTR       4
#define XIN_IOMODULE_PIT_1_INTERRUPT_INTR       3
#define XIN_IOMODULE_UART_RX_INTERRUPT_INTR     2
#define XIN_IOMODULE_UART_TX_INTERRUPT_INTR     1
#define XIN_IOMODULE_UART_ERROR_INTERRUPT_INTR  0

// External interrrupt begin from bits 16
#define XIN_IOMODULE_EXTERNAL_INTERRUPT_INTR   16


////////////////////////////////////////////////////////////
// microblaze only support one interrupt,
// and the microblaze gcc implement "__attribute__ ((interrupt_handler))"
// when we add this after function declare,
// some push registers and pop registers codes is added automatic.
void __interrupt_handler (void) __attribute__ ((interrupt_handler));
typedef void (*iomodule_intc_handler)(void);

// this implement in xilinx bsp, written in assembly,
// so I just copy that files.
void microblaze_enable_interrupts(void);
void microblaze_disable_interrupts(void);

// 80x86 windows like interface
#define inp(offs)           (*(volatile unsigned int*)(XIOMODULE_BASEADDR + (offs)))
#define outp(offs, val)     (*(volatile unsigned int*)(XIOMODULE_BASEADDR + (offs)) = (val))


#endif

