<?php

chdir(__DIR__);

$view = new WebKit\WebView("My Window");

$view->loadURI("http://google.com/");
$view->setIcon("icon.svg");
$view->resize(1000, 780);
$view->move(0, 0);
$view->show();

$view->resize(1000, 780);
$view->show();

$view->loadHTML("<h1>Hello World</h1>");
$view->setTitle("Testing...");
$view->show();
