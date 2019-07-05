<?php

namespace WebKitGtk;

class Settings
{
    const HARDWARE_ACCELERATION_POLICY_ON_DEMAND = 0;
    const HARDWARE_ACCELERATION_POLICY_ALWAYS = 1;
    const HARDWARE_ACCELERATION_POLICY_NEVER = 2;

    public function getAutoLoadImages(): bool {}

    public function setAutoLoadImages(bool $enabled){}
        
    public function getEnableJavaScript(): bool {}

    public function setEnableJavaScript(bool $enabled){}
        
    public function getEnableSmoothScrolling(): bool {}

    public function setEnableSmoothScrolling(bool $enabled){}
        
    public function getEnableAccelerated2dCanvas(): bool {}

    public function setEnableAccelerated2dCanvas(bool $enabled){}
        
    public function getHardwareAccelerationPolicy(): int {}

	public function setHardwareAccelerationPolicy(int $policy){}
}