<?php
    /**
     * rfm_filter.inc 提供RfmFilter类的声明。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.jtita.com/license.txt
     *
     * @package     jtita.controllers.filters
     * @version     1.0 beta
     *
     * @create      2013-04-08
     * @modify      2013-04-08
     */

    /**
     * Request format method 过滤器，为非html格式和非get方法自动增加pass参数。
     *
     * @package jtita.controllers.filters
     */
    class RfmFilter implements Filter {
        /**
         * 执行前过滤，不进行任何处理，直接返回true。
         *
         * @implement Filter::execute
         */
        public function execute ($controller, $actionMethod) {
            return true;
        }

        /**
         * 绘制前过滤，为$options自动增加pass参数。
         *
         * @implement Filter::render
         */
        public function render ($controller, &$options) {
            # 如果请求格式不为默认值或者非get请求。
            if (($controller->request->format () != JTPARAM_DEFAULT_FORMAT) || ! $controller->isGet ()) {
                # 如果未指定pass选项；
                if (! isset($options['pass'])) {
                    $options['pass'] = array();
                }
                else if (! is_array ($options['pass'])) {
                    $options = array($options['pass']);
                }

                # 如果请求方法为非get方法
                if (! $controller->isGet ()) {
                    array_unshift ($options['pass'], strtolower ($controller->request->method ()));
                }

                # 如果请求格式为非默认值；
                if ($controller->request->format () != JTPARAM_DEFAULT_FORMAT) {
                    array_unshift ($options['pass'], $controller->request->format ());
                }
            }

            return true;
        }
    }
