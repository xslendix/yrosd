#include <Drivers/PWMServo.h>

#include <Backend/Backend.h>
#include <Application.h>

namespace Driver {
/*!
 *  @brief  Instantiates a new PCA9685 PWM driver chip with the I2C address on a
 * TwoWire interface
 *  @param  addr The 7-bit I2C address to locate this chip, default is 0x40
 *  @param  i2c  A reference to a 'TwoWire' object that we'll use to communicate
 *  with
 */
PWMServo::PWMServo(const uint8_t addr)
    : m_i2caddr(addr)
{
}

/*!
 *  @brief  Setups the I2C interface and hardware
 *  @param  prescale
 *          Sets External Clock (Optional)
 */
void
PWMServo::begin(uint8_t prescale)
{
  m_i2c_handle = App::the().backend()->OpenI2C(0, m_i2caddr, 0);
  reset();
  if (prescale) {
    setExtClk(prescale);
  } else {
    // set a default frequency
    setPWMFreq(1000);
  }
  // set the default internal frequency
  setOscillatorFrequency(FREQUENCY_OSCILLATOR);
}

void
PWMServo::end()
{
  App::the().backend()->CloseI2C(m_i2c_handle);
  m_i2c_handle = -1;
}

/*!
 *  @brief  Sends a reset command to the PCA9685 chip over I2C
 */
void
PWMServo::reset()
{
  write8(PCA9685_MODE1, MODE1_RESTART);
  usleep(10);
}

/*!
 *  @brief  Puts board into sleep mode
 */
void
PWMServo::sleep()
{
  uint8_t awake = read8(PCA9685_MODE1);
  uint8_t sleep = awake | MODE1_SLEEP; // set sleep bit high
  write8(PCA9685_MODE1, sleep);
  usleep(5); // wait until cycle ends for sleep to be active
}

/*!
 *  @brief  Wakes board from sleep
 */
void
PWMServo::wakeup()
{
  uint8_t sleep = read8(PCA9685_MODE1);
  uint8_t wakeup = sleep & ~MODE1_SLEEP; // set sleep bit low
  write8(PCA9685_MODE1, wakeup);
}

/*!
 *  @brief  Sets EXTCLK pin to use the external clock
 *  @param  prescale
 *          Configures the prescale value to be used by the external clock
 */
void
PWMServo::setExtClk(uint8_t prescale)
{
  uint8_t oldmode = read8(PCA9685_MODE1);
  uint8_t newmode = (oldmode & ~MODE1_RESTART) | MODE1_SLEEP; // sleep
  write8(PCA9685_MODE1, newmode);                             // go to sleep, turn off internal oscillator

  // This sets both the SLEEP and EXTCLK bits of the MODE1 register to switch to
  // use the external clock.
  write8(PCA9685_MODE1, (newmode |= MODE1_EXTCLK));

  write8(PCA9685_PRESCALE, prescale); // set the prescaler

  usleep(5);
  // clear the SLEEP bit to start
  write8(PCA9685_MODE1, (newmode & ~MODE1_SLEEP) | MODE1_RESTART | MODE1_AI);
}

/*!
 *  @brief  Sets the PWM frequency for the entire chip, up to ~1.6 KHz
 *  @param  freq Floating point frequency that we will attempt to match
 */
void
PWMServo::setPWMFreq(float freq)
{
  // Range output modulation frequency is dependant on oscillator
  if (freq < 1)
    freq = 1;
  if (freq > 3500)
    freq = 3500; // Datasheet limit is 3052=50MHz/(4*4096)

  float prescaleval = ((m_oscillator_freq / (freq * 4096.0)) + 0.5) - 1;
  if (prescaleval < PCA9685_PRESCALE_MIN)
    prescaleval = PCA9685_PRESCALE_MIN;
  if (prescaleval > PCA9685_PRESCALE_MAX)
    prescaleval = PCA9685_PRESCALE_MAX;
  uint8_t prescale = (uint8_t)prescaleval;

  uint8_t oldmode = read8(PCA9685_MODE1);
  uint8_t newmode = (oldmode & ~MODE1_RESTART) | MODE1_SLEEP; // sleep
  write8(PCA9685_MODE1, newmode);                             // go to sleep
  write8(PCA9685_PRESCALE, prescale);                         // set the prescaler
  write8(PCA9685_MODE1, oldmode);
  usleep(5);
  // This sets the MODE1 register to turn on auto increment.
  write8(PCA9685_MODE1, oldmode | MODE1_RESTART | MODE1_AI);
}

/*!
 *  @brief  Sets the output mode of the PCA9685 to either
 *  open drain or push pull / totempole.
 *  Warning: LEDs with integrated zener diodes should
 *  only be driven in open drain mode.
 *  @param  totempole Totempole if true, open drain if false.
 */
void
PWMServo::setOutputMode(bool totempole)
{
  uint8_t oldmode = read8(PCA9685_MODE2);
  uint8_t newmode;
  if (totempole) {
    newmode = oldmode | MODE2_OUTDRV;
  } else {
    newmode = oldmode & ~MODE2_OUTDRV;
  }
  write8(PCA9685_MODE2, newmode);
}

/*!
 *  @brief  Reads set Prescale from PCA9685
 *  @return prescale value
 */
uint8_t
PWMServo::readPrescale(void)
{
  return read8(PCA9685_PRESCALE);
}

/*!
 *  @brief  Gets the PWM output of one of the PCA9685 pins
 *  @param  num One of the PWM output pins, from 0 to 15
 *  @return requested PWM output value
 */
uint8_t
PWMServo::getPWM(uint8_t num)
{
  return App::the().backend()->I2CReadByteData(m_i2c_handle, PCA9685_LED0_ON_L + 4 * num);
}

/*!
 *  @brief  Sets the PWM output of one of the PCA9685 pins
 *  @param  num One of the PWM output pins, from 0 to 15
 *  @param  on At what point in the 4096-part cycle to turn the PWM output ON
 *  @param  off At what point in the 4096-part cycle to turn the PWM output OFF
 */
void
PWMServo::setPWM(uint8_t num, uint16_t on, uint16_t off)
{
  App::the().backend()->I2CWriteByte(m_i2c_handle, PCA9685_LED0_ON_L + 4 * num);
  App::the().backend()->I2CWriteByte(m_i2c_handle, on);
  App::the().backend()->I2CWriteByte(m_i2c_handle, on >> 8);
  App::the().backend()->I2CWriteByte(m_i2c_handle, off);
  App::the().backend()->I2CWriteByte(m_i2c_handle, off >> 8);
}

/*!
 *   @brief  Helper to set pin PWM output. Sets pin without having to deal with
 * on/off tick placement and properly handles a zero value as completely off and
 * 4095 as completely on.  Optional invert parameter supports inverting the
 * pulse for sinking to ground.
 *   @param  num One of the PWM output pins, from 0 to 15
 *   @param  val The number of ticks out of 4096 to be active, should be a value
 * from 0 to 4095 inclusive.
 *   @param  invert If true, inverts the output, defaults to 'false'
 */
void
PWMServo::setPin(uint8_t num, uint16_t val, bool invert)
{
  // Clamp value between 0 and 4095 inclusive.
  val = min(val, (uint16_t)4095);
  if (invert) {
    if (val == 0) {
      // Special value for signal fully on.
      setPWM(num, 4096, 0);
    } else if (val == 4095) {
      // Special value for signal fully off.
      setPWM(num, 0, 4096);
    } else {
      setPWM(num, 0, 4095 - val);
    }
  } else {
    if (val == 4095) {
      // Special value for signal fully on.
      setPWM(num, 4096, 0);
    } else if (val == 0) {
      // Special value for signal fully off.
      setPWM(num, 0, 4096);
    } else {
      setPWM(num, 0, val);
    }
  }
}

/*!
 *  @brief  Sets the PWM output of one of the PCA9685 pins based on the input
 * microseconds, output is not precise
 *  @param  num One of the PWM output pins, from 0 to 15
 *  @param  Microseconds The number of Microseconds to turn the PWM output ON
 */
void
PWMServo::writeMicroseconds(uint8_t num, uint16_t Microseconds)
{
  double pulse = Microseconds;
  double pulselength;
  pulselength = 1000000; // 1,000,000 us per second

  // Read prescale
  uint16_t prescale = readPrescale();

  // Calculate the pulse for PWM based on Equation 1 from the datasheet section
  // 7.3.5
  prescale += 1;
  pulselength *= prescale;
  pulselength /= m_oscillator_freq;

  pulse /= pulselength;

  setPWM(num, 0, pulse);
}

/*!
 *  @brief  Getter for the internally tracked oscillator used for freq
 * calculations
 *  @returns The frequency the PCA9685 thinks it is running at (it cannot
 * introspect)
 */
uint32_t
PWMServo::getOscillatorFrequency(void)
{
  return m_oscillator_freq;
}

/*!
 *  @brief Setter for the internally tracked oscillator used for freq
 * calculations
 *  @param freq The frequency the PCA9685 should use for frequency calculations
 */
void
PWMServo::setOscillatorFrequency(uint32_t freq)
{
  m_oscillator_freq = freq;
}

/******************* Low level I2C interface */
u8
PWMServo::read8(u8 addr)
{
  return App::the().backend()->I2CReadByteData(m_i2c_handle, addr);
}

void
PWMServo::write8(u8 addr, u8 d)
{
  App::the().backend()->I2CWriteByteData(m_i2c_handle, addr, d);
}

}
