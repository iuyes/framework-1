<?php
    /**
     * notify_test.inc 提供对通知服务的单元测试。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.logs
     * @version     1.0 beta
     *
     * @create      2013-04-05
     * @modify      2013-04-28
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
    require_once(JTPATH_ROOT . '/jtita/kernels/notifies/notify.inc');

    /**
     * 通知消息单元测试类。
     *
     * @package jtita.kernels.notifies
     */
    class NotifyTest extends PHPUnit_Framework_TestCase{
        public function setUp () {
            Jtita::clear ();
            NotifyDeliveryMock::$messages = array();
        }

        /**
         * 测试执行初始化，未提供任何设置
         */
        public function testInitializeForNotSet () {
            NotifyMock::reinitialize ();

            $configs = NotifyMock::configs ();

            $this->assertTrue (is_array ($configs));
            $this->assertEquals ('kernels.notifies.deliveries.email_notify_delivery', $configs['default']['class']);
        }

        /**
         * 测试重复初始化
         *
         * @expectedException NotifyException
         */
        #public function testInitializeRepeat () {
        #    Notify::initialize ();
        #    Notify::initialize ();
        #}

        /**
         * 测试默认投送
         */
        public function testDeliverForDefault () {
            Jtita::write('kernels/notifies', array(
                'default' => array('class' => 'notify_delivery_mock')
            ));

            NotifyMock::reinitialize();
            Notify::deliver('zhenggp','Hello World');

            $this->assertEquals('zhenggp',NotifyDeliveryMock::$messages[0]['receiver']);
            $this->assertEquals('Hello World',NotifyDeliveryMock::$messages[0]['message']);
        }

        /**
         * 测试指定投送
         */
        public function testDeliverForSetDeliver () {
            Jtita::write('kernels/notifies', array(
                'example' => array('class' => 'notify_delivery_mock')
            ));

            NotifyMock::reinitialize();
            Notify::deliver('zhenggp','Hello World','example');

            $this->assertEquals('zhenggp',NotifyDeliveryMock::$messages[0]['receiver']);
            $this->assertEquals('Hello World',NotifyDeliveryMock::$messages[0]['message']);
        }

        /**
         * 测试投送未提供的投送器。
         *
         * @expectedException NotifyException
         */
        public function testDeliverForNotSupportDeliver () {
            NotifyMock::reinitialize();
            Notify::deliver('zhenggp','Hello World','example');
        }

    }

    /**
     * 通知模拟对象，提供Notify单元测试所需要模拟服务。
     */
    class NotifyMock extends Notify{
        public static function reinitialize () {
            self::$_initialized = false;
            self::$_config = array();
            self::$_deliveries = array();

            self::initialize ();
        }

        public static function configs () {
            return self::$_config;
        }
    }

    /**
     * 通知投放模拟对象，提供通知的投放模拟服务。
     */
    class NotifyDeliveryMock implements NotifyDelivery{
        public static $messages = array();

        public function deliver ($receiver, $message) {
            array_push(self::$messages,array('receiver' => $receiver,
                                             'message' => $message));
        }
    }

