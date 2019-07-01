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
#ifndef HAVE_PHP_WEBKIT_WEBVIEW_H
#define HAVE_PHP_WEBKIT_WEBVIEW_H

extern zend_class_entry *webkitWebView_ce;

typedef struct _php_webkit_webview_t {
	WebKitWebView* view;
	GtkWidget* window;
	zend_object* parent;
	struct php_webkit_webview_close_t {
		zend_fcall_info fci;
		zend_fcall_info_cache fcc;
	} close;
	struct php_webkit_webview_load_changed_t {
		zend_fcall_info fci;
		zend_fcall_info_cache fcc;
	} load_changed;
	struct php_webkit_webview_destroy_t {
		zend_fcall_info fci;
		zend_fcall_info_cache fcc;
	} destroy;
	zend_object std;
} php_webkit_webview_t;

#define php_webkit_webview_from(o) ((php_webkit_webview_t*) ((char*) o - XtOffsetOf(php_webkit_webview_t, std)))
#define php_webkit_webview_fetch(z) php_webkit_webview_from(Z_OBJ_P(z))

PHP_MINIT_FUNCTION(WebKit_WebView);

#endif
