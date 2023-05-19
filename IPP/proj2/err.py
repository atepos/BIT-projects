import sys as s

class ErrorHandling:
    OK = 0
    paramsErr = 10
    stdinErr = 11
    stdoutErr = 12
    stdinXMLBadformatErr = 31
    unexpectedStructureErr = 32
    semanticErr = 52
    badOperandErr = 53
    varnonErr = 54
    labelnonErr = 55
    valueMissingErr = 56
    badOperandValueErr = 57
    stringErr = 58
    internalErr = 99

class CustomException(Exception):
    def __init__(self, message, error_code):
        super().__init__(message)
        self.error_code = error_code

    def print_error():
        exc_type, exc_value, exc_traceback = s.exc_info()
        print("\033[31mERROR\033[0m:", exc_value.args[0], file=s.stderr)