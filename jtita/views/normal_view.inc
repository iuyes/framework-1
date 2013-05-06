<?php
    /**
     * normal_view.inc 提供普通视图类声明。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.views
     * @version     1.0 beta
     *
     * @create      2012-08-14
     * @modify      2013-04-28
     */

    # 导入视图接口。
    Jtita::importInterface ('views.view');

    /**
     * 普通视图对象，提供常规的视图显示服务
     *
     * @package jtita.views
     */
    class NormalView implements View {
        /**
         * 实现方法，使用$controllers、$vars和$options初始化视图对象
         *
         * @see View::initialize
         */
        public function initialize ($context, $options = array()) {
            $this->_initializeOptions ($context->request, $options);
            $this->_response = $context->response;
            $this->_request = $context->request;
            $this->_context = $context;
        }

        /**
         * 实现方法，读取和写入视图消息为$message。
         *
         * @see View::message
         */
        public function message ($message = null) {
            if (! is_null ($message)) {
                $this->_message = $message;
            }

            return $this->_message;
        }

        /**
         * 实现方法，读取或设置视图相关变量vars；
         *
         * @see View::vars
         */
        public function vars ($vars = null) {
            if (! is_null ($vars)) {
                $this->_vars = $vars;
            }

            return $this->_vars;
        }

        /**
         * 输出模板显示的内容
         *
         * @see View::render
         */
        public function render () {
            # 获取模板文件和模板路径。
            $files = $this->_templateFiles ();
            $paths = $this->_templatePaths ();

            # 在模板路径中找出模板文件。
            $templateFile = false;
            foreach ($files as $file) {
                $templateFile = Jtita::lookup ($file, self::TEMPLATE_EXT, $paths);
                # 如果找到模板文件
                if ($templateFile) break;
            }

            # 如果模板文件未找到，引发视图异常。
            if (! $templateFile) {
                throw new ViewException('Normal View - Not found template file: ' . implode ($files, ' or ') . ' at ' . implode ($paths, ','));
            }

            # 重新设置内容集、布局文件和布局选项
            $this->_contents = array();
            $this->_layoutFile = false;
            $this->_layoutVars = array();

            # 执行输出
            return self::invokeRender ($this, $templateFile);
        }

        /**
         * ArrayAccess 判断变量$index指定的视图变量是否存在
         *
         * @param mixed $index
         *
         * @return boolean
         */
        public function offsetExists ($index) {
            return isset($this->_vars[$index]);
        }

        /**
         * ArrayAccess 获取$index指定的视图变量信息，如果不存在则返回null
         *
         * @param mixed $index 变量名称
         *
         * @return mixed|null
         */
        public function offsetGet ($index) {
            if (isset($this->_vars[$index])) {
                return $this->_vars[$index];
            }

            return null;
        }

        /**
         * ArrayAccess 设置$index指定的视图变量值为$value。
         *
         * @param string $index 视图变量名称
         * @param mixed  $value 视图变量值
         *
         * @return void
         */
        public function offsetSet ($index, $value) {
            $this->_vars[$index] = $value;
        }

        /**
         * ArrayAccess 取消$index指定的视图变量值
         *
         * @param string $index 视图变量名称
         *
         * @return void
         */
        public function offsetUnset ($index) {
            unset($this->_vars[$index]);
        }

        /**
         * 将$helpers指定的视图助手导入到当前环境中
         *
         * @param array|string $helpers 助手名称
         *
         * @return View
         */
        public function helpers ($helpers) {
            # 如果导入为字符串
            if (is_string ($helpers)) {
                $helpers = array($helpers);
            }

            # 获取导入助手路径并执行导入
            $paths = $this->_helperPaths ();
            foreach ($helpers as $helper) {
                if (isset($this->_helpers[$helper]) == false) {
                    $this->_helpers[$helper] = Jtita::import ($helper, Jtita::DEFAULT_FILE_EXT, $paths);
                }
            }

            return $this;
        }

        /**
         * 设置当前视图的布局文件为$layoutFile，并设置布局文件变量$vars
         *
         * @param string $layoutFile 布局文件
         * @param array  $vars       布局变量
         *
         * @return View
         *
         * @example
         *        $this->layoutFile('default');
         *        $this->layoutFile('member',array('op' => 'ov'));
         */
        public function layout ($layoutFile, $vars = array()) {
            $this->_layoutFile = $layoutFile;
            $this->_layoutVars = $vars;

            return $this;
        }

        /**
         * 将$cid指定的内容填充为$callback，$callback允许为回调方法或字符串。
         *
         * @example
         *  <pre>
         *        $view->contentFor('sub_menu','sub menu');
         *        $view->contentFor('sub_menu',function() {
         *
         *        });
         *  </pre>
         *
         * @param string $cid      内容编号
         * @param mixed  $callback 重写回调内容
         *
         * @return View
         */
        public function contentFor ($cid, $callback) {
            $this->_contents[$cid] = $callback;

            # 返回当前对象实例
            return $this;
        }

        /**
         * 输出$cid指定的内容信息，如果指定内容未重写则输出$default内容
         *
         * @example
         *  <pre>
         *        $this->contentRender('sub_menu','sub menu');
         *        $this->contentRender('sub_menu',function() {
         *
         *        });
         *  </pre>
         *
         * @param string $cid     内容编号ID
         * @param mixed  $default 默认值
         *
         * @return View
         */
        public function contentRender ($cid, $default = null) {
            # 判断$cid指定的内容是否重写
            $content = isset($this->_contents[$cid]) ? $this->_contents[$cid] : $default;

            # 返回内容绘制结果
            return self::invokeContentRender ($this, $content);
        }

        /**
         * 输出$elemName指定的元素信息，$vars表示元素将使用到的变量
         *
         * @example
         *   <pre> $this->elementRender('members/login'); </pre>
         *
         * @param string $elemName 元素位置
         * @param array  $vars     相关变量
         *
         * @return View
         */
        public function elementRender ($elemName, $vars = array()) {
            return self::invokeElementRender ($this, $elemName, $vars);
        }

        /**
         * 根据$param查询传入参数值，如果值不存在返回$default值。
         *
         * @param string     $paramName   参数名称
         * @param bool|mixed $default     默认值
         *
         * @return mixed
         */
        public function query ($paramName, $default = false) {
            return $this->_request->params ($paramName, $default);
        }

        /**
         * 设置响应的status为$status；
         *
         * @param string $status 状态码；
         */
        public function status ($status) {
            $this->_response->status ($status);
        }

        /**
         * 设置缓存状态为$cache。
         *
         * @param mixed $cache 缓存状态；
         */
        public function cache ($cache) {
            $this->_response->cache ($cache);
        }

        /**
         * 开启客户端缓存；
         *
         * @param string $control
         */
        public function enableCache ($control = 'max-age=20') {
            $this->_response->enableCache ($control);
        }

        /**
         * 关客户端缓存；
         */
        public function disableCache () {
            $this->_response->disableCache ();
        }

        /**
         * 设置响应的contentType为$contentType 。
         *
         * @param string $contentType 响应类型；
         */
        public function contentType ($contentType) {
            $this->_response->contentType ($contentType);
        }

        /**
         * 设置响应header $name的值为$value；
         *
         * @param string $name  名称；
         * @param mixed  $value 值；
         */
        public function header ($name, $value) {
            $this->_response->header ($name, $value);
        }

        /**
         * 将消息设置为flash消息。
         */
        public function flashMessage () {
            if ($msg = $this->_message) {
                $_SESSION[array_kpop ($msg, 'name', 'flash')] = $msg;
            }
        }

        /**
         * 文本消息。
         */
        public function textMessage ($name = 'flash') {
            # 获取通知消息。
            if (! ($msg = $this->_message)) {
                $msg = array_kpop ($_SESSION, $name, false);
            }
            if (! $msg) return '';

            $sign = array_kvalue($msg,'sign',JTMESSAGE_INFO);
            $msg  = array_kvalue($msg,'message','');

            return "<div class='alert alert-$sign'>$msg</div>";
        }

        /**
         * 使用$request和$options初始化选项
         *
         * @param ApplicationRequest $request 应用请求
         * @param array              $options 选项
         *
         * @return void
         */
        protected function _initializeOptions ($request, $options) {
            # 视图默认选项
            $defaultOptions = array(self::OPT_MODULE => $request->module (), self::OPT_PACKAGE => $request->package (),
                self::OPT_CONTROLLER => $request->controller (), self::OPT_ACTION => $request->action (),
                self::OPT_PASS => array(), self::OPT_TEMPLATE => false);

            # 合并选项生成选项结果
            $this->_options = array_merge ($defaultOptions, $options);
        }

        /**
         * 获取视图布局文件路径
         *
         * @return string
         */
        protected function _layoutPaths () {
            # 声明路径集；
            $paths = array();
            if ($module = $this->_options[self::OPT_MODULE]) {
                array_push ($paths, JTPATH_MODULE . '/' . $module . '/views/_layouts');
            }

            array_push ($paths, JTPATH_APP . '/views/_layouts');
            array_push ($paths, JTPATH_JTITA . '/views/_layouts');

            # 返回路径
            return $paths;
        }

        /*
         * 获取视图模板路径
         */
        protected function _templatePaths () {
            # 声明路径集
            $paths = array();

            if ($module = $this->_options[self::OPT_MODULE]) {
                array_push ($paths, JTPATH_MODULE . '/' . $module . '/views');
                array_push ($paths, JTPATH_APP . '/views/' . $module);
            }
            else {
                array_push ($paths, JTPATH_APP . '/views');
                array_push ($paths, JTPATH_JTITA . '/views');
            }

            return $paths;
        }


        /**
         * 获取视图元素路径
         */
        protected function _elementPaths () {
            # 声明路径集
            $paths = array();

            if ($module = $this->_options[self::OPT_MODULE]) {
                array_push ($paths, JTPATH_MODULE . '/' . $module . '/views/_elements');
            }

            array_push ($paths, JTPATH_APP . '/views/_elements');
            array_push ($paths, JTPATH_JTITA . '/views/_elements');

            return $paths;
        }

        /**
         * 获取辅助助手路径
         */
        protected function _helperPaths () {
            # 声明路径集
            $paths = array();

            if ($module = $this->_options[self::OPT_MODULE]) {
                array_push ($paths, JTPATH_MODULE . '/' . $module . '/views/_helpers');
            }

            array_push ($paths, JTPATH_APP . '/views/_helpers');
            array_push ($paths, JTPATH_JTITA . '/views/_helpers');

            return $paths;
        }

        /**
         * 获取模板文件集合
         *
         * @example
         *  <pre>
         *        members/zhenggp/2012-12-02
         *        zhenggp.2012.12-02-14
         *        zhenggp.2012
         *        zhenggp
         *  </pre>
         */
        protected function _templateFiles () {
            # 判断是否直接设置模板名称
            if ($this->_options[self::OPT_TEMPLATE]) {
                if (! is_array ($this->_options[self::OPT_TEMPLATE])) {
                    return array($this->_options[self::OPT_TEMPLATE]);
                }
                else {
                    return $this->_options[self::OPT_TEMPLATE];
                }
            }
            else {
                # admins.
                $package = $this->_options[self::OPT_PACKAGE] ? ($this->_options[self::OPT_PACKAGE] . Jtita::PACKAGE_SEPARATOR) : '';
                $pass = $this->_options[self::OPT_PASS];
                $files = array();

                # 判断是否包含$pass选项，如果包含则指定pass文件路径
                if (is_string ($pass)) $pass = array($pass);
                while (! empty($pass)) {
                    # admins.order.1.2.submit,admins.order.1.submit
                    array_push ($files, $package . $this->_options[self::OPT_CONTROLLER] . Jtita::PACKAGE_SEPARATOR . implode (Jtita::PACKAGE_SEPARATOR, $pass) . Jtita::PACKAGE_SEPARATOR . $this->_options[self::OPT_ACTION]);
                    array_pop ($pass);
                }

                # admins.order.submit
                array_push ($files, $package . $this->_options[self::OPT_CONTROLLER] . Jtita::PACKAGE_SEPARATOR . $this->_options[self::OPT_ACTION]);

                return $files;
            }
        }

        /**
         * 执行$view的输出，使用模板文件$fileName
         *
         * @param NormalView $view     视图对象
         * @param string     $fileName 模板文件
         *
         * @return string
         */
        protected static function invokeRender (&$view, $fileName) {
            # 开始记录输出
            ob_start ();

            # 必须使用include方法，不能使用Jtita::import方法
            include($fileName);

            # 判断是否指定有布局文件
            if ($view->_layoutFile) {
                self::invokeLayoutRender ($view);
            }

            # 获取输出内容并且清除内容
            $cc = ob_get_contents ();
            ob_end_clean ();

            # 返回模板生成的结果
            return $cc;
        }

        /**
         * 执行$view的布局文件输出
         *
         * @param NormalView $view
         *
         * @return View
         *
         * @throws ViewException
         */
        protected static function invokeLayoutRender (&$view) {
            # 获取视图布局文件地址
            $fileName = Jtita::lookup ($view->_layoutFile, self::TEMPLATE_EXT, $view->_layoutPaths ());

            if (! $fileName) {
                throw new ViewException('Normal view: Not found layout file:' . $view->_layoutFile);
            }
            else {
                extract ($view->_layoutVars);
                include($fileName);
            }

            return $view;
        }

        /**
         * 将$callback中的内容输出到视图$view中
         *
         * @param NormalView $view     视图对象
         * @param mixed      $callback 回调内容输出
         *
         * @return View
         */
        protected static function invokeContentRender (&$view, $callback) {
            if (is_string ($callback)) {
                echo $callback;
            }
            else if (is_callable ($callback)) {
                $callback($view);
            }

            return $view;
        }

        /**
         * 根据$view、$elemName和$vars输出元素内容
         *
         * @param NormalView $view     视图对象
         * @param string     $elemName 元素名称
         * @param array      $vars     注入变量
         *
         * @return View
         * @throws ViewException
         */
        protected static function invokeElementRender (&$view, $elemName, &$vars) {
            # 查找元素内容
            $fileName = Jtita::lookup ($elemName, self::TEMPLATE_EXT, $view->_elementPaths ());

            # 如果指定的元素未找到，则输出错误信息
            if ($fileName) {
                extract ($vars);
                include($fileName);
            }
            else if (JTDEBUG) {
                throw new ViewException('View: not found element:' . $elemName);
            }
            else {
                trigger_error ('Not found elements:' . $elemName);
            }

            return $view;
        }

        /**
         * @var mixed 视图控制器；
         */
        protected $_context = null;

        /**
         * @var array 视图选项
         */
        protected $_options = array();

        /**
         * @var ApplicationRequest 控制器应用请求对象；
         */
        protected $_request = null;

        /**
         * @var ApplicationResponse 控制器应用响应对象；
         */
        protected $_response = null;

        /**
         * @var array 视图通知消息；
         */
        protected $_message = false;

        /**
         * @var array 视图变量
         */
        protected $_vars = null;

        /**
         * @var string|boolean 布局文件名称，默认不采用布局文件
         */
        protected $_layoutFile = false;

        /**
         * @var array 布局文件选项，默认为空选项
         */
        protected $_layoutVars = array();

        /**
         * @var array 模板内容块，默认为空
         *
         * @example
         *      $contents = array('sub_menu' => function() {},'work' => 'Hello world');
         */
        protected $_contents = array();

        /**
         * @var array 模板辅助助手集
         */
        protected $_helpers = array();
    }