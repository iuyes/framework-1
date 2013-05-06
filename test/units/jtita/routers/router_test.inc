<?php
    /**
     * router_test.inc 声明路由服务组件测试服务；
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.routers
     * @version     1.0 beta
     *
     * @create      2012-07-23
     * @modify      2013-04-28
     */

    # 声明根节点
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (__FILE__))))));
    defined('JTROUTER_DEFAULT_DOMAIN') or define('JTROUTER_DEFAULT_DOMAIN','www.jtita.com');

    # 引用测试依赖
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');

    Jtita::paths (JTPATH_ROOT . '/jtita');

    # 引用被测试类
    require_once(JTPATH_ROOT . '/jtita/routers/router.inc');

    /**
     * 路由测试类，提供路由服务的单元测试
     *
     * @package jtita.routers
     */
    class RouterTest extends PHPUnit_Framework_TestCase {
        public function setUp() {
            RouterMock::reset();
        }

        /**
         * 测试增加域名。
         */
        public function testDomain () {
            RouterMock::domain ('www.jtita.com', array(
                                'style' => 'normal',
                                'domain' => 'http://www.jtita.com'));
            RouterMock::domain('www.jtita.com');

            $domains = RouterMock::domains();

            $this->assertTrue(is_array($domains));
            $this->assertEquals('normal',$domains['www.jtita.com']['style']);
            $this->assertEquals('http://www.jtita.com',$domains['www.jtita.com']['domain']);
        }

        /**
         * 测试增加域名，增加规则。
         */
        public function testDomainRule() {
            # 注册域名信息。
            RouterMock::domain('news.jtita.com',array(
                               'style' => 'normal',
                               'domain' => 'http://news.jtita.com'))
                        ->rule(array('class' => 'normal_router_style',
                                     'controller' => 'goods'));

            $rules = RouterMock::rules();

            $this->assertEquals(1,count($rules));
            $this->assertEquals('goods',$rules[0]['rule']['controller']);
            $this->assertEquals('news.jtita.com',$rules[0]['domain']);
            $this->assertEquals('normal_router_style',$rules[0]['rule']['class']);
        }

        /**
         * 测试增加规则，多次增加
         */
        public function testDomainRuleForMultiRule() {
            # 注册域名信息。
            RouterMock::domain('news.jtita.com',array('style' => 'normal','domain' => 'http://news.jtita.com'));
            RouterMock::domain('www.jtita.com',array('style' => 'rewrite','domain' => 'http://www.jtita.com'));

            RouterMock::domain('news.jtita.com')->rule(array(
                               'class' => 'normal_router_style','controller' => 'goods'));
            RouterMock::domain('www.jtita.com')->rule(array(
                'class' => 'regex_router_style','express' => '/:controller/:action'));

            $rules = RouterMock::rules();

            $this->assertEquals(2,count($rules));
            $this->assertEquals('goods',$rules[0]['rule']['controller']);
            $this->assertEquals('news.jtita.com',$rules[0]['domain']);
            $this->assertEquals('normal_router_style',$rules[0]['rule']['class']);

            $this->assertEquals('www.jtita.com',$rules[1]['domain']);
            $this->assertEquals('regex_router_style',$rules[1]['rule']['class']);
            $this->assertEquals('/:controller/:action',$rules[1]['rule']['express']);
        }

        /**
         * 测试增加规则，指定名称。
         */
        public function testDomainRuleForStringName() {
            RouterMock::domain('www.jtita.com',array('domain' => 'http://www.jtita.com'));
            RouterMock::domain('www.jtita.com')
                ->rule(array('class' => 'regex','express' => 'hello'),'archive')
                ->rule(array('class' => 'normal','express' => 'goods'),'goods');

            $rules  = RouterMock::rules();
            $links = RouterMock::links();

            $this->assertEquals(2,count($rules));
            $this->assertEquals(3,count($links));
        }

        /**
         * 测试增加规则，指定数组名称
         */
        public function testDomainRuleForArrayName() {
            RouterMock::domain('www.jtita.com',array('domain' => 'http://www.jtita.com'));
            RouterMock::domain('www.jtita.com')
                ->rule(array('class' => 'regex','express' => 'hello'),array('archive','goods'))
                ->rule(array('class' => 'normal','express' => 'goods'),array('goods','archive'));

            $rules  = RouterMock::rules();
            $links = RouterMock::links();

            $this->assertEquals(2,count($rules));
            $this->assertEquals(3,count($links));
        }

        /**
         * 测试参数匹配。
         */
        public function testMatch() {
            # 初始化路由
            RouterMock::domain('www.jtita.com',array('domain' => 'http://www.jtita.com','style'  => 'normal'))
                   ->rule(array('class' => 'normal_router_rule'));

            $params = RouterMock::match(array('controller' => 'goods','action' => 'show'));

            $this->assertTrue(is_array($params));
            $this->assertEquals('goods',$params['controller']);
            $this->assertEquals('show',$params['action']);
        }

        /**
         * 测试参数匹配失，没有规则。
         */
        public function testMatchForRuleEmptyFail() {
            # 初始化路由
            RouterMock::domain('www.jtita.com',array('domain' => 'http://www.jtita.com','style'  => 'normal'));
            $this->assertFalse(RouterMock::match(array('controller' => 'goods','action' => 'show')));
        }

        /**
         * 测试参数匹配，验证失败。
         */
        public function testMatchForRuleFail() {
            # 初始化路由
            RouterMock::domain('www.jtita.com',array('domain' => 'http://www.jtita.com','style'  => 'normal'))
                        ->rule(array('class' => 'normal_router_rule','controller' => array(null,'members')));

            $this->assertFalse(RouterMock::match(array('controller' => 'goods','action' => 'show')));
        }

        /**
         * 测试URL生成。
         */
        public function testUrl() {
            # 初始化路由
            RouterMock::domain('www.jtita.com',array('domain' => 'http://www.jtita.com','style'  => 'normal_router_style'))
                                ->rule(array('class' => 'normal_router_rule'));

            $this->assertEquals('http://www.jtita.com/?controller=goods&action=show',
                                RouterMock::url(array('controller' => 'goods','action' => 'show')));
        }

        /**
         * 测试URL生成，指定规则名字。
         */
        public function testUrlForName() {
            # 初始化路由
            RouterMock::domain('goods.jtita.com',array('domain' => 'http://goods.jtita.com','style'  => 'normal_router_style'))
                ->rule(array('class' => 'normal_router_rule'),'goods');

            $this->assertEquals('http://goods.jtita.com/?controller=goods&action=show',
                                 RouterMock::url(array('controller' => 'goods','action' => 'show'),'goods'));
        }

        /**
         * 测试URL生成，生成失败，领域对象位提供
         *
         * @expectedException RouterException
         */
        public function testUrlForDomainNotSupport() {
            # 初始化路由
            RouterMock::domain('goods.jtita.com')
                ->rule(array('class' => 'normal_router_rule'),'goods');

            RouterMock::url(array('controller' => 'goods','action' => 'show'),'goods');
        }

        /**
         * 测试URL生成，生成反转失败
         *
         * @expectedException RouterException
         */
        public function testUrlForNotSupportRule() {
            # 初始化路由
            RouterMock::domain('goods.jtita.com',array('domain' => 'http://goods.jtita.com'));

            RouterMock::url(array('controller' => 'goods','action' => 'show'),'goods');
        }
    }

    /**
     * 路由模拟测试类，提供路由需要测试的服务。
     *
     * @package jtita.routers
     */
    class RouterMock extends Router {
        public static function reset () {
            self::$_domains = array();
            self::$_links = array();
            self::$_rules = array();
        }

        public static function domains () {
            return self::$_domains;
        }

        public static function links () {
            return self::$_links;
        }

        public static function rules () {
            return self::$_rules;
        }
    }