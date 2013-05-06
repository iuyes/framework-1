<?php
    /**
     *  datasource_test.inc 数据源单元测试类声明文件
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.datasources
     * @version     1.0 beta
     *
     * @create      2012-08-20
     * @modify      2013-04-28
     */

    # 声明测试根目录
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (dirname (__FILE__)))))));

    # 引用测试依赖文件
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');

    # 设置测试引用
    Jtita::paths (JTPATH_ROOT . '/jtita');

    # 引用数据源测试类
    require_once(JTPATH_ROOT . '/jtita/kernels/datasources/datasource.inc');

    /**
     * 数据源单元测试类，提供对数据源的单元测试服务
     *
     * @package jtita.kernels.datasources
     */
    class DatasourceTest extends PHPUnit_Framework_TestCase{
        /**
         * 测试获取默认数据库
         */
        public function testDatabaseForDefault () {
            # 重新设置配置信息
            Jtita::write ('kernels/datasources', array('default' => array(
                'class'  => 'datasource_database_mock',
                'server' => '192.168.0.1',
                'dbname' => 'example',
                'user'   => 'zhenggp',
                'pass'   => '123321'
            )));

            DatasourceMock::reinitialize ();
            $database = DatasourceMock::database ();

            $this->assertTrue ($database instanceof DatasourceDatabaseMock);
            $this->assertEquals('192.168.0.1',$database->server);
            $this->assertEquals('example',$database->dbname);
            $this->assertEquals('zhenggp',$database->user);
            $this->assertEquals('123321',$database->pass);
        }

        /**
         * 测试根据名称获取数据库实例
         */
        public function testDatabaseForName () {
            $cfgs = array('class' => 'datasource_database_mock', 'server' => '127.0.0.1', 'db' => 'test');
            # 重新设置配置信息
            jtita::write ('kernels/datasources', array('test' => $cfgs));

            DatasourceMock::reinitialize ();
            $database = DatasourceMock::database ('test');

            $this->assertTrue ($database instanceof DatasourceDatabaseMock);
        }

        /**
         * 测试获取未声明的数据库对象
         *
         * @expectedException DatasourceException
         */
        public function testDatabaseForNotExists () {
            jtita::write ('kernels/datasources', array());

            DatasourceMock::reinitialize ();
            DatasourceMock::database ('not_exists');
        }
    }


    /**
     * 数据源模拟对象，提供数据源单元测试的模拟服务
     *
     * @package jtita.kernels.datasources
     */
    class DatasourceMock extends Datasource{
        /**
         * 重新初始化
         */
        public static function reinitialize () {
            self::$_initialized = false;
            self::$_databases = array();
            self::initialize ();
        }
    }

    /**
     * 数据源数据库模拟对象
     *
     * @package jtita.kernels.datasources
     */
    class DatasourceDatabaseMock{

        public $initialized = false;

        public $server = '127.0.0.1';
        public $dbname = 'test';
        public $user   = 'root';
        public $pass   = 'pass';

        /**
         * 使用$options初始化对象
         */
        function initialize () {
            $this->initialized = true;
        }
    }
