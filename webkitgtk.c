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

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_webkitgtk.h"

#include <classes/webview.h>
#include <classes/settings.h>

void php_webkitgtk_set_call(
	zend_object* object, 
	const char* name, 
	size_t nlength, 
	zend_fcall_info* fci, 
	zend_fcall_info_cache* fcc
){
	zend_function *function = zend_hash_str_find_ptr(
		&object->ce->function_table, name, nlength
	);

	if (!function || function->type == ZEND_INTERNAL_FUNCTION) {
		return;
	}

	fci->size = sizeof(zend_fcall_info);
	fci->object = object;
	fci->no_separation = 1;

	fcc->object = object;
	fcc->function_handler = function;
	fcc->calling_scope = object->ce;
	fcc->called_scope = object->ce;
}

int php_webkitgtk_call(zend_fcall_info* fci, zend_fcall_info_cache* fcc) {
	int result = zend_call_function(fci, fcc);

	if (result != SUCCESS) {
		zend_try_exception_handler();
	}

	return result;
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(webkitgtk)
{
	PHP_MINIT(WebKitGtk_WebView)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(WebKitGtk_Settings)(INIT_FUNC_ARGS_PASSTHRU);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(webkitgtk)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(webkitgtk)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "webkitgtk support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ webkitgtk_module_entry
 */
zend_module_entry webkitgtk_module_entry = {
	STANDARD_MODULE_HEADER,
	"webkitgtk",
	NULL,
	PHP_MINIT(webkitgtk),
	PHP_MSHUTDOWN(webkitgtk),
	NULL,
	NULL,
	PHP_MINFO(webkitgtk),
	PHP_WEBKITGTK_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_WEBKITGTK
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(webkitgtk)
#endif