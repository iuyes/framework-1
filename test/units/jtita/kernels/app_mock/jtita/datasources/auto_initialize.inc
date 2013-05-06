<?php
    class AutoInitialize {
        public $initialized = false;

        public function initialize() {
            $this->initialized = true;
        }
    }