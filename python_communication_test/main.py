#!/usr/bin/python3
""" Main file """

import oca
import time

def main():
    """ Main function """
    oca_hw = oca.OpenCanalyzerHw('/dev/tty.usbmodem401')
    oca_hw.start()

    oca_hw.sync()
    oca_hw.send_message([0x33, 0x34])
    oca_hw.send_message([0x35, 0x36])
    oca_hw.send_message(b"Hello World!")

    time.sleep(2)

    for i in range(oca_hw.get_number_of_messages()):
        print(oca_hw.get_message())

    oca_hw.stop()


if __name__ == "__main__":
  # execute only if run as a script
    main()
