<?php
    /**
     * application_request.inc 应用请求对象类声明文件
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
     * 定义JTITA所需的固定参数,如果不使用默认值则可以在configs/core.php 中优先声明；
     */
    defined ('JTPARAM_CONTROLLER') or define('JTPARAM_CONTROLLER', 'controller');
    defined ('JTPARAM_PACKAGE')    or define('JTPARAM_PACKAGE', 'package');
    defined ('JTPARAM_MODULE')     or define('JTPARAM_MODULE', 'module');
    defined ('JTPARAM_ACTION')     or define('JTPARAM_ACTION', 'action');
    defined ('JTPARAM_FORMAT')     or define('JTPARAM_FORMAT', 'format');

    /**
     * 定义默认控制器和活动，如果不使用默认值可以在configs/core.php 中优先声明；
     */
    defined ('JTPARAM_DEFAULT_CONTROLLER') or define('JTPARAM_DEFAULT_CONTROLLER', 'index');
    defined ('JTPARAM_DEFAULT_PACKAGE')    or define('JTPARAM_DEFAULT_PACKAGE', false);
    defined ('JTPARAM_DEFAULT_MODULE')     or define('JTPARAM_DEFAULT_MODULE', false);
    defined ('JTPARAM_DEFAULT_ACTION')     or define('JTPARAM_DEFAULT_ACTION', 'index');
    defined ('JTPARAM_DEFAULT_FORMAT')     or define('JTPARAM_DEFAULT_FORMAT', 'html');
    /**
     * 如果未定义规则参数，那么定义规则参数为'url';
     */
    defined ('JTPARAM_RULE') or define('JTPARAM_RULE', 'url');

    /**
     * 应用请求对象，封装客户的请求信息。
     *
     * @package .applications
     */
    class ApplicationRequest implements ArrayAccess {

        /**
         * 使用$params和$forms初始化ApplicationRequest对象
         *
         * @param array $params 请求参数
         * @param array $forms  请求表单
         *
         * @return void
         */
        public function initialize ($params = array(), $forms = array()) {
            # 合并参数，默认值。
            $this->_params = array_merge (self::$_defaultParams, $params);
            $this->_forms = $forms;
        }

        /**
         * 根据$name获取请求头信息，如果值不存在返回$default值。
         *
         * @param string $name    请求名称
         * @param mixed  $default 默认值
         *
         * @return string|boolean
         */
        public static function header ($name, $default = false) {
            $name = 'HTTP_' . strtoupper (str_replace ('-', '_', $name));
            if (! empty($_SERVER[$name])) {
                return $_SERVER[$name];
            }

            return $default;
        }

        /**
         * 使用$name和$value增加请求参数
         *
         * 如果$name为数组，则数组的KEY为$name，VALUE为$value值。
         *
         * @param array|string $name  参数名称
         * @param mixed|null   $value 参数值
         *
         * @return void
         */
        public function addParams ($name, $value = null) {
            if (is_array ($name)) {
                $this->_params = array_merge ($this->_params, $name);
            }
            else {
                $this->_params[$name] = $value;
            }
        }

        /**
         * 根据$name移动参数信息
         *
         * 如果$name为string则表示单个信息，$name为数组表示移除多个参数。
         *
         * @param string|array $name 参数名称
         *
         * @return void
         */
        public function removeParams ($name) {
            if (is_array ($name)) {
                foreach ($name as $n) {
                    unset($this->_params[$n]);
                }
            }
            else {
                unset($this->_params[$name]);
            }
        }

        /**
         * 根据$name获取参数信息，如果指定参数不存在则返回$default
         *
         * @param string $name    参数名称
         * @param mixed  $default 默认值，默认为false。
         *
         * @return mixed
         */
        public function params ($name = null, $default = false) {
            # 如果设置有指定参数值。
            if (isset($this->_params[$name])) {
                return $this->_params[$name];
            }

            return $default;
        }

        /**
         * 返回所有参数信息；
         */
        public function allParams () {
            return $this->_params;
        }

        /**
         * 根据$name和$value值增加表单参数。
         *
         * 如果$name为数组，则数组的KEY为$name，VALUE为$value值。
         *
         * @param string|array $name  表单参数名称
         * @param mixed|null   $value 表单值
         *
         * @return void
         */
        public function addForms ($name, $value = null) {
            if (is_array ($name)) {
                $this->_forms = array_merge ($this->_forms, $name);
            }
            else {
                $this->_forms[$name] = $value;
            }
        }

        /**
         * 根据$name移除表单信息，如果$name为数组表示移除多个表单值。
         *
         * @param string $name 表单参数名称
         *
         * @return void
         */
        public function removeForms ($name) {
            if (is_array ($name)) {
                foreach ($name as $n) {
                    unset($this->_forms[$n]);
                }
            }
            else {
                unset($this->_forms[$name]);
            }
        }

        /**
         * 根据$name获取表单参数信息，如果指定$name的值不存在则返回$default值。
         *
         * @param string $name    表单参数名称
         * @param mixed  $default 默认值，默认为false
         *
         * @return mixed
         */
        public function forms ($name = null, $default = false) {
            # 如果包含有指定的表单名。
            if (isset($this->_forms[$name])) {
                return $this->_forms[$name];
            }

            return $default;
        }

        /**
         * 返回所有表单信息。
         */
        public function allForms () {
            return $this->_forms;
        }

        /**
         * 获取应用请求的模块名称。
         */
        public function module () {
            return $this->params (JTPARAM_MODULE, JTPARAM_DEFAULT_MODULE);
        }

        /**
         * 获取应用请求的包名称
         */
        public function package () {
            return $this->params (JTPARAM_PACKAGE, JTPARAM_DEFAULT_PACKAGE);
        }

        /**
         * 获取应用请求的控制器名称
         */
        public function controller () {
            return $this->params (JTPARAM_CONTROLLER, JTPARAM_DEFAULT_CONTROLLER);
        }

        /**
         * 获取应用请求的动作名称
         */
        public function action () {
            return $this->params (JTPARAM_ACTION, JTPARAM_DEFAULT_ACTION);
        }

        /**
         * 获取数据清求格式。
         */
        public function format () {
            return $this->params (JTPARAM_FORMAT, JTPARAM_DEFAULT_FORMAT);
        }

        /**
         * 返回请求方法；
         */
        public function method () {
            return Renv::method ();
        }

        /**
         * 使用数组方式访问查询参数是否存在。
         *
         * @implement ArrayAccess::offsetExists
         */
        public function offsetExists ($index) {
            return isset($this->_params[$index]);
        }

        /**
         * 使用数组方式访问查询参数。
         *
         * @implement ArrayAccess::offsetGet
         */
        public function  offsetGet ($index) {
            # 判断查询参数是否存在。
            if (isset($this->_params[$index])) {
                return $this->_params[$index];
            }

            return false;
        }

        /**
         * 使用数组方式访问设置查询参数。
         *
         * @implement ArrayAccess::offsetSet
         */
        public function offsetSet ($index, $newValue) {
            $this->_params[$index] = $newValue;
        }

        /**
         * 使用数组方式，删除查询参数。
         *
         * @implement ArrayAccess::offset
         */
        public function offsetUnset ($index) {
            # 如果是删除，默认参数。
            if (array_key_exists ($index, self::$_defaultParams)) {
                $this->_params[$index] = self::$_defaultParams[$index];
            }
            else {
                unset($this->_params[$index]);
            }
        }

        /**
         * @var array 请求参数信息
         */
        protected $_params = array();

        /**
         * @var array 提交表单参数
         */
        protected $_forms = array();

        /**
         * @var array 默认参数。
         */
        protected static $_defaultParams = array(
            JTPARAM_CONTROLLER => JTPARAM_DEFAULT_CONTROLLER,
            JTPARAM_PACKAGE => JTPARAM_DEFAULT_PACKAGE,
            JTPARAM_ACTION => JTPARAM_DEFAULT_ACTION,
            JTPARAM_MODULE => JTPARAM_DEFAULT_MODULE,
            JTPARAM_FORMAT => JTPARAM_DEFAULT_FORMAT);
    }