<?php
    /**
     * normal_view_test.inc 提供对普通视图对象的测试服务；
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.views
     * @version     1.0 beta
     *
     * @create      2012-01-29
     * @modify      2013-04-28
     */

     # 定义顶级路径；
    defined ('JTPATH_ROOT')     or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (__FILE__))))));

    defined ('JTPATH_APP')      or define('JTPATH_APP',     JTPATH_ROOT . '/app');
    defined ('JTPATH_MODULE')   or define('JTPATH_MODULE',  JTPATH_ROOT . '/app/modules');
    defined ('JTPATH_JTITA')    or define('JTPATH_JTITA',   JTPATH_ROOT . '/jtita');

    /**
     * 导入测试内容和依赖文件；
     */
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/applications/application_response.inc');

    # 导入被测试类
    require_once(JTPATH_ROOT . '/jtita/views/view.inc');
    require_once(JTPATH_ROOT . '/jtita/views/normal_view.inc');
    require_once(JTPATH_ROOT . '/test/units/jtita/views/view_mock.inc');

    /**
     * 普通视图模拟对象，提供普通视图测试所需的服务。
     *
     * @package jtita.views
     */
    class NormalViewMock extends NormalView {
        public function templateFiles () {
            return $this->_templateFiles ();
        }

        public function layoutPaths () {
            return $this->_layoutPaths ();
        }

        public function templatePaths () {
            return $this->_templatePaths ();
        }

        public function helperPaths () {
            return $this->_helperPaths ();
        }

        public function elementPaths () {
            return $this->_elementPaths ();
        }
    }

    /**
     * 普通对象测试服务。
     *
     * @package jtita.views
     */
    class NormalViewTest extends PHPUnit_Framework_TestCase {
        /**
         * 测试query方法
         */
        public function testQuery () {
            $controller = new ViewControllerMock(array('s' => 'Hello World'));
            $view = new NormalViewMock();
            $view->initialize ($controller);

            $this->assertEquals ('Hello World', $view->query ('s'));
            $this->assertEquals ('hello php', $view->query ('default-value', 'hello php'));
            $this->assertFalse ($view->query ('not-exists'));
        }

        /**
         * 测试模板文件集；
         */
        public function testTemplateFiles () {
            $controller = new ViewControllerMock(array(
                                                      'action' => 'index',
                                                      'controllers' => 'users'
                                                 ));
            $view = new NormalViewMock();
            $view->initialize ($controller);

            $files = $view->templateFiles ();
            $this->assertEquals ('users.index', $files[0]);
        }

        /**
         * 测试模板文件路径，包含pass信息；
         *
         * @depends testTemplateFiles
         */
        public function testTemplateFilesForIncludePass () {
            $controller = new ViewControllerMock(array(
                                                      'action' => 'index',
                                                      'controllers' => 'users'
                                                 ));
            $view = new NormalViewMock();
            $view->initialize ($controller, array(
                                                 'pass' => array(
                                                     'ajax',
                                                     '1'
                                                 )
                                            ));

            $files = $view->templateFiles ();
            $this->assertEquals ('users.ajax.1.index', $files[0]);
            $this->assertEquals ('users.ajax.index', $files[1]);
            $this->assertEquals ('users.index', $files[2]);
        }

        /**
         * 测试模板文件集，包含模块；
         */
        public function testTemplateFilesForIncludePackage () {
            $controller = new ViewControllerMock(array(
                                                      'action' => 'index',
                                                      'controllers' => 'users',
                                                      'package' => 'admins'
                                                 ));
            $view = new NormalViewMock();
            $view->initialize ($controller);

            $files = $view->templateFiles ();
            $this->assertEquals ('admins.users.index', $files[0]);
        }

        /**
         * 测试模板文件路径，包含pass信息；
         *
         * @depends testTemplateFilesForIncludePackage
         */
        public function testTemplateFilesForIncludePackageAndPass () {
            $controller = new ViewControllerMock(array(
                                                      'action' => 'index',
                                                      'controllers' => 'users',
                                                      'package' => 'admins'
                                                 ));
            $view = new NormalViewMock();
            $view->initialize ($controller, array(
                                                 'pass' => array(
                                                     'ajax',
                                                     '1'
                                                 )
                                            ));

            $files = $view->templateFiles ();
            $this->assertEquals ('admins.users.ajax.1.index', $files[0]);
            $this->assertEquals ('admins.users.ajax.index', $files[1]);
            $this->assertEquals ('admins.users.index', $files[2]);
        }

        /**
         * 测试模型文件，包含字条串pass信息
         */
        public function testTemplateFilesForIncludePackageAndPassIsString () {
            $controller = new ViewControllerMock(array(
                                                      'action' => 'index',
                                                      'controllers' => 'users',
                                                      'package' => 'admins'
                                                 ));
            $view = new NormalViewMock();
            $view->initialize ($controller, array('pass' => 'ajax'));

            $files = $view->templateFiles ();
            $this->assertEquals ('admins.users.ajax.index', $files[0]);
            $this->assertEquals ('admins.users.index', $files[1]);
        }

        /**
         * 测试模板文件集，包含文件名选项；
         */
        public function testTemplateFilesForIncludeFileName () {
            $controller = new ViewControllerMock(array(
                                                      'action' => 'index',
                                                      'controllers' => 'users',
                                                      'package' => 'admins'
                                                 ));
            $view = new NormalViewMock();
            $view->initialize ($controller, array(
                                                 'template' => 'elements.mini_top',
                                                 'pass' => array('ajax')
                                            ));

            $files = $view->templateFiles ();
            $this->assertEquals ('elements.mini_top', $files[0]);
        }


        /**
         * 测试模板文件路径；
         */
        public function testTemplatePaths () {
            $controller = new ViewControllerMock(array(
                                                      'action' => 'index',
                                                      'controllers' => 'users'
                                                 ));
            $view = new NormalViewMock();
            $view->initialize ($controller);

            $paths = $view->templatePaths ();

            $this->assertEquals (JTPATH_APP . '/views', $paths[0]);
            $this->assertEquals (JTPATH_JTITA . '/views', $paths[1]);
        }

        /**
         * 测试模板文件路径，包含模块
         */
        public function testTemplatePathsForIncludeModule () {
            $controller = new ViewControllerMock(array(
                                                      'action' => 'index',
                                                      'controllers' => 'users',
                                                      'module' => 'members'
                                                 ));
            $view = new NormalViewMock();
            $view->initialize ($controller);

            $paths = $view->templatePaths ();

            $this->assertEquals (JTPATH_MODULE . '/members/views', $paths[0]);
            $this->assertEquals (JTPATH_APP . '/views/members', $paths[1]);
        }


        /**
         * 测试布局文件路径；
         */
        public function testLayoutPaths () {
            $controller = new ViewControllerMock(array(
                                                      'action' => 'index',
                                                      'controllers' => 'users'
                                                 ));
            $view = new NormalViewMock();
            $view->initialize ($controller);

            $paths = $view->layoutPaths ();
            $this->assertEquals (JTPATH_APP . '/views/_layouts', $paths[0]);
            $this->assertEquals (JTPATH_JTITA . '/views/_layouts', $paths[1]);
        }

        /**
         * 测试布局文件路径，包含模块；
         */
        public function testLayoutPathsForIncludeModule () {
            $controller = new ViewControllerMock(array(
                                                      'action' => 'index',
                                                      'controllers' => 'users',
                                                      'module' => 'admins'
                                                 ));
            $view = new NormalViewMock();
            $view->initialize ($controller);

            $paths = $view->layoutPaths ();

            $this->assertEquals (JTPATH_MODULE . '/admins/views/_layouts', $paths[0]);
            $this->assertEquals (JTPATH_APP . '/views/_layouts', $paths[1]);
            $this->assertEquals (JTPATH_JTITA . '/views/_layouts', $paths[2]);
        }

        /**
         * 测试助手路径；
         */
        public function testHelperPaths () {
            $controller = new ViewControllerMock(array(
                                                      'action' => 'index',
                                                      'controllers' => 'users'
                                                 ));
            $view = new NormalViewMock();
            $view->initialize ($controller);

            $paths = $view->helperPaths ();

            $this->assertEquals (JTPATH_APP . '/views/_helpers', $paths[0]);
            $this->assertEquals (JTPATH_JTITA . '/views/_helpers', $paths[1]);
        }

        /**
         * 测试助手路径，包含模板；
         */
        public function testHelperPathsForIncludeModule () {
            $controller = new ViewControllerMock(array(
                                                      'action' => 'index',
                                                      'controllers' => 'users',
                                                      'module' => 'members'
                                                 ));
            $view = new NormalViewMock();
            $view->initialize ($controller);

            $paths = $view->helperPaths ();

            $this->assertEquals (JTPATH_MODULE . '/members/views/_helpers', $paths[0]);
            $this->assertEquals (JTPATH_APP . '/views/_helpers', $paths[1]);
            $this->assertEquals (JTPATH_JTITA . '/views/_helpers', $paths[2]);
        }

        /**
         * 测试元素路径；
         */
        public function testElementPaths () {
            $controller = new ViewControllerMock(array(
                                                      'action' => 'index',
                                                      'controllers' => 'users'
                                                 ));
            $view = new NormalViewMock();
            $view->initialize ($controller);

            $paths = $view->elementPaths ();

            $this->assertEquals (JTPATH_APP . '/views/_elements', $paths[0]);
            $this->assertEquals (JTPATH_JTITA . '/views/_elements', $paths[1]);
        }

        /**
         * 测试元素路径，包含模块；
         */
        public function testElementPathsForModule () {
            $controller = new ViewControllerMock(array(
                                                      'action' => 'index',
                                                      'controllers' => 'users',
                                                      'module' => 'members'
                                                 ));
            $view = new NormalViewMock();
            $view->initialize ($controller);

            $paths = $view->elementPaths ();

            $this->assertEquals (JTPATH_MODULE . '/members/views/_elements', $paths[0]);
            $this->assertEquals (JTPATH_APP . '/views/_elements', $paths[1]);
            $this->assertEquals (JTPATH_JTITA . '/views/_elements', $paths[2]);
        }
    }
