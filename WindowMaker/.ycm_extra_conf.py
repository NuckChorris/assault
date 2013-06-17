flags = [
'-Wall',
'-Wextra',
'-Werror',
'-fexceptions',
'-DNDEBUG',
'-DUSE_CLANG_COMPLETER',
'-pthread',
'-I/usr/include/gtk-3.0',
'-I/usr/include/at-spi2-atk/2.0',
'-I/usr/include/gtk-3.0',
'-I/usr/include/gio-unix-2.0/',
'-I/usr/include/cairo',
'-I/usr/include/pango-1.0',
'-I/usr/include/harfbuzz',
'-I/usr/include/pango-1.0',
'-I/usr/include/atk-1.0',
'-I/usr/include/cairo',
'-I/usr/include/pixman-1',
'-I/usr/include/freetype2',
'-I/usr/include/libdrm',
'-I/usr/include/libpng16',
'-I/usr/include/gdk-pixbuf-2.0',
'-I/usr/include/libpng16',
'-I/usr/include/glib-2.0',
'-I/usr/lib/glib-2.0/include',
'-lgtk-3',
'-lgdk-3',
'-lpangocairo-1.0',
'-lpango-1.0',
'-latk-1.0',
'-lcairo-gobject',
'-lcairo',
'-lgdk_pixbuf-2.0',
'-lgio-2.0',
'-lgobject-2.0',
'-lglib-2.0'
]

def FlagsForFile( filename ):
  return {
    'flags': flags,
    'do_cache': True
  }
