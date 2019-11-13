import gclib
import serial
import time


class MaggieConnection(object):
    def __init__(self):
        self.gc = gclib.py()

    def __enter__(self):
        self.gc.GOpen('10.63.32.199 -d')
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.gc.GClose()

    def kick_handles_and_stop(self):
        self.gc.GCommand('IHT => -3')
        self.gc.GCommand('ST')

    def check_handle(self):
        return self.gc.GCommand('WH')[0:2] == 'IH'


CONNECTION_BYTE = 'C'
STOP_BYTE = 'S'

CONNECTION_PERIOD = 0.5
last_connection_time = 0

stop_counter = 0

with MaggieConnection() as mag:
    with serial.Serial(port='/dev/ttyUSB0') as ser:
        while True:
            current_time = time.time()
            if current_time > last_connection_time + CONNECTION_PERIOD:
                if mag.check_handle():
                    ser.write(CONNECTION_BYTE)
                    last_connection_time = current_time

            while ser.in_waiting:
                in_byte = ser.read()
                if in_byte == 'S':
                    mag.kick_handles_and_stop()
                    stop_counter = stop_counter + 1
                    print 'Stop {}'.format(stop_counter)

