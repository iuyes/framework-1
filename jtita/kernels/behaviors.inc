<?php
    /**
     * behaviors.inc 声明架构中对于对象附加行为支持。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels
     * @version     1.0 beta
     *
     * @create      2012-06-14
     * @modify      2013-04-26
     */

    /**
     * 对象行为异常，记录在行为执行过程中的异常情况。
     *
     * @package jtita.kernels
     */
    class ObjectBehaviorException extends JtitaException{
    }

    /**
     * 对象行为接口，提供对象执行附加行为的能力。
     *
     * @package jtita.kernels
     */
    interface ObjectBehavior{
        /**
         * 设置下一个节点为$next对象。
         *
         * @param ObjectBehavior $next 下一个对象行为
         *
         * @return mixed
         */
        function next ($next);

        /**
         * 使用$args参数执行$target目标方法。
         *
         * <b>注意：</b> $target = array($sender,$method);
         *
         * @param array $target 行为目标对象
         * @param array $args   方法调用参数；
         *
         * @return mixed
         */
        function invoke ($target, $args);
    }

    /**
     * 对象包装器，包装目标对象的附加行为，转发对象调用请求。
     *
     * @package jtita.kernels
     */
    class ObjectWrapper {
        /**
         * @var mixed 请求执行的目标对象
         */
        public $target = null;

        /**
         * 使用$target初始化实例；
         *
         * @param mixed $target 目标
         */
        public function __construct ($target = null) {
            $this->target = $target;
        }

        /**
         * 设置针对当前目标对象的行为集合。
         *
         * @param array $behaviors 行为集；
         */
        public function behaviors ($behaviors) {
            $this->_behaviors = $behaviors;
        }

        /**
         * PHP魔法方法，设置目标字段属性值。
         *
         * @param string $prop  属性名
         * @param mixed  $value 属性值
         */
        public function __set ($prop, $value) {
            $this->target->{$prop} = $value;
        }

        /**
         * PHP魔法方法，获取目标字段属性值。
         *
         * <b>注意：</b>直接读取未声明的属性将引发运行错误。
         *
         * @param string $prop 属性名
         *
         * @return mixed 属性值
         */
        public function __get ($prop) {
            return $this->target->{$prop};
        }

        /**
         * PHP魔法方法，转发对目标的方法调用请求，ObjectWrapper中将附加行为对象。
         *
         * @param string $method 目标方法名称
         * @param array  $args   目标方法参数
         *
         * @return mixed
         *
         * @throws MethodNotFoundException 目标方法$method在目标对象中不存在将引发本异常。
         */
        public function __call ($method, $args) {
            # 如果方法存在，调用并执行方法；
            if (method_exists ($this->target, $method)){
                $behavior = $this->_methodBehavior ($method);
                $value = $behavior->invoke (array($this->target, $method), $args);

                # 判断返回值是否为引用目标；
                return ($value === $this->target) ? $this : $value;
            }

            throw new MethodNotFoundException('JT0201: method not found - ' . $method . ' at ' . get_class ($this->target));
        }

        /**
         * 根据$method获取方法行为实例的首节点。
         *
         * @param string $method 方法名称
         *
         * @return AbstractBehavior|ObjectBehavior
         */
        protected function _methodBehavior ($method) {
            if (! isset($this->_objBehaviors[$method])){
                $this->_objBehaviors[$method] = $this->_createMethodBehavior ($method);
            }

            return $this->_objBehaviors[$method];
        }

        /**
         * 根据$method创建对象的行为实例。
         *
         * @param string $method 方法名称
         *
         * @return ObjectBehavior
         */
        protected function _createMethodBehavior ($method) {
            # 获取行为注册行为
            $behaviors = isset($this->_behaviors[$method]) ? $this->_behaviors[$method] : array();
            $next = new CallMethodBehavior();

            # 重新反向行为设置，设置正确的优先级；
            $behaviors = array_reverse ($behaviors, true);
            foreach ($behaviors as $behavior){
                # 调用行为初始化方法，初始化行为实例
                $objBehavior = Jtita::ioc ($behavior);
                $objBehavior->next ($next);

                $next = $objBehavior;
            }

            return $next;
        }

        /**
         * @var array 请求执行目标对象的注册行为集
         */
        protected $_behaviors = array();

        /**
         * @var array 方法行为实例；
         */
        protected $_objBehaviors = array();
    }

    /**
     * 抽象行为对象，提供行为所需的基本职责。
     *
     * @package jtita.kernels
     * @abstract
     */
    abstract class AbstractBehavior implements ObjectBehavior{
        /**
         * 初始化AbstractBehavior。
         */
        public function __construct () {
            $this->_next = null;
        }

        /**
         * 获取或设置下一个行为，如果$next非null设置值。
         *
         * @see Behavior::next
         *
         * @param ObjectBehavior $next 下一个行为对象。
         *
         * @return ObjectBehavior
         */
        public function next ($next) {
            $this->_next = $next;
        }

        /**
         * 使用$target和$args执行具体行为。
         *
         * @param array $target 执行目标
         * @param array $args   方法参数
         *
         * @return mixed
         */
        public function invoke ($target, $args) {
            return $this->_invoke ($target, $args);
        }

        /**
         * 执行行为，默认什么也不执行。
         *
         * @param array $target 目标
         * @param array $args   参数
         *
         * @return mixed
         */
        protected abstract function _invoke ($target, $args);

        /**
         * @var ObjectBehavior 下一个行为对象。
         */
        protected $_next;
    }

    /**
     * 调用方法行为，提供类方法调用服务。
     *
     * @package jtita.kernels
     */
    class CallMethodBehavior extends AbstractBehavior{
        /**
         * 使用$target和$args调用实例方法。
         *
         * @param mixed $target 目标方法
         * @param array $args   方法参数
         *
         * @return mixed
         */
        protected function _invoke ($target, $args) {
            return call_user_func_array ($target, $args);
        }
    }

    /**
     * 方法回调行为，提供通过回调用方法完成服务的行为能力。
     *
     *  <b>说明：回调方法函数签名</b>
     *  <pre>
     *        function ($target,$args,$next) { }
     *  </pre>
     *
     * @example
     *  <pre>
     *      Jtita::behaviors('kernels.caches.memcached_engine','read',function($target,$args,$next) {
     *          // do something
     *          return $next->invoke($target,$target);
     *      });
     *  </pre>
     *
     * @package   jtita.kernels
     */
    class CallbackBehavior extends AbstractBehavior{
        /**
         * @var callback 回调方法。
         */
        public $callback = false;

        /**
         * 使用$target和$args执行回调用方法。
         *
         * @param mixed $target 目标方法
         * @param array $args   方法参数
         *
         * @return mixed
         *
         * @throws ObjectBehaviorException
         */
        protected function _invoke ($target, $args) {
            if ($this->callback){
                return call_user_func_array ($this->callback, array($target, $args, $this->_next));
            }

            throw new ObjectBehaviorException('JTK0202: Callback behavior not support callback.');
        }
    }

    /**
     * 懒加载行为对象，用于处理加载需要大资源的行为对象。
     *
     * @package jtita.kernels
     */
    class LazyBehavior extends AbstractBehavior{
        /**
         * @var ObjectBehavior 懒加载行为对象。
         */
        public $behavior = false;

        /**
         * 使用$target和$args执行具体行为。
         *
         * @see AbstractBehavior::_invoke
         */
        protected function _invoke ($target, $args) {
            # 如果真实行为对象还没有创建；
            if (! $this->_objBehavior){
                # 判断是否设置行为属性；
                if (! $this->behavior){
                    throw new ObjectBehaviorException('JTK0203: lazy behavior not support behavior.');
                }

                $this->_objBehavior = Jtita::ioc ($this->behavior);
                $this->_objBehavior->next ($this->_next);
            }

            # 执行具体服务；
            return $this->_objBehavior->invoke ($target, $args);
        }

        /**
         * @var bool|ObjectBehavior
         */
        protected $_objBehavior = false;
    }