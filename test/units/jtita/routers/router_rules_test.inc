<?php
    /**
     * router_rules_test.inc 声明路由规则单元测试服务；
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.routers
     * @version     1.0 beta
     *
     * @create      2012-08-05
     * @modify      2013-04-28
     */

    /**
     * 定义顶级路径；
     */
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (__FILE__))))));
    defined('JTROUTER_DEFAULT_DOMAIN') or define('JTROUTER_DEFAULT_DOMAIN','www.jtita.com');

    /**
     * 导入测试内容和依赖文件；
     */
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');

    Jtita::paths (JTPATH_ROOT . '/jtita');

    require_once(JTPATH_ROOT . '/jtita/routers/router.inc');


    /**
     * 路由规则测试，提供对默认路由规则的测试服务。
     *
     * @package jtital.routers
     */
    class RouterRuleTest extends PHPUnit_Framework_TestCase {
        /**
         * 测试默认规则。
         */
        public function testNormalRuleParse () {
            $rule = Jtita::ioc (array('class' => 'normal_router_rule'));
            $params = $rule->parse (array('name' => 'zhenggp', 'age' => '30'));

            $this->assertEquals ('zhenggp', $params['name']);
            $this->assertEquals ('30', $params['age']);
        }

        /**
         * 测试普通规则转换，指定默认值。
         */
        public function testNormalRuleParseForDefaults () {
            # 初始化规则
            $rule = Jtita::ioc (array('class' => 'normal_router_rule',
                                      'name'  => 'zhenggp',
                                      'age'   => array('30')));

            $params = $rule->parse (array('age' => '31'));

            $this->assertEquals ('zhenggp', $params['name']);
            $this->assertEquals ('31', $params['age']);
        }

        /**
         * 测试普通规则，返回转换值，指定验证器。
         */
        public function testNormalRuleParseForValidatorSuccess() {
            $rule = Jtita::ioc (array('class' => 'normal_router_rule',
                                      'age'  => array(null,'^\d+$'),
                                      'name' => array('zhenggp','^[a-z]+$'),
                                      'package' => array(null,'admins|admin.report')));

            $this->assertNotEmpty($rule->parse(array('name' => 'chongqing','age' => 30))) ;
            $this->assertNotEmpty($rule->parse(array('name' => 'china')));
            $this->assertNotEmpty($rule->parse(array('package' => 'admin.report')));
        }

        /**
         * 测试普通规则，返回转换值，转换失败。
         */
        public function testNormalRuleParseForValidatorFail() {
            $rule = Jtita::ioc (array('class' => 'normal_router_rule',
                                      'age'  => array(null,'^\d+$'),
                                      'name' => array('zhenggp','^[a-z]+$')));

            $this->assertFalse($rule->parse(array('name' => 'chongqing','age' => '12 two'))) ;
            $this->assertFalse($rule->parse(array('name' => 'china13')));
        }

        /**
         * 测试普通规则，返回转换，指定必选值
         */
        public function testNormalRuleParseForRequired() {
            $rule = Jtita::ioc (array('class' => 'normal_router_rule',
                                      'age'  => array(null,false,true),
                                      'name' => array('zhenggp',false,true)));

            $this->assertFalse($rule->parse(array('name' => 'chongqing'))) ;
            $this->assertNotEmpty($rule->parse(array('age' => '15')));
        }

        /**
         * 测试普通规则，返回参数，包含默认值。
         */
        public function testNormalRuleReverseForDefaults () {
            # 初始化规则
            $rule = Jtita::ioc (array('class' => 'normal_router_rule',
                                      'name'  => 'zhenggp'));

            $params = $rule->reverse (array('name' => 'zhenggp', 'age' => '30'));

            $this->assertFalse (isset($params['name']));
            $this->assertEquals ('30', $params['age']);
        }

        /**
         * 测试普通规则反转，指定验证器验证成功。
         */
        public function testNormalRuleReverseForValidatorSuccess() {
            $rule = Jtita::ioc (array('class' => 'normal_router_rule',
                                      'age'  => array(null,'^\d+$'),
                                      'name' => array('zhenggp','^[a-z]+$'),
                                      'package' => array(null,'admins|admin.report')));

            $this->assertNotEmpty($rule->reverse(array('name' => 'chongqing','age' => 30))) ;
            $this->assertNotEmpty($rule->reverse(array('name' => 'china')));
            $this->assertNotEmpty($rule->reverse(array('package' => 'admin.report')));
        }

        /**
         * 测试普通规则，返回转换值，转换失败。
         */
        public function testNormalRuleReverseForValidatorFail() {
            $rule = Jtita::ioc (array('class' => 'normal_router_rule',
                                      'age'  => array(null,'^\d+$'),
                                      'name' => array('zhenggp','^[a-z]+$')));

            $this->assertFalse($rule->reverse(array('name' => 'chongqing','age' => '12 two'))) ;
            $this->assertFalse($rule->reverse(array('name' => 'china13')));
        }

        /**
         * 测试普通规则，返回转换，指定必选值
         */
        public function testNormalRuleReverseForRequired() {
            $rule = Jtita::ioc (array('class' => 'normal_router_rule',
                                      'age'  => array(null,false,true),
                                      'name' => array('zhenggp',false,true)));

            $this->assertFalse($rule->reverse(array('name' => 'chongqing'))) ;
            $this->assertNotEmpty($rule->reverse(array('age' => '15')));
        }

        /**
         * 测试正则表达式转换，验证表达式为空。
         */
        public function testRegexRuleParseForEmptyExpress() {
            $rule = Jtita::ioc(array('class'    => 'regex_router_rule'));

            $this->assertFalse($rule->parse(array('name' => 'zhenggp')));
        }



        /**
         * 测试正则表达式转换。
         */
        public function testRegexRuleParse() {
            $rule = Jtita::ioc(array('class'    => 'regex_router_rule',
                                     'express'  => '/:controller/:action'));

            $this->assertFalse($rule->parse(array('url' => '/af1/f1')));

            $params = $rule->parse(array('url' => '/goods/show'));

            $this->assertEquals('goods',$params['controller']);
            $this->assertEquals('show',$params['action']);
        }

        /**
         * 测试正则表达式转换，不同的分隔。
         */
        public function testRegexRuleParseSep() {
            $rule = Jtita::ioc(array('class'    => 'regex_router_rule',
                                     'express'  => '/:controller-:action'));
            $params = $rule->parse(array('url' => '/member-profile'));

            $this->assertEquals('member',$params['controller']);
            $this->assertEquals('profile',$params['action']);
        }

        /**
         * 测试正则表达式转换，指定默认值。
         */
        public function testRegexRuleParseForDefault() {
            $rule = Jtita::ioc(array('class'    => 'regex_router_rule',
                                     'express'  => '/:controller-:action',
                                     'cat'      => 10,
                                     'controller' => 'index'));

            $params = $rule->parse(array('url' => '/member-profile'));

            $this->assertEquals('member',$params['controller']);
            $this->assertEquals('profile',$params['action']);
            $this->assertEquals(10,$params['cat']);
        }

        /**
         * 测试正则表达式转换，指定验证器。
         */
        public function testRegexRuleParseForValidator() {
            $rule = Jtita::ioc(array('class'    => 'regex_router_rule',
                                     'express'  => '/:controller/:action/:id',
                                     'id' => array(null,'\d+')));

            $this->assertFalse($rule->parse(array('url' => '/goods/show/abcdef')));

            $params = $rule->parse(array('url' => '/goods/show/123'));

            $this->assertEquals(3,count($params));
            $this->assertEquals('goods',$params['controller']);
            $this->assertEquals('show',$params['action']);
            $this->assertEquals('123',$params['id']);
        }

        /**
         * 测试正则表达式转换，指定必须值
         */
        public function testRegexRuleParseForRequired() {
            $rule = Jtita::ioc(array('class'    => 'regex_router_rule',
                                     'express'  => '/:controller/:action',
                                     'id' => array(null,'\d+',true)));

            $this->assertFalse($rule->parse(array('url' => '/goods/show')));

            $params = $rule->parse(array('url' => '/goods/show','id' => 123));

            $this->assertEquals(3,count($params));
            $this->assertEquals('goods',$params['controller']);
            $this->assertEquals('show',$params['action']);
            $this->assertEquals('123',$params['id']);
        }

        /**
         * 测试正则规则转换，参数值。
         */
        public function testRegexRuleParseForParams() {
            $rule = Jtita::ioc(array('class'    => 'regex_router_rule',
                                     'express'  => '/:controller/:action'));

            $params = $rule->parse(array('url' => '/goods/show/id-123/name-中文'));

            $this->assertEquals(4,count($params));
            $this->assertEquals('123',$params['id']);
            $this->assertEquals('goods',$params['controller']);
            $this->assertEquals('show',$params['action']);
            $this->assertEquals('中文',$params['name']);
        }

        /**
         * 测试转换参数，有关键字参数。
         */
        public function testRegexRuleParseForParamsKeyName() {
            $rule = Jtita::ioc(array('class'    => 'regex_router_rule',
                                     'express'  => '/:controller/:action'));

            $params = $rule->parse(array('url' => '/goods/show/id-123/name-%3Caf%2F'));

            $this->assertEquals(4,count($params));
            $this->assertEquals('123',$params['id']);
            $this->assertEquals('goods',$params['controller']);
            $this->assertEquals('show',$params['action']);
            $this->assertEquals('<af/',$params['name']);
        }

        /**
         * 测试正则规则转换，验证参数。
         */
        public function testRegexRuleParseForParamsValidator() {
            $rule = Jtita::ioc(array('class'    => 'regex_router_rule',
                                     'express'  => '/:controller/:action',
                                     'id'       => array(null,'\d+')));

            $this->assertFalse($rule->parse(array('url' => '/goods/show/id-abc')));
        }

        /**
         * 测试正则规则，指定可选参数
         */
        public function testRegexRuleParseForOptional() {
            $rule = Jtita::ioc(array('class'    => 'regex_router_rule',
                                     'express'  => '/:controller[/:action][.:fmt]'));

            $params = $rule->parse(array('url' => '/goods.html'));

            $this->assertEquals('goods',$params['controller']);
            $this->assertEquals('html',$params['fmt']);
        }



        /**
         * 使用正则规则反转，空表示式。
         */
        public function testRegexRuleReverseForEmptyExpress() {
            $rule = Jtita::ioc(array('class' => 'regex_router_rule'));

            $this->assertFalse($rule->reverse(array()));
        }

        /**
         * 测试正则规则反转，重写参数为空。
         */
        public function testRegexRuleReverseForRewrite() {
            $rule = Jtita::ioc(array('class' => 'regex_router_rule',
                                     'express' => '/:controller/:action'));

            $params = $rule->reverse(array('controller' => 'goods','action' => 'show'));

            $this->assertEquals(1,count($params));
            $this->assertEquals('/goods/show',$params['url']);
        }

        /**
         * 测试正则规则反转，有参数的反转
         */
        public function testRegexRuleReverseForParams() {
            $rule = Jtita::ioc(array('class' => 'regex_router_rule',
                              'express' => '/:controller/:action'));

            $params = $rule->reverse(array('controller' => 'goods','action' => 'show','name' => 'zhenggp'));

            $this->assertEquals(1,count($params));
            $this->assertEquals('/goods/show/name-zhenggp',$params['url']);
        }

        /**
         * 测试正则规则反转，有参数并且有非法参数。
         */
        public function testRegexRuleReverseForParamsKeyName() {
            $rule = Jtita::ioc(array('class' => 'regex_router_rule',
                                     'express' => '/:controller/:action'));

            $params = $rule->reverse(array('controller' => 'goods','action' => 'show','name' => '<af/'));

            $this->assertEquals(1,count($params));
            $this->assertEquals('/goods/show/name-%3Caf%2F',$params['url']);
        }

        /**
         * 测试正则反转，反转失败
         */
        public function testRegexRuleReverseForFail() {
            $rule = Jtita::ioc(array('class' => 'regex_router_rule',
                                     'express' => '/:controller/:action'));

            $this->assertFalse($rule->reverse(array('action' => 'show')));
        }

        /**
         * 测试正则反转，包含默认值。
         */
        public function testRegexRuleReverseForDefault() {
            $rule = Jtita::ioc(array('class' => 'regex_router_rule',
                                     'express' => '/:controller/:action',
                                     'cat' => 123));

            $params = $rule->reverse(array('controller' => 'goods','action' => 'show','cat' => 123));
            $this->assertEquals('/goods/show',$params['url']);

            $params1 = $rule->reverse(array('controller' => 'goods','action' => 'show','cat' => 23));
            $this->assertEquals('/goods/show/cat-23',$params1['url']);
        }

        /**
         * 测试正则反转，包含特殊默认值。
         */
        public function testRegexRuleReverseForValidator() {
            $rule = Jtita::ioc(array('class' => 'regex_router_rule',
                                     'express' => '/goods/:action',
                                     'controller' => array('member','member')));

            $params = $rule->reverse(array('controller' => 'member','action' => 'show','id' => 12));
            $this->assertEquals('/goods/show/id-12',$params['url']);

            $this->assertFalse($rule->reverse(array('controller' => 'goods','action' => 'show')));
        }

        /**
         * 测试反转，必须值。
         */
        public function testRegexRuleReverseForRequired() {
            $rule = Jtita::ioc(array('class' => 'regex_router_rule',
                                     'express' => '/goods/:action',
                                     'controller' => array(null,'member',true)));

            $this->assertFalse($rule->reverse(array('action' => 'show')));
        }

        /**
         * 测试正则反转，可选值。
         */
        public function testRegexRuleReverseForOptional() {
            $rule = Jtita::ioc(array('class' => 'regex_router_rule',
                                     'express' => '/:controller/[:action][.:fmt]',
                                     'fmt' => 'html',
                                     'action' => 'index'));

            $params = $rule->reverse(array('controller' => 'index','action' => 'index','fmt' => 'html'));

            $this->assertNotEmpty($params);
            $this->assertEquals('/index/',$params['url']);
        }

        /**
         * 测试正则反转，可选值。
         */
        public function testRegexRuleReverseForOptional1() {
            $rule = Jtita::ioc(array('class' => 'regex_router_rule',
                                     'express' => '/:controller[/:action][.:fmt]',
                                     'fmt' => 'html',
                                     'action' => 'index'));

            $params = $rule->reverse(array('controller' => 'goods','action' => 'show','fmt' => 'ajax'));

            $this->assertNotEmpty($params);
            $this->assertEquals('/goods/show.ajax',$params['url']);
        }

        /**
         * 测试正则规则，包含可先参数的更多值。
         */
        public function testRegexRuleReverseParams() {
            $rule = Jtita::ioc(array('class' => 'regex_router_rule',
                                     'express' => ':controller/:action'));
            $params = $rule->reverse(array('controller' => 'goods','action' => 'show','cat' => 1));

            $this->assertEquals('goods/show/cat-1',$params['url']);
        }

        /**
         * 测试正则规则，包含更多参数和可选参数
         */
        public function testRegexRuleReverseParamsAndOptional() {
            $rule = Jtita::ioc(array('class' => 'regex_router_rule',
                                     'express' => ':controller/[:action]',
                                     'action' => 'index'));
            $params = $rule->reverse(array('controller' => 'goods','action' => 'index','cat' => 1));

            $this->assertEquals('goods/cat-1',$params['url']);
        }

        /**
         * 测试正则规则，包含更多参数和可选参数
         */
        public function testRegexRuleParseParamsAndOptional() {
            $rule = Jtita::ioc(array('class' => 'regex_router_rule',
                                    'express' => ':controller/[:action]',
                                    'action' => array('index')));
            $params = $rule->parse(array('url' => 'goods/cat-1'));

            # TODO:没有完美解决
            #$this->assertEquals(3,count($params));
            # $this->assertEquals('goods',$params['controller']);
            # this->assertEquals('index',$params['action']);
            #$this->assertEquals('1',$params['cat']);

        }

    }
