<?php
	
	class CtorObject { 
		public $id;
		public $name;
		
		public function __construct($options) {
			$this->id = $options['id'];
			$this->name = $options['name'];
		}
	}