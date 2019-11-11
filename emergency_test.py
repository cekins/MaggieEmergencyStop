import serial
import time


CONNECTION_BYTE = 'C'
STOP_BYTE = 'S'

CONNECTION_PERIOD = 0.5
last_connection_time = 0

with serial.Serial(port='/dev/ttyUSB0') as ser:
    while True:
        current_time = time.time()
        if current_time > last_connection_time + CONNECTION_PERIOD:
            ser.write(CONNECTION_BYTE)
            last_connection_time = current_time

        while ser.in_waiting:
            in_byte = ser.read()
            if in_byte == 'S':
                print "STOPPED"

