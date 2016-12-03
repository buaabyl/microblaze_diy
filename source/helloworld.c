#include "xiomodule.h"

static volatile int _ticks = 0;

void systemtick_handler(void)
{
    _ticks++;
}

void x_putchar(char c)
{
    while ((inp(XUL_STATUS_REG_OFFSET) & XUL_SR_TX_FIFO_FULL) == XUL_SR_TX_FIFO_FULL) {
    }

    outp(XUL_TX_OFFSET, c);
}

void x_puts(const char* s)
{
    while (*s) {
        x_putchar(*s++);
    }
}

void x_puthex4(char v)
{
    if (v < 10) {
        x_putchar(v + '0');
    } else {
        x_putchar(v - 10 + 'A');
    }
}

void x_puthex8(uint8_t v)
{
    x_puthex4(v >> 4);
    x_puthex4(v & 0x0F);
}

void x_puthex32(int v)
{
    uint8_t* p;
    p = (uint8_t*)&v;

    x_puthex8(p[3]);
    x_puthex8(p[2]);
    x_puthex8(p[1]);
    x_puthex8(p[0]);
}

//system clock is 50Mhz
//FIT1 count is 50000
//
//  udelay(5000000) ~ ticks == 1106 (ms)
//  so the fellow performance is 10 instruction per count
#define DELAY_1USEC 5
#define DELAY_1MSEC 5000
#define DELAY_1SEC  5000000
void udelay(unsigned int count)
{
    volatile uint8_t tmp;
    do {
        tmp++;
    } while (count--);
}

void msleep(unsigned int count)
{
    unsigned int tk  = _ticks;
    unsigned int end = tk + count;

    //wait overflow
    if (end < count) {
        while (tk < _ticks) {
        }
    }
    //wait normal or looback
    while (_ticks < end) {
    }
}

extern int _stack;
extern int _stack_end;

static int demo_with_timer(void)
{
    int i;
    int led;

    //disable all interrupts
    outp(XIN_IER_OFFSET, 0);
    outp(XIN_IMR_OFFSET, 0);
    //clear all interrupts
    outp(XIN_IAR_OFFSET, 0xFFFFFFFF);

    //enable system tick interrupt
    outp(XIN_IER_OFFSET, 1 << XIN_IOMODULE_FIT_1_INTERRUPT_INTR);
    microblaze_enable_interrupts();

    x_puts("Hello World\r\n");
    x_puts(__DATE__ " " __TIME__ "\r\n");

    x_puts(" stack_bottom  0x");
    x_puthex32((int)&_stack);
    x_puts("\r\n");

    x_puts(" stack_current 0x");
    x_puthex32((int)&led);
    x_puts("\r\n");

    x_puts(" stack_end     0x");
    x_puthex32((int)&_stack_end);
    x_puts("\r\n");


    led = 0;
    outp(0 * XGPO_CHAN_OFFSET + XGPO_DATA_OFFSET, led++);

    while (1) {
        msleep(500);
        //udelay(100 * DELAY_1MSEC);
        outp(0 * XGPO_CHAN_OFFSET + XGPO_DATA_OFFSET, led++);
        //x_puts("0x");
        //x_puthex32(_ticks);
        //x_puts("\r\n");
    }

    return 0;
}

static int demo_loop(void)
{
    int led;

    outp(XIN_IER_OFFSET, 0);
    outp(XIN_IMR_OFFSET, 0);
    outp(XIN_IAR_OFFSET, 0xFFFFFFFF);

    x_puts("Hello World\r\n");
    x_puts(__DATE__ " " __TIME__ "\r\n");

    led = 0xFF;
    outp(0 * XGPO_CHAN_OFFSET + XGPO_DATA_OFFSET, led++);

    while (1) {
        udelay(5000000);// ~ 1s
        outp(0 * XGPO_CHAN_OFFSET + XGPO_DATA_OFFSET, led++);
        x_puts("LED 0x");
        x_puthex32(led);
        x_puts("\r\n");
    }

    return 0;
}

// !!!this must using in XPS, not Microblaze MCS!!!
// you can enable hardware MUL and FPU in XPS,
// Microblaze MCS is not configurable for this features
// If you do this, the core may halt!!!
volatile float f1 = 1.1;
volatile float f2 = 2.1;

static int demo_mul(void)
{
    int i;
    int a;
    int b;
    int c;
    float v;
    int expect = 0x4053D70A;

    udelay(DELAY_1MSEC);
    a = ((int)(&c)) >> 8;
    b = ((int)(&c)) & 0xFF;

    c = a * b;
    x_puthex32(a);
    x_puts(" x ");
    x_puthex32(b);
    x_puts(" = ");
    x_puthex32(c);
    x_puts("\r\n");

    v = f1 * f2 + 1.0f;
    x_puts("float result: ");
    c = *(int*)&v;
    x_puthex32(c);
    x_puts(" (calculated)\r\n");

    x_puts("float result: ");
    x_puthex32(expect);
    x_puts(" (expect)\r\n");

    return 0;
}

int main(void)
{
    return demo_with_timer();
    //return demo_loop();
    //return demo_mul();
}

////////////////////////////////////////////////////////////
static const iomodule_intc_handler _services[15] = {
    NULL,               // 0: UART_ERROR
    NULL,               // 1: UART_TX
    NULL,               // 2: UART_RX
    NULL,               // 3: PIT_1
    NULL,               // 4: PIT_2
    NULL,               // 5: PIT_3
    NULL,               // 6: PIT_4
    systemtick_handler, // 7: FIT_1
    NULL,               // 8: FIT_2
    NULL,               // 9: FIT_3
    NULL,               //10: FIT_4
    NULL,               //11: GPI_1
    NULL,               //12: GPI_2
    NULL,               //13: GPI_3
    NULL,               //14: GPI_4
};

void __interrupt_handler(void)
{
    uint32_t status;
    uint32_t mask = 1;
    int i;

    status = inp(XIN_IPR_OFFSET);
    for (i = 0;i < sizeof(_services);i++) {
        if (status & 1) {
            if (_services[i] != NULL) {
                _services[i]();
            }
            outp(XIN_IAR_OFFSET, mask);
        }
        status >>= 1;
        mask   <<= 1;
        if (status == 0) {
            return;
        }
    }
}

void exit(int code)
{
    while (1) {
    }
}

