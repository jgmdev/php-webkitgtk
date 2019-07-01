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
#ifndef HAVE_PHP_WEBKIT_WEBVIEW
#define HAVE_PHP_WEBKIT_WEBVIEW
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

#include "php.h"

#include <classes/webview.h>

zend_object_handlers php_webkit_webview_handlers;

zend_class_entry *webkitWebView_ce;

extern void php_webkit_set_call(
	zend_object *object, 
	const char *name, 
	size_t nlength, 
	zend_fcall_info *fci, 
	zend_fcall_info_cache *fcc
);

extern int php_webkit_call(zend_fcall_info *fci, zend_fcall_info_cache *fcc);

zend_object* php_webkit_webview_create(zend_class_entry *ce) {
	php_webkit_webview_t *w = (php_webkit_webview_t*) 
		ecalloc(
			1, 
			sizeof(php_webkit_webview_t) + zend_object_properties_size(ce)
		)
	;

	zend_object_std_init(&w->std, ce);

	object_properties_init(&w->std, ce);

	w->std.handlers = &php_webkit_webview_handlers;

	php_webkit_set_call(
		&w->std, 
		ZEND_STRL("onclose"), 
		&w->close.fci, 
		&w->close.fcc
	);

	php_webkit_set_call(
		&w->std, 
		ZEND_STRL("onloadchange"), 
		&w->load_changed.fci, 
		&w->load_changed.fcc
	);

	php_webkit_set_call(
		&w->std, 
		ZEND_STRL("ondestroy"), 
		&w->destroy.fci, 
		&w->destroy.fcc
	);

	return &w->std;
}

gboolean php_webkit_webview_close_handler(WebKitWebView *view, void *arg) {
	php_webkit_webview_t *web_view = (php_webkit_webview_t *) arg;
	zval rv;
	gboolean result = 0;

	if (!web_view->close.fci.size) {
		gtk_widget_destroy(web_view->window);

		return TRUE;
	}

	ZVAL_UNDEF(&rv);

	web_view->close.fci.retval = &rv;

	if (php_webkit_call(&web_view->close.fci, &web_view->close.fcc) != SUCCESS) {
		gtk_widget_destroy(web_view->window);

		return TRUE;
	}

	if (Z_TYPE(rv) != IS_UNDEF) {
		result = (gboolean) zval_get_long(&rv);
		zval_ptr_dtor(&rv);
	}

	return result;
}

void php_webkit_webview_load_changed_handler(
	WebKitWebView *view, WebKitLoadEvent load_event,  void *arg
){
	php_webkit_webview_t *web_view = (php_webkit_webview_t *) arg;

	if (
		!web_view->load_changed.fci.size
		||
		php_webkit_call(
			&web_view->load_changed.fci, 
			&web_view->load_changed.fcc
		) 
		!= 
		SUCCESS
	) {
		if(load_event == WEBKIT_LOAD_FINISHED){
			gtk_window_set_title(
				GTK_WINDOW(web_view->window), 
				webkit_web_view_get_title(view)
			);
		}
	}
}

static void php_webkit_webview_destroy_handler(
	GtkWidget* widget, void* arg
)
{
	php_webkit_webview_t *web_view = (php_webkit_webview_t *) arg;

	if (
		!web_view->load_changed.fci.size
		||
		php_webkit_call(
			&web_view->load_changed.fci, 
			&web_view->load_changed.fcc
		) 
		!= 
		SUCCESS
	) {
		gtk_main_quit();
	}
}

ZEND_BEGIN_ARG_INFO_EX(php_webkit_webview_construct_info, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, title, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Window Window::__construct(string title, Size size, bool menu) */
PHP_METHOD(WebView, __construct) 
{
	php_webkit_webview_t *win = php_webkit_webview_fetch(getThis());

	zend_string *title = NULL;

	if (
		zend_parse_parameters(
			ZEND_NUM_ARGS(), 
			"S", 
			&title
		) != SUCCESS
	) {
		return;
	}

	gtk_init(0, NULL);

	win->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(win->window), 800, 600);
	gtk_window_set_position(GTK_WINDOW(win->window), GTK_WIN_POS_CENTER);

	if(ZSTR_LEN(title) > 0)
	{
		gtk_window_set_title(GTK_WINDOW(win->window), ZSTR_VAL(title));
	}

	win->view = WEBKIT_WEB_VIEW(webkit_web_view_new());

	// Put the browser area into the main window
    gtk_container_add(GTK_CONTAINER(win->window), GTK_WIDGET(win->view));

	g_signal_connect(
		win->view, 
		"close", 
		G_CALLBACK(php_webkit_webview_close_handler), 
		win
	);

    g_signal_connect(
		win->view, 
		"load-changed", 
		G_CALLBACK(php_webkit_webview_load_changed_handler), 
		win
	);

	g_signal_connect(
		win->window, 
		"destroy", 
		G_CALLBACK(php_webkit_webview_destroy_handler), 
		win
	);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkit_webview_load_uri_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, uri, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void WebView::loadURI(string uri) */
PHP_METHOD(WebView, loadURI)
{
	php_webkit_webview_t *win = php_webkit_webview_fetch(getThis());
	zend_string *uri = NULL;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &uri) != SUCCESS) {
		return;
	}

	webkit_web_view_load_uri(win->view, ZSTR_VAL(uri));
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkit_webview_show_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto string Window::show(void) */
PHP_METHOD(WebView, show) 
{
	php_webkit_webview_t *win = php_webkit_webview_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	// Make sure that when the browser area becomes visible, it will get mouse
    // and keyboard events
    gtk_widget_grab_focus(GTK_WIDGET(win->view));

    // Make sure the main window and all its contents are visible
    gtk_widget_show_all(win->window);

    // Run the main GTK+ event loop
    gtk_main();
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkit_webview_close_info, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(WebView, onClose) {}

ZEND_BEGIN_ARG_INFO_EX(php_webkit_webview_load_change_info, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(WebView, onLoadChange) {}

ZEND_BEGIN_ARG_INFO_EX(php_webkit_webview_destroy_info, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(WebView, onDestroy) {}

/* {{{ */
const zend_function_entry php_webkit_webview_methods[] = {
	PHP_ME(WebView, __construct,    php_webkit_webview_construct_info,     ZEND_ACC_PUBLIC)
	PHP_ME(WebView, loadURI,        php_webkit_webview_load_uri_info,      ZEND_ACC_PUBLIC)
	PHP_ME(WebView, show,           php_webkit_webview_show_info,          ZEND_ACC_PUBLIC)
	PHP_ME(WebView, onClose,        php_webkit_webview_close_info,         ZEND_ACC_PROTECTED)
	PHP_ME(WebView, onLoadChange,   php_webkit_webview_load_change_info,   ZEND_ACC_PROTECTED)
	PHP_ME(WebView, onDestroy,      php_webkit_webview_close_info,         ZEND_ACC_PROTECTED)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(WebKit_WebView) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "WebKit", "WebView", php_webkit_webview_methods);

	webkitWebView_ce = zend_register_internal_class(&ce);
	webkitWebView_ce->create_object = php_webkit_webview_create;

	memcpy(
		&php_webkit_webview_handlers, 
		zend_get_std_object_handlers(), 
		sizeof(zend_object_handlers)
	);

	php_webkit_webview_handlers.offset = XtOffsetOf(php_webkit_webview_t, std);

	return SUCCESS;
} /* }}} */
#endif
