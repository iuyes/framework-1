<?php
    /**
     * router_styles_test.inc 提供路由样式单元测试服务；
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.routers
     * @version     1.0 beta
     *
     * @create      2012-08-02
     * @modify      2013-04-28
     */

    /**
     * 定义顶级路径；
     */
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (__FILE__))))));

    # 导入基础方法
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');

    # 初始化导入路径。
    Jtita::paths (JTPATH_ROOT . '/jtita');

    /**
     * 引用测试内容；
     */
    require_once(JTPATH_ROOT . '/jtita/routers/router.inc');

    /**
     * 路由样式单元测试服务
     *
     * @package jtita.routers
     */
    class RouterStylesTest extends PHPUnit_Framework_TestCase {
        /**
         * 测试普通URL样式。
         */
        public function testNormalStyle () {
            $style = new NormalRouterStyle();
            $url = $style->url ('http://www.example.com:8080');

            $this->assertEquals ('http://www.example.com:8080', $url);
        }

        /**
         * 测试普通URL，指定参数。
         */
        public function testNormalStyleForParams () {
            $style = new NormalRouterStyle();
            $url = $style->url ('http://www.example.com', array('ctl' => 'index', 'action' => 'hello'));

            $this->assertEquals ('http://www.example.com/?ctl=index&action=hello', $url);
        }

        /**
         * 测试普通URL，指定关键字参数
         */
        public function testNormalStyleForKeyParams () {
            $style = new NormalRouterStyle();
            $url = $style->url ('http://www.example.com', array('ctl' => '<>a:"'));

            $this->assertEquals ('http://www.example.com/?ctl=%3C%3Ea%3A%22', $url);
        }

        /**
         * 测试普通URL,指定脚本名称。
         */
        public function testNormalStyleForScriptName () {
            $style = new NormalRouterStyle();
            $style->scriptName = 'admin.php';

            $url = $style->url ('http://www.example.com', array('ctl' => 'index'));
            $url1 = $style->url ('http://www.example.com');

            $this->assertEquals ('http://www.example.com/admin.php?ctl=index', $url);
            $this->assertEquals ('http://www.example.com/admin.php', $url1);
        }

        /**
         * 测试普通URL，指定锚
         */
        public function testNormalStyleForAnchor () {
            $style = new NormalRouterStyle();
            $url = $style->url ('http://www.example.com', array('ctl' => 'index', 'action' => 'hello', '#' => '/af>k'));
            $emUrl = $style->url ('http://www.example.com', array('#' => '/af>k'));

            $this->assertEquals ('http://www.example.com/?ctl=index&action=hello#%2Faf%3Ek', $url);
            $this->assertEquals ('http://www.example.com/#%2Faf%3Ek', $emUrl);
        }

        /**
         * 测试PathInfo
         */
        public function testPathInfo () {
            $style = new PathinfoRouterStyle();
            $url = $style->url ('http://example.com');

            $this->assertEquals ('http://example.com', $url);
        }

        /**
         * 测试pathinfo，指定参数信息
         */
        public function testPathInfoForParams () {
            $style = new PathinfoRouterStyle();
            $url = $style->url ('http://example.com', array('ctl' => 'hello', 'act' => 'index'));

            $this->assertEquals ('http://example.com/index.php?ctl=hello&act=index', $url);
        }

        /**
         * 测试pathinfo，使用关键字参数
         */
        public function testPathInfoForKeyParams () {
            $style = new PathinfoRouterStyle();
            $url = $style->url ('http://example.com', array('ctl' => '<>/', 'act' => '13/ab'));

            $this->assertEquals ('http://example.com/index.php?ctl=%3C%3E%2F&act=13%2Fab', $url);
        }

        /**
         * 测试pathinfo，使用重写参数
         */
        public function testPathInfoForRewrite () {
            $style = new PathinfoRouterStyle();
            $url = $style->url ('http://example.com', array('url' => 'index/hello', 'cat' => '1023', 'id' => '19f'));

            $this->assertEquals ('http://example.com/index.php/index/hello?cat=1023&id=19f', $url);
        }

        /**
         * 测试pathinfo，指定script名称
         */
        public function testPathInfoForScript () {
            $style = new PathinfoRouterStyle();
            $style->scriptName = 'admin.php';

            $url = $style->url ('http://www.example.com', array('ctl' => 'index'));
            $url1 = $style->url ('http://www.example.com');

            $this->assertEquals ('http://www.example.com/admin.php?ctl=index', $url);
            $this->assertEquals ('http://www.example.com/admin.php', $url1);
        }

        /**
         * 测试pathinfo，指定锚
         */
        public function testPathInfoForAnchor () {
            $style = new PathinfoRouterStyle();

            $url = $style->url ('http://www.example.com', array('ctl' => 'index', 'action' => 'hello', '#' => '/af>k'));
            $reUrl = $style->url ('http://www.example.com', array('url' => 'index/hello', '#' => '/af>k'));
            $emUrl = $style->url ('http://www.example.com', array('#' => '/af>k'));

            $this->assertEquals ('http://www.example.com/index.php?ctl=index&action=hello#%2Faf%3Ek', $url);
            $this->assertEquals ('http://www.example.com/index.php/index/hello#%2Faf%3Ek', $reUrl);
            $this->assertEquals ('http://www.example.com/index.php#%2Faf%3Ek', $emUrl);
        }

        /**
         * 测试重写样式
         */
        public function testRewriteForNormal () {
            $style = new RewriteRouterStyle();
            $url = $style->url ('http://www.example.com');

            $this->assertEquals ('http://www.example.com', $url);
        }

        /**
         * 测试重写样式，指定参数
         */
        public function testRewriteForParams() {
            $style = new RewriteRouterStyle();
            $url = $style->url ('http://example.com', array('ctl' => 'hello', 'act' => 'index'));

           $this->assertEquals ('http://example.com?ctl=hello&act=index', $url);
        }

        /**
         * 测试重写样式，使用关键字参数
         */
        public function testRewriteForKeyParams () {
            $style = new RewriteRouterStyle();
            $url = $style->url ('http://example.com', array('ctl' => '<>/', 'act' => '13/ab'));

            $this->assertEquals ('http://example.com?ctl=%3C%3E%2F&act=13%2Fab', $url);
        }

        /**
         * 测试重写样式，使用重写参数
         */
        public function testRewriteForRewrite () {
            $style = new RewriteRouterStyle();
            $url = $style->url ('http://example.com', array('url' => 'index/hello', 'cat' => '1023', 'id' => '19f'));

            $this->assertEquals ('http://example.com/index/hello?cat=1023&id=19f', $url);
        }

        /**
         * 测试重写样式，指定锚
         */
        public function testRewriteForAnchor () {
            $style = new RewriteRouterStyle();
            $url = $style->url ('http://www.example.com', array('ctl' => 'index', 'action' => 'hello', '#' => '/af>k'));
            $reUrl = $style->url ('http://www.example.com', array('url' => 'index/hello', '#' => '/af>k'));
            $emUrl = $style->url ('http://www.example.com', array('#' => '/af>k'));

            $this->assertEquals ('http://www.example.com?ctl=index&action=hello#%2Faf%3Ek', $url);
            $this->assertEquals ('http://www.example.com/index/hello#%2Faf%3Ek', $reUrl);
            $this->assertEquals ('http://www.example.com/#%2Faf%3Ek', $emUrl);
        }

    }
