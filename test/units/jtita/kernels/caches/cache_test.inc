<?php
    /**
     * cache_test.inc 提供缓存单元测试服务。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.caches
     * @version     1.0 beta
     *
     * @create      2012-08-20
     * @modify      2013-04-28
     */

    # 声明测试根目录
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (dirname (__FILE__)))))));
    defined ('JTITA_CACHE_DIR') or define('JTITA_CACHE_DIR', JTPATH_ROOT . '/rtime/caches');

    # 引用测试依赖文件
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');

    # 设置测试引用
    Jtita::paths (JTPATH_ROOT . '/jtita');

    # 引用数据源测试类
    require_once(JTPATH_ROOT . '/jtita/kernels/caches/cache.inc');

    /**
     * 测试服务类
     */
    class CacheTest extends PHPUnit_Framework_TestCase{
        public function setUp () {
            Jtita::clear ();
        }

        /**
         * 测试初始化
         */
        public function testInitialize () {
            CacheMock::reinitialize ();
            $cfgs = CacheMock::cfgs ();

            $this->assertTrue (is_array ($cfgs));
            $this->assertEquals ('kernels.caches.engines.file_cache_engine', $cfgs['default']['class']);
            $this->assertEquals (JTPATH_ROOT . '/rtime/caches', $cfgs['default']['dir']);
        }

        /**
         * 测试初始化，指定缓存引荐
         */
        public function testInitializeForEngines () {
            Jtita::write ('kernels/caches', array('default' => array('class' => 'cache_engine_mock'),
                                                  'example' => array('class' => 'example_engine_mock')));

            CacheMock::reinitialize ();
            $cfgs = CacheMock::cfgs ();

            $this->assertEquals ('cache_engine_mock', $cfgs['default']['class']);
            $this->assertEquals ('example_engine_mock', $cfgs['example']['class']);
        }

        /**
         * 测试写入缓存。
         */
        public function testWrite () {
            Jtita::write ('kernels/caches', array('default' => array('class' => 'cache_engine_mock')));

            CacheMock::reinitialize ();
            CacheMock::write ('key', array('hello world', 'hello php', 'hello jtita'));
            CacheMock::write ('key1', array('goods'));

            $this->assertEquals ('hello world', CacheEngineMock::$caches['key'][0]);
            $this->assertEquals ('hello php', CacheEngineMock::$caches['key'][1]);
            $this->assertEquals ('goods', CacheEngineMock::$caches['key1'][0]);
        }

        /**
         * 测试写入缓存，缓存引擎未提供
         *
         * @expectedException CacheException
         */
        public function testWriteForEngineNotSupport () {
            Jtita::write ('kernels/caches', array('default' => array('class' => 'cache_engine_mock')));

            CacheMock::reinitialize ();
            CacheMock::write ('key', 'key1', 30, 'not_exists');
        }

        /**
         * 测试写入缓存，指定缓存名称
         */
        public function testWriteForSupportEngine () {
            Jtita::write ('kernels/caches', array('default' => array('class' => 'example'),
                                                  'example' => array('class' => 'cache_engine_mock')));

            CacheMock::reinitialize ();
            CacheEngineMock::$caches = array();

            CacheMock::write ('key', array('hello world', 'hello php', 'hello jtita'), 30, 'example');
            CacheMock::write ('key1', array('goods'), 30, 'example');

            $this->assertEquals ('hello world', CacheEngineMock::$caches['key'][0]);
            $this->assertEquals ('hello php', CacheEngineMock::$caches['key'][1]);
            $this->assertEquals ('goods', CacheEngineMock::$caches['key1'][0]);
        }

        /**
         * 测试读取
         */
        public function testRead () {
            Jtita::write ('kernels/caches', array('default' => array('class' => 'cache_engine_mock')));

            CacheMock::reinitialize ();
            CacheEngineMock::$caches = array();

            CacheMock::write ('key', array('hello world', 'hello php', 'hello jtita'));
            $caches = CacheMock::read ('key');

            $this->assertEquals ('hello world', $caches[0]);
            $this->assertEquals ('hello php', $caches[1]);
        }

        /**
         * 测试读取，key值不存在。
         */
        public function testReadForKeyNotExists () {
            Jtita::write ('kernels/caches', array('default' => array('class' => 'cache_engine_mock')));

            CacheMock::reinitialize ();
            CacheEngineMock::$caches = array();

            $this->assertFalse (CacheMock::read ('not-exists'));
        }

        /**
         * 测试写入缓存，缓存引擎未提供
         *
         * @expectedException CacheException
         */
        public function testReadForEngineNotSupport () {
            Jtita::write ('kernels/caches', array('default' => array('class' => 'cache_engine_mock')));

            CacheMock::reinitialize ();
            CacheMock::read ('key', 'example');
        }

        /**
         * 测试读取，指定引擎
         */
        public function testReadForEngine () {
            Jtita::write ('kernels/caches', array('example' => array('class' => 'cache_engine_mock')));

            CacheMock::reinitialize ();
            CacheEngineMock::$caches = array();

            CacheMock::write ('key', array('hello world', 'hello php', 'hello jtita'), 30, 'example');
            $caches = CacheMock::read ('key', 'example');

            $this->assertEquals ('hello world', $caches[0]);
            $this->assertEquals ('hello php', $caches[1]);
        }

        /**
         * 测试删除。
         */
        public function testClear () {
            Jtita::write ('kernels/caches', array('default' => array('class' => 'cache_engine_mock')));

            CacheMock::reinitialize ();
            CacheEngineMock::$caches = array();

            CacheMock::write ('key', array('hello world', 'hello php', 'hello jtita'));
            CacheMock::clear ('key');

            $this->assertFalse (CacheMock::read ('key'));
        }

        /**
         * 测试删除指定引荐的key值。
         */
        public function testDeleteForEngine () {
            Jtita::write ('kernels/caches', array('example' => array('class' => 'cache_engine_mock')));

            CacheMock::reinitialize ();
            CacheEngineMock::$caches = array();

            CacheMock::write ('key', array('hello world', 'hello php', 'hello jtita'), 30, 'example');
            CacheMock::clear ('key', 'example');

            $this->assertFalse (CacheMock::read ('key', 'example'));
        }

        /**
         * 测试删除，指定引擎未提供
         *
         * @expectedException CacheException
         */
        public function testClearForEngineNotSupport () {
            Jtita::write ('kernels/caches', array('default' => array('class' => 'cache_engine_mock')));

            CacheMock::reinitialize ();
            CacheMock::clear ('key', 'example');
        }

        /**
         * 测试清除引荐内容
         */
        public function testFlush () {
            Jtita::write ('kernels/caches', array('default' => array('class' => 'cache_engine_mock')));

            CacheMock::reinitialize ();
            CacheEngineMock::$caches = array();

            CacheMock::write ('key', array('hello world', 'hello php', 'hello jtita'));
            CacheMock::flush ();

            $this->assertEmpty (CacheEngineMock::$caches);
        }

        /**
         * 测试清除指定引擎内容
         */
        public function testFlushForEngine () {
            Jtita::write ('kernels/caches', array('example' => array('class' => 'cache_engine_mock')));

            CacheMock::reinitialize ();
            CacheEngineMock::$caches = array();

            CacheMock::write ('key', array('hello world', 'hello php', 'hello jtita'), 30, 'example');
            CacheMock::flush ('example');

            $this->assertEmpty (CacheEngineMock::$caches);
        }

        /**
         * 测试清除，指定引荐不存在。
         *
         * @expectedException CacheException
         */
        public function testFlushForEngineNotSupport () {
            Jtita::write ('kernels/caches', array('default' => array('class' => 'cache_engine_mock')));

            CacheMock::reinitialize ();
            CacheMock::flush ('example');
        }
    }

    /**
     * 缓存模拟类
     */
    class CacheMock extends Cache{
        public static function  reinitialize () {
            self::$_initialized = false;
            self::$_engines = array();

            self::initialize ();
        }

        public static function cfgs () {
            return self::$_config;
        }
    }

    /**
     * 缓存引荐模拟对象
     */
    class CacheEngineMock implements CacheEngine{
        public static $caches = array();

        public function write ($key, $value, $expire) {
            self::$caches[$key] = $value;
        }

        public function read ($key) {
            if (isset(self::$caches[$key])){
                return self::$caches[$key];
            }

            return false;
        }

        public function clear ($key) {
            unset(self::$caches[$key]);
        }

        public function flush () {
            self::$caches = array();
        }
    }