PERIPHERALS = pit pwm adc
DRIVERS = pacer usb_serial adxl345 panic nrf24 ledtape 
SRC = hat.c adxl345_PWM.c radio_module.c ledtape_utils.c battery_level.c
# Use low level of optimisation to simplify debugging
OPT = -O1

BOARD = hat

include ../../boards/board.mk
