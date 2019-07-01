# PHP-WEBKIT

A php extension that wraps the [webkitgtk](https://webkitgtk.org/) library and 
allows you to start a new web browser instance from your php scripts. For now 
it doesn't offers much but a window where you can load your webui using the php 
built-in webserver or anything else.

A lot of stuff needs to be done and implemented but for now it gives you a web
browser where you can load your PHP/HTML/CSS/JS Apps!

## Example

```php
$view = new WebKit\WebView("The initial Window Title");
$view->loadURI("http://localhost:8080");
$view->show();
```

# Installation

For now the extension only works on Linux since I don't know the whole process
needed to compile gtk and webkitgtk library on anything else...

## Steps to install

```sh
git clone https://github.com/jgmdev/php-webkit
cd webkit
phpize
./configure
make
sudo make install
```

# Credits

I used [krakjoe ui](https://github.com/krakjoe/ui) repository as the boiler 
plate to start developing this extension, thanks to him!