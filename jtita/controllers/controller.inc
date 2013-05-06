<?php
    /**
     * controller.inc 控制器类声明文件
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.controllers
     * @version     1.0 beta
     *
     * @create      2012-07-22
     * @modify      2013-04-28
     */

    /**
     * @const string JT消息通知，普通消息；
     */
    define('JTMESSAGE_INFO', 'info');

    /**
     * @const string JT消息通知，错误消息
     */
    define('JTMESSAGE_ERROR', 'error');

    /**
     * @const string JT消息通知，警告消息
     */
    define('JTMESSAGE_WARNING', 'warning');

    /**
     * @const string JT消息通知，成功完成；
     */
    define('JTMESSAGE_SUCCESS', 'success');

    /**
     * @const string JT消息通知，消息失败。
     */
    define("JTMESSAGE_FAIL", 'fail');

    /**
     * 过滤器接口，提供控制器额外的过滤检查。
     *
     * @说明
     *      过滤不仅仅允许执行过滤检查，还允许执行其它相关的服务。
     *
     * @package jtita.controllers
     */
    interface Filter {
        /**
         * 活动执行前，返回true将继续执行false表示中止执行。
         *
         * @param Controller $controller   控制器
         * @param string     $actionMethod 动作方法
         *
         * @return bool
         */
        function execute ($controller, $actionMethod);

        /**
         * 内容绘制前执行，返回true将继续执行false表示中止执行。
         *
         * @param Controller $controller 控制器
         * @param array      $options    绘制选项
         *
         * @return bool
         */
        function render ($controller, &$options);
    }

    /**
     * 控制器对象，提供Web响应处理。
     *
     * @package jtita.controllers
     */
    abstract class Controller {
        /**
         * @const 默认视图类名；
         */
        const DEFAULT_VIEW_CLASS = 'views.normal_view';

        /**
         * @const 默认错误显示模板；
         */
        const DEFAULT_ERROR_TEMPLATE = '_errors.error';

        /**
         * @var ApplicationRequest 请求封装对象；
         */
        public $request = null;

        /**
         * @var ApplicationResponse 请求响应对象；
         */
        public $response = null;

        /**
         * @var bool 自动绘制选项;
         */
        public $autoRender = true;

        /**
         * @var array 过滤器声明
         */
        public $filters = array();

        /**
         * 根据$request和$response执行控制器响应。
         *
         * @param ApplicationRequest  应用请求对象；
         * @param ApplicationResponse 应用响应对象；
         *
         * @return void
         */
        public function execute ($request, $response) {
            # 初始化请求并分发活动方法。
            $this->_initialize ($request, $response);
            $actionMethod = $this->_actionMethod ();

            if (method_exists ($this, $actionMethod)) {
                if ($this->_actionFilters ($actionMethod)) {
                    $this->_actionInvoke ($actionMethod);
                }
            }
            else {
                $this->blank ($actionMethod);
            }
        }

        /**
         * 返回当前请求的查询参数$name的值，如果未包含则返回$default；
         *
         * @param string $name    查询关键字；
         * @param mixed  $default 默认值；
         *
         * @return mixed
         */
        public function query ($name, $default = false) {
            return $this->request->params ($name, $default);
        }

        /**
         * 返回当前提供表单$name的值，如果未包含则返回$default；
         *
         * @param string $name    表单域名字；
         * @param mixed  $default 默认值；
         *
         * @return mixed
         */
        public function forms ($name, $default = false) {
            return $this->request->forms ($name, $default);
        }

        /**
         * 返回一个布尔值，表示当前请求是否为提交；
         *
         * @return boolean
         */
        public function isPost () {
            return Renv::isM ('post');
        }

        /**
         * 返回一个布求值，表示当前请求是否为GET请求；
         *
         * @return boolean
         */
        public function isGet () {
            return Renv::isM ('get');
        }

        /**
         * 利用PHP魔术方法，设置视图变量；
         *
         * @param string $name  变量名称
         * @param mixed  $value 变量值
         *
         * @return void
         */
        public function __set ($name, $value) {
            $this->_viewVars[$name] = $value;
        }

        /**
         * 利用PHP魔术方法，获取视图变量；
         *
         * @param string $name 变量名称
         *
         * @return mixed
         */
        public function __get ($name) {
            return $this->_viewVars[$name];
        }

        /**
         * 返回一个布尔值，表示是否存在指定的视图变量，存在返回true,否则返回false。
         *
         * @param string $name 变量名称
         *
         * @return boolean
         */
        public function hasVar ($name) {
            return isset($this->_viewVars[$name]);
        }

        /**
         * 根据$options绘制控制器结果；
         *
         * @param array $options   视图选项；
         *
         * @return boolean
         */
        public function render ($options = array()) {
            # 如果选项是字符串，则设置指定为活动；
            if (is_string ($options)) {
                $options = array(JTPARAM_ACTION => $options);
            }

            # 已经绘制，不在需要自动绘制；
            $this->autoRender = false;
            if ($this->_renderFilters ($options)) {
                $this->_renderInvoke ($options);
            }

            return true;
        }

        /**
         * 设置视图响应消息。
         *
         * @param string|array $message 消息内容
         * @param string       $sign    消息标识
         * @return void
         */
        public function message ($message, $sign = JTMESSAGE_INFO) {
            # 如果指定消息不是数组值。
            if (! is_array ($message)) {
                $message = array('message' => $message, 'sign' => $sign);
            }

            $this->_viewMessage = $message;
        }

        /**
         * 返回视图IOC创建信息。
         *
         * @return array
         */
        public function view () {
            return self::DEFAULT_VIEW_CLASS;
        }

        /**
         * 如果指定的$actionMethod未找到时，将执行本方法；
         *
         * @param string $actionMethod 活动方法；
         * @return boolean
         *
         * @throws ActionNotFoundException
         */
        public function blank ($actionMethod) {
            # 如果是测试模式，则产生异常。
            if (JTDEBUG > 0) {
                throw new ActionNotFoundException('JTT0101:Not found action :' . $actionMethod);
            }
            else {
                return $this->error (404);
            }
        }

        /**
         * 根据$error和$options输出错误消息；
         *
         * @param string      $error    错误代码；
         *
         * @return boolean
         */
        public function error ($error) {
            return $this->render (array('module' => false, 'package' => false,
                'template' => array('_errors.' . $error, self::DEFAULT_ERROR_TEMPLATE,)));
        }

        /**
         * 根据$url执行跳转服务。
         *
         * @param mixed|string|array $url   跳转路径；
         * @param int                $delay 跳转秒数；
         *
         * @return boolean
         */
        public function redirect ($url, $delay = 0) {
            print_r($url);
            # 生成完成URL值；
            if (is_array ($url)) {
                $url = Router::url ($url);
            }
            else {
                $url = Router::url (array('action' => $url));
            }

            # 设置视图变量；
            $this->url = $url;
            $this->delay = $delay;

            return $this->render (array('module' => false, 'package' => false, 'controller' => '_funs',
                'action' => 'redirect' # 'template' => '_funs.redirect'
            ));
        }

        /**
         * 使用$request和$response执行控制器初始化；
         *
         * @param ApplicationRequest  应用请求对象
         * @param ApplicationResponse 应用响应对象；
         */
        protected function _initialize ($request, $response) {
            $this->response = $response;
            $this->request = $request;
        }

        /**
         * 返回action的执行方法，未找到或不是Action方法返回false。
         *
         * @return string|boolean
         */
        protected function _actionMethod () {
            return camel_upper ($this->request->action ()) . 'Action';
        }

        /**
         * 根据$actionMethod执行过滤方法，返回false表示结束处理。
         *
         * @param string $actionMethod 活动方法。
         *
         * @return boolean
         */
        protected function _actionFilters ($actionMethod) {
            foreach ($this->_objFilters () as $filter) {
                if ($filter->execute ($this, $actionMethod) === false) {
                    return false;
                }
            }

            return true;
        }

        /**
         * 根据$options执行绘制前过滤，返回false表示结束处理。
         *
         * @param mixed $options
         *
         * @return boolean
         */
        protected function _renderFilters (&$options) {
            foreach ($this->_objFilters () as $filter) {
                if ($filter->render ($this, $options) === false) {
                    return false;
                }
            }
            return true;
        }

        /**
         * 执行活动方法$actionMethod。
         *
         * @param string $actionMethod 活动方法名称；
         */
        protected function _actionInvoke ($actionMethod) {
            # 执行活动方法，并返回响应
            $this->$actionMethod();

            # 如果自动绘制；
            if ($this->autoRender) {
                $this->render ();
            }
        }

        /**
         * 根据$options执行模板的绘制处理；
         *
         * @param array $options 视图选项；
         *
         * @return void
         */
        protected function _renderInvoke (&$options) {
            $view = $this->_createView ($options);
            $this->response->body ($view->render ());
        }

        /**
        $    * 返回当前控制器的全部过滤器。
        $        *
         * @r�turn array
         */
        protected function _objFilters () {
            # 如果过滤器对象还没有创建
            if ($this->_objFilters === false) {
                $this->_objFilters = array();
                foreach ($this->filters as $name => $iocFilter) {
                    $this->_objFilters[$name] = Jtita::ioc ($iocFilter);
                }
            }

            return $this->_objFilters;
        }

        /**
         * 根据$options创建并返回视图对象。
         *
         * @param array $options 初始化选项；
         *
         * @return View
         */
        protected function _createView ($options = array()) {
            $view = Jtita::ioc ($this->view ());

            # 倝始化覆图信息
            $view->initialize ($this, $options);
            $view->message ($this->_viewMessage);
            $view->vars ($this->_viewVars);

            return $view;
        }

        /**
         * @var array 视图变量传递选项；
         */
        protected $_viewVars = array();

        /**
         * @var bool|array 处理响应消息。
         */
        protected $_viewMessage = false;

        /**
         * @var bool|array
         */
        protected $_objFilters = false;
    }