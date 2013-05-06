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

    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (dirname (dirname (__FILE__))))))));
    defined ('JTITA_CACHE_DIR') or define('JTITA_CACHE_DIR','/root');

    /**
     * 导入基础服务类；
     */
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/behaviors.inc');

    # 导入被测试类
    require_once(JTPATH_ROOT . '/jtita/kernels/caches/cache.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/caches/behaviors/autocache_behavior.inc');

    /**
     * 自动缓存测试服务。
     *
     * @package jtita.kernals.caches.behaviors
     */
    class AutocacheBehaviorTest extends PHPUnit_Framework_TestCase{
        public function setUp () {
            Jtita::clear ();
            Jtita::clsBehaviors ();

            Jtita::write ('kernels/caches', array(
                'default' => array('class' => 'autocache_engine_mock')
            ));

            AutocacheDo::$callQuery = false;
            AutocacheEngineMock::$caches = array();
            AutocacheMock::reinitialize();
        }

        /**
         * 测试行为表达式。
         */
        public function testBehaviorForExpress() {
            Jtita::behaviors('autocache_do','query',array('class' => 'autocache_behavior','key' => 'do-{0}'));
            $cacheDo = Jtita::create('autocache_do');

            $cacheDo->query('good');
            $cacheDo->query('book');
            $this->assertTrue(AutocacheDo::$callQuery);

            AutocacheDo::$callQuery = false;
            # 调用缓存
            $good = $cacheDo->query('good');
            $book = $cacheDo->query('book');

            $this->assertTrue(is_array($good));
            $this->assertEquals('good',$good[0]);
            $this->assertEquals('flush',$good[1]);

            $this->assertTrue(is_array($book));
            $this->assertEquals('book',$book[0]);
            $this->assertEquals('jtita developer',$book[1]);
        }

        /**
         * 测试行为，未指定KEY表达式。
         */
        public function testBehaviorForNotKeyExpress () {
            Jtita::behaviors ('autocache_do', 'query', 'autocache_behavior');
            $cacheDo = Jtita::create ('autocache_do');
            $rows = $cacheDo->query ('good');

            $this->assertTrue(is_array($rows));
            $this->assertEquals('good',$rows[0]);
            $this->assertEquals('flush',$rows[1]);
        }
    }

    /**
     * 自动缓存模拟对象。
     */
    class AutocacheDo{
        public static $callQuery = false;

        public function query ($cmd) {
            self::$callQuery = true;

            switch ($cmd){
                case 'good':
                    return array('good', 'flush');
                case 'book':
                    return array('book', 'jtita developer');
            }

            return array('ebook', 'jtita man');
        }
    }

    class AutocacheMock extends Cache{
        public static function reinitialize () {
            self::$_config = array();
            self::$_engines = array();
            self::$_initialized = false;

            self::initialize ();
        }
    }


    /**
     * 自动缓存模拟引擎。
     */
    class AutocacheEngineMock implements CacheEngine{
        public static $caches = array();

        public function write ($key, $value, $expire) {
            self::$caches[$key] = $value;
        }

        public function read ($key) {
            if (isset(self::$caches[$key])) {
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