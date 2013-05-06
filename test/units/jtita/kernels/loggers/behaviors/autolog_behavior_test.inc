<?php
    /**
     * autolog_behavior_test.inc 自动日志行为单元测试
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.logs.behaviors
     * @version     1.0 beta
     *
     * @create      2012-07-15
     * @modify      2013-04-28
     */

    # 引用根据路径地址
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (dirname (dirname (__FILE__))))))));

    /**
     * 导入基础服务类；
     */
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/behaviors.inc');

    /**
     * 导入测试事务类；
     */
    require_once(JTPATH_ROOT . '/jtita/kernels/loggers/behaviors/autolog_behavior.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/loggers/logger.inc');

    /**
     * 服务模拟对象；
     */
    class AutologBehaviorServiceMock {
        /**
         * 提交数据；
         */
        public function submit () {
        }

        public function add ($message, $good) {
            return 'Hello phper';
        }
    }

    /**
     * 日志记录模拟器；
     */
    class AutologBehaviorLogWriterMock implements LogWriter {
        static $logs = array();

        static function cls () {
            self::$logs = array();
        }

        function write ($subject, $message, $category, $user) {
            array_push (self::$logs, array(
                                          'subject' => $subject,
                                          'message' => $message,
                                          'category' => $category,
                                          'user' => $user
                                     ));
        }
    }

    class AutologBehaviorLogMock extends Logger {
        public static function reinitialize () {
            self::$_initialized = false;
            self::$_writers = array();

            self::initialize ();
        }
    }

    /**
     * 自动调试跟踪行为测试；
     */
    class AutologBehaviorTest extends PHPUnit_Framework_TestCase {

        /**
         * 测试初始化方法；
         */
        public function setUp () {
            AutologBehaviorLogWriterMock::cls ();

            Jtita::paths (JTPATH_ROOT . '/jtita');
            Jtita::clsBehaviors ();
            Jtita::clear ();

            Jtita::write ('kernels/loggers', array(
                           'writers' => array('default' => 'autolog_behavior_log_writer_mock'),
                           'rules' => array()));

            AutologBehaviorLogMock::reinitialize ();
        }

        /**
         * 测试默认选项；
         */
        public function testAutolog () {
            Jtita::behaviors ('autolog_behavior_service_mock', 'add', 'autolog_behavior');

            $service = Jtita::create ('autolog_behavior_service_mock');
            $result = $service->add ('Hello World', 'Hello PHP');

            $this->assertEquals ('Hello phper', $result);
            $this->assertEquals (1, count (AutologBehaviorLogWriterMock::$logs));
            $this->assertEquals ('normal', AutologBehaviorLogWriterMock::$logs[0]['category']);
            $this->assertEquals ('Autolog subject: default', AutologBehaviorLogWriterMock::$logs[0]['subject']);
            $this->assertEquals ('Autolog message: default', AutologBehaviorLogWriterMock::$logs[0]['message']);
        }

        /**
         * 测试主题选项；
         */
        public function testSubject () {
            Jtita::behaviors ('autolog_behavior_service_mock', 'add', array(
                              'class' => 'autolog_behavior',
                              'subject' => 'new subject:{0} {1}'));

            $service = Jtita::create ('autolog_behavior_service_mock');
            $result = $service->add ('Hello World', 'Hello PHP');

            $this->assertEquals ('Hello phper', $result);
            $this->assertEquals (1, count (AutologBehaviorLogWriterMock::$logs));
            $this->assertEquals ('normal', AutologBehaviorLogWriterMock::$logs[0]['category']);
            $this->assertEquals ('new subject:Hello World Hello PHP', AutologBehaviorLogWriterMock::$logs[0]['subject']);
            $this->assertEquals ('Autolog message: default', AutologBehaviorLogWriterMock::$logs[0]['message']);
        }

        /**
         * 测试消息选项；
         */
        public function testMessage () {
            Jtita::behaviors ('autolog_behavior_service_mock', 'add', array(
                              'class' => 'autolog_behavior',
                              'message' => 'new message:{0} - {1}'));

            $service = Jtita::create ('autolog_behavior_service_mock');
            $result = $service->add ('Hello World', 'Hello PHP');

            $this->assertEquals ('Hello phper', $result);
            $this->assertEquals (1, count (AutologBehaviorLogWriterMock::$logs));
            $this->assertEquals ('normal', AutologBehaviorLogWriterMock::$logs[0]['category']);
            $this->assertEquals ('Autolog subject: default', AutologBehaviorLogWriterMock::$logs[0]['subject']);
            $this->assertEquals ('new message:Hello World - Hello PHP', AutologBehaviorLogWriterMock::$logs[0]['message']);
        }

        /**
         * 测试分类选项；
         */
        public function testCategory () {
            Jtita::behaviors ('autolog_behavior_service_mock', 'add', array(
                              'class' => 'autolog_behavior',
                              'category' => 'error'));

            $service = Jtita::create ('autolog_behavior_service_mock');
            $result = $service->add ('Hello World', 'Hello PHP');

            $this->assertEquals ('Hello phper', $result);
            $this->assertEquals (1, count (AutologBehaviorLogWriterMock::$logs));
            $this->assertEquals ('error', AutologBehaviorLogWriterMock::$logs[0]['category']);
            $this->assertEquals ('Autolog subject: default', AutologBehaviorLogWriterMock::$logs[0]['subject']);
            $this->assertEquals ('Autolog message: default', AutologBehaviorLogWriterMock::$logs[0]['message']);
        }

    }