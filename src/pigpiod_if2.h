#ifndef PIGPIOD_IF2_H
#define PIGPIOD_IF2_H
#include <stddef.h>
#define PIGPIOD_IF2_VERSION 17
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

#define PI_MIN_GPIO       0
#define PI_MAX_GPIO      53

#define PI_MAX_USER_GPIO 31

#define PI_OFF   0
#define PI_ON    1

#define PI_CLEAR 0
#define PI_SET   1

#define PI_LOW   0
#define PI_HIGH  1

#define PI_TIMEOUT 2

#define PI_INPUT  0
#define PI_OUTPUT 1
#define PI_ALT0   4
#define PI_ALT1   5
#define PI_ALT2   6
#define PI_ALT3   7
#define PI_ALT4   3
#define PI_ALT5   2

#define PI_PUD_OFF  0
#define PI_PUD_DOWN 1
#define PI_PUD_UP   2

#define PI_DEFAULT_DUTYCYCLE_RANGE   255

#define PI_MIN_DUTYCYCLE_RANGE        25
#define PI_MAX_DUTYCYCLE_RANGE     40000

#define PI_SERVO_OFF 0
#define PI_MIN_SERVO_PULSEWIDTH 500
#define PI_MAX_SERVO_PULSEWIDTH 2500

#define PI_HW_PWM_MIN_FREQ 1
#define PI_HW_PWM_MAX_FREQ      125000000
#define PI_HW_PWM_MAX_FREQ_2711 187500000
#define PI_HW_PWM_RANGE 1000000

#define PI_HW_CLK_MIN_FREQ       4689
#define PI_HW_CLK_MIN_FREQ_2711 13184
#define PI_HW_CLK_MAX_FREQ      250000000
#define PI_HW_CLK_MAX_FREQ_2711 375000000

#define PI_NOTIFY_SLOTS  32

#define PI_NTFY_FLAGS_EVENT    (1 <<7)
#define PI_NTFY_FLAGS_ALIVE    (1 <<6)
#define PI_NTFY_FLAGS_WDOG     (1 <<5)
#define PI_NTFY_FLAGS_BIT(x) (((x)<<0)&31)

#define PI_WAVE_BLOCKS     4
#define PI_WAVE_MAX_PULSES (PI_WAVE_BLOCKS * 3000)
#define PI_WAVE_MAX_CHARS  (PI_WAVE_BLOCKS *  300)

#define PI_BB_I2C_MIN_BAUD     50
#define PI_BB_I2C_MAX_BAUD 500000

#define PI_BB_SPI_MIN_BAUD     50
#define PI_BB_SPI_MAX_BAUD 250000

#define PI_BB_SER_MIN_BAUD     50
#define PI_BB_SER_MAX_BAUD 250000

#define PI_BB_SER_NORMAL 0
#define PI_BB_SER_INVERT 1

#define PI_WAVE_MIN_BAUD      50
#define PI_WAVE_MAX_BAUD 1000000

#define PI_SPI_MIN_BAUD     32000
#define PI_SPI_MAX_BAUD 125000000

#define PI_MIN_WAVE_DATABITS 1
#define PI_MAX_WAVE_DATABITS 32

#define PI_MIN_WAVE_HALFSTOPBITS 2
#define PI_MAX_WAVE_HALFSTOPBITS 8

#define PI_WAVE_MAX_MICROS (30 * 60 * 1000000) /* half an hour */

#define PI_MAX_WAVES 250

#define PI_MAX_WAVE_CYCLES 65535
#define PI_MAX_WAVE_DELAY  65535

#define PI_WAVE_COUNT_PAGES 10

#define PI_WAVE_MODE_ONE_SHOT      0
#define PI_WAVE_MODE_REPEAT        1
#define PI_WAVE_MODE_ONE_SHOT_SYNC 2
#define PI_WAVE_MODE_REPEAT_SYNC   3

#define PI_WAVE_NOT_FOUND  9998 /* Transmitted wave not found. */
#define PI_NO_TX_WAVE      9999 /* No wave being transmitted. */

#define PI_FILE_SLOTS 16
#define PI_I2C_SLOTS  512
#define PI_SPI_SLOTS  32
#define PI_SER_SLOTS  16

#define PI_MAX_I2C_ADDR 0x7F

#define PI_NUM_AUX_SPI_CHANNEL 3
#define PI_NUM_STD_SPI_CHANNEL 2

#define PI_MAX_I2C_DEVICE_COUNT (1<<16)
#define PI_MAX_SPI_DEVICE_COUNT (1<<16)

#define  PI_I2C_RDRW_IOCTL_MAX_MSGS 42

#define PI_I2C_M_WR           0x0000 /* write data */
#define PI_I2C_M_RD           0x0001 /* read data */
#define PI_I2C_M_TEN          0x0010 /* ten bit chip address */
#define PI_I2C_M_RECV_LEN     0x0400 /* length will be first received byte */
#define PI_I2C_M_NO_RD_ACK    0x0800 /* if I2C_FUNC_PROTOCOL_MANGLING */
#define PI_I2C_M_IGNORE_NAK   0x1000 /* if I2C_FUNC_PROTOCOL_MANGLING */
#define PI_I2C_M_REV_DIR_ADDR 0x2000 /* if I2C_FUNC_PROTOCOL_MANGLING */
#define PI_I2C_M_NOSTART      0x4000 /* if I2C_FUNC_PROTOCOL_MANGLING */

#define PI_I2C_END          0
#define PI_I2C_ESC          1
#define PI_I2C_START        2
#define PI_I2C_COMBINED_ON  2
#define PI_I2C_STOP         3
#define PI_I2C_COMBINED_OFF 3
#define PI_I2C_ADDR         4
#define PI_I2C_FLAGS        5
#define PI_I2C_READ         6
#define PI_I2C_WRITE        7

#define PI_SPI_FLAGS_BITLEN(x) ((x&63)<<16)
#define PI_SPI_FLAGS_RX_LSB(x)  ((x&1)<<15)
#define PI_SPI_FLAGS_TX_LSB(x)  ((x&1)<<14)
#define PI_SPI_FLAGS_3WREN(x)  ((x&15)<<10)
#define PI_SPI_FLAGS_3WIRE(x)   ((x&1)<<9)
#define PI_SPI_FLAGS_AUX_SPI(x) ((x&1)<<8)
#define PI_SPI_FLAGS_RESVD(x)   ((x&7)<<5)
#define PI_SPI_FLAGS_CSPOLS(x)  ((x&7)<<2)
#define PI_SPI_FLAGS_MODE(x)    ((x&3))

#define BSC_DR         0
#define BSC_RSR        1
#define BSC_SLV        2
#define BSC_CR         3
#define BSC_FR         4
#define BSC_IFLS       5
#define BSC_IMSC       6
#define BSC_RIS        7
#define BSC_MIS        8
#define BSC_ICR        9
#define BSC_DMACR     10
#define BSC_TDR       11
#define BSC_GPUSTAT   12
#define BSC_HCTRL     13
#define BSC_DEBUG_I2C 14
#define BSC_DEBUG_SPI 15

#define BSC_CR_TESTFIFO 2048
#define BSC_CR_RXE  512
#define BSC_CR_TXE  256
#define BSC_CR_BRK  128
#define BSC_CR_CPOL  16
#define BSC_CR_CPHA   8
#define BSC_CR_I2C    4
#define BSC_CR_SPI    2
#define BSC_CR_EN     1

#define BSC_FR_RXBUSY 32
#define BSC_FR_TXFE   16
#define BSC_FR_RXFF    8
#define BSC_FR_TXFF    4
#define BSC_FR_RXFE    2
#define BSC_FR_TXBUSY  1

#define BSC_SDA      18
#define BSC_MOSI     20
#define BSC_SCL_SCLK 19
#define BSC_MISO     18
#define BSC_CE_N     21

#define BSC_SDA_2711      10
#define BSC_MOSI_2711      9
#define BSC_SCL_SCLK_2711 11
#define BSC_MISO_2711     10
#define BSC_CE_N_2711      8

#define PI_MAX_BUSY_DELAY 100

#define PI_MIN_WDOG_TIMEOUT 0
#define PI_MAX_WDOG_TIMEOUT 60000

#define PI_MIN_TIMER 0
#define PI_MAX_TIMER 9

#define PI_MIN_MS 10
#define PI_MAX_MS 60000

#define PI_MAX_SCRIPTS       32

#define PI_MAX_SCRIPT_TAGS   50
#define PI_MAX_SCRIPT_VARS  150
#define PI_MAX_SCRIPT_PARAMS 10

#define PI_SCRIPT_INITING 0
#define PI_SCRIPT_HALTED  1
#define PI_SCRIPT_RUNNING 2
#define PI_SCRIPT_WAITING 3
#define PI_SCRIPT_FAILED  4

#define PI_MIN_SIGNUM 0
#define PI_MAX_SIGNUM 63

#define PI_TIME_RELATIVE 0
#define PI_TIME_ABSOLUTE 1

#define PI_MAX_MICS_DELAY 1000000 /* 1 second */
#define PI_MAX_MILS_DELAY 60000   /* 60 seconds */

#define PI_BUF_MILLIS_MIN 100
#define PI_BUF_MILLIS_MAX 10000

#define PI_CLOCK_PWM 0
#define PI_CLOCK_PCM 1

#define PI_MIN_DMA_CHANNEL 0
#define PI_MAX_DMA_CHANNEL 15

#define PI_MIN_SOCKET_PORT 1024
#define PI_MAX_SOCKET_PORT 32000


#define PI_DISABLE_FIFO_IF   1
#define PI_DISABLE_SOCK_IF   2
#define PI_LOCALHOST_SOCK_IF 4
#define PI_DISABLE_ALERT     8

#define PI_MEM_ALLOC_AUTO    0
#define PI_MEM_ALLOC_PAGEMAP 1
#define PI_MEM_ALLOC_MAILBOX 2

#define PI_MAX_STEADY  300000
#define PI_MAX_ACTIVE 1000000

#define PI_CFG_DBG_LEVEL         0 /* bits 0-3 */
#define PI_CFG_ALERT_FREQ        4 /* bits 4-7 */
#define PI_CFG_RT_PRIORITY       (1<<8)
#define PI_CFG_STATS             (1<<9)
#define PI_CFG_NOSIGHANDLER      (1<<10)

#define PI_CFG_ILLEGAL_VAL       (1<<11)


#define RISING_EDGE  0
#define FALLING_EDGE 1
#define EITHER_EDGE  2


#define PI_MAX_PAD 2

#define PI_MIN_PAD_STRENGTH 1
#define PI_MAX_PAD_STRENGTH 16

#define PI_FILE_NONE   0
#define PI_FILE_MIN    1
#define PI_FILE_READ   1
#define PI_FILE_WRITE  2
#define PI_FILE_RW     3
#define PI_FILE_APPEND 4
#define PI_FILE_CREATE 8
#define PI_FILE_TRUNC  16
#define PI_FILE_MAX    31

#define PI_FROM_START   0
#define PI_FROM_CURRENT 1
#define PI_FROM_END     2

#define MAX_CONNECT_ADDRESSES 256

#define PI_MAX_EVENT 31

#define PI_EVENT_BSC 31

double time_time(void);
void time_sleep(double seconds);
char *pigpio_error(int errnum);
unsigned pigpiod_if_version(void);
int pigpio_start(const char *addrStr, const char *portStr);
void pigpio_stop(int pi);
int set_mode(int pi, unsigned gpio, unsigned mode);
int get_mode(int pi, unsigned gpio);
int set_pull_up_down(int pi, unsigned gpio, unsigned pud);
int gpio_read(int pi, unsigned gpio);
int gpio_write(int pi, unsigned gpio, unsigned level);
int set_PWM_dutycycle(int pi, unsigned user_gpio, unsigned dutycycle);
int get_PWM_dutycycle(int pi, unsigned user_gpio);
int set_PWM_range(int pi, unsigned user_gpio, unsigned range);
int get_PWM_range(int pi, unsigned user_gpio);
int get_PWM_real_range(int pi, unsigned user_gpio);
int set_PWM_frequency(int pi, unsigned user_gpio, unsigned frequency);
int get_PWM_frequency(int pi, unsigned user_gpio);
int set_servo_pulsewidth(int pi, unsigned user_gpio, unsigned pulsewidth);
int get_servo_pulsewidth(int pi, unsigned user_gpio);
int notify_open(int pi);
int notify_begin(int pi, unsigned handle, u32 bits);
int notify_pause(int pi, unsigned handle);
int notify_close(int pi, unsigned handle);
int set_watchdog(int pi, unsigned user_gpio, unsigned timeout);
int set_glitch_filter(int pi, unsigned user_gpio, unsigned steady);
int set_noise_filter(
   int pi, unsigned user_gpio, unsigned steady, unsigned active);
u32 read_bank_1(int pi);
u32 read_bank_2(int pi);
int clear_bank_1(int pi, u32 bits);
int clear_bank_2(int pi, u32 bits);
int set_bank_1(int pi, u32 bits);
int set_bank_2(int pi, u32 bits);
int hardware_clock(int pi, unsigned gpio, unsigned clkfreq);
int hardware_PWM(int pi, unsigned gpio, unsigned PWMfreq, u32 PWMduty);
u32 get_current_tick(int pi);
u32 get_hardware_revision(int pi);
u32 get_pigpio_version(int pi);
typedef struct
{
   uint16_t func;
   uint16_t size;
} gpioHeader_t;
typedef struct
{
   size_t size;
   void *ptr;
   u32 data;
} gpioExtent_t;

typedef struct
{
   u32 tick;
   u32 level;
} gpioSample_t;

typedef struct
{
   u16 seqno;
   u16 flags;
   u32 tick;
   u32 level;
} gpioReport_t;

typedef struct
{
   u32 gpioOn;
   u32 gpioOff;
   u32 usDelay;
} gpioPulse_t;
int wave_clear(int pi);
int wave_add_new(int pi);
int wave_add_generic(int pi, unsigned numPulses, gpioPulse_t *pulses);
int wave_add_serial
   (int pi, unsigned user_gpio, unsigned baud, unsigned data_bits,
    unsigned stop_bits, unsigned offset, unsigned numBytes, char *str);
int wave_create(int pi);
int wave_create_and_pad(int pi, int percent);
int wave_delete(int pi, unsigned wave_id);
int wave_send_once(int pi, unsigned wave_id);
int wave_send_repeat(int pi, unsigned wave_id);
int wave_send_using_mode(int pi, unsigned wave_id, unsigned mode);
int wave_chain(int pi, char *buf, unsigned bufSize);
int wave_tx_at(int pi);
int wave_tx_busy(int pi);
int wave_tx_stop(int pi);
int wave_get_micros(int pi);
int wave_get_high_micros(int pi);
int wave_get_max_micros(int pi);
int wave_get_pulses(int pi);
int wave_get_high_pulses(int pi);
int wave_get_max_pulses(int pi);
int wave_get_cbs(int pi);
int wave_get_high_cbs(int pi);
int wave_get_max_cbs(int pi);
int gpio_trigger(int pi, unsigned user_gpio, unsigned pulseLen, unsigned level);
int store_script(int pi, char *script);
int run_script(int pi, unsigned script_id, unsigned numPar, u32 *param);
int update_script(int pi, unsigned script_id, unsigned numPar, u32 *param);
int script_status(int pi, unsigned script_id, u32 *param);
int stop_script(int pi, unsigned script_id);
int delete_script(int pi, unsigned script_id);
int bb_serial_read_open(int pi, unsigned user_gpio, unsigned baud, unsigned data_bits);
int bb_serial_read(int pi, unsigned user_gpio, void *buf, size_t bufSize);
int bb_serial_read_close(int pi, unsigned user_gpio);
int bb_serial_invert(int pi, unsigned user_gpio, unsigned invert);
int i2c_open(int pi, unsigned i2c_bus, unsigned i2c_addr, unsigned i2c_flags);
int i2c_close(int pi, unsigned handle);
int i2c_write_quick(int pi, unsigned handle, unsigned bit);
int i2c_write_byte(int pi, unsigned handle, unsigned bVal);
int i2c_read_byte(int pi, unsigned handle);
int i2c_write_byte_data(
   int pi, unsigned handle, unsigned i2c_reg, unsigned bVal);
int i2c_write_word_data(
   int pi, unsigned handle, unsigned i2c_reg, unsigned wVal);
int i2c_read_byte_data(int pi, unsigned handle, unsigned i2c_reg);
int i2c_read_word_data(int pi, unsigned handle, unsigned i2c_reg);
int i2c_process_call(int pi, unsigned handle, unsigned i2c_reg, unsigned wVal);
int i2c_write_block_data(
   int pi, unsigned handle, unsigned i2c_reg, char *buf, unsigned count);
int i2c_read_block_data(int pi, unsigned handle, unsigned i2c_reg, char *buf);
int i2c_block_process_call(
   int pi, unsigned handle, unsigned i2c_reg, char *buf, unsigned count);
int i2c_read_i2c_block_data(
   int pi, unsigned handle, unsigned i2c_reg, char *buf, unsigned count);

int i2c_write_i2c_block_data(
   int pi, unsigned handle, unsigned i2c_reg, char *buf, unsigned count);
int i2c_read_device(int pi, unsigned handle, char *buf, unsigned count);
int i2c_write_device(int pi, unsigned handle, char *buf, unsigned count);
int i2c_zip(
   int pi,
   unsigned handle,
   char    *inBuf,
   unsigned inLen,
   char    *outBuf,
   unsigned outLen);
int bb_i2c_open(int pi, unsigned SDA, unsigned SCL, unsigned baud);
int bb_i2c_close(int pi, unsigned SDA);
int bb_i2c_zip(
   int pi,
   unsigned SDA,
   char    *inBuf,
   unsigned inLen,
   char    *outBuf,
   unsigned outLen);
int bb_spi_open(
   int pi,
   unsigned CS, unsigned MISO, unsigned MOSI, unsigned SCLK,
   unsigned baud, unsigned spi_flags);
int bb_spi_close(int pi, unsigned CS);
int bb_spi_xfer(
   int pi,
   unsigned CS,
   char    *txBuf,
   char    *rxBuf,
   unsigned count);
int spi_open(int pi, unsigned spi_channel, unsigned baud, unsigned spi_flags);
int spi_close(int pi, unsigned handle);
int spi_read(int pi, unsigned handle, char *buf, unsigned count);
int spi_write(int pi, unsigned handle, char *buf, unsigned count);
int spi_xfer(
   int pi, unsigned handle, char *txBuf, char *rxBuf, unsigned count);
int serial_open(int pi, char *ser_tty, unsigned baud, unsigned ser_flags);
int serial_close(int pi, unsigned handle);
int serial_write_byte(int pi, unsigned handle, unsigned bVal);
int serial_read_byte(int pi, unsigned handle);
int serial_write(int pi, unsigned handle, char *buf, unsigned count);
int serial_read(int pi, unsigned handle, char *buf, unsigned count);
int serial_data_available(int pi, unsigned handle);
int custom_1(int pi, unsigned arg1, unsigned arg2, char *argx, unsigned argc);
int custom_2(int pi, unsigned arg1, char *argx, unsigned argc,
             char *retBuf, unsigned retMax);
int get_pad_strength(int pi, unsigned pad);
int set_pad_strength(int pi, unsigned pad, unsigned padStrength);
int shell_(int pi, char *scriptName, char *scriptString);

typedef enum
{
   pigif_bad_send           = -2000,
   pigif_bad_recv           = -2001,
   pigif_bad_getaddrinfo    = -2002,
   pigif_bad_connect        = -2003,
   pigif_bad_socket         = -2004,
   pigif_bad_noib           = -2005,
   pigif_duplicate_callback = -2006,
   pigif_bad_malloc         = -2007,
   pigif_bad_callback       = -2008,
   pigif_notify_failed      = -2009,
   pigif_callback_not_found = -2010,
   pigif_unconnected_pi     = -2011,
   pigif_too_many_pis       = -2012,
} pigifError_t;

#ifdef __cplusplus
}
#endif
#endif

