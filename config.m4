PHP_ARG_WITH(webkit,       for webkit support,
[  --with-webkitgtk        Include webkit support])

if test "$PHP_WEBKIT" != "no"; then
  AC_MSG_CHECKING([for webkit2gtk availability])
  pkg-config --cflags --libs webkit2gtk-4.0 2>/dev/null > /dev/null
  WEBKIT_FOUND=$(echo $?)
  if test $WEBKIT_FOUND -lt 1; then
    AC_MSG_RESULT(found)
  else
    AC_MSG_RESULT(not found)
    AC_MSG_ERROR([Please install webkit2gtk])
  fi

  dnl Add header search paths to the PHP build system
  PHP_EVAL_INCLINE($(pkg-config --cflags webkit2gtk-4.0))
    
  dnl Add libraries and or library search paths to the PHP build system 
  PHP_EVAL_LIBLINE($(pkg-config --libs webkit2gtk-4.0), WEBKIT_SHARED_LIBADD)

  PHP_ADD_BUILD_DIR($ext_builddir/classes, 1)
  PHP_ADD_INCLUDE($ext_builddir)

  PHP_SUBST(WEBKIT_SHARED_LIBADD)

  PHP_NEW_EXTENSION(
    webkit, 
    webkit.c \
      classes/webview.c,
    $ext_shared,,-DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi