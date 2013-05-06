<?php
    /**
     * debugger.inc 提供调试服务组件接口Debugger类声明。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.debuggers
     * @version     1.0 beta
     *
     * @create      2012-07-15
     * @modify      2013-04-26
     */

    /**
     * @const bool 调试组件服务开启标志。
     */
    define('JTITA_DEBUGGER', true);

    /**
     * 调试器类，提供程序的运行时调试跟踪服务。
     *
     * @package jtita.kernels.debuggers
     * @abstract
     */
    abstract class Debugger {

        /**
         * 根据$name和$value记录测试信息。
         *
         * <b>注意：</b>记录相同的$name不会覆盖前面的值，只会在记录中增加一条内容。
         *
         * @param string $name  测试关键字。
         * @param mixed  $value 记录内容。
         */
        public static function write ($name, $value) {
            if (! isset(self::$_bugs[$name])) {
                self::$_bugs[$name] = array();
            }

            array_push (self::$_bugs[$name], $value);
        }

        /**
         * 根据$name读取调试信息，如果调试信息为空返回空数组。
         *
         * @param string $name 调试关键字。
         * @return array
         */
        public static function read ($name) {
            if (isset(self::$_bugs[$name])) {
                return self::$_bugs[$name];
            }

            return array();
        }

        /**
         * @var array 测试跟踪信息
         */
        private static $_bugs = array();
    }