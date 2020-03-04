import time
from pyb import UART

uart = UART(3, 19200)

while(True):
    a = [0,1,2]
    b = str(a)
    print(b)
    uart.write(b)
    time.sleep(1000)
