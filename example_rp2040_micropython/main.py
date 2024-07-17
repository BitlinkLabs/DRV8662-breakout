import math
import time

import rp2
from machine import Pin, PWM

PWM_FREQ = 125_000
PWM_PLUS_PIN = 9
EN_PIN = 12

U16_MAX = 0xFFFF

pwm_plus = PWM(Pin(PWM_PLUS_PIN))
pwm_plus.init(freq=PWM_FREQ, duty_u16=int(U16_MAX * 0.5))

en = Pin(EN_PIN, Pin.OUT)


def reset_drv():
    en.value(0)
    set_output(0)
    time.sleep(0.1)
    en.value(1)
    time.sleep(0.1)


def set_output(out: float):
    """
    :param out: -1 to 1
    """
    pwm_plus.duty_u16(max(0, min(U16_MAX, int((out / 2 + 0.5) * U16_MAX))))


def wait_for_button_press():
    while rp2.bootsel_button() == 0:
        pass


def test_sine_signal(duration, frequency):
    reset_drv()

    start_time = time.ticks_us()

    dt = 0
    while dt < duration:
        dt = time.ticks_diff(time.ticks_us(), start_time) / 1000_000.0
        set_output(math.sin(dt * 2 * math.pi * frequency))

    en.value(0)


print("0")
wait_for_button_press()

time.sleep(1)

while True:
    wait_for_button_press()
    test_sine_signal(1, 100)
