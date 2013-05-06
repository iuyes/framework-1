<?php
    /**
     * view_mock.php 视图模拟对象；
     */

    /**
     * 应用请求模拟对象；
     */
    class NormalViewRequestMock {
        private $_query = array();

        public function __construct ($query = array()) {
            $this->_query = $query;
        }

        public function module () { return $this->params ('module'); }

        public function package () { return $this->params ('package'); }

        public function controller () { return $this->params ('controllers'); }

        public function action () { return $this->params ('action'); }

        public function params ($name, $default = false) {
            if (isset($this->_query[$name])) {
                return $this->_query[$name];
            }

            return $default;
        }

    }

    /**
     * 控制器模拟对象，提供控制模拟服务。
     *
     * @package jtita.views
     */
    class ViewControllerMock {
        public $request;
        public $response;

        public function __construct ($query = array()) {
            $this->request = new NormalViewRequestMock($query);
            $this->response = new ApplicationResponse();
        }

        public function query ($name, $default = false) {
            return $this->request->params ($name, $default);
        }
    }