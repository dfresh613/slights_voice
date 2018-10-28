from aiy import cloudspeech
import aiy.audio
from snowboy import snowboydecoder
import signal
interrupted = False


def signal_handler(signal, frame):
    global interrupted
    interrupted = True


def interrupt_callback():
    global interrupted
    return interrupted


class Unrecognized(Exception):
    pass


class Interepreter(object):
    recognizer = cloudspeech.get_recognizer()

    def __init__(self):
        self.failed_recognizes = 0
        self._init_expected_phrases()
        print("google recognizer intialized")

    def _init_expected_phrases(self):
        self.recognizer.expect_phrase('hey will')
        self.recognizer.expect_phrase('where are you')
        self.recognizer.expect_phrase('sound')
        self.recognizer.expect_phrase('christmas')

    def interpret(self):
        failed_recognizes = 0
        in_text = ""

        while failed_recognizes <= 3 and not in_text:
            recorder = aiy.audio.get_recorder()
            recorder.start()

            try:
                print("waiting for question")
                in_text = self.recognizer.recognize()
                if not in_text:
                    raise Unrecognized("no text found")
            except Exception as e:
                failed_recognizes += 1
                print("failed recognizing: {}".format(e))
                recorder.stop()
                aiy.audio._voicehat_recorder = None

        if not in_text:
            raise Unrecognized("No input text found after 3 tries")
        print("<< {}".format(in_text))
        return self.respond(in_text)

    def respond(self, text):
        if text == "where are you":
            return "im right here"
        elif text == "sound":
            aiy.audio.play_wave('sounds/deeplaugh.wav')
            aiy.audio._voicehat_recorder = None
        elif text == "christmas":
            return "christmas"
        else:
            return "run away"

        return None


signal.signal(signal.SIGINT, signal_handler)


class SnowBoyListener(object):

    def __init__(self, callback, model="/home/pi/hey_will.pmdl"):
        self.model = model
        self.callback = callback
        self.detector = snowboydecoder.HotwordDetector(model, sensitivity=0.5)
        print("listener initialized")

    def listen(self):
        print("listening...")
        self.detector.start(detected_callback=self.callback,
                            interrupt_check=interrupt_callback,
                            sleep_time=0.03
                            )
        self.detector.terminate()

    def interrupt(self):
        self.interrupted = True



