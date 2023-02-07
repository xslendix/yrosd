#ifndef PIGPIOD_IF2_H
#define PIGPIOD_IF2_H
#include <stddef.h>
#define PIGPIOD_IF2_VERSION 17
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

#define PI_MIN_GPIO 0
#define PI_MAX_GPIO 53

#define PI_MAX_USER_GPIO 31

#define PI_OFF 0
#define PI_ON  1

#define PI_CLEAR 0
#define PI_SET   1

#define PI_LOW  0
#define PI_HIGH 1

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

#define PI_DEFAULT_DUTYCYCLE_RANGE 255

#define PI_MIN_DUTYCYCLE_RANGE 25
#define PI_MAX_DUTYCYCLE_RANGE 40000

#define PI_SERVO_OFF            0
#define PI_MIN_SERVO_PULSEWIDTH 500
#define PI_MAX_SERVO_PULSEWIDTH 2500

#define PI_HW_PWM_MIN_FREQ      1
#define PI_HW_PWM_MAX_FREQ      125000000
#define PI_HW_PWM_MAX_FREQ_2711 187500000
#define PI_HW_PWM_RANGE         1000000

#define PI_HW_CLK_MIN_FREQ      4689
#define PI_HW_CLK_MIN_FREQ_2711 13184
#define PI_HW_CLK_MAX_FREQ      250000000
#define PI_HW_CLK_MAX_FREQ_2711 375000000

#define PI_NOTIFY_SLOTS 32

#define PI_NTFY_FLAGS_EVENT  (1 << 7)
#define PI_NTFY_FLAGS_ALIVE  (1 << 6)
#define PI_NTFY_FLAGS_WDOG   (1 << 5)
#define PI_NTFY_FLAGS_BIT(x) (((x) << 0) & 31)

#define PI_WAVE_BLOCKS     4
#define PI_WAVE_MAX_PULSES (PI_WAVE_BLOCKS * 3000)
#define PI_WAVE_MAX_CHARS  (PI_WAVE_BLOCKS * 300)

#define PI_BB_I2C_MIN_BAUD 50
#define PI_BB_I2C_MAX_BAUD 500000

#define PI_BB_SPI_MIN_BAUD 50
#define PI_BB_SPI_MAX_BAUD 250000

#define PI_BB_SER_MIN_BAUD 50
#define PI_BB_SER_MAX_BAUD 250000

#define PI_BB_SER_NORMAL 0
#define PI_BB_SER_INVERT 1

#define PI_WAVE_MIN_BAUD 50
#define PI_WAVE_MAX_BAUD 1000000

#define PI_SPI_MIN_BAUD 32000
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

#define PI_WAVE_NOT_FOUND 9998 /* Transmitted wave not found. */
#define PI_NO_TX_WAVE     9999 /* No wave being transmitted. */

#define PI_FILE_SLOTS 16
#define PI_I2C_SLOTS  512
#define PI_SPI_SLOTS  32
#define PI_SER_SLOTS  16

#define PI_MAX_I2C_ADDR 0x7F

#define PI_NUM_AUX_SPI_CHANNEL 3
#define PI_NUM_STD_SPI_CHANNEL 2

#define PI_MAX_I2C_DEVICE_COUNT (1 << 16)
#define PI_MAX_SPI_DEVICE_COUNT (1 << 16)

#define PI_I2C_RDRW_IOCTL_MAX_MSGS 42

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

#define PI_SPI_FLAGS_BITLEN(x)  ((x & 63) << 16)
#define PI_SPI_FLAGS_RX_LSB(x)  ((x & 1) << 15)
#define PI_SPI_FLAGS_TX_LSB(x)  ((x & 1) << 14)
#define PI_SPI_FLAGS_3WREN(x)   ((x & 15) << 10)
#define PI_SPI_FLAGS_3WIRE(x)   ((x & 1) << 9)
#define PI_SPI_FLAGS_AUX_SPI(x) ((x & 1) << 8)
#define PI_SPI_FLAGS_RESVD(x)   ((x & 7) << 5)
#define PI_SPI_FLAGS_CSPOLS(x)  ((x & 7) << 2)
#define PI_SPI_FLAGS_MODE(x)    ((x & 3))

#define BSC_DR        0
#define BSC_RSR       1
#define BSC_SLV       2
#define BSC_CR        3
#define BSC_FR        4
#define BSC_IFLS      5
#define BSC_IMSC      6
#define BSC_RIS       7
#define BSC_MIS       8
#define BSC_ICR       9
#define BSC_DMACR     10
#define BSC_TDR       11
#define BSC_GPUSTAT   12
#define BSC_HCTRL     13
#define BSC_DEBUG_I2C 14
#define BSC_DEBUG_SPI 15

#define BSC_CR_TESTFIFO 2048
#define BSC_CR_RXE      512
#define BSC_CR_TXE      256
#define BSC_CR_BRK      128
#define BSC_CR_CPOL     16
#define BSC_CR_CPHA     8
#define BSC_CR_I2C      4
#define BSC_CR_SPI      2
#define BSC_CR_EN       1

#define BSC_FR_RXBUSY 32
#define BSC_FR_TXFE   16
#define BSC_FR_RXFF   8
#define BSC_FR_TXFF   4
#define BSC_FR_RXFE   2
#define BSC_FR_TXBUSY 1

#define BSC_SDA      18
#define BSC_MOSI     20
#define BSC_SCL_SCLK 19
#define BSC_MISO     18
#define BSC_CE_N     21

#define BSC_SDA_2711      10
#define BSC_MOSI_2711     9
#define BSC_SCL_SCLK_2711 11
#define BSC_MISO_2711     10
#define BSC_CE_N_2711     8

#define PI_MAX_BUSY_DELAY 100

#define PI_MIN_WDOG_TIMEOUT 0
#define PI_MAX_WDOG_TIMEOUT 60000

#define PI_MIN_TIMER 0
#define PI_MAX_TIMER 9

#define PI_MIN_MS 10
#define PI_MAX_MS 60000

#define PI_MAX_SCRIPTS 32

#define PI_MAX_SCRIPT_TAGS   50
#define PI_MAX_SCRIPT_VARS   150
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

#define PI_MAX_STEADY 300000
#define PI_MAX_ACTIVE 1000000

#define PI_CFG_DBG_LEVEL    0 /* bits 0-3 */
#define PI_CFG_ALERT_FREQ   4 /* bits 4-7 */
#define PI_CFG_RT_PRIORITY  (1 << 8)
#define PI_CFG_STATS        (1 << 9)
#define PI_CFG_NOSIGHANDLER (1 << 10)

#define PI_CFG_ILLEGAL_VAL (1 << 11)

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
char *pigpio_error(i32 errnum);
unsigned pigpiod_if_version(void);
i32 pigpio_start(const char *addrStr, const char *portStr);
void pigpio_stop(i32 pi);
i32 set_mode(i32 pi, unsigned gpio, unsigned mode);
i32 get_mode(i32 pi, unsigned gpio);
i32 set_pull_up_down(i32 pi, unsigned gpio, unsigned pud);
i32 gpio_read(i32 pi, unsigned gpio);
i32 gpio_write(i32 pi, unsigned gpio, unsigned level);
i32 set_PWM_dutycycle(i32 pi, unsigned user_gpio, unsigned dutycycle);
i32 get_PWM_dutycycle(i32 pi, unsigned user_gpio);
i32 set_PWM_range(i32 pi, unsigned user_gpio, unsigned range);
i32 get_PWM_range(i32 pi, unsigned user_gpio);
i32 get_PWM_real_range(i32 pi, unsigned user_gpio);
i32 set_PWM_frequency(i32 pi, unsigned user_gpio, unsigned frequency);
i32 get_PWM_frequency(i32 pi, unsigned user_gpio);
i32 set_servo_pulsewidth(i32 pi, unsigned user_gpio, unsigned pulsewidth);
i32 get_servo_pulsewidth(i32 pi, unsigned user_gpio);
i32 notify_open(i32 pi);
i32 notify_begin(i32 pi, unsigned handle, u32 bits);
i32 notify_pause(i32 pi, unsigned handle);
i32 notify_close(i32 pi, unsigned handle);
i32 set_watchdog(i32 pi, unsigned user_gpio, unsigned timeout);
i32 set_glitch_filter(i32 pi, unsigned user_gpio, unsigned steady);
i32 set_noise_filter(i32 pi, unsigned user_gpio, unsigned steady,
                     unsigned active);
u32 read_bank_1(i32 pi);
u32 read_bank_2(i32 pi);
i32 clear_bank_1(i32 pi, u32 bits);
i32 clear_bank_2(i32 pi, u32 bits);
i32 set_bank_1(i32 pi, u32 bits);
i32 set_bank_2(i32 pi, u32 bits);
i32 hardware_clock(i32 pi, unsigned gpio, unsigned clkfreq);
i32 hardware_PWM(i32 pi, unsigned gpio, unsigned PWMfreq, u32 PWMduty);
u32 get_current_tick(i32 pi);
u32 get_hardware_revision(i32 pi);
u32 get_pigpio_version(i32 pi);
typedef struct {
  u16 func;
  u16 size;
} gpioHeader_t;
typedef struct {
  size_t size;
  void *ptr;
  u32 data;
} gpioExtent_t;

typedef struct {
  u32 tick;
  u32 level;
} gpioSample_t;

typedef struct {
  u16 seqno;
  u16 flags;
  u32 tick;
  u32 level;
} gpioReport_t;

typedef struct {
  u32 gpioOn;
  u32 gpioOff;
  u32 usDelay;
} gpioPulse_t;
i32 wave_clear(i32 pi);
i32 wave_add_new(i32 pi);
i32 wave_add_generic(i32 pi, unsigned numPulses, gpioPulse_t *pulses);
i32 wave_add_serial(i32 pi, unsigned user_gpio, unsigned baud,
                    unsigned data_bits, unsigned stop_bits, unsigned offset,
                    unsigned numBytes, char *str);
i32 wave_create(i32 pi);
i32 wave_create_and_pad(i32 pi, i32 percent);
i32 wave_delete(i32 pi, unsigned wave_id);
i32 wave_send_once(i32 pi, unsigned wave_id);
i32 wave_send_repeat(i32 pi, unsigned wave_id);
i32 wave_send_using_mode(i32 pi, unsigned wave_id, unsigned mode);
i32 wave_chain(i32 pi, char *buf, unsigned bufSize);
i32 wave_tx_at(i32 pi);
i32 wave_tx_busy(i32 pi);
i32 wave_tx_stop(i32 pi);
i32 wave_get_micros(i32 pi);
i32 wave_get_high_micros(i32 pi);
i32 wave_get_max_micros(i32 pi);
i32 wave_get_pulses(i32 pi);
i32 wave_get_high_pulses(i32 pi);
i32 wave_get_max_pulses(i32 pi);
i32 wave_get_cbs(i32 pi);
i32 wave_get_high_cbs(i32 pi);
i32 wave_get_max_cbs(i32 pi);
i32 gpio_trigger(i32 pi, unsigned user_gpio, unsigned pulseLen, unsigned level);
i32 store_script(i32 pi, char *script);
i32 run_script(i32 pi, unsigned script_id, unsigned numPar, u32 *param);
i32 update_script(i32 pi, unsigned script_id, unsigned numPar, u32 *param);
i32 script_status(i32 pi, unsigned script_id, u32 *param);
i32 stop_script(i32 pi, unsigned script_id);
i32 delete_script(i32 pi, unsigned script_id);
i32 bb_serial_read_open(i32 pi, unsigned user_gpio, unsigned baud,
                        unsigned data_bits);
i32 bb_serial_read(i32 pi, unsigned user_gpio, void *buf, size_t bufSize);
i32 bb_serial_read_close(i32 pi, unsigned user_gpio);
i32 bb_serial_invert(i32 pi, unsigned user_gpio, unsigned invert);
i32 i2c_open(i32 pi, unsigned i2c_bus, unsigned i2c_addr, unsigned i2c_flags);
i32 i2c_close(i32 pi, unsigned handle);
i32 i2c_write_quick(i32 pi, unsigned handle, unsigned bit);
i32 i2c_write_byte(i32 pi, unsigned handle, unsigned bVal);
i32 i2c_read_byte(i32 pi, unsigned handle);
i32 i2c_write_byte_data(i32 pi, unsigned handle, unsigned i2c_reg,
                        unsigned bVal);
i32 i2c_write_word_data(i32 pi, unsigned handle, unsigned i2c_reg,
                        unsigned wVal);
i32 i2c_read_byte_data(i32 pi, unsigned handle, unsigned i2c_reg);
i32 i2c_read_word_data(i32 pi, unsigned handle, unsigned i2c_reg);
i32 i2c_process_call(i32 pi, unsigned handle, unsigned i2c_reg, unsigned wVal);
i32 i2c_write_block_data(i32 pi, unsigned handle, unsigned i2c_reg, char *buf,
                         unsigned count);
i32 i2c_read_block_data(i32 pi, unsigned handle, unsigned i2c_reg, char *buf);
i32 i2c_block_process_call(i32 pi, unsigned handle, unsigned i2c_reg, char *buf,
                           unsigned count);
i32 i2c_read_i2c_block_data(i32 pi, unsigned handle, unsigned i2c_reg,
                            char *buf, unsigned count);

i32 i2c_write_i2c_block_data(i32 pi, unsigned handle, unsigned i2c_reg,
                             char *buf, unsigned count);
i32 i2c_read_device(i32 pi, unsigned handle, char *buf, unsigned count);
i32 i2c_write_device(i32 pi, unsigned handle, char *buf, unsigned count);
i32 i2c_zip(i32 pi, unsigned handle, char *inBuf, unsigned inLen, char *outBuf,
            unsigned outLen);
i32 bb_i2c_open(i32 pi, unsigned SDA, unsigned SCL, unsigned baud);
i32 bb_i2c_close(i32 pi, unsigned SDA);
i32 bb_i2c_zip(i32 pi, unsigned SDA, char *inBuf, unsigned inLen, char *outBuf,
               unsigned outLen);
i32 bb_spi_open(i32 pi, unsigned CS, unsigned MISO, unsigned MOSI,
                unsigned SCLK, unsigned baud, unsigned spi_flags);
i32 bb_spi_close(i32 pi, unsigned CS);
i32 bb_spi_xfer(i32 pi, unsigned CS, char *txBuf, char *rxBuf, unsigned count);
i32 spi_open(i32 pi, unsigned spi_channel, unsigned baud, unsigned spi_flags);
i32 spi_close(i32 pi, unsigned handle);
i32 spi_read(i32 pi, unsigned handle, char *buf, unsigned count);
i32 spi_write(i32 pi, unsigned handle, char *buf, unsigned count);
i32 spi_xfer(i32 pi, unsigned handle, char *txBuf, char *rxBuf, unsigned count);
i32 serial_open(i32 pi, char *ser_tty, unsigned baud, unsigned ser_flags);
i32 serial_close(i32 pi, unsigned handle);
i32 serial_write_byte(i32 pi, unsigned handle, unsigned bVal);
i32 serial_read_byte(i32 pi, unsigned handle);
i32 serial_write(i32 pi, unsigned handle, char *buf, unsigned count);
i32 serial_read(i32 pi, unsigned handle, char *buf, unsigned count);
i32 serial_data_available(i32 pi, unsigned handle);
i32 custom_1(i32 pi, unsigned arg1, unsigned arg2, char *argx, unsigned argc);
i32 custom_2(i32 pi, unsigned arg1, char *argx, unsigned argc, char *retBuf,
             unsigned retMax);
i32 get_pad_strength(i32 pi, unsigned pad);
i32 set_pad_strength(i32 pi, unsigned pad, unsigned padStrength);
i32 shell_(i32 pi, char *scriptName, char *scriptString);

typedef enum {
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
