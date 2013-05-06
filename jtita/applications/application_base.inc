<?php
    /**
     * application_base.inc 声明应用前端控制器服务类。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.applications
     * @version     1.0 beta
     *
     * @create      2012-07-23
     * @modify      2013-04-28
     */

    /*
    * 控制器未找到异常，当指定的控制器未找到时引发本异常。
    *
    * @package jtita.applications
    */
    class ControllerNotFoundException extends JtitaException{
    }

    /**
     * 活动未找到异常，指定控制器中的活动不存在时引发本异常
     *
     * @package jtita.applications
     */
    class ActionNotFoundException extends JtitaException{
    }

    /*
     * 应用前端控制器基础服务类，提供应用前端控制的基础服务。
     *
     * @package jtita.applications
     */
    class ApplicationBase{
        /**
         * @const string 未找到控制器，当指定控制器未找到时跳转到该控制器。
         */
        const NOT_FOUND_CONTROLLER = 'controllers.not_found_controller';

        /**
         * @const string 错误控制器，当发生错误时跳转到该控制器
         */
        const ERROR_CONTROLLER = 'controllers.error_controller';

        /**
         * @const string 默认应用请求类
         */
        const DEFAULT_REQUEST = 'applications.application_request';

        /**
         * @const string 默认应用响应类
         */
        const DEFAULT_RESPONSE = 'applications.application_response';

        /**
         * @const string 应用请求参数选项
         */
        const OPT_REQUEST = 'request';

        /**
         * @const string 应用响应参数选项；
         */
        const OPT_RESPONSE = 'response';

        /**
         * 使用$options执行应用前端初始化
         *
         * @param array $options 初始化选项
         *
         * @return void
         */
        public function initialize ($options = array()) {
            $this->_initializeOptions ($options);
            $this->_initializeRequest ();
            $this->_initializeResponse ();
            $this->_initializeRouting ();
            $this->_initializeConfig ();
        }

        /**
         * 执行控制器分并输出响应结果。
         */
        public function dispatching () {
            # 获取控制器。
            $controller = $this->_controller ($this->_request);
            $controller->execute ($this->_request, $this->_response);

            # 输出响应内容
            $this->_response->render ();
        }

        /**
         * 错误执行方法，当发生错误时将调用本方法处理错误信息。
         *
         * @param Exception $ex 异常信息
         *
         * @return void
         */
        public function error ($ex) {
            # 如果发生错误时，还未初始化Controller。
            Jtita::importInterface ('controllers.controller');

            # 获取错误处理控制器并回调控制器。
            $ctlError = Jtita::create (self::ERROR_CONTROLLER);

            # 执行错误显示
            $ctlError->exception = $ex;
            $ctlError->execute ($this->_request, $this->_response);

            # 输出响应内容。
            $this->_response->render ();
        }

        /**
         * 应用结束方法，当整个应用执行完成后调用本方法。
         *
         * @return void
         */
        public function end () {

        }

        /**
         * 根据$clsName获取应用前端控制器的唯一实例 。
         *
         * @param string $className 应用前端控制器类名称，默认为appservs.application。
         *
         * @return ApplicationBase
         */
        public static function instance ($className = 'appservs.application') {
            # 判断是否已经创建唯一实例对象。
            if (self::$_instance == false){
                self::$_instance = Jtita::create ($className);
            }

            return self::$_instance;
        }

        /**
         * 获取当前应用请求对象。
         *
         * @return ApplicationRequest
         */
        public static function request () {
            return self::$_instance->_request;
        }

        /**
         * 获取当前应用请求最后返回的响应对象，如果未处理完成将返回false。
         *
         * @return ApplicationResponse
         */
        public static function response () {
            return self::$_instance->_response;
        }

        /**
         * 使用$options初始化应用选项信息。
         *
         * @param array $options 初始化选项
         *
         * @return void
         */
        protected function _initializeOptions ($options) {
            $this->_options = array_merge (self::$_defaultOptions, $options);
        }

        /**
         * 初始化应用请求对象
         */
        protected function _initializeRequest () {
            $this->_request = Jtita::create ($this->_options[self::OPT_REQUEST]);
        }

        /**
         * 初始化应用响应对象
         */
        protected function _initializeResponse () {
            $this->_response = Jtita::create ($this->_options[self::OPT_RESPONSE]);
        }

        /**
         * 初始化配置信息。
         */
        protected function _initializeConfig () {

        }

        /**
         * 初始化路由服务
         */
        protected function _initializeRouting () {
            # 导入路由服务和路由配置信息
            Jtita::import ('routers.router');
            Jtita::import ('boots.routing');

            # 获取请求参数
            $params = Router::match ($_GET);

            # 如果匹配失败
            if (! $params){
                $params = $this->_notFoundParams();
            }

            # 初始化请求
            $request = $this->_request;
            $request->initialize ($params, $_POST);

            # 初始化路中默认参数值
            $basicParams = array(JTPARAM_CONTROLLER => $request->controller (),
                                 JTPARAM_ACTION => $request->action ());
            if ($package = $request->package ()) {
                $basicParams[JTPARAM_PACKAGE] = $package;
            }
            if ($module = $request->module ()) {
                $basicParams[JTPARAM_MODULE] = $module;
            }

            Router::defaultUrlParams ($basicParams, $params);
        }

        /**
         * 根据$request获取控制器，如果指定控制器不存在则返回false。
         *
         * @param ApplicationRequest $request 应用请求对象
         *
         * @return Controller|boolean
         */
        protected function _controller ($request) {
            # 导入应用控制器文件和应用模型文件。
            Jtita::import ('appservs.application_controller');
            Jtita::import ('appservs.application_model');

            # 获取控制器类名称
            $ctlName = $this->_controllerName ($request);
            $ctlPath = $this->_controllerPath ($request);

            # 如果找到应用控制器，则创建应用控制器。
            if (Jtita::lookup ($ctlName, Jtita::DEFAULT_FILE_EXT, $ctlPath)){
                return Jtita::create ($ctlName);
            }

            # 如果未找到控制器，则返回默认未找到控制器。
            return Jtita::create (self::NOT_FOUND_CONTROLLER, Jtita::DEFAULT_FILE_EXT, array(JTPATH_APP, JTPATH_JTITA));
        }

        /**
         * 根据$request获取请求的控制器类名称并返回。
         *
         * @param ApplicationRequest $request 应用请求。
         *
         * @return string
         */
        protected function _controllerName ($request) {
            # index_controller
            $className = $request->controller () . '_controller';
            # admins.index_controller
            if ($package = $request->package ()){
                $className = ($package . '.' . $className);
            }

            # controllers.admins.index_controller
            $className = ('controllers.' . $className);
            # members.controllers.admins.index_controller
            if ($module = $request->module ()){
                $className = ($module . '.' . $className);
            }

            return $className;
        }

        /**
         * 根据$request获取控制器路径。
         *
         * @param ApplicationRequest $request 应用请求。
         *
         * @return string
         */
        protected function _controllerPath ($request) {
            return ($request->module ()) ? JTPATH_MODULE : JTPATH_APP;
        }

        /**
         * @return array 返回参数为未找到参数。
         */
        protected function _notFoundParams() {
            return array(JTPARAM_CONTROLLER => self::NOT_FOUND_CONTROLLER,
                         JTPARAM_ACTION => JTPARAM_DEFAULT_ACTION,
                         JTPARAM_PACKAGE => JTPARAM_DEFAULT_PACKAGE,
                         JTPARAM_MODULE  => JTPARAM_DEFAULT_MODULE);
        }

        /**
         * @var array 声明默认选项参数
         */
        protected static $_defaultOptions = array(self::OPT_REQUEST => self::DEFAULT_REQUEST,
                                                  self::OPT_RESPONSE => self::DEFAULT_RESPONSE);


        /**
         * @var ApplicationBase|boolean 唯一实例对象
         */
        protected static $_instance = false;

        /**
         * @var array 初始化选项
         */
        protected $_options = array();

        /**
         * @var ApplicationRequest|boolean 应用请求对象
         */
        protected $_request = false;

        /**
         * @var ApplicationResponse|boolean 应用响应对象
         */
        protected $_response = false;
    }