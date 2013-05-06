<?php
    /**
     * error_controller.inc 异常控制器声明。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.controllers
     * @version     1.0 beta
     *
     * @create      2013-02-08
     * @modify      2013-04-28
     */

    /**
     * error controllers 错误处理控制器，当异常发生错误时将自动调用本控制器；
     *
     * @package jtita.controllers
     */
    class ErrorController extends Controller {

        /**
         * @var Exception 异常信息；
         */
        public $exception = null;

        /**
         * 使用$request和$options执行控制器未找到方法；
         *
         * @param ApplicationRequest        $request  应用请求；
         * @param ApplicationResponse       $response 响应；
         *
         * @return ApplicationResponse
         */
        public function execute ($request, $response) {
            # 初始化
            $this->_initialize ($request, $response);
            $this->ex = $this->exception;

            return $this->error (500);
        }
    }

