#!/usr/bin/env python3
from arduino_interface import ArduinoInterface
import argparse
from stranger_speech import Interepreter, Unrecognized
import os
from stranger_speech import SnowBoyListener



def arg_parser():
    """
    Parses args required for communicating with api
    :return:
    """

    parser = argparse.ArgumentParser(
        description="Provide parameters for interacting with the "
                    "strangerlights api, or strangerarduino interface")
    parser.add_argument('-m', '--model', type=str, required=False,
                        help='The SnowBoy hotword trained model for triggering',
                        default="/home/pi/hey_will.pmdl")
    parser.add_argument('-s', '--serial', type=str, required=False,
                        default=os.getenv('SARDUINO_SERIAL', '/dev/ttyACM0'),
                        help='Serial connection location for '
                             'connected arduino. Defaults to /dev/ttyACM0')

    args = parser.parse_args()
    return args


def main():
    args = arg_parser()
    arduino_comm = ArduinoInterface(args.serial)
    interpreter = Interepreter()
    ghost = Ghost(arduino_comm, interpreter)
    listener = SnowBoyListener(ghost.ghostify, args.model)

    listener.listen()


class Ghost(object):

    def __init__(self, arduino_comm, interpreter):
        self.arduino_comm = arduino_comm
        self.interpreter = interpreter

    def ghostify(self):
        print("working...")
        self.arduino_comm.push_message("thinking")
        try:
            response = self.interpreter.interpret()
        except Unrecognized:
            # default if no text responded
            response = "lightaround"

        self.arduino_comm.push_message(response)


if __name__ == "__main__":
    main()