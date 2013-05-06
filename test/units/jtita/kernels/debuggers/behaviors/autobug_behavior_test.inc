<?php
    /**
     * autobug_behavior_test.inc 自动调试行为测试
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.debuggers.behaviors
     * @version     1.0 beta
     *
     * @create      2012-07-15
     * @modify      2014-04-02
     */

    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (dirname (dirname (__FILE__))))))));

    # 导入测试依赖类
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/behaviors.inc');

    /**
     * 导入测试事务类；
     */
    require_once(JTPATH_ROOT . '/jtita/kernels/debuggers/debugger.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/debuggers/behaviors/autobug_behavior.inc');

    /**
     * 服务模拟对象；
     */
    class AutobugServiceMock {
        /**
         * 提交数据；
         */
        public function submit () {
        }

        public function add ($message, $good) {
            return 'Hello php';
        }
    }

    /**
     * 自动调试跟踪行为测试；
     */
    class AutobugBehaviorTest extends PHPUnit_Framework_TestCase {

        /**
         * 测试初始化方法；
         */
        public function setUp () {
            Jtita::paths (JTPATH_ROOT . '/jtita');
            Jtita::clsBehaviors ();
        }

        /**
         * 测试名称选项；
         */
        public function testName () {
            Jtita::behaviors ('autobug_service_mock', 'submit',
                              array('class' => 'autobug_behavior','name' => 'ServiceMock'));

            $service = Jtita::create ('autobug_service_mock');
            $service->submit ();
            $service->submit ();

            $bugs = Debugger::read ('ServiceMock');

            $this->assertTrue (! empty($bugs));
            $this->assertEquals (2, count ($bugs));
        }

        /**
         * 测试默认名字选项；
         */
        public function testDefaultName () {
            Jtita::behaviors ('autobug_service_mock', 'submit', 'autobug_behavior');

            $service = Jtita::create ('autobug_service_mock');
            $service->submit ();
            $service->submit ();

            $bugs = Debugger::read ('default');

            $this->assertTrue (! empty($bugs));
            $this->assertEquals (2, count ($bugs));
        }
    }