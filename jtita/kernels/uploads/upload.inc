<?php
    /**
     * upload.inc 声明架构上传服务组件。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.uploads
     * @version     1.0 beta
     *
     * @create      2013-04-08
     * @modify      2013-04-28
     */

    /**
     * @const 声明上传服务开启标志。
     */
    defined ('JTITA_UPLOAD', true);

    /**
     * 上传驱动接口，执行将文件上传到具体位置的服务。
     *
     * @package jtita.kernels.uploads
     */
    interface UploadDriver {
        /**
         * 将$file指定的内容上传到具体位置。
         *
         * @param mixed $file 上传的文件
         * @return string 目标URI地址。
         */
        function upload ($file);
    }

    /**
     * 上传服务组件。
     *
     * <example>
     *  <pre>
     *      Jtita::write('kernels/uploads',array(
     *      ));
     *  </pre>
     *
     * @package jtita.kernels.uploads
     * @abstract
     */
    abstract class Upload {

        /**
         * 将$file上传到指定的$driver中，返回URI地址。
         *
         * @param mixed  $file   上传文件
         * @param string $driver 使用驱动
         * @return string 上传后的URI地址。
         */
        public static function upload ($file, $driver = 'default') {

        }

        /**
         * 执行上传服务组件初始化。
         */
        public static function initialize () {

        }

        /**
         * 返回一个布求值，表示是否已完成初始化。
         */
        public static function initialized () {
            return self::$_initialized;
        }

        /**
         * @var bool 初始化标志。
         */
        protected static $_initialized = false;
    }
