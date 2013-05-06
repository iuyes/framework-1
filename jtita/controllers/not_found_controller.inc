<?php
    /**
     * not_found_controller.inc 请求不存在控制器声明文件。
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
     * Not found controllers 提供对于不存在控制器处理；
     *
     * @package jtita.controllers
     */
    class NotFoundController extends Controller {

        /**
         * 使用$request和$options执行控制器未找到方法；
         *
         * @param ApplicationRequest        $request  应用请求；
         * @param ApplicationResponse       $response 响应；
         *
         * @return ApplicationResponse
         */
        public function execute($request, $response) {
            $this->_initialize($request, $response);
            # 提示未找到错误。
            return $this->error(404);
        }
    }

