<?php

chdir(__DIR__);

$view = new WebKitGtk\WebView("My Window");

$view->loadURI("http://slashdot.org/");
$view->setIcon("icon.svg");
$view->setTitle("Loading...");
$settings = $view->getSettings();
$settings->setAutoLoadImages(false);
$settings->setEnableSmoothScrolling(true);
$settings->setHardwareAccelerationPolicy(
    WebKitGtk\Settings::HARDWARE_ACCELERATION_POLICY_ALWAYS
);
$view->show();
