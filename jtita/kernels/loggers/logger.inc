<?php
    /**
     * logger.inc 日志组件服务接口Logger类声明文件。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.logs
     * @version     1.0 beta
     *
     * @create      2012-07-12
     * @modify      2013-04-26
     */

    /**
     * 定义日志服务开启标志
     */
    define('JTITA_LOGGER', true);

    /**
     * 日志记录异常，记录日志异常信息
     *
     * @package jtita.kernels.loggers
     */
    class LoggerException extends JtitaException {
    }

    /**
     * 日志组件服务接口类，提供将日志信息记录到持久化存储设备的服务。
     *
     * @package jtita.kernels.loggers
     */
    interface LogWriter {
        /**
         * 将$subject、$message和$category写入持久化存储服务中；
         *
         * @param string      $subject     日志主题
         * @param string      $message     日志消息
         * @param int|string  $category    日志类型
         * @param string      $user        用户
         * @return void
         */
        function write ($subject, $message, $category, $user);
    }

    /*
     * 如果还未初始化日志功能，初始化；
     */
    if (Logger::initialized () == false) {
        Logger::initialize ();
    }

    /**
     * 日志组件服务接口对象，提供日志服务的外部服务接口。
     *
     * @abstract
     * @package jtita.kernels.loggers
     *
     * @example
     * <pre>
     *      Jtita::write('kernels/loggers',array(
     *          'writers' => array (
     *              'writerName' => array('class' => 'clsName')
     *          ),
     *          'rules' => array (
     *              'category'  => 'writerName',
     *              'category1' => 'writeName1'
     *          )
     *      ));
     * </pre>
     */
    abstract class Logger {
        /**
         * @const string 定义配置节点常量
         */
        const CFG_LOG = 'kernels/loggers';

        /**
         * @const string 配置写入器节点常量
         */
        const CFG_WRITERS = 'writers';

        /**
         * @const string 规则节点
         */
        const CFG_RULES = 'rules';

        /**
         * 读取配设置信息，初始化Log类服务；
         *
         * @return null|boolean
         */
        public static function initialize () {
            # 如果已完成初始化
            if (self::$_initialized) {
                trigger_error ('JTK0104:Logger repeat initialized.');
            }

            # 读取配设信息；
            $config = Jtita::read (self::CFG_LOG, array());
            # 如果未指定日志记录器。
            if (! isset($config[self::CFG_WRITERS])) {
                $config[self::CFG_WRITERS] = array('default' => 'kernels.loggers.writers.file_log_writer');
            }

            # 如果没有指定规则
            if (! isset($config[self::CFG_RULES])) {
                $config[self::CFG_RULES] = array();
            }

            self::$_config = $config;
            self::$_initialized = true;
        }

        /**
         * 返回一个布尔值，判断是否已经完成初始化；
         */
        public static function initialized () {
            return self::$_initialized;
        }

        /**
         * 记录日志操作；
         */
        public static function write ($subject, $category = 'normal', $user = 'system', $message = '') {
            # 如果指定为调试模式。
            if (defined('JTITA_DEBUGGER')) {
                Debugger::write('logger',array('method' => 'write','category' => 'category',
                                               'user' => 'system','message' => $message));
            }

            $writer = self::_writerObject ($category);
            $writer->write ($subject, $message, $category, $user);
        }

        /**
         * 根据分类编辑返回对应的日志记录器；
         *
         * @param string $category 分类
         * @return LogWriter
         */
        private static function _writerObject ($category) {
            # 根据分类获取写入名字。
            $writerName = self::_writerName ($category);
            if (! isset(self::$_writers[$writerName])) {
                self::$_writers[$writerName] = self::_createWriter ($writerName);
            }

            return self::$_writers[$writerName];
        }

        /**
         * 根据分类信息，获取记录器名称；
         */
        private static function _writerName ($category) {
            # 如果指定规则没有设置记录器。
            if (isset(self::$_config[self::CFG_RULES][$category])) {
                return self::$_config[self::CFG_RULES][$category];
            }

            return 'default';
        }

        /**
         * 根据$writerName创建日志记录器；
         *
         * @throws LoggerException
         */
        private static function _createWriter ($writerName) {
            if (! isset(self::$_config[self::CFG_WRITERS][$writerName])) {
                throw new LoggerException('JTK0104:Logger not support writer - ' . $writerName);
            }

            return Jtita::ioc (self::$_config[self::CFG_WRITERS][$writerName]);
        }

        /**
         * @var array 配置信息。
         */
        protected static $_config = array();

        /**
         * @var array 日志记录器对象集。
         */
        protected static $_writers = array();

        /**
         * @var boolean 初始化检查。
         */
        protected static $_initialized = false;
    }