<?php
    /**
     * controller_test.inc  实现对控制器的单元测试服务。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.controllers
     * @version     1.0 beta
     *
     * @create      2013-04-08
     * @modify      2013-04-28
     */

    # 声明测试根路径;
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (__FILE__))))));
    defined ('JTDEBUG') or define('JTDEBUG', 4);

    # 引用被测试类
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');

    JTita::paths (JTPATH_ROOT . '/jtita');

    require_once(JTPATH_ROOT . '/jtita/controllers/controller.inc');
    require_once(JTPATH_ROOT . '/jtita/views/normal_view.inc');
    require_once(JTPATH_ROOT . '/jtita/applications/application_base.inc');
    require_once(JTPATH_ROOT . '/jtita/applications/application_request.inc');
    require_once(JTPATH_ROOT . '/jtita/applications/application_response.inc');


    /**
     * Controller 测试类，实现Controller类的单元测试。
     *
     * @package jtita.controllers
     */
    class ControllerTest extends PHPUnit_Framework_TestCase {
        /**
         * 测试查询。
         */
        public function testQuery () {
            $request = new ApplicationRequest();
            $response = new ApplicationResponse();
            $ctl = new TestControllerMock();

            $request->initialize (array('name' => 'zhenggp', 'id' => 'emp001'));
            $ctl->autoRender = false;
            $ctl->execute ($request, $response);

            $this->assertEquals ('zhenggp', $ctl->query ('name'));
            $this->assertEquals ('emp001', $ctl->query ('id'));
            $this->assertFalse ($ctl->query ('not-exists'));
            $this->assertEquals ('default', $ctl->query ('default', 'default'));
        }

        /**
         * 测试获取POST值。
         */
        public function testForms () {
            $request = new ApplicationRequest();
            $response = new ApplicationResponse();
            $ctl = new TestControllerMock();

            $request->initialize (array(), array('name' => 'zhenggp', 'id' => 'emp001'));
            $ctl->autoRender = false;
            $ctl->execute ($request, $response);

            $this->assertEquals ('zhenggp', $ctl->forms ('name'));
            $this->assertEquals ('emp001', $ctl->forms ('id'));
            $this->assertFalse ($ctl->forms ('not-exists'));
            $this->assertEquals ('default', $ctl->forms ('default', 'default'));
        }

        /**
         * 测试执行动作。
         */
        public function testExecuteAction () {
            $request = new ApplicationRequest();
            $response = new ApplicationResponse();
            $ctl = new TestControllerMock();

            $request->initialize (array(JTPARAM_ACTION => 'call', 'name' => 'zhenggp'));

            $ctl->autoRender = false;
            $ctl->execute ($request, $response);

            $this->assertEquals ('zhenggp', $ctl->callValue);
        }

        /**
         * 测试执行，活动方法未找到。
         */
        public function testExecuteActionForNotFoundAction () {
            $request = new ApplicationRequest();
            $response = new ApplicationResponse();
            $ctl = new TestControllerMock();

            $request->initialize (array(JTPARAM_ACTION => 'not_exists', 'name' => 'zhenggp'));

            $ctl->autoRender = false;
            $ctl->execute ($request, $response);

            $this->assertEquals ('notExistsAction', $ctl->notFoundAction);
        }

        /**
         * 测试执行自动绘制
         */
        public function testExecuteForRenderVars () {
            $request = new ApplicationRequest();
            $response = new ApplicationResponse();
            $ctl = new TestControllerMock();

            $request->initialize (array(JTPARAM_ACTION => 'var', 'name' => 'zhenggp', 'id' => 'emp001'));
            $ctl->execute ($request, $response);

            $vars = ControllerTestViewMock::$vars;

            $this->assertEquals ('zhenggp', $vars['name']);
            $this->assertEquals ('emp001', $vars['id']);
        }

        /**
         * 测试执行自动绘制，指定消息。
         */
        public function testExecuteForRenderMessage () {
            $request = new ApplicationRequest();
            $response = new ApplicationResponse();
            $ctl = new TestControllerMock();

            $request->initialize (array(JTPARAM_ACTION => 'msg', 'name' => 'zhenggp'));
            $ctl->execute ($request, $response);

            $msg = ControllerTestViewMock::$msg;

            $this->assertEquals ('zhenggp', $msg['message']);
        }

        /**
         * 测试执行绘制
         */
        public function testExecuteForAutoRender () {
            $request = new ApplicationRequest();
            $response = new ApplicationResponse();
            $ctl = new TestControllerMock();

            $request->initialize (array(JTPARAM_ACTION => 'ren', 'name' => 'zhenggp'));
            $ctl->execute ($request, $response);

            $this->assertEquals ('zhenggp', $response->body ());
        }

        /**
         * 测试执行过滤器。
         */
        public function testFilterForExecute () {
            $request = new ApplicationRequest();
            $response = new ApplicationResponse();
            $ctl = new ControllerFilterController();

            ControllerTestFilter::$callExecute = false;
            ControllerTestFilter::$callRender = false;
            ControllerTestFilter::$callExecuteReturn = true;
            ControllerTestFilter::$callRenderReturn = true;

            $request->initialize (array(JTPARAM_ACTION => 'exe'));
            $ctl->execute ($request, $response);

            $this->assertTrue (ControllerTestFilter::$callExecute);
            $this->assertTrue (ControllerTestFilter::$callRender);
        }

        /**
         * 测试执行过滤器，返回false值。
         */
        public function testFilterForExecuteAndReturnFalse () {
            $request = new ApplicationRequest();
            $response = new ApplicationResponse();
            $ctl = new ControllerFilterController();

            ControllerTestFilter::$callExecuteReturn = false;
            ControllerTestFilter::$callRenderReturn = false;
            ControllerFilterController::$invokeExecute = false;

            $request->initialize (array(JTPARAM_ACTION => 'exe'));
            $ctl->execute ($request, $response);

            $this->assertFalse (ControllerFilterController::$invokeExecute);
        }

        /**
         * 测试执行过滤器，返回false值。
         */
        public function testFilterForRenderAndReturnFalse () {
            $request = new ApplicationRequest();
            $response = new ApplicationResponse();
            $ctl = new ControllerFilterController();

            ControllerTestFilter::$callExecuteReturn = true;
            ControllerTestFilter::$callRenderReturn = false;
            ControllerFilterController::$invokeRender = false;

            $request->initialize (array(JTPARAM_ACTION => 'exe'));
            $ctl->execute ($request, $response);

            $this->assertFalse (ControllerFilterController::$invokeRender);
        }
    }

    /**
     * 测试控制器模拟对象，提供控制器测试所需的服务。
     *
     * @package jtita.controllers
     */
    class TestControllerMock extends Controller {
        public $callValue = false;
        public $notFoundAction = false;

        public function indexAction () {

        }

        public function callAction () {
            $this->callValue = $this->query ('name');
        }

        public function varAction () {
            $this->name = $this->query ('name');
            $this->id = $this->query ('id');
        }

        public function msgAction () {
            $this->message ($this->query ('name'));
        }

        public function renAction () {

        }

        public function blank($actionMethod) {
            $this->notFoundAction = $actionMethod;
        }

        public function view () {
            return array('class' => 'ControllerTestViewMock');
        }
    }

    class ControllerTestViewMock extends NormalView {
        public static $vars = array();
        public static $msg = array();

        public function vars ($vars = null) {
            self::$vars = $vars;
        }

        public function message ($msg = null) {
            self::$msg = $msg;
        }

        public function render ($options = array()) {
            return 'zhenggp';
        }
    }

    class ControllerTestFilter implements Filter {
        public static $callExecute = false;
        public static $callExecuteReturn = true;

        public static $callRender = false;
        public static $callRenderReturn = true;

        public function execute ($controller, $actionMethod) {
            self::$callExecute = true;
            return self::$callExecuteReturn;
        }

        public function render ($controller, &$options) {
            self::$callRender = true;
            return self::$callRenderReturn;
        }
    }

    /**
     * filter 测试控制器。
     */
    class ControllerFilterController extends Controller {
        # 配置过滤器。
        public $filters = array(
            'test' => array('class' => 'ControllerTestFilter'));
        public static $invokeRender = true;
        public static $invokeExecute = true;

        protected function _renderInvoke (&$options) {
            self::$invokeRender = true;
            parent::_renderInvoke ($options);
        }

        protected function _actionInvoke ($actionMethod) {
            self::$invokeExecute = true;
            parent::_actionInvoke ($actionMethod);
        }

        public function exeAction () {
        }

        public function view () {
            return array('class' => 'ControllerTestViewMock');
        }
    }
