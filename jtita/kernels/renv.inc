<?php
    /**
     * renv.php 请求环境Renv类声明文件。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.jtita.com/license.txt
     *
     * @package     jtita.kernels
     * @version     1.0 beta
     *
     * @create      2012-08-07
     * @modify      2013-04-01
     */

    /**
     * 定义请求方法常量；
     */
    define('JTMETHOD_POST', 'POST');
    define('JTMETHOD_GET', 'GET');
    define('JTMETHOD_PUT', 'PUT');
    define('JTMETHOD_DELETE', 'DELETE');

    /**
     * 请求环境工具类，提供获取当前请求的相关信息、
     *
     * @abstract 禁止实例化
     * @package jtita.kernels
     */
    abstract class Renv {
        /**
         * 获取当前请求的访问设备；
         */
        public static function machine () {

        }

        /**
         * 获取当前请求的访问操作系统；
         */
        public static function os () {

        }

        /**
         * 获取当前请求的浏览器信息；
         */
        public static function browser () {

        }

        /**
         * 获取当前请求浏览器的名称；
         */
        public static function browserName () {

        }

        /**
         * 获取当前请求的浏览器版本；
         */
        public static function browserVersion () {

        }

        /**
         * 获取当前请求接受的格式；
         */
        public static function accept () {

        }

        /**
         * 获取当前请求接受的编码格式；
         */
        public static function acceptEncoding () {

        }

        /**
         * 获取当前请求接受的语言集；
         */
        public static function acceptLanguage () {

        }

        /**
         * 获取当前请求的方法信息；
         */
        public static function method () {
            return $_SERVER['REQUEST_METHOD'];
        }

        /**
         * 返回布求值，判断当前请求是否是$method方法；
         */
        public static function isM ($method) {
            return (strtoupper ($method) == $_SERVER['REQUEST_METHOD']);
        }

        /**
         * 获取当前请求的IP地址；
         *
         * @return string
         */
        public static function ip () {
            if (getenv ("HTTP_CLIENT_IP")) return getenv ("HTTP_CLIENT_IP");
            else if (getenv ("HTTP_X_FORWARDED_FOR")) return getenv ("HTTP_X_FORWARDED_FOR");
            else if (getenv ("REMOTE_ADDR")) return getenv ("REMOTE_ADDR");
            else return "Unknow";
        }

        /**
         * 获取当前地址；
         */
        public static function here () {
            return $_SERVER['URL'];
        }

        /**
         * 获取当前请求的方法引用信息；
         */
        public static function refer () {

        }

        /**
         * 获取当前请求的域名；
         */
        public static function domain () {
            return $_SERVER['HTTP_HOST'];
        }

        /**
         * 获取当前请求的端口号；
         */
        public static function port () {
            return $_SERVER['SERVER_PORT'];
        }

        /**
         * 获取当前请求的服务协议；
         */
        public static function protocol () {
            static $protocol = false;

            if (! $protocol) {
                return strtolower (substr ($_SERVER['SERVER_PROTOCOL'], 0, strpos ($_SERVER['SERVER_PROTOCOL'], '/')));
            }

            return $protocol;
        }

        /**
         * 获取当前请求的脚本名字；
         */
        public static function scriptName () {
            return substr ($_SERVER['SCRIPT_NAME'], 1);
        }
    }