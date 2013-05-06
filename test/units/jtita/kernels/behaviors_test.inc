<?php
    /**
     * objs_test.inc 实现对objs.inc 中类和方法的单元测试。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels
     * @version     1.0 beta
     *
     * @create      2012-07-04
     * @modify      2013-04-26
     */

    # 声明测试根路径;
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (__FILE__))))));

    # 导入被测试文件。
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/behaviors.inc');

    /**
     * 行为测试，提供对于behaviors.inc
     */
    class BehaviorTest extends PHPUnit_Framework_TestCase{
        /**
         * 测试包装器实例化；
         */
        public function testWrapperForConstructor () {
            $target = new BehaviorTargetMock();
            $wrapper = new ObjectWrapper($target);

            $this->assertNotNull ($wrapper->target);
            $this->assertTrue ($target == $wrapper->target);
        }

        /**
         * 测试包装器目标方法调用；
         */
        public function testWrapperForTargetMethodCall () {
            $target = new BehaviorTargetMock();
            $wrapper = new ObjectWrapper($target);

            $wrapper->setField ('new value');

            $this->assertEquals ('new value', $wrapper->getField ());
            $this->assertEquals ('new value', $target->getField ());
        }

        /**
         * 测试目标方法调用，返回参数为目标对象；
         */
        public function testWrapperForTargetMethodReturnTarget () {
            $target = new BehaviorTargetMock();
            $wrapper = new ObjectWrapper($target);

            $result = $wrapper->returnThis ();

            $this->assertEquals ($wrapper, $result);
        }

        /**
         * 测试包装器，不存在的方法调用；
         *
         * @expectedException MethodNotFoundException
         */
        public function testWrapperForNotFoundMethodCall () {
            $target = new BehaviorTargetMock();
            $wrapper = new ObjectWrapper($target);

            $wrapper->notexists ('not method args');
        }

        /**
         * 测试包装器，属性设置；
         */
        public function testWrapperForProp () {
            $target = new BehaviorTargetMock();
            $wrapper = new ObjectWrapper($target);

            $this->assertFalse (isset($wrapper->field));
            $wrapper->field = 'new prop';
            $this->assertEquals ('new prop', $wrapper->field);
        }

        /**
         * 测试包装器，不存在的属性；
         */
        public function testWrapperForNotFoundProp () {
            $target = new BehaviorTargetMock();
            $wrapper = new ObjectWrapper($target);

            $wrapper->notExistsProp = 'new value';

            $this->assertEquals ('new value', $wrapper->notExistsProp);
        }

        /**
         * 测试包装器，读取未设置的属性值；
         *
         * @depends testWrapperForConstructor
         */
        public function testWrapperForReadNotFoundProp () {
            $target = new BehaviorTargetMock();
            $wrapper = new ObjectWrapper($target);
            # 中断程序运行；
            # $this->assertEquals('new value',$wrapper->notExistsProp);
        }

        /**
         * 测试方法调用行为；
         */
        public function testCallMethodBehavior () {
            $behavior = new CallMethodBehavior();
            $target = new BehaviorTargetMock();

            $behavior->invoke (array($target, 'setField'), array('fieldValue' => 'new value'));

            $this->assertEquals ('new value', $target->field);
            $this->assertEquals ('new value', $behavior->invoke (array($target, 'getField'), array()));
        }

        /**
         * 测试包装器，指定行为；
         */
        public function testWrapperForBehaviors () {
            $target = new BehaviorTargetMock();
            $wrapper = new ObjectWrapper($target);

            $wrapper->behaviors (array('setField' => array('object_behavior_mock',
                                                     array('class' => 'object_behavior_mock1'))));
            $wrapper->setField ('new value');

            $this->assertTrue (ObjectBehaviorMock::$callback);
            $this->assertTrue (ObjectBehaviorMock1::$callback);
            $this->assertEquals ('new value', $wrapper->getField ());
        }

        /**
         * 测试懒加载行为；
         *
         * @expectedException ObjectBehaviorException
         */
        public function testLazyBehaviorForNotSupportBehavior() {
            $target = new BehaviorTargetMock();
            $behavior = new LazyBehavior();
            $behavior->invoke (array($target, 'getField'), array());
        }

        /**
         * 测试懒加载功能；
         */
        public function testLazyBehavior () {
            $lazyBehavior = new LazyBehavior();
            $nextBehavior = new CallMethodBehavior();

            $lazyBehavior->behavior = 'object_behavior_mock3';
            $lazyBehavior->next ($nextBehavior);

            $target = new BehaviorTargetMock();

            $lazyBehavior->invoke (array($target, 'setField'), array('Hello World'));

            $this->assertEquals ('Hello World', $target->field);
            $this->assertTrue (ObjectBehaviorMock3::$callback);
        }

        /**
         * 测试回调行为函数；
         */
        public function testCallbackBehavior () {
            $callbackBehavior = new CallbackBehavior();
            $callbackBehavior->next (new CallMethodBehavior());
            $callbackBehavior->callback = function ($target, $args, $next) {
                $args[0] = 'Hello PHP';

                return $next->invoke ($target, $args);
            };

            $target = new BehaviorTargetMock();
            $callbackBehavior->invoke (array($target, 'setField'), array('Hello World'));

            $this->assertEquals ('Hello PHP', $target->field);
        }

        /**
         * 测试回调方法，没有提供回调。
         *
         * @expectedException ObjectBehaviorException
         */
        public function testCallbackBehaviorForNotSupportCallback() {
            $callbackBehavior = new CallbackBehavior();
            $callbackBehavior->next(new CallbackBehavior());

            $callbackBehavior->invoke(array(new BehaviorTargetMock(),'setField'),array('goods'));
        }

        /**
         * 测试行为顺序
         */
        public function testBehaviorsPriority () {
            $target = new BehaviorTargetMock();
            $wrapper = new ObjectWrapper($target);
            $called = false;
            $assert = $this;

            $callback = function ($target, $args, $next) use ($assert, &$called) {
                $called = true;
                $assert->assertEquals ('ObjectBehaviorMock1', get_class ($next));
            };

            $wrapper->behaviors (array('setField' => array(array('class' => 'callback_behavior',
                                                                 'callback' => $callback),
                                                           'object_behavior_mock1')));
            $wrapper->setField ('test');

            $this->assertTrue ($called);
        }
    }


    /**
     * 目标模拟对象；
     */
    class BehaviorTargetMock{
        public $field;

        function setField ($fieldValue) {
            $this->field = $fieldValue;
        }

        function getField () {
            return $this->field;
        }

        function returnThis () {
            return $this;
        }
    }


    /**
     * 行为模拟对象，提供Behavior所需的模拟功能；
     */
    class ObjectBehaviorMock implements ObjectBehavior{
        static $callback = false;

        function next ($next) {
            $this->_next = $next;
        }

        function invoke ($target, $args) {
            self::$callback = true;

            $this->_next->invoke ($target, $args);
        }

        /**
         * @var ObjectBehavior
         */
        private $_next;
    }

    /**
     * 行为模板对象1，提供Behavior所需的模拟功能；
     */
    class ObjectBehaviorMock1 implements ObjectBehavior{
        static $callback;

        function next ($next) {
            $this->_next = $next;
        }

        function invoke ($target, $args) {
            self::$callback = true;

            $this->_next->invoke ($target, $args);
        }

        /**
         * @var ObjectBehavior
         */
        private $_next;
    }

    class ObjectBehaviorMock3 implements ObjectBehavior{
        static $callback;

        function __construct () {
            self::$callback = false;
        }

        function next ($next) {
            $this->_next = $next;
        }

        function invoke ($target, $args) {
            self::$callback = true;
            $this->_next->invoke ($target, $args);
        }

        /**
         * @var ObjectBehavior
         */
        private $_next;
    }

