import sys
# for windows
try:
    import colorama
    colorama.init()
except:
    pass


def info(message):
    print("[\033[1;36mINFO\033[0;0m]    " + message)


def warning(message):
    print("[\033[1;33mWARNING\033[0;0m] " + message)


def error(message):
    print("[\033[1;31mERROR\033[0;0m]   " + message)
    sys.exit(1)


def skip(message):
    pass


for arg in sys.argv:
    if arg == '--no-print':
        sys.modules[__name__].info = skip
