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
  | Author: jgmdev <jgmdev@gmail.com>                                    |
  +----------------------------------------------------------------------+
*/
#ifndef HAVE_PHP_WEBKITGTK_SETTINGS_H
#define HAVE_PHP_WEBKITGTK_SETTINGS_H

extern zend_class_entry *webkitgtkSettings_ce;

typedef struct _php_webkitgtk_settings_t {
	zval* this;
	WebKitSettings* settings;
	zend_object std;
} php_webkitgtk_settings_t;

#define php_webkitgtk_settings_from(o) ((php_webkitgtk_settings_t*) ((char*) o - XtOffsetOf(php_webkitgtk_settings_t, std)))
#define php_webkitgtk_settings_fetch(z) php_webkitgtk_settings_from(Z_OBJ_P(z))

zval *php_webkitgtk_settings_construct(zval *settings, WebKitSettings* native);

PHP_MINIT_FUNCTION(WebKitGtk_Settings);

#endif
