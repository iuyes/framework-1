<?php
    /**
     * file_cache_engine.inc 缓存组件服务接口Cache类声明文件。
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
     * @modify      2013-04-28
     */

    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (dirname (dirname (__FILE__))))))));

    # 声明缓存目录
    define('JTITA_CACHE_DIR', dirname (__FILE__));

    /**
     * 导入基础服务类；
     */
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');

    # 导入被测试类
    require_once(JTPATH_ROOT . '/jtita/kernels/caches/cache.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/caches/engines/file_cache_engine.inc');

    /**
     * 文件缓存测试。
     *
     * @package jtita.kernels.caches.engines
     */
    class FileCacheEngineTest extends PHPUnit_Framework_TestCase {
        /**
         * 测试内容写入缓存
         */
        public function testWrite () {
            $engine = Jtita::ioc (array('class' => 'file_cache_engine'));
            $engine->write ('key', 'Hello world', 30);

            $this->assertTrue (file_exists (JTITA_CACHE_DIR . '/cache-file-key.jmp'));
            $this->assertEquals ('Hello world', $engine->read ('key'));
        }

        /**
         * 写入缓存数据；
         */
        public function testWriteForArray () {
            $engine = Jtita::ioc (array('class' => 'file_cache_engine'));
            $engine->write ('number', array('Key' => 'Hello world', 'look'), 30);

            $this->assertTrue (file_exists (JTITA_CACHE_DIR . '/cache-file-number.jmp'));

            $value = $engine->read ('number');

            $this->assertTrue (is_array ($value));
            $this->assertEquals ('Hello world', $value['Key']);
            $this->assertEquals ('look', $value[0]);
        }

        /**
         * 写入缓存对象。
         */
        public function testWriteForObject () {
            $engine = Jtita::ioc (array('class' => 'file_cache_engine'));
            $engine->write ('object', new CacheExample(), 30);

            $this->assertTrue (file_exists (JTITA_CACHE_DIR . '/cache-file-object.jmp'));

            $value = $engine->read ('object');

            $this->assertTrue ($value instanceof CacheExample);
            $this->assertEquals ('emp12', $value->empId);
            $this->assertEquals ('zgo', $value->empName);
        }

        /**
         * 测试写入到指定的prefix。
         */
        public function testWriteForPrefix () {
            $engine = Jtita::ioc(array('class' => 'file_cache_engine',
                                       'prefix' => 'goods-cache-file-'));
            $engine->write ('object', new CacheExample(), 30);
            $this->assertTrue (file_exists (JTITA_CACHE_DIR . '/goods-cache-file-object.jmp'));

            $value = $engine->read ('object');

            $this->assertEquals ('emp12', $value->empId);
            $this->assertEquals ('zgo', $value->empName);
        }

        /**
         * 测试读取过期内容
         */
        public function testReadForExpire () {
            $engine = Jtita::ioc (array('class' => 'file_cache_engine'));
            $engine->write ('expire', new CacheExample(), - 30);

            $this->assertTrue (file_exists (JTITA_CACHE_DIR . '/cache-file-expire.jmp'));
            $this->assertFalse ($engine->read ('expire'));
            $this->assertFalse (file_exists (JTITA_CACHE_DIR . '/cache-file-expire.jmp'));
        }

        /**
         * 测试读取缓存，缓存KEY未提供
         */
        public function testReadForNotCache () {
            $engine = Jtita::ioc (array('class' => 'file_cache_engine'));
            $this->assertFalse ($engine->read ('not-exists'));
        }

        /**
         * 测试删除
         */
        public function testClear () {
            $engine = Jtita::ioc (array('class' => 'file_cache_engine'));
            $engine->write ('delete', 'hello', 30);
            $engine->clear ('delete');

            $this->assertFalse ($engine->read ('delete'));
            $this->assertFalse (file_exists (JTITA_CACHE_DIR . '/cache-file-delete.jmp'));
        }

        /**
         * 测试删除未缓存的文件
         */
        public function testClearForNotCache () {
            $engine = Jtita::ioc (array('class' => 'file_cache_engine'));
            $engine->clear ('delete-nocache');
        }

        /**
         * 测试清除。
         */
        public function testFlush () {
            $engine = Jtita::ioc (array('class' => 'file_cache_engine'));
            $engine->write ('clear', 'Hello world', 10);

            $this->assertTrue (file_exists (JTITA_CACHE_DIR . '/cache-file-clear.jmp'));
            echo 'DIR:', JTITA_CACHE_DIR;
            $engine->flush ();

            $files = file_names (JTITA_CACHE_DIR, 'jmp');

            $this->assertEquals(1,count($files));
            $this->assertNotEmpty (file_names (JTITA_CACHE_DIR));
        }
    }

    class CacheExample {
        public $empId = 'emp12';
        public $empName = 'zgo';
    }