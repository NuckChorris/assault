import subprocess

flags = [
  '-Wall',
  '-Wextra',
  '-Werror',
  '-fexceptions',
  '-DNDEBUG',
  '-DUSE_CLANG_COMPLETER',
  '-pthread',
]

flags += [s.strip() for s in subprocess.check_output(['pkg-config', '--cflags', 'gtk+-3.0']).strip().split(' ')]
flags += [s.strip() for s in subprocess.check_output(['pkg-config', '--libs', 'gtk+-3.0']).strip().split(' ')]

def FlagsForFile( filename ):
  return {
    'flags': flags,
    'do_cache': True
  }
