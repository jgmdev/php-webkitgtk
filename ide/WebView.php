<?php

namespace WebKitGtk;

class WebView
{
    const LOAD_STARTED = 0;
    const LOAD_REDIRECTED = 1;
    const LOAD_COMMITTED = 2;
    const LOAD_FINISHED = 3;

    public function __construct(string $title) {}

    public function loadURI(string $uri) {}

    public function loadHTML(string $content, string $base_uri="") {}

    public function loadPlainText(string $plain_text) {}

    public function setIcon(string $filename) {}

    public function setSettings(Settings $settings) {}

    public function setTitle(string $title) {}

    public function getSettings(): Settings {}

    public function getTitle(): string {}

    public function getURI(): string {}

    public function reload(): void {}

    public function reloadBypassCache(): void {}

    public function stopLoading(): void {}

    public function resize(int $width, int $height) {}

    public function move(int $x, int $y) {}

    public function goBack(): void {}

    public function goForward(): void {}

    public function canGoBack(): bool {}

    public function canGoForward(): bool {}

    public function show(): void {}

    public function hide(): void {}

    public function onClose(): void {}

    public function onLoadChanged(int $load_state): void {}

    public function onLoadFailed(
        int $load_state, 
        string $failing_uri, 
        int $error_code, 
        string $error_message
    ): bool {}

    public function onDestroy(): void {}
}