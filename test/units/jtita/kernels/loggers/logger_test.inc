<?php
    /**
     * logger_test.inc 提供对日志组件接口服务类Logger的单元测试服务。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.logs
     * @version     1.0 beta
     *
     * @create      2012-07-13
     * @modify      2013-04-26
     */

    # 初始化开始位置；
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (dirname (__FILE__)))))));

    /**
     * 导入基础服务类；
     */
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/behaviors.inc');

    /**
     * 导入被测试类；
     */
    require_once(JTPATH_ROOT . '/jtita/kernels/loggers/logger.inc');

    /**
     * 日志组件接口服务类，单元测试类。
     */
    class LoggerTest extends PHPUnit_Framework_TestCase {
        /**
         * 测试初始化器；
         */
        public function setUp () {
            Jtita::clear ();

            LogWriterMock::clear ();
            LogWriterMock1::clear ();
        }

        /**
         * 测试记录到默认记录器中；
         */
        public function testWriteForDefaultWriter () {
            Jtita::write (Logger::CFG_LOG, array(
                          'writers' => array('default' => 'log_writer_mock')));

            LoggerMock::reinitialize ();
            LoggerMock::write ('T001 hw', 'hello world message', 'normal');

            $logs = LogWriterMock::$logs;

            $this->assertFalse (empty($logs));
            $this->assertEquals (1, count ($logs));
        }

        /**
         * 测试根据规则记录日志；
         */
        public function testWriteForRule () {
            Jtita::write (Logger::CFG_LOG, array(
                        'writers' => array(
                            'default' => 'log_writer_mock',
                            'mock1'   => 'log_writer_mock1'),
                        'rules' => array('error' => 'mock1')));
            LoggerMock::reinitialize ();

            LoggerMock::write ('T001 hw', 'error', 'hello world message');
            LoggerMock::write ('T002 hw', 'normal', 'hello world message2');

            $this->assertEquals (1, count (LogWriterMock1::$logs));
            $this->assertEquals (1, count (LogWriterMock::$logs));
        }

        /**
         * 测试指定记录器不存在；
         *
         * @expectedException LoggerException
         */
        public function testWriteForWriterNotSupport () {
            Jtita::write (Logger::CFG_LOG, array('writers' => array()));

            LoggerMock::reinitialize ();
            LoggerMock::write ('T001 hw', 'normal', 'hello world message');
        }
    }

    /**
     * Log模拟对象，提供测试Log的服务；
     */
    class LoggerMock extends Logger {
        /**
         * 执行重新初始化；
         */
        public static function reinitialize () {
            self::$_initialized = false;
            self::$_writers = array();

            self::initialize ();
        }
    }

    /**
     * 日志记录模拟器；
     */
    class LogWriterMock implements LogWriter {
        /**
         * 日志记录内容集；
         */
        public static $logs = array();

        /**
         * 清除所有日志记录；
         */
        public static function clear () {
            self::$logs = array();
        }

        /**
         * 写入日志内容；
         */
        function write ($subject, $message, $category, $user) {
            array_push (self::$logs, array('subject' => $subject, 'message' => $message,
                                           'category' => $category,'user' => $user));
        }
    }

    /**
     * 日志记录模拟器；
     */
    class LogWriterMock1 implements LogWriter {
        /**
         * 日志记录内容集；
         */
        public static $logs = array();

        /**
         * 清除所有日志记录；
         */
        public static function clear () {
            self::$logs = array();
        }

        /**
         * 写入日志内容；
         */
        function write ($subject, $message, $category,$user) {
            array_push (self::$logs, array('subject' => $subject,
                'message' => $message, 'category' => $category,'user' => $user));
        }
    }