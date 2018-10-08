from aiy import cloudspeech
import snowboydecoder


class Interepreter(object):
    recognizer = cloudspeech.get_recognizer()

    def __init__(self):
        self._init_expected_phrases()

    def _init_expected_phrases(self):
        self.recognizer.expect_phrase('hey will')
        self.recognizer.expect_phrase('where are you')

    def interpret(self):
        text = self.recognizer.recognize()
        return self.respond(text)

    def respond(self, text):
        if text == "where are you":
            return "im right here"


class SnowBoyListener(object):
    interrupted = False

    def __init__(self, callback, model="/home/pi/hey_will.pmdl"):
        self.model = model
        self.callback = callback
        self.detector = snowboydecoder.HotwordDetector(model, sensitivity=0.3)

    def interrupt_check(self):
        return self.interrupted

    def listen(self):
        self.detector.start(detected_callback=self.callback,
                            interrupt_check=self.interrupt_check,
                            sleep_time=0.03
                            )

    def interrupt(self):
        # TODO What action should call this?
        self.interrupted = True



