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
#ifndef HAVE_php_WEBKITGTK_SETTINGS
#define HAVE_php_WEBKITGTK_SETTINGS

#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

#include "php.h"

#include <classes/settings.h>

zend_object_handlers php_webkitgtk_settings_handlers;

zend_class_entry* webkitgtkSettings_ce;

zval* php_webkitgtk_settings_construct(zval* settings, WebKitSettings* native)
{
    php_webkitgtk_settings_t* s;

	object_init_ex(settings, webkitgtkSettings_ce);

    s = php_webkitgtk_settings_fetch(settings);

    if(s->settings)
    {
        g_free(s->settings);
    }

    s->this = settings;
    s->settings = native;

	return settings;
}

zend_object* php_webkitgtk_settings_create(zend_class_entry *ce) {
	php_webkitgtk_settings_t* s = (php_webkitgtk_settings_t*) 
		ecalloc(
			1, 
			sizeof(php_webkitgtk_settings_t) + zend_object_properties_size(ce)
		)
	;

	zend_object_std_init(&s->std, ce);

	object_properties_init(&s->std, ce);

	s->std.handlers = &php_webkitgtk_settings_handlers;

	return &s->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_settings_construct_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Settings Settings::__construct() */
PHP_METHOD(Settings, __construct) 
{
	php_webkitgtk_settings_t* s = php_webkitgtk_settings_fetch(getThis());

	s->this = getThis();

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

    s->settings = webkit_settings_new();
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_webkitgtk_settings_get_auto_load_images_info, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool Settings::getAutoLoadImages(void) */
PHP_METHOD(Settings, getAutoLoadImages)
{
	php_webkitgtk_settings_t* s = php_webkitgtk_settings_fetch(getThis());
	
	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_BOOL(
        (zend_bool) webkit_settings_get_auto_load_images(s->settings)
    )
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_settings_set_auto_load_images_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, enabled, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Settings::setAutoLoadImages(bool enabled) */
PHP_METHOD(Settings, setAutoLoadImages)
{
	php_webkitgtk_settings_t* s = php_webkitgtk_settings_fetch(getThis());
	zend_bool enabled;
	
	if (
		zend_parse_parameters_throw(
			ZEND_NUM_ARGS(), 
			"b", 
			&enabled
		) != SUCCESS
	) {
		return;
	}

	webkit_settings_set_auto_load_images(s->settings, (gboolean) enabled);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_webkitgtk_settings_get_enable_javascript_info, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool Settings::getEnableJavaScript(void) */
PHP_METHOD(Settings, getEnableJavaScript)
{
	php_webkitgtk_settings_t* s = php_webkitgtk_settings_fetch(getThis());
	
	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_BOOL(
        (zend_bool) webkit_settings_get_enable_javascript(s->settings)
    )
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_settings_set_enable_javascript_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, enabled, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Settings::setEnableJavaScript(bool enabled) */
PHP_METHOD(Settings, setEnableJavaScript)
{
	php_webkitgtk_settings_t* s = php_webkitgtk_settings_fetch(getThis());
	zend_bool enabled;
	
	if (
		zend_parse_parameters_throw(
			ZEND_NUM_ARGS(), 
			"b", 
			&enabled
		) != SUCCESS
	) {
		return;
	}

	webkit_settings_set_enable_javascript(s->settings, (gboolean) enabled);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_webkitgtk_settings_get_enable_smooth_scrolling_info, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool Settings::getEnableSmoothScrolling(void) */
PHP_METHOD(Settings, getEnableSmoothScrolling)
{
	php_webkitgtk_settings_t* s = php_webkitgtk_settings_fetch(getThis());
	
	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_BOOL(
        (zend_bool) webkit_settings_get_enable_smooth_scrolling(s->settings)
    )
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_settings_set_enable_smooth_scrolling_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, enabled, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Settings::setEnableSmoothScrolling(bool enabled) */
PHP_METHOD(Settings, setEnableSmoothScrolling)
{
	php_webkitgtk_settings_t* s = php_webkitgtk_settings_fetch(getThis());
	zend_bool enabled;
	
	if (
		zend_parse_parameters_throw(
			ZEND_NUM_ARGS(), 
			"b", 
			&enabled
		) != SUCCESS
	) {
		return;
	}

	webkit_settings_set_enable_smooth_scrolling(s->settings, (gboolean) enabled);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_webkitgtk_settings_get_enable_accelerated_2d_canvas_info, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool Settings::getEnableAccelerated2dCanvas(void) */
PHP_METHOD(Settings, getEnableAccelerated2dCanvas)
{
	php_webkitgtk_settings_t* s = php_webkitgtk_settings_fetch(getThis());
	
	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_BOOL(
        (zend_bool) webkit_settings_get_enable_accelerated_2d_canvas(s->settings)
    )
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_settings_set_enable_accelerated_2d_canvas_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, enabled, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Settings::setEnableAccelerated2dCanvas(bool enabled) */
PHP_METHOD(Settings, setEnableAccelerated2dCanvas)
{
	php_webkitgtk_settings_t* s = php_webkitgtk_settings_fetch(getThis());
	zend_bool enabled;
	
	if (
		zend_parse_parameters_throw(
			ZEND_NUM_ARGS(), 
			"b", 
			&enabled
		) != SUCCESS
	) {
		return;
	}

	webkit_settings_set_enable_accelerated_2d_canvas(s->settings, (gboolean) enabled);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_webkitgtk_settings_get_hardware_acceleration_policy_info, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto int Settings::getHardwareAccelerationPolicy(void) */
PHP_METHOD(Settings, getHardwareAccelerationPolicy)
{
	php_webkitgtk_settings_t* s = php_webkitgtk_settings_fetch(getThis());
	
	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_LONG(
        (zend_long) webkit_settings_get_hardware_acceleration_policy(s->settings)
    )
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_webkitgtk_settings_set_hardware_acceleration_policy_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, policy, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Settings::setHardwareAccelerationPolicy(bool enabled) */
PHP_METHOD(Settings, setHardwareAccelerationPolicy)
{
	php_webkitgtk_settings_t* s = php_webkitgtk_settings_fetch(getThis());
	zend_long policy;
	
	if (
		zend_parse_parameters_throw(
			ZEND_NUM_ARGS(), 
			"l", 
			&policy
		) != SUCCESS
	) {
		return;
	}

	webkit_settings_set_enable_accelerated_2d_canvas(
        s->settings, 
        (WebKitHardwareAccelerationPolicy) policy
    );
} /* }}} */

/* {{{ */
const zend_function_entry php_webkitgtk_settings_methods[] = {
	PHP_ME(Settings, __construct,          php_webkitgtk_settings_construct_info,                ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(Settings, getAutoLoadImages,    php_webkitgtk_settings_get_auto_load_images_info,     ZEND_ACC_PUBLIC)
	PHP_ME(Settings, setAutoLoadImages,    php_webkitgtk_settings_set_auto_load_images_info,     ZEND_ACC_PUBLIC)
    PHP_ME(Settings, getEnableJavaScript,  php_webkitgtk_settings_get_enable_javascript_info,    ZEND_ACC_PUBLIC)
	PHP_ME(Settings, setEnableJavaScript,  php_webkitgtk_settings_set_enable_javascript_info,    ZEND_ACC_PUBLIC)
    PHP_ME(Settings, getEnableSmoothScrolling,  php_webkitgtk_settings_get_enable_smooth_scrolling_info,    ZEND_ACC_PUBLIC)
	PHP_ME(Settings, setEnableSmoothScrolling,  php_webkitgtk_settings_set_enable_smooth_scrolling_info,    ZEND_ACC_PUBLIC)
    PHP_ME(Settings, getEnableAccelerated2dCanvas,  php_webkitgtk_settings_get_enable_accelerated_2d_canvas_info,    ZEND_ACC_PUBLIC)
	PHP_ME(Settings, setEnableAccelerated2dCanvas,  php_webkitgtk_settings_set_enable_accelerated_2d_canvas_info,    ZEND_ACC_PUBLIC)
    PHP_ME(Settings, getHardwareAccelerationPolicy,    php_webkitgtk_settings_get_hardware_acceleration_policy_info,     ZEND_ACC_PUBLIC)
	PHP_ME(Settings, setHardwareAccelerationPolicy,    php_webkitgtk_settings_set_hardware_acceleration_policy_info,     ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(WebKitGtk_Settings) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(
		ce, 
		"WebKitGtk", 
		"Settings", 
		php_webkitgtk_settings_methods
	);

	webkitgtkSettings_ce = zend_register_internal_class(&ce);
	webkitgtkSettings_ce->create_object = php_webkitgtk_settings_create;

	memcpy(
		&php_webkitgtk_settings_handlers, 
		zend_get_std_object_handlers(), 
		sizeof(zend_object_handlers)
	);

	php_webkitgtk_settings_handlers.offset = XtOffsetOf(
		php_webkitgtk_settings_t, 
		std
	);

    zend_declare_class_constant_long(
		webkitgtkSettings_ce, 
		ZEND_STRL("HARDWARE_ACCELERATION_POLICY_ON_DEMAND"), 
		WEBKIT_HARDWARE_ACCELERATION_POLICY_ON_DEMAND
	);

	zend_declare_class_constant_long(
		webkitgtkSettings_ce, 
		ZEND_STRL("HARDWARE_ACCELERATION_POLICY_ALWAYS"), 
		WEBKIT_HARDWARE_ACCELERATION_POLICY_ALWAYS
	);

	zend_declare_class_constant_long(
		webkitgtkSettings_ce, 
		ZEND_STRL("HARDWARE_ACCELERATION_POLICY_NEVER"), 
		WEBKIT_HARDWARE_ACCELERATION_POLICY_NEVER
	);

	return SUCCESS;
} /* }}} */

#endif
