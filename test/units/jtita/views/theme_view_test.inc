<?php
    /**
     * theme_view_test.php 提供对主题视图对象的测试服务；
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
     * @modify      2013-04-07
     */

    /**
     * 定义顶级路径；
     */
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (__FILE__))))));

    defined ('JTPATH_APP')    or define('JTPATH_APP',    JTPATH_ROOT . '/app');
    defined ('JTPATH_MODULE') or define('JTPATH_MODULE', JTPATH_ROOT . '/app/modules');
    defined ('JTPATH_JTITA')  or define('JTPATH_JTITA',  JTPATH_ROOT . '/jtita');

    /**
     * 导入测试内容和依赖文件；
     */
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/applications/application_response.inc');

    # 导入被测试类
    require_once(JTPATH_ROOT . '/jtita/views/view.inc');
    require_once(JTPATH_ROOT . '/jtita/views/normal_view.inc');
    require_once(JTPATH_ROOT . '/jtita/views/theme_view.inc');
    require_once(JTPATH_ROOT . '/test/units/jtita/views/view_mock.inc');

    /**
     * 普通视图模拟对象，提供普通视图测试所需的服务。
     *
     * @package jtita.views
     */
    class ThemeViewMock extends ThemeView {
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
     * 视图控制器模拟对象；
     *
     * @package jtita.views
     */
    class ThemeControllerMock extends ViewControllerMock {

    }

    /**
     * 提供主题视图测试服务
     *
     * @package jtita.views
     */
    class TestThemeView extends PHPUnit_Framework_TestCase {
        /**
         * 测试主题名称，未提供主题名称；
         */
        public function testThemeNameForNotSupportTheme () {
            $controller = new ViewControllerMock();
            $view = new ThemeViewMock();
            $view->initialize ($controller);

            $this->assertEquals (ThemeView::DEFAULT_THEME, $view->themeName);
        }

        /**
         * 测试模板路径；
         */
        public function testTemplatePaths () {
            $controller = new ThemeControllerMock();
            $view = new ThemeViewMock();
            $view->initialize ($controller);
            $view->themeName = '__bootstrap';

            $paths = $view->templatePaths ();

            $this->assertEquals (JTPATH_APP . '/themes/__bootstrap', $paths[0]);
            $this->assertEquals (JTPATH_APP . '/views', $paths[1]);
        }

        /**
         * 测试模板路径，包含模块；
         */
        public function testTemplatePathsForIncludeModule () {
            $controller = new ThemeControllerMock(array('module' => 'members'));
            $view = new ThemeViewMock();
            $view->initialize ($controller);
            $view->themeName = '__bootstrap';

            $paths = $view->templatePaths ();

            $this->assertEquals (JTPATH_APP . '/themes/__bootstrap/members', $paths[0]);
            $this->assertEquals (JTPATH_MODULE . '/members/views', $paths[1]);
            $this->assertEquals (JTPATH_APP . '/views/members', $paths[2]);
        }

        /**
         * 测试布局路径；
         */
        public function testLayoutPaths () {
            $controller = new ThemeControllerMock();
            $view = new ThemeViewMock();
            $view->initialize ($controller);
            $view->themeName = '__bootstrap';

            $paths = $view->layoutPaths ();

            $this->assertEquals (JTPATH_APP . '/themes/__bootstrap/_layouts', $paths[0]);
            $this->assertEquals (JTPATH_APP . '/views/_layouts', $paths[1]);
            $this->assertEquals (JTPATH_JTITA . '/views/_layouts', $paths[2]);
        }

        /**
         * 测试布局文件路径，包含模块；
         */
        public function testLayoutPathsForIncludeModule () {
            $controller = new ThemeControllerMock(array('module' => 'members'));
            $view = new ThemeViewMock();
            $view->initialize ($controller);
            $view->themeName = '__bootstrap';

            $paths = $view->layoutPaths ();

            $this->assertEquals (JTPATH_APP . '/themes/__bootstrap/members/_layouts', $paths[0]);
            $this->assertEquals (JTPATH_APP . '/themes/__bootstrap/_layouts', $paths[1]);
            $this->assertEquals (JTPATH_MODULE . '/members/views/_layouts', $paths[2]);
            $this->assertEquals (JTPATH_APP . '/views/_layouts', $paths[3]);
            $this->assertEquals (JTPATH_JTITA . '/views/_layouts', $paths[4]);
        }

        /**
         * 测试助手路径；
         */
        public function testHelperPaths () {
            $controller = new ThemeControllerMock(array(
                                                       'action' => 'index',
                                                       'controllers' => 'users'
                                                  ));
            $view = new ThemeViewMock();
            $view->initialize ($controller);
            $view->themeName = '__bootstrap';

            $paths = $view->helperPaths ();

            $this->assertEquals (JTPATH_APP . '/themes/__bootstrap/_helpers', $paths[0]);
            $this->assertEquals (JTPATH_APP . '/views/_helpers', $paths[1]);
            $this->assertEquals (JTPATH_JTITA . '/views/_helpers', $paths[2]);
        }

        /**
         * 测试助手路径，包含模板；
         */
        public function testHelperPathsForIncludeModule () {
            $controller = new ThemeControllerMock(array(
                                                       'action' => 'index',
                                                       'controllers' => 'users',
                                                       'module' => 'members'
                                                  ));
            $view = new ThemeViewMock();
            $view->initialize ($controller);
            $view->themeName = '__bootstrap';

            $paths = $view->helperPaths ();

            $this->assertEquals (JTPATH_APP . '/themes/__bootstrap/members/_helpers', $paths[0]);
            $this->assertEquals (JTPATH_APP . '/themes/__bootstrap/_helpers', $paths[1]);
            $this->assertEquals (JTPATH_MODULE . '/members/views/_helpers', $paths[2]);
            $this->assertEquals (JTPATH_APP . '/views/_helpers', $paths[3]);
            $this->assertEquals (JTPATH_JTITA . '/views/_helpers', $paths[4]);
        }

        /**
         * 测试元素路径；
         */
        public function testElementPaths () {
            $controller = new ThemeControllerMock(array(
                                                       'action' => 'index',
                                                       'controllers' => 'users'
                                                  ));
            $view = new ThemeViewMock();
            $view->initialize ($controller);
            $view->themeName = '__bootstrap';

            $paths = $view->elementPaths ();

            $this->assertEquals (JTPATH_APP . '/themes/__bootstrap/_elements', $paths[0]);
            $this->assertEquals (JTPATH_APP . '/views/_elements', $paths[1]);
            $this->assertEquals (JTPATH_JTITA . '/views/_elements', $paths[2]);
        }

        /**
         * 测试元素路径，包含模块；
         */
        public function testElementPathsForModule () {
            $controller = new ThemeControllerMock(array(
                                                       'action' => 'index',
                                                       'controllers' => 'users',
                                                       'module' => 'members'
                                                  ));
            $view = new ThemeViewMock();
            $view->initialize ($controller);
            $view->themeName = '__bootstrap';

            $paths = $view->elementPaths ();

            $this->assertEquals (JTPATH_APP . '/themes/__bootstrap/members/_elements', $paths[0]);
            $this->assertEquals (JTPATH_APP . '/themes/__bootstrap/_elements', $paths[1]);
            $this->assertEquals (JTPATH_MODULE . '/members/views/_elements', $paths[2]);
            $this->assertEquals (JTPATH_APP . '/views/_elements', $paths[3]);
            $this->assertEquals (JTPATH_JTITA . '/views/_elements', $paths[4]);
        }

    }
