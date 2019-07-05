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
#ifndef HAVE_php_WEBKITGTK_WEBVIEW
#define HAVE_php_WEBKITGTK_WEBVIEW

#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

#include "php.h"

#include <classes/settings.h>
#include <classes/webview.h>

zend_object_handlers php_webkitgtk_webview_handlers;

zend_class_entry* webkitgtkWebView_ce;

extern void php_webkitgtk_set_call(
	zend_object* object, 
	const char* name, 
	size_t nlength, 
	zend_fcall_info* fci, 
	zend_fcall_info_cache* fcc
);

extern int php_webkitgtk_call(zend_fcall_info* fci, zend_fcall_info_cache* fcc);

void php_webkitgtk_webview_close_handler(WebKitWebView* view, void* arg);
void php_webkitgtk_webview_load_changed_handler(
	WebKitWebView* view, WebKitLoadEvent load_event,  void* arg
);
gboolean php_webkitgtk_webview_load_failed_handler (
	WebKitWebView* view,
	WebKitLoadEvent load_event,
	gchar* failing_uri,
	GError* error,
	void* arg
);
void php_webkitgtk_webview_destroy_handler(GtkWidget* widget, void* arg);

zend_object* php_webkitgtk_webview_create(zend_class_entry* ce) {
	php_webkitgtk_webview_t* w = (php_webkitgtk_webview_t*) 
		ecalloc(
			1, 
			sizeof(php_webkitgtk_webview_t) + zend_object_properties_size(ce)
		)
	;

	zend_object_std_init(&w->std, ce);

	object_properties_init(&w->std, ce);

	w->std.handlers = &php_webkitgtk_webview_handlers;

	php_webkitgtk_set_call(
		&w->std, 
		ZEND_STRL("onclose"), 
		&w->close.fci, 
		&w->close.fcc
	);

	php_webkitgtk_set_call(
		&w->std, 
		ZEND_STRL("onloadchanged"), 
		&w->load_changed.fci, 
		&w->load_changed.fcc
	);

	php_webkitgtk_set_call(
		&w->std, 
		ZEND_STRL("onloadfailed"), 
		&w->load_failed.fci, 
		&w->load_failed.fcc
	);

	php_webkitgtk_set_call(
		&w->std, 
		ZEND_STRL("ondestroy"), 
		&w->destroy.fci, 
		&w->destroy.fcc
	);

	return &w->std;
}

void php_webkitgtk_webview_recreate(zval* object)
{
	php_webkitgtk_webview_t* web_view = php_webkitgtk_webview_fetch(object);

	web_view->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(web_view->window), 800, 600);
	gtk_window_set_position(GTK_WINDOW(web_view->window), GTK_WIN_POS_CENTER);

	zend_string* title = zval_get_string(
		zend_read_property(
			webkitgtkWebView_ce, 
			web_view->this, 
			ZEND_STRL("title"), 
			TRUE,
			NULL
		)
	);

	if(ZSTR_LEN(title) > 0)
	{
		gtk_window_set_title(GTK_WINDOW(web_view->window), ZSTR_VAL(title));
	}

	zend_string* icon = zval_get_string(
		zend_read_property(
			webkitgtkWebView_ce, 
			web_view->this, 
			ZEND_STRL("icon"), 
			TRUE,
			NULL
		)
	);

	if(ZSTR_LEN(icon) > 0)
	{
		gtk_window_set_icon_from_file(
			GTK_WINDOW(web_view->window), 
			ZSTR_VAL(icon), 
			NULL
		);
	}

	web_view->view = WEBKIT_WEB_VIEW(webkit_web_view_new());

	g_signal_connect(
		web_view->view, 
		"close", 
		G_CALLBACK(php_webkitgtk_webview_close_handler), 
		web_view
	);

	g_signal_connect(
		web_view->view, 
		"load-changed", 
		G_CALLBACK(php_webkitgtk_webview_load_changed_handler), 
		web_view
	);

	g_signal_connect(
		web_view->view, 
		"load-failed", 
		G_CALLBACK(php_webkitgtk_webview_load_failed_handler), 
		web_view
	);

	g_signal_connect(
		web_view->window, 
		"destroy", 
		G_CALLBACK(php_webkitgtk_webview_destroy_handler), 
		web_view
	);

	zend_string* uri = zval_get_string(
		zend_read_property(
			webkitgtkWebView_ce, 
			web_view->this, 
			ZEND_STRL("uri"), 
			TRUE,
			NULL
		)
	);

	if(ZSTR_LEN(uri) > 0)
	{
		webkit_web_view_load_uri(web_view->view, ZSTR_VAL(uri));
	}

	// Put the browser area into the main window
    gtk_container_add(GTK_CONTAINER(web_view->window), GTK_WIDGET(web_view->view));

	// Make sure that when the browser area becomes visible, it will get mouse
    // and keyboard events
    gtk_widget_grab_focus(GTK_WIDGET(web_view->view));
}

void php_webkitgtk_webview_close_handler(WebKitWebView* view, void* arg) {
	php_webkitgtk_webview_t *web_view = (php_webkitgtk_webview_t*) arg;

	if (web_view->close.fci.size) {
		php_webkitgtk_call(&web_view->close.fci, &web_view->close.fcc);
	}
}

void php_webkitgtk_webview_load_changed_handler(
	WebKitWebView* view, WebKitLoadEvent load_event,  void* arg
){
	php_webkitgtk_webview_t* web_view = (php_webkitgtk_webview_t*) arg;

	zval load_event_val;

	ZVAL_LONG(&load_event_val, (long) load_event);

	zend_fcall_info_argn(&web_view->load_changed.fci, 1, &load_event_val);

	if(web_view->load_changed.fci.size)
	{
		php_webkitgtk_call(
			&web_view->load_changed.fci, 
			&web_view->load_changed.fcc
		);
	}
	else
	{
		gtk_window_set_title(
			GTK_WINDOW(web_view->window), 
			webkit_web_view_get_title(view)
		);
	}

	if(load_event == WEBKIT_LOAD_FINISHED)
	{
		const gchar* title = webkit_web_view_get_title(view);
		const gchar* uri = webkit_web_view_get_uri(view);

		zend_update_property_string(
			webkitgtkWebView_ce, 
			web_view->this, 
			ZEND_STRL("title"), 
			title == NULL ? "" : title
		);

		zend_update_property_string(
			webkitgtkWebView_ce, 
			web_view->this, 
			ZEND_STRL("uri"), 
			uri == NULL ? "" : uri
		);
	}

	zval_ptr_dtor(&load_event_val);
}

gboolean php_webkitgtk_webview_load_failed_handler (
	WebKitWebView* view,
	WebKitLoadEvent load_event,
	gchar* failing_uri,
	GError* error,
	void* arg
)
{
	php_webkitgtk_webview_t* web_view = (php_webkitgtk_webview_t*) arg;

	zval load_event_val;
	zval failing_uri_val;
	zval error_code_val;
	zval error_message_val;

	zval rv;
	gboolean result = FALSE;

	if(!web_view->load_failed.fci.size)
	{
		return result;
	}

	ZVAL_LONG(&load_event_val, (long) load_event);
	ZVAL_STRING(&failing_uri_val, failing_uri);
	ZVAL_LONG(&error_code_val, error->code);
	ZVAL_STRING(&error_message_val, error->message);

	zend_fcall_info_argn(
		&web_view->load_failed.fci, 
		4, 
		&load_event_val,
		&failing_uri_val,
		&error_code_val,
		&error_message_val
	);

	ZVAL_UNDEF(&rv);

	web_view->load_failed.fci.retval = &rv;

	if(
		php_webkitgtk_call(
			&web_view->load_failed.fci, 
			&web_view->load_failed.fcc
		) == SUCCESS
	) 
	{
		if (Z_TYPE(rv) != IS_UNDEF) {
			result = (gboolean) zval_get_long(&rv);
			zval_ptr_dtor(&rv);
		}
	}

	zval_ptr_dtor(&load_event_val);
	zval_ptr_dtor(&failing_uri_val);
	zval_ptr_dtor(&error_code_val);
	zval_ptr_dtor(&error_message_val);

	return result;
}

void php_webkitgtk_webview_destroy_handler(
	GtkWidget* widget, void* arg
)
{
	php_webkitgtk_webview_t* web_view = (php_webkitgtk_webview_t*) arg;

	if (
		!web_view->load_changed.fci.size
		||
		php_webkitgtk_call(
			&web_view->load_changed.fci, 
			&web_view->load_changed.fcc
		) 
		!= 
		SUCCESS
	) {
		gtk_widget_destroy(web_view->window);

		if(gtk_main_level() > 0)
		{
			gtk_main_quit();
			php_webkitgtk_webview_recreate(web_view->this);
		}
	}
}

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_webview_construct_info, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, title, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto WebView WebView::__construct(string title) */
PHP_METHOD(WebView, __construct) 
{
	static bool initialize = false;

	php_webkitgtk_webview_t* wv = php_webkitgtk_webview_fetch(getThis());

	wv->this = getThis();

	zend_string* title = NULL;

	if (
		zend_parse_parameters(
			ZEND_NUM_ARGS(), 
			"S", 
			&title
		) != SUCCESS
	) {
		return;
	}

	if(!initialize)
	{
		gtk_init(0, NULL);
		initialize = true;
	}

	wv->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(wv->window), 800, 600);
	gtk_window_set_position(GTK_WINDOW(wv->window), GTK_WIN_POS_CENTER);

	if(ZSTR_LEN(title) > 0)
	{
		zend_update_property_string(
			webkitgtkWebView_ce, 
			getThis(), 
			ZEND_STRL("title"), 
			ZSTR_VAL(title)
		);

		gtk_window_set_title(GTK_WINDOW(wv->window), ZSTR_VAL(title));
	}

	wv->view = WEBKIT_WEB_VIEW(webkit_web_view_new());

	g_signal_connect(
		wv->view, 
		"close", 
		G_CALLBACK(php_webkitgtk_webview_close_handler), 
		wv
	);

    g_signal_connect(
		wv->view, 
		"load-changed", 
		G_CALLBACK(php_webkitgtk_webview_load_changed_handler), 
		wv
	);

	g_signal_connect(
		wv->view, 
		"load-failed", 
		G_CALLBACK(php_webkitgtk_webview_load_failed_handler), 
		wv
	);

	g_signal_connect(
		wv->window, 
		"destroy", 
		G_CALLBACK(php_webkitgtk_webview_destroy_handler), 
		wv
	);

	// Put the browser area into the main window
    gtk_container_add(GTK_CONTAINER(wv->window), GTK_WIDGET(wv->view));

	// Make sure that when the browser area becomes visible, it will get mouse
    // and keyboard events
    gtk_widget_grab_focus(GTK_WIDGET(wv->view));
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_webview_destruct_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void WebView::__destruct() */
PHP_METHOD(WebView, __destruct) 
{
	php_webkitgtk_webview_t* wv = php_webkitgtk_webview_fetch(getThis());

	gtk_widget_destroy(wv->window);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_webview_load_uri_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, uri, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void WebView::loadURI(string uri) */
PHP_METHOD(WebView, loadURI)
{
	php_webkitgtk_webview_t* wv = php_webkitgtk_webview_fetch(getThis());
	zend_string* uri = NULL;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &uri) != SUCCESS) {
		return;
	}

	zend_update_property_string(
		webkitgtkWebView_ce, 
		getThis(), 
		ZEND_STRL("uri"), 
		ZSTR_VAL(uri)
	);

	webkit_web_view_load_uri(wv->view, ZSTR_VAL(uri));
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_webview_load_html_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, content, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, base_uri, IS_STRING, 1)
ZEND_END_ARG_INFO()

/* {{{ proto void WebView::loadHTML(string content, string base_uri) */
PHP_METHOD(WebView, loadHTML)
{
	php_webkitgtk_webview_t* wv = php_webkitgtk_webview_fetch(getThis());

	zend_string* content = NULL;
	zend_string* base_uri = NULL;
	
	if(
		zend_parse_parameters_throw(
			ZEND_NUM_ARGS(), 
			"S|S", 
			&content,
			&base_uri
		) != SUCCESS
	) {
		return;
	}

	if(base_uri == NULL)
	{
		webkit_web_view_load_html(wv->view, ZSTR_VAL(content), NULL);
	}
	else
	{
		webkit_web_view_load_html(
			wv->view, 
			ZSTR_VAL(content), 
			ZSTR_VAL(base_uri)
		);
	}
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_webview_load_plain_text_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, plain_text, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void WebView::loadPlainText(string plain_text) */
PHP_METHOD(WebView, loadPlainText)
{
	php_webkitgtk_webview_t* wv = php_webkitgtk_webview_fetch(getThis());

	zend_string* plain_text = NULL;
	
	if(
		zend_parse_parameters_throw(
			ZEND_NUM_ARGS(), 
			"S", 
			&plain_text
		) != SUCCESS
	) {
		return;
	}

	webkit_web_view_load_plain_text(
		wv->view, 
		ZSTR_VAL(plain_text)
	);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_webkitgtk_webview_set_icon_info, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, filename, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool WebView::setIcon(string filename) */
PHP_METHOD(WebView, setIcon)
{
	php_webkitgtk_webview_t* wv = php_webkitgtk_webview_fetch(getThis());
	zend_string* file_name = NULL;
	
	if(
		zend_parse_parameters_throw(
			ZEND_NUM_ARGS(), 
			"S", 
			&file_name
		) != SUCCESS
	) {
		return;
	}

	bool icon_return = FALSE;

	if(
		icon_return = gtk_window_set_icon_from_file(
			GTK_WINDOW(wv->window), 
			ZSTR_VAL(file_name), 
			NULL
		)
	)
	{
		zend_update_property_string(
			webkitgtkWebView_ce, 
			getThis(), 
			ZEND_STRL("icon"), 
			ZSTR_VAL(file_name)
		);
	}

	RETURN_BOOL(icon_return)
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_webview_set_settings_info, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, size, WebKit\\Settings, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool WebView::setSettings(WebKit\Settings settings) */
PHP_METHOD(WebView, setSettings)
{
	php_webkitgtk_webview_t* wv = php_webkitgtk_webview_fetch(getThis());
	zval* settings = NULL;
	php_webkitgtk_settings_t* settings_t;
	
	if (
		zend_parse_parameters_throw(
			ZEND_NUM_ARGS(), 
			"O", 
			&settings, 
			webkitgtkSettings_ce
		) != SUCCESS
	) {
		return;
	}

	settings_t = php_webkitgtk_settings_fetch(settings);

	webkit_web_view_set_settings(wv->view, settings_t->settings);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_webview_set_title_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, title, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool WebView::setTitle(string title) */
PHP_METHOD(WebView, setTitle)
{
	php_webkitgtk_webview_t* wv = php_webkitgtk_webview_fetch(getThis());
	zend_string* title = NULL;
	
	if (
		zend_parse_parameters_throw(
			ZEND_NUM_ARGS(), 
			"S", 
			&title
		) != SUCCESS
	) {
		return;
	}

	gtk_window_set_title(GTK_WINDOW(wv->window), ZSTR_VAL(title));
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(php_webkitgtk_webview_get_settings_info, 0, 0, "WebKitGtk\\Settings", 0)
ZEND_END_ARG_INFO()

/* {{{ proto Settings WebView::getSettings(void) */
PHP_METHOD(WebView, getSettings) 
{
	php_webkitgtk_webview_t* wv = php_webkitgtk_webview_fetch(getThis());
	int width = 0, height = 0;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	WebKitSettings* settings = webkit_web_view_get_settings(wv->view);

	php_webkitgtk_settings_construct(return_value, settings);	
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_webkitgtk_webview_get_title_info, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto string WebView::getTitle(void) */
PHP_METHOD(WebView, getTitle)
{
	php_webkitgtk_webview_t* wv = php_webkitgtk_webview_fetch(getThis());
	
	if (
		zend_parse_parameters_none() != SUCCESS
	) {
		return;
	}

	const gchar* title = webkit_web_view_get_title(wv->view);

	RETURN_STRING(title != NULL ? title : "")
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_webkitgtk_webview_get_uri_info, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto string WebView::getURI(void) */
PHP_METHOD(WebView, getURI)
{
	php_webkitgtk_webview_t* wv = php_webkitgtk_webview_fetch(getThis());
	
	if (
		zend_parse_parameters_none() != SUCCESS
	) {
		return;
	}

	const gchar* uri = webkit_web_view_get_uri(wv->view);

	RETURN_STRING(uri != NULL ? uri : "")
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_webview_reload_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void WebView::reload(void) */
PHP_METHOD(WebView, reload)
{
	php_webkitgtk_webview_t* wv = php_webkitgtk_webview_fetch(getThis());
	
	if (
		zend_parse_parameters_none() != SUCCESS
	) {
		return;
	}

	webkit_web_view_reload(wv->view);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_webview_reload_bypass_cache_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void WebView::reloadBypassCache(void) */
PHP_METHOD(WebView, reloadBypassCache)
{
	php_webkitgtk_webview_t* wv = php_webkitgtk_webview_fetch(getThis());
	
	if (
		zend_parse_parameters_none() != SUCCESS
	) {
		return;
	}

	webkit_web_view_reload_bypass_cache(wv->view);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_webview_stop_loading_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void WebView::stopLoading(void) */
PHP_METHOD(WebView, stopLoading)
{
	php_webkitgtk_webview_t* wv = php_webkitgtk_webview_fetch(getThis());
	
	if (
		zend_parse_parameters_none() != SUCCESS
	) {
		return;
	}

	webkit_web_view_stop_loading(wv->view);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_webview_resize_info, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void WebView::resize(int width, int height) */
PHP_METHOD(WebView, resize)
{
	php_webkitgtk_webview_t* wv = php_webkitgtk_webview_fetch(getThis());
	
	zend_long width;
	zend_long height;
	
	if (
		zend_parse_parameters_throw(
			ZEND_NUM_ARGS(), 
			"LL", 
			&width,
			&height
		) != SUCCESS
	) {
		return;
	}

	zend_update_property_long(
		webkitgtkWebView_ce, 
		getThis(), 
		ZEND_STRL("width"), 
		width
	);

	zend_update_property_long(
		webkitgtkWebView_ce, 
		getThis(), 
		ZEND_STRL("height"), 
		height
	);

	gtk_window_resize(GTK_WINDOW(wv->window), width, height);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_webview_move_info, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, x, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void WebView::move(int x, int y) */
PHP_METHOD(WebView, move)
{
	php_webkitgtk_webview_t* wv = php_webkitgtk_webview_fetch(getThis());
	
	zend_long x;
	zend_long y;
	
	if (
		zend_parse_parameters_throw(
			ZEND_NUM_ARGS(), 
			"LL", 
			&x,
			&y
		) != SUCCESS
	) {
		return;
	}

	zend_update_property_long(
		webkitgtkWebView_ce, 
		getThis(), 
		ZEND_STRL("x"), 
		x
	);

	zend_update_property_long(
		webkitgtkWebView_ce, 
		getThis(), 
		ZEND_STRL("y"), 
		y
	);

	gtk_window_move(GTK_WINDOW(wv->window), x, y);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_webview_go_back_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void WebView::goBack(void) */
PHP_METHOD(WebView, goBack)
{
	php_webkitgtk_webview_t* wv = php_webkitgtk_webview_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

    webkit_web_view_go_back(wv->view);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_webview_go_forward_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void WebView::goForward(void) */
PHP_METHOD(WebView, goForward)
{
	php_webkitgtk_webview_t* wv = php_webkitgtk_webview_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

    webkit_web_view_go_forward(wv->view);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_webkitgtk_webview_can_go_back_info, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool WebView::canGoBack(void) */
PHP_METHOD(WebView, canGoBack)
{
	php_webkitgtk_webview_t* wv = php_webkitgtk_webview_fetch(getThis());
	
	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_BOOL(
		webkit_web_view_can_go_back(
			wv->view
		)
	)
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_webkitgtk_webview_can_go_forward_info, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool WebView::canGoForward(void) */
PHP_METHOD(WebView, canGoForward)
{
	php_webkitgtk_webview_t* wv = php_webkitgtk_webview_fetch(getThis());
	
	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_BOOL(
		webkit_web_view_can_go_forward(
			wv->view
		)
	)
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_webview_show_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void WebView::show(void) */
PHP_METHOD(WebView, show)
{
	php_webkitgtk_webview_t* wv = php_webkitgtk_webview_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

    // Make sure the main window and all its contents are visible
    gtk_widget_show_all(wv->window);

    // Run the main GTK+ event loop
    gtk_main();
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_webview_hide_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void WebView::hide(void) */
PHP_METHOD(WebView, hide) 
{
	php_webkitgtk_webview_t* wv = php_webkitgtk_webview_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

    // Make sure the main window and all its contents are visible
    gtk_widget_hide(wv->window);

    // Run the main GTK+ event loop
    gtk_main_quit();
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_webview_close_info, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(WebView, onClose) {}

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_webview_load_changed_info, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, load_state, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void WebView::onLoadChanged(long load_state) */
PHP_METHOD(WebView, onLoadChanged) {}
/* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_webkitgtk_webview_load_failed_info, 0, 0, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, load_state, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, failing_uri, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, error_code, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, error_message, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool WebView::onLoadFailed(long load_state, string failing_uri, int error_code, string error_message) */
PHP_METHOD(WebView, onLoadFailed) {}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_webview_destroy_info, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(WebView, onDestroy) {}

/* {{{ */
const zend_function_entry php_webkitgtk_webview_methods[] = {
	PHP_ME(WebView, __construct,       php_webkitgtk_webview_construct_info,            ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(WebView, __destruct,        php_webkitgtk_webview_destruct_info,             ZEND_ACC_DTOR|ZEND_ACC_PUBLIC)
	PHP_ME(WebView, loadURI,           php_webkitgtk_webview_load_uri_info,             ZEND_ACC_PUBLIC)
	PHP_ME(WebView, loadHTML,          php_webkitgtk_webview_load_html_info,            ZEND_ACC_PUBLIC)
	PHP_ME(WebView, loadPlainText,     php_webkitgtk_webview_load_plain_text_info,      ZEND_ACC_PUBLIC)
	PHP_ME(WebView, setIcon,           php_webkitgtk_webview_set_icon_info,             ZEND_ACC_PUBLIC)
	PHP_ME(WebView, setSettings,       php_webkitgtk_webview_set_settings_info,         ZEND_ACC_PUBLIC)
	PHP_ME(WebView, setTitle,          php_webkitgtk_webview_set_title_info,            ZEND_ACC_PUBLIC)
	PHP_ME(WebView, getSettings,       php_webkitgtk_webview_get_settings_info,         ZEND_ACC_PUBLIC)
	PHP_ME(WebView, getTitle,          php_webkitgtk_webview_get_title_info,            ZEND_ACC_PUBLIC)
	PHP_ME(WebView, getURI,            php_webkitgtk_webview_get_uri_info,              ZEND_ACC_PUBLIC)
	PHP_ME(WebView, reload,            php_webkitgtk_webview_reload_info,               ZEND_ACC_PUBLIC)
	PHP_ME(WebView, reloadBypassCache, php_webkitgtk_webview_reload_bypass_cache_info,  ZEND_ACC_PUBLIC)
	PHP_ME(WebView, stopLoading,       php_webkitgtk_webview_stop_loading_info,         ZEND_ACC_PUBLIC)
	PHP_ME(WebView, resize,            php_webkitgtk_webview_resize_info,               ZEND_ACC_PUBLIC)
	PHP_ME(WebView, move,              php_webkitgtk_webview_move_info,                 ZEND_ACC_PUBLIC)
	PHP_ME(WebView, goBack,            php_webkitgtk_webview_go_back_info,              ZEND_ACC_PUBLIC)
	PHP_ME(WebView, goForward,         php_webkitgtk_webview_go_forward_info,           ZEND_ACC_PUBLIC)
	PHP_ME(WebView, canGoBack,         php_webkitgtk_webview_can_go_back_info,          ZEND_ACC_PUBLIC)
	PHP_ME(WebView, canGoForward,      php_webkitgtk_webview_can_go_forward_info,       ZEND_ACC_PUBLIC)
	PHP_ME(WebView, show,              php_webkitgtk_webview_show_info,                 ZEND_ACC_PUBLIC)
	PHP_ME(WebView, hide,              php_webkitgtk_webview_hide_info,                 ZEND_ACC_PUBLIC)
	PHP_ME(WebView, onClose,           php_webkitgtk_webview_close_info,                ZEND_ACC_PROTECTED)
	PHP_ME(WebView, onLoadChanged,     php_webkitgtk_webview_load_changed_info,         ZEND_ACC_PROTECTED)
	PHP_ME(WebView, onLoadFailed,      php_webkitgtk_webview_load_failed_info,          ZEND_ACC_PROTECTED)
	PHP_ME(WebView, onDestroy,         php_webkitgtk_webview_close_info,                ZEND_ACC_PROTECTED)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(WebKitGtk_WebView) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(
		ce, 
		"WebKitGtk", 
		"WebView", 
		php_webkitgtk_webview_methods
	);

	webkitgtkWebView_ce = zend_register_internal_class(&ce);
	webkitgtkWebView_ce->create_object = php_webkitgtk_webview_create;

	memcpy(
		&php_webkitgtk_webview_handlers, 
		zend_get_std_object_handlers(), 
		sizeof(zend_object_handlers)
	);

	php_webkitgtk_webview_handlers.offset = XtOffsetOf(
		php_webkitgtk_webview_t, 
		std
	);

	zend_declare_class_constant_long(
		webkitgtkWebView_ce, 
		ZEND_STRL("LOAD_STARTED"), 
		WEBKIT_LOAD_STARTED
	);

	zend_declare_class_constant_long(
		webkitgtkWebView_ce, 
		ZEND_STRL("LOAD_REDIRECTED"), 
		WEBKIT_LOAD_REDIRECTED
	);

	zend_declare_class_constant_long(
		webkitgtkWebView_ce, 
		ZEND_STRL("LOAD_COMMITTED"), 
		WEBKIT_LOAD_COMMITTED
	);

	zend_declare_class_constant_long(
		webkitgtkWebView_ce, 
		ZEND_STRL("LOAD_FINISHED"), 
		WEBKIT_LOAD_FINISHED
	);

	return SUCCESS;
} /* }}} */

#endif
