<?php
    /**
     * cache.inc 缓存组件服务接口Cache类声明文件。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.caches
     * @version     1.0 beta
     *
     * @create      2012-07-16
     * @modify      2013-04-26
     */

    /**
     * @const bool 缓存服务开启标志。
     */
    define('JTITA_CACHE', true);

    /**
     * 缓存异常类，当缓存发生异常情况时将引发本类。
     *
     * @package jtita.kernels.caches
     */
    class CacheException extends JtitaException{
    }

    /**
     * 缓存引擎接口，提供将缓存持久化保存的引擎方案。
     *
     * @package jtita.kernels.caches
     */
    interface CacheEngine{
        /**
         * 根据$key和$value写入缓存信息。
         *
         * @param string     $key       缓存标识
         * @param mixed      $value     缓存内容
         * @param integer    $expire    过期时间
         *
         * @return void
         */
        function write ($key, $value, $expire);

        /**
         * 根据$key读取缓存信息，如果缓存不存在则返回false。
         *
         * @param string $key 缓存标识
         *
         * @return mixed|boolean
         */
        function read ($key);

        /**
         * 根据$key清除尘缓存内容，如果$key为空值则清除全部。
         *
         * @param string $key 缓存关键字
         *
         * @return void
         */
        function clear ($key);

        /**
         * 清除引荐的全部内容。
         *
         * @return mixed
         */
        function flush ();
    }

    # 如果缓存组件当前还未初始化，则执行初始化
    if (! Cache::initialized ()){
        Cache::initialize ();
    }

    /**
     * 缓存组件服务接口对象，提供缓存组件的外部访问接口。
     *
     * @example
     *   <pre>
     *          Jtita::write('kernels/caches',array(
     *              'default' => array (
     *                  'class' => 'kernels.caches.engines.file_cache_engine',
     *                  'path' => RTIME_PATH . 'caches/default'
     *               ),
     *               'pages' => array (
     *                   'class' => 'kernels.caches.engines.memcached_cache_engine',
     *                   'host'  => '127.0.0.1',
     *                   'port' => 9000
     *               )
     *          ));
     *  </pre>
     *
     * @package jtita.kernels.caches
     * @abstract
     */
    abstract class Cache{
        /**
         * @const 缓存配置节点。
         */
        const CFG_CACHE = 'kernels/caches';

        /**
         * 执行缓存的初始化。
         */
        public static function initialize () {
            # 读取配置信息
            $cfgs = Jtita::read (self::CFG_CACHE, false);
            # 如果没有配置缓存引荐
            if ($cfgs == false){
                $cfgs = array('default' => array('class' => 'kernels.caches.engines.file_cache_engine',
                                                 'dir' => JTITA_CACHE_DIR));
            }

            self::$_config = $cfgs;
            self::$_initialized = true;
        }

        /**
         * 返回一个布求值，判断是否已经完成初始化
         * @return boolean
         */
        public static function initialized () {
            return self::$_initialized;
        }

        /**
         * 将$key和$value的缓存信息写入到引擎$engine中。
         *
         * @param string $key    缓存标识
         * @param mixed  $value  缓存值
         * @param int    $expire 过期时间
         * @param string $engine 缓存引擎
         *
         * @return void
         */
        public static function write ($key, $value, $expire = 30, $engine = 'default') {
            # 如果当前指定有调试组件。
            if (defined ('JTITA_DEBUGGER')){
                Debugger::write ('cache', array('method' => 'write', 'key' => $key, 'value' => $value,
                                                'expire' => $expire, 'engine' => $engine));
            }

            $engine = self::_cacheEngine ($engine);
            $engine->write ($key, $value, $expire);
        }

        /**
         * 从缓存引荐$engine中读取缓存$key的值，读取失败返回false。
         *
         * @param string $key    缓存标识
         * @param string $engine 缓存引荐
         *
         * @return mixed|boolean
         */
        public static function read ($key, $engine = 'default') {
            # 如果当前指定有调试组件。
            if (defined ('JTITA_DEBUGGER')){
                Debugger::write ('cache', array('method' => 'read', 'key' => $key, 'engine' => $engine));
            }

            $engine = self::_cacheEngine ($engine);
            return $engine->read ($key);
        }

        /**
         * 请除缓存引荐$engine中的缓存$key的值。
         *
         * @param string $key    缓存标识
         * @param string $engine 缓存引荐
         *
         * @return void
         */
        public static function clear ($key, $engine = 'default') {
            # 如果当前指定有调试组件。
            if (defined ('JTITA_DEBUGGER')){
                Debugger::write ('cache', array('method' => 'clear', 'engine' => $engine));
            }

            $engine = self::_cacheEngine ($engine);
            $engine->clear ($key);
        }

        /**
         * 清除尘$engine指定的全部缓存信息。
         *
         * @param string $engine
         */
        public static function flush ($engine = 'default') {
            # 如果当前指定有调试组件。
            if (defined ('JTITA_DEBUGGER')){
                Debugger::write ('cache', array('method' => 'flush', 'engine' => $engine));
            }

            $engine = self::_cacheEngine ($engine);
            $engine->flush ();
        }

        /**
         * 根据缓存引擎名称$engine获取引擎。
         *
         * @param $engine 引擎名称
         *
         * @return CacheEngine
         *
         * @throws CacheException 如果指定引荐未提供，将引发本异常。
         */
        private static function _cacheEngine ($engine) {
            # 判断引擎是否已创建。
            if (! isset(self::$_engines[$engine])){
                # 检查是否配置指定引荐。
                if (! isset(self::$_config[$engine])){
                    throw new CacheException('JTK0104:Cache not support engine - ' . $engine);
                }

                self::$_engines[$engine] = Jtita::ioc (self::$_config[$engine]);
            }

            return self::$_engines[$engine];
        }

        /**
         * @var bool 是否初始化。
         */
        protected static $_initialized = false;

        /**
         * @var array 引擎实例。
         */
        protected static $_engines = array();

        /**
         * @var array 配置信息
         */
        protected static $_config = array();
    }