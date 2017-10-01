#!/usr/bin/python3
"""Oca comm test library"""

import threading
import serial

class OpenCanalyzer():
    """ Abstraction from the hardware """
    def __init__(self):
        pass

class OpenCanalyzerHw():
    """ Internal class for communicating with the hardware"""
    def __init__(self, port):
        self.rxt = threading.Thread(target=self.rx_thread)
        self.serial_port = serial.Serial()
        self.serial_port.baudrate = 19200
        self.serial_port.port = port
        self.serial_port.timeout = 1.0
        self.stop_thread = False
        self.rx_char_buffer = []
        self.rx_message_buffer = []

    def start(self):
        self.serial_port.open()
        self.stop_thread = False
        self.rxt.start()

    def stop(self):
        self.stop_thread = True
        self.rxt.join()
        self.serial_port.close()

    def sync(self):
        for i in range(3):
            self.serial_port.write([0x55, 0xDA, 0x00])

    def send_message(self, data):
        length = len(data)
        self.serial_port.write([0x55, 0xDA, length])
        self.serial_port.write(data)

    def get_message(self):
        copy = self.rx_message_buffer[0]
        del self.rx_message_buffer[0]
        return copy

    def get_number_of_messages(self):
        return len(self.rx_message_buffer)

    def stream_to_data_frames(self):
        start_found = False
        frame_size = 0
        frame_location = 0

        length = len(self.rx_char_buffer)
        if length < 3:
            return None

        for i in range(length):
            if length - i >= 3:
                if self.rx_char_buffer[i] == 0x55 and self.rx_char_buffer[i+1] == 0xDA:
                    start_found = True
                    frame_size = self.rx_char_buffer[i+2]
                    frame_location = i+3
                    break

        if start_found:
            message = ''.join(chr(i) for i in self.rx_char_buffer[frame_location:frame_location+frame_size])
            del self.rx_char_buffer[0:frame_location+frame_size]
            return message

        return None

    def rx_thread(self):
        while not self.stop_thread:
            data = self.serial_port.read(self.serial_port.in_waiting or 1)
            if data:
                self.rx_char_buffer.extend(list(data))
                message = self.stream_to_data_frames()
                if message:
                    self.rx_message_buffer.append(message)
                    print(self.rx_message_buffer)

if __name__ == "__main__":
    # execute only if run as a script
    raise NotImplementedError('This is not the main file Pannekoek')
