<?php
    /**
     * application_request_test.inc 实现对ApplicationRequest类的单元测试；
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.applications
     * @version     1.0 beta
     *
     * @create      2012-07-26
     * @modify      2013-04-07
     */

    # 引入根节点
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (__FILE__))))));

    # 引用待测试类；
    require_once (JTPATH_ROOT . '/jtita/applications/application_request.inc');

    /**
     * ApplicationRequestTest 提供对ApplicationRequest类的测试服务。
     */
    class ApplicationRequestTest extends PHPUnit_Framework_TestCase {
        /**
         * 测试构造函数，默认参数；
         */
        public function testConstructor () {
            $request = new ApplicationRequest();

            $this->assertEmpty ($request->params ());
            $this->assertEmpty ($request->forms ());
        }

        /**
         * 测试构造函数；
         */
        public function testInitialize () {
            $request = new ApplicationRequest();
            $request->initialize (array('id' => 1), array('name' => 'zhenggp'));

            $params = $request->allParams ();
            $forms = $request->allForms ();

            $this->assertTrue (is_array ($params));
            $this->assertTrue (is_array ($forms));

            $this->assertEquals (1, $params['id']);
            $this->assertEquals ('zhenggp', $forms['name']);
        }

        /**
         * 测试增加查询参数；
         *
         * @depends testConstructor
         */
        public function testAddParams () {
            $request = new ApplicationRequest();
            $request->addParams ('name', 'zhenggp');

            $params = $request->allParams ();
            $this->assertEquals (1, count ($params));
            $this->assertEquals ('zhenggp', $params['name']);
        }

        /**
         * 测试增加数组参数；
         *
         * @depends testConstructor
         */
        public function testAddParamsForArray () {
            $request = new ApplicationRequest();
            $request->addParams (array('name' => 'zhenggp', 'id' => 12));

            $params = $request->allParams ();

            $this->assertEquals (2, count ($params));
            $this->assertEquals ('zhenggp', $params['name']);
            $this->assertEquals (12, $params['id']);
        }

        /**
         * 测试添加已存在的参数；
         *
         * @depends testConstructor
         * @depends testAddParamsForArray
         * @depends testAddParams
         */
        public function testAddParamsForExists () {
            $request = new ApplicationRequest();

            $request->addParams ('name', 'zhenggp');
            $request->addParams ('id', 1);
            $request->addParams (array('name' => 'nzhenggp', 'id' => 10, 'des' => 'ndes'));

            $params = $request->allParams ();

            $this->assertEquals (3, count ($params));
            $this->assertEquals ('nzhenggp', $params['name']);
            $this->assertEquals (10, $params['id']);
            $this->assertEquals ('ndes', $params['des']);
        }

        /**
         * 测试获取参数；
         *
         * @depends testInitialize
         */
        public function testParams () {
            $request = new ApplicationRequest();
            $request->initialize (array('name' => 'zhenggp'));

            $this->assertEquals ('zhenggp', $request->params ('name'));
            $this->assertFalse ($request->params ('notexists'));
            $this->assertEquals ('default value', $request->params ('notexists', 'default value'));
        }

        /**
         * 测试移除查询参数数据；
         *
         * @depends testInitialize
         */
        public function testRemoveParams () {
            $request = new ApplicationRequest();
            $request->initialize (array('id' => 1, 'name' => 'zhenggp'), array());

            $request->removeParams ('id');

            $params = $request->allParams ();
            $this->assertEquals ('zhenggp', $params['name']);
            $this->assertFalse (isset($params['id']));
        }

        /**
         * 测试移除多个查询参数数据；
         *
         * @depends testInitialize
         */
        public function testRemoveParamsForArray () {
            $request = new ApplicationRequest();
            $request->initialize (array('id' => 1, 'name' => 'zhenggp'), array());

            $request->removeParams (array('id'));

            $params = $request->allParams ();
            $this->assertEquals ('zhenggp', $params['name']);
            $this->assertFalse (isset($params['id']));
        }

        /**
         * 测试增加post参数；
         *
         * @depends testConstructor
         */
        public function testAddForms () {
            $request = new ApplicationRequest();
            $request->addForms ('name', 'zhenggp');
            $request->addForms ('id', 1);

            $forms = $request->allForms ();

            $this->assertEquals (2, count ($forms));
            $this->assertEquals ('zhenggp', $forms['name']);
            $this->assertEquals (1, $forms['id']);
        }

        /**
         * 测试增加post数组参数；
         *
         * @depends testConstructor
         */
        public function testAddFormsForArray () {
            $request = new ApplicationRequest();
            $request->addForms (array('name' => 'zhenggp', 'id' => 1));

            $forms = $request->allForms ();

            $this->assertEquals (2, count ($forms));
            $this->assertEquals ('zhenggp', $forms['name']);
            $this->assertEquals (1, $forms['id']);
        }

        /**
         * 测试移除表单数据；
         *
         * @depends testInitialize
         */
        public function testRemoveForms () {
            $request = new ApplicationRequest();
            $request->initialize (array(), array('id' => 1, 'name' => 'zhenggp'));
            $request->removeForms ('id');

            $forms = $request->allForms ();
            $this->assertEquals (1, count ($forms));
            $this->assertEquals ('zhenggp', $forms['name']);
        }

        /**
         * 测试移除表单数据；
         *
         * @depends testInitialize
         */
        public function testRemoveFormsForArray () {
            $request = new ApplicationRequest();
            $request->initialize (array(), array('id' => 1, 'name' => 'zhenggp'));

            $request->removeForms (array('id'));

            $forms = $request->allForms ();
            $this->assertEquals (1, count ($forms));
            $this->assertEquals ('zhenggp', $forms['name']);
        }

        /**
         * 测试获取表单数据；
         *
         * @depends testInitialize
         */
        public function testForms () {
            $request = new ApplicationRequest();
            $request->initialize (array(), array('name' => 'zhenggp'));

            $this->assertEquals ('zhenggp', $request->forms ('name'));
            $this->assertFalse ($request->forms ('notexists'));
            $this->assertEquals ('default value', $request->forms ('notexists', 'default value'));
        }

        /**
         * 测试获取模块名称；
         *
         * @depends testInitialize
         */
        public function testModule () {
            $request = new ApplicationRequest();
            $request->initialize (array(JTPARAM_MODULE => 'members'));

            $this->assertEquals ('members', $request->module ());
        }

        /**
         * 测试模块默认值；
         *
         * @depends testConstructor
         */
        public function testModuleForDefault () {
            $request = new ApplicationRequest();

            $this->assertFalse ($request->module ());
        }

        /**
         * 测试获取包参数；
         *
         * @depends testInitialize
         */
        public function testPackage () {
            $request = new ApplicationRequest();
            $request->initialize (array(JTPARAM_PACKAGE => 'admins.reports'));

            $this->assertEquals ('admins.reports', $request->package ());
        }

        /**
         * 测试获取默认包参数；
         *
         * @depends testConstructor
         */
        public function testPackageForDefault () {
            $request = new ApplicationRequest();

            $this->assertFalse ($request->package ());
        }

        /**
         * 测试获取当前的控制器参数；
         *
         * @depends testInitialize
         */
        public function testController () {
            $request = new ApplicationRequest();
            $request->initialize (array(JTPARAM_CONTROLLER => 'order'));

            $this->assertEquals ('order', $request->controller ());
        }

        /**
         * 测试获取默认控制器参数；
         *
         * @depends testConstructor
         */
        public function testControllerForDefault () {
            $request = new ApplicationRequest();

            $this->assertEquals (JTPARAM_DEFAULT_CONTROLLER, $request->controller ());
        }

        /**
         * 测试获取活动参数；
         *
         * @depends testInitialize
         */
        public function testAction () {
            $request = new ApplicationRequest();
            $request->initialize (array(JTPARAM_ACTION => 'submit'));

            $this->assertEquals ('submit', $request->action ());
        }

        /**
         * 测试获取默认活动；
         *
         * @depends testConstructor
         */
        public function testActionForDefault () {
            $request = new ApplicationRequest();

            $this->assertEquals (JTPARAM_DEFAULT_ACTION, $request->action ());
        }

        /**
         * 测试使用数组方式判断查询参数是否存在
         */
        public function testArrayAccessForOffsetExists () {
            $request = new ApplicationRequest();
            $request->initialize (array('name' => 'zhenggp', 'age' => 30));

            $this->assertTrue (isset($request['name']));
            $this->assertTrue (isset($request['age']));
            $this->assertFalse (isset($request['not-exists']));
        }

        /**
         * 测试使用数组方式判断查询参数是否存在，查询默认参数。
         */
        public function testArrayAccessForOffsetExistsDefaultParam () {
            $request = new ApplicationRequest();
            $request->initialize ();

            $this->assertTrue (isset($request[JTPARAM_CONTROLLER]));
            $this->assertTrue (isset($request[JTPARAM_ACTION]));
            $this->assertTrue (isset($request[JTPARAM_MODULE]));
            $this->assertTrue (isset($request[JTPARAM_PACKAGE]));
            $this->assertTrue (isset($request[JTPARAM_FORMAT]));
        }

        /**
         * 测试使用数组方式访问查询参数
         */
        public function testArrayAccessForOffsetGet () {
            $request = new ApplicationRequest();
            $request->initialize (array('name' => 'zhenggp', 'age' => 30));

            $this->assertEquals ('zhenggp', $request['name']);
            $this->assertEquals (30, $request['age']);
        }

        /**
         * 测试使用数组访问查询参数，索引不存在。
         */
        public function testArrayAccessForOffsetGetNotExists () {
            $request = new ApplicationRequest();
            $request->initialize (array('name' => 'zhenggp', 'age' => 30));

            $this->assertFalse ($request['not-exists']);
        }

        /**
         * 测试使用数组访问默认参数。
         */
        public function testArrayAccessForOffsetGetDefaultParam () {
            $request = new ApplicationRequest();
            $request->initialize ();

            $this->assertEquals (JTPARAM_DEFAULT_CONTROLLER, $request[JTPARAM_CONTROLLER]);
            $this->assertEquals (JTPARAM_DEFAULT_ACTION, $request[JTPARAM_ACTION]);
            $this->assertEquals (JTPARAM_DEFAULT_MODULE, $request[JTPARAM_MODULE]);
            $this->assertEquals (JTPARAM_DEFAULT_PACKAGE, $request[JTPARAM_PACKAGE]);
            $this->assertEquals (JTPARAM_DEFAULT_FORMAT, $request[JTPARAM_FORMAT]);
        }

        /**
         * 测试使用数组方式设置查询参数值。
         */
        public function testArrayAccessForOffsetSet () {
            $request = new ApplicationRequest();
            $request->initialize (array('name' => 'zhenggp', 'age' => 30));

            $request['name'] = 'chenjuan';
            $request['age'] = '26';

            $this->assertEquals ('chenjuan', $request['name']);
            $this->assertEquals (26, $request['age']);
        }

        /**
         * 测试使用数组，删除查询参数。
         */
        public function testArrayAccessForOffsetUnset () {
            $request = new ApplicationRequest();
            $request->initialize (array('name' => 'zhenggp', 'age' => 30));

            unset($request['name']);
            unset($request['age']);

            $this->assertFalse ($request['name']);
            $this->assertFalse ($request['age']);
        }

        /**
         * 测试使用数组，删除默认查询参数。
         */
        public function testArrayAccessForOffsetUnsetDefaultParams () {
            $request = new ApplicationRequest();
            $request->initialize (array('name' => 'zhenggp', 'age' => 30));

            unset($request[JTPARAM_CONTROLLER]);
            unset($request[JTPARAM_ACTION]);
            unset($request[JTPARAM_MODULE]);
            unset($request[JTPARAM_PACKAGE]);
            unset($request[JTPARAM_FORMAT]);

            $this->assertEquals (JTPARAM_DEFAULT_CONTROLLER, $request[JTPARAM_CONTROLLER]);
            $this->assertEquals (JTPARAM_DEFAULT_ACTION, $request[JTPARAM_ACTION]);
            $this->assertEquals (JTPARAM_DEFAULT_MODULE, $request[JTPARAM_MODULE]);
            $this->assertEquals (JTPARAM_DEFAULT_PACKAGE, $request[JTPARAM_PACKAGE]);
            $this->assertEquals (JTPARAM_DEFAULT_FORMAT, $request[JTPARAM_FORMAT]);
        }

        /**
         * 测试使用二维数组
         */
        public function testArrayInRequest () {
            $request = new ApplicationRequest();
            $request->initialize (array('name' => 'zhenggp'));
            $args = array($request);

            $this->assertEquals ('zhenggp', $args[0]['name']);
            $this->assertTrue (isset($args[0]['name']));
        }
    }