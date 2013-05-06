<?php
    /**
     * autocache_behavior.inc 自动缓存行为
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.caches.behaviors
     * @version     1.0 beta
     *
     * @create      2012-04-03
     * @modify      2013-04-28
     */

    if (! defined ('JTITA_CACHE')){
        Jtita::import ('kernels.caches.cache');
    }

    /**
     * 自动缓存行为，提供缓存自动支持。
     *
     * @package jtita.kernels.caches.behaviors
     */
    class AutocacheBehavior extends AbstractBehavior{
        /**
         * @var string 缓存引擎。
         */
        public $engine = 'default';

        /**
         * @var int 过期时效。
         */
        public $expire = 3600;

        /**
         * @var bool 缓存关键字，false将不缓存。
         */
        public $key = false;

        /**
         * @override AbstractBehavior
         */
        protected function _invoke ($target, $args) {
            # 如果没有指定关键字表达式
            if (! $this->key){
                return $this->_next->invoke ($target, $args);
            }

            $key = array_express ($args, $this->key);
            $value = Cache::read ($key, $this->engine);

            if ($value == false){
                $value = $this->_next->invoke ($target, $args);
                Cache::write ($key, $value, $this->expire, $this->engine);
            }

            return $value;
        }
    }