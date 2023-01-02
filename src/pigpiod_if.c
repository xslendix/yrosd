#include "pigpiod_if.h"

#ifdef __cplusplus
extern "C" {
#endif

double time_time(void){return 0;}
void time_sleep(double seconds){}
char *pigpio_error(int errnum){return 0;}
unsigned pigpiod_if_version(void){return 0;}
int pigpio_start(char *addrStr, char *portStr){return 0;}
void pigpio_stop(void){}
int set_mode(unsigned gpio, unsigned mode){return 0;}
int get_mode(unsigned gpio){return 0;}
int set_pull_up_down(unsigned gpio, unsigned pud){return 0;}
int gpio_read(unsigned gpio){return 0;}
int gpio_write(unsigned gpio, unsigned level){return 0;}
int set_PWM_dutycycle(unsigned user_gpio, unsigned dutycycle){return 0;}
int get_PWM_dutycycle(unsigned user_gpio){return 0;}
int set_PWM_range(unsigned user_gpio, unsigned range){return 0;}
int get_PWM_range(unsigned user_gpio){return 0;}
int get_PWM_real_range(unsigned user_gpio){return 0;}
int set_PWM_frequency(unsigned user_gpio, unsigned frequency){return 0;}
int get_PWM_frequency(unsigned user_gpio){return 0;}
int set_servo_pulsewidth(unsigned user_gpio, unsigned pulsewidth){return 0;}
int get_servo_pulsewidth(unsigned user_gpio){return 0;}
int notify_open(void){return 0;}
int notify_begin(unsigned handle, uint32_t bits){return 0;}
int notify_pause(unsigned handle){return 0;}
int notify_close(unsigned handle){return 0;}
int set_watchdog(unsigned user_gpio, unsigned timeout){return 0;}
int set_glitch_filter(unsigned user_gpio, unsigned steady){return 0;}
int set_noise_filter(unsigned user_gpio, unsigned steady, unsigned active){return 0;}
uint32_t read_bank_1(void){return 0;}
uint32_t read_bank_2(void){return 0;}
int clear_bank_1(uint32_t bits){return 0;}
int clear_bank_2(uint32_t bits){return 0;}
int set_bank_1(uint32_t bits){return 0;}
int set_bank_2(uint32_t bits){return 0;}
int hardware_clock(unsigned gpio, unsigned clkfreq){return 0;}
int hardware_PWM(unsigned gpio, unsigned PWMfreq, uint32_t PWMduty){return 0;}
uint32_t get_current_tick(void){return 0;}
uint32_t get_hardware_revision(void){return 0;}
uint32_t get_pigpio_version(void){return 0;}
int wave_clear(void){return 0;}
int wave_add_new(void){return 0;}
int wave_add_serial
   (unsigned user_gpio, unsigned baud, unsigned data_bits,
    unsigned stop_bits, unsigned offset, unsigned numBytes, char *str){return 0;}
int wave_create(void){return 0;}
int wave_delete(unsigned wave_id){return 0;}
int wave_send_once(unsigned wave_id){return 0;}
int wave_send_repeat(unsigned wave_id){return 0;}
int wave_chain(char *buf, unsigned bufSize){return 0;}
int wave_tx_busy(void){return 0;}
int wave_tx_stop(void){return 0;}
int wave_get_micros(void){return 0;}
int wave_get_high_micros(void){return 0;}
int wave_get_max_micros(void){return 0;}
int wave_get_pulses(void){return 0;}
int wave_get_high_pulses(void){return 0;}
int wave_get_max_pulses(void){return 0;}
int wave_get_cbs(void){return 0;}
int wave_get_high_cbs(void){return 0;}
int wave_get_max_cbs(void){return 0;}
int gpio_trigger(unsigned user_gpio, unsigned pulseLen, unsigned level){return 0;}
int store_script(char *script){return 0;}
int run_script(unsigned script_id, unsigned numPar, uint32_t *param){return 0;}
int script_status(unsigned script_id, uint32_t *param){return 0;}
int stop_script(unsigned script_id){return 0;}
int delete_script(unsigned script_id){return 0;}
int bb_serial_read_open(unsigned user_gpio, unsigned baud, unsigned data_bits){return 0;}
int bb_serial_read(unsigned user_gpio, void *buf, size_t bufSize){return 0;}
int bb_serial_read_close(unsigned user_gpio){return 0;}
int bb_serial_invert(unsigned user_gpio, unsigned invert){return 0;}
int i2c_open(unsigned i2c_bus, unsigned i2c_addr, unsigned i2c_flags){return 0;}
int i2c_close(unsigned handle){return 0;}
int i2c_write_quick(unsigned handle, unsigned bit){return 0;}
int i2c_write_byte(unsigned handle, unsigned bVal){return 0;}
int i2c_read_byte(unsigned handle){return 0;}
int i2c_write_byte_data(unsigned handle, unsigned i2c_reg, unsigned bVal){return 0;}
int i2c_write_word_data(unsigned handle, unsigned i2c_reg, unsigned wVal){return 0;}
int i2c_read_byte_data(unsigned handle, unsigned i2c_reg){return 0;}
int i2c_read_word_data(unsigned handle, unsigned i2c_reg){return 0;}
int i2c_process_call(unsigned handle, unsigned i2c_reg, unsigned wVal){return 0;}
int i2c_write_block_data(
unsigned handle, unsigned i2c_reg, char *buf, unsigned count){return 0;}
int i2c_read_block_data(unsigned handle, unsigned i2c_reg, char *buf){return 0;}
int i2c_block_process_call(
unsigned handle, unsigned i2c_reg, char *buf, unsigned count){return 0;}
int i2c_read_i2c_block_data(
unsigned handle, unsigned i2c_reg, char *buf, unsigned count){return 0;}
int i2c_write_i2c_block_data(
unsigned handle, unsigned i2c_reg, char *buf, unsigned count){return 0;}
int i2c_read_device(unsigned handle, char *buf, unsigned count){return 0;}
int i2c_write_device(unsigned handle, char *buf, unsigned count){return 0;}
int bb_i2c_open(unsigned SDA, unsigned SCL, unsigned baud){return 0;}
int bb_i2c_close(unsigned SDA){return 0;}
int spi_open(unsigned spi_channel, unsigned baud, unsigned spi_flags){return 0;}
int spi_close(unsigned handle){return 0;}
int spi_read(unsigned handle, char *buf, unsigned count){return 0;}
int spi_write(unsigned handle, char *buf, unsigned count){return 0;}
int spi_xfer(unsigned handle, char *txBuf, char *rxBuf, unsigned count){return 0;}
int serial_open(char *ser_tty, unsigned baud, unsigned ser_flags){return 0;}
int serial_close(unsigned handle){return 0;}
int serial_write_byte(unsigned handle, unsigned bVal){return 0;}
int serial_read_byte(unsigned handle){return 0;}
int serial_write(unsigned handle, char *buf, unsigned count){return 0;}
int serial_read(unsigned handle, char *buf, unsigned count){return 0;}
int serial_data_available(unsigned handle){return 0;}
int custom_1(unsigned arg1, unsigned arg2, char *argx, unsigned argc){return 0;}
int custom_2(unsigned arg1, char *argx, unsigned argc,
             char *retBuf, unsigned retMax){return 0;}
int callback(unsigned user_gpio, unsigned edge, CBFunc_t f){return 0;}
int callback_ex
   (unsigned user_gpio, unsigned edge, CBFuncEx_t f, void *userdata){return 0;}
int callback_cancel(unsigned callback_id){return 0;}
int wait_for_edge(unsigned user_gpio, unsigned edge, double timeout){return 0;}
#ifdef __cplusplus
}
#endif

