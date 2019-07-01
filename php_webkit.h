/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: jgmdev                                                       |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_WEBKIT_H
#define PHP_WEBKIT_H

extern zend_module_entry webkit_module_entry;
#define phpext_webkit_ptr &webkit_module_entry

#define PHP_WEBKIT_VERSION "0.1"

#ifdef PHP_WIN32
#	define PHP_WEBKIT_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_WEBKIT_API __attribute__ ((visibility("default")))
#else
#	define PHP_WEBKIT_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#if defined(ZTS) && defined(COMPILE_DL_WEBKIT)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif	/* PHP_WEBKIT_H */