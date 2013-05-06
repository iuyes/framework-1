<?php
    /**
     * rfm_filter_test.inc 提供RfmFilter的单元测试服务。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.jtita.com/license.txt
     *
     * @package     jtita.controllers
     * @version     1.0 beta
     *
     * @create      2013-04-08
     * @modify      2013-04-08
     */

    # 声明测试根路径;
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (dirname (__FILE__)))))));

    # 引用被测试类
    require_once(JTPATH_ROOT . '/jtita/controllers/controller.inc');
    require_once(JTPATH_ROOT . '/jtita/controllers/filters/rfm_filter.inc');
    require_once(JTPATH_ROOT . '/jtita/applications/application_request.inc');

    /**
     * Controller 测试类，实现Controller类的单元测试。
     *
     * @package jtita.controllers
     */
    class RfmFilterTest extends PHPUnit_Framework_TestCase {

        /**
         * 测试执行反馈
         */
        public function testExecute () {
            $filter = new RfmFilter();

            $this->assertTrue ($filter->execute (new RfmFilterControllerMock(),'ok'));
        }

        /**
         * 测试执行render前过滤
         */
        public function testRender () {
            $options = array();
            $filter = new RfmFilter();

            $this->assertTrue ($filter->render (new RfmFilterControllerMock(), $options));
            $this->assertEmpty ($options);
        }

        /**
         * 测试执行render前过滤，指定非html格式。
         */
        public function testRenderForFormat () {
            $options = array();
            $filter = new RfmFilter();

            $this->assertTrue ($filter->render (new RfmFilterControllerMock(array('format' => 'ajax')), $options));
            $this->assertEquals ('ajax', $options['pass'][0]);
        }

        /**
         * 测试执行render前过滤，指定非get方法。
         */
        public function testRenderForMethod () {
            $options = array();
            $filter = new RfmFilter();

            $this->assertTrue ($filter->render (new RfmFilterControllerMock(array(), 'post'), $options));
            $this->assertEquals ('post', $options['pass'][0]);
        }

        /**
         * 测试render前过滤，指定格式和方法
         */
        public function testRenderForFormatAndMethod () {
            $options = array();
            $filter = new RfmFilter();

            $this->assertTrue ($filter->render (new RfmFilterControllerMock(array('format' => 'ajax'), 'post'), $options));
            $this->assertTrue (is_array ($options['pass']));
            $this->assertEquals ('ajax', $options['pass'][0]);
            $this->assertEquals ('post', $options['pass'][1]);
        }
    }

    /**
     * 测试请求模拟类
     *
     * @package jtita.controllers.filters
     */
    class RmfFilterRequestMock extends ApplicationRequest {
        public $method = 'get';

        public function method () {
            return $this->method;
        }
    }

    /**
     * 测试模拟类。
     */
    class RfmFilterControllerMock {
        public $request = null;

        public function __construct ($params = array(), $method = 'get') {
            $this->request = new RmfFilterRequestMock();
            $this->request->initialize ($params);
            $this->request->method = $method;
        }

        public function isGet () {
            return $this->request->method == 'get';
        }
    }