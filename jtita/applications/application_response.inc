<?php
    /**
     * application_response.inc 应用请求响应类声明文件
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.jtita.com/license.txt
     *
     * @package     jtita.applications
     * @version     1.0 beta
     *
     * @create      2012-07-23
     * @modify      2013-04-07
     */

    /**
     * 应用请求响应类，响应客户的请求信息。
     *
     * @package jtita.applications
     */
    class ApplicationResponse {
        /**
         * @var int 响应编码
         */
        protected $_status = false;

        /**
         * @var string 内容类型，默认为false，表示不控制；
         */
        protected $_contentType = false;

        /**
         * @var string 缓存状态。
         */
        protected $_cache = false;

        /**
         * @var array 响应的headers集合；
         */
        protected $_headers = array();

        /**
         * @var string 响应主体。
         */
        protected $_body = '';

        /**
         * 获取或设置响应状态码，如果$statusCode为空表示获取状态码。
         *
         * @param int|null $status 响应状态码
         *
         * @return mixed
         */
        public function status($status = null) {
            # 如果指定状态码为空值。
            if (!is_null($status)) {
                $this->_status = $status;
            }

            return $this->_status;
        }

        /**
         * 获取或设置响应的MINE类型
         *
         * @param string|null $contentType 内容类型
         *
         * @return ApplicationResponse
         */
        public function contentType($contentType = null) {
            # 如果contentType不为空
            if (!is_null($contentType)) {
                $this->_contentType = $contentType;
            }

            return $this->_contentType;
        }

        /**
         * 设置或获取响应缓存状态，如果$cache参数为空表示获取值。
         *
         * @param mixed|null $cache 缓存值；
         *
         * @return mixed
         */
        public function cache($cache = null) {
            # 如果设置缓存值不为空。
            if (!is_null($cache)) {
                $this->_cache = $cache;
            }

            return $this->_cache;
        }

        /**
         * 使用$time开启缓存。
         *
         * @param string $control 控制内容
         *
         * @return ApplicationResponse
         */
        public function enableCache($control = 'max-age=20') {
            # 设置缓存值
            $this->_cache = array( 'cache-control' => $control );

            # 返回当前实例
            return $this;
        }

        /**
         * 禁用缓存
         *
         * @return ApplicationResponse
         */
        public function disableCache() {
            $this->_cache = array(
                'cache-control' => 'no-cache',
                'Pragma'        => 'no-cache'
            );

            # 返回实例
            return $this;
        }

        /**
         * 设置或获取自定义响应头信息。
         *
         * @example
         *  <pre>
         *        $response->header('cache-control','no-cache');
         *  </pre>
         *
         * @param mixed $name  头名称
         * @param mixed $value 值
         *
         * @return mixed
         */
        public function header($name = null, $value = null) {
            # 如果$value值为空，表示获取指定$name的值。
            if (!is_null($value)) {
                $this->_headers[$name] = $value;
            }

            return $this->_headers[$name];
        }

        /**
         * 返回全部自定义headers信息；
         */
        public function allHeaders() {
            return $this->_headers;
        }

        /**
         * 获取或设置$body信息 ；
         *
         * @param string|mixed $body 内容主题；
         *
         * @return string|mixed
         */
        public function body($body = null) {
            if (!is_null($body)) {
                $this->_body = $body;
            }

            return $this->_body;
        }

        /**
         * 输出响应内容；
         */
        public function render() {
            $this->_headersRender();
            $this->_bodyRender();
        }

        /**
         * 头节点绘制输出；
         */
        protected function _headersRender() {
            # 输出状态
            if ($status = $this->_status) {
                header('Status:' . $status);
            }

            # 输出缓存控制
            if ($cache = $this->_cache) {
                $this->_headerRender($cache, 'Cache-Control');
            }

            # 输出mine
            if ($contentType = $this->_contentType) {
                header('Content-type:' . $contentType);
            }

            # 输出自定义头信息；
            foreach ($this->_headers as $name => $value) {
                $this->_headerRender($value, $name);
            }
        }

        /**
         * 使用$value和$name输出头信息。
         *
         * @param mixed|string $value 头节点值；
         * @param string       $name  名称；
         *
         * @return void
         */
        protected function _headerRender($value, $name) {
            # 如果是数组值。
            if (is_array($value)) {
                foreach ($value as $n => $v) {
                    header($n . ':' . $v);
                }
            } else {
                header($name . ':' . $value);
            }
        }

        /**
         * 主题内容输出；
         */
        protected function _bodyRender() {
            echo $this->_body;
        }
    }