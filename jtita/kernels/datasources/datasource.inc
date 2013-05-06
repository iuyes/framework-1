<?php
    /**
     * datasource.inc 数据源组件服务接口Datasource类声明文件
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.datasources
     * @version     1.0 beta
     *
     * @create      2012-07-17
     * @modify      2013-04-28
     */

    /**
     * 声明组件服务开启标志
     */
    define('JTITA_DATASOURCE', true);

    /**
     * 普通参数，当字符串参数需要直接表示为参数值时需要使用本类。
     *
     * @package jtita.kernels.datasources
     */
    class NormalParameter {

        /**
         * 使用$value初始化NormalParameter参数。
         *
         * @param $value
         */
        public function  __construct ($value) {
            $this->_value = $value;
        }

        public function __toString () {
            if (is_null ($this->_value)) return '';
            else return $this->_value . '';
        }

        /**
         * @var string 参数值。
         */
        protected $_value = '';
    }

    /**
     * 数据源异常类，当数据源发生错误时引发本类
     *
     * @package jtita.kernels.datasources
     */
    class DatasourceException extends JtitaException {
    }

    # 导入数据库接口声明
    Jtita::importInterface ('kernels.datasources.database');

    /**
     * 如果未初始化数据源组件，执行初始化；
     */
    if (! Datasource::initialized ()) {
        Datasource::initialize ();
    }

    /**
     * 数据源组件服务接口类，提供从数据源返回数据库的服务
     *
     * @example
     *          Jtita::write('kernels/datasources',array(
     *              'default' => array('class' => 'kernels.datasources.databases.mysql',
     *                                 'server'=> '127.0.0.1','dbname' => 'test','user' => 'root','pass' => '123321'),
     *              'image'   => array('class' => 'kernels.datasources.database.mysql',
     *                                 'server'=> '127.0.0.1','dbname' => 'test','user' => 'root','pass' => '123321'),
     *        );
     *
     * @package jtita.kernels.datasources
     * @abstract
     */
    abstract class Datasource {
        /**
         * @const string 数据源配置节点常量
         */
        const CFG_DATASOURCE = 'kernels/datasources';

        /**
         * 执行组件初始化
         *
         * @return void
         */
        public static function initialize () {
            # 如果已经初始化；
            if (self::$_initialized) {
                trigger_error ('Datasource repeat initialized', E_USER_NOTICE);
            }

            self::$_config = Jtita::read (self::CFG_DATASOURCE, array());
            self::$_initialized = true;
        }

        /**
         * 返回一个布尔值，表示是否已完成初始化工作；
         *
         * @return boolean
         */
        public static function initialized () {
            return self::$_initialized;
        }

        /**
         * 返回当前配置的所有数据库信息名称。
         */
        public static function databaseNames () {
            return array_keys (self::$_config);
        }

        /**
         * 根据$databaseName返回数据库对象
         *
         * @param string $databaseName 数据库名称
         *
         * @return Database|boolean
         *
         * @throws DatasourceException 如果指定$databaseName数据库信息未配置
         */
        public static function database ($databaseName = 'default') {
            # 如果指定数据库还未创建，创建数据库；
            if (isset(self::$_databases[$databaseName]) == false) {
                self::$_databases[$databaseName] = self::_create ($databaseName);
            }

            return self::$_databases[$databaseName];
        }

        /**
         * 根据$databaseName创建数据库访问对象
         *
         * @param string $databaseName 数据库名称
         * @return Database|boolean
         *
         * @throws DatasourceException 如果指定$databaseName数据库信息未配置
         * @throws JtitaException 如果指定数据库的配置信息错误；
         */
        private static function _create ($databaseName) {
            # 如果指定数据库位定义或为配设置类名，执行错误处理；
            if (! isset(self::$_config[$databaseName])) {
                throw new DatasourceException('Datasource not support database name:' . $databaseName);
            }

            # 根据配置信息返回数据库信息。
            return Jtita::ioc (self::$_config[$databaseName]);
        }

        /**
         * @var boolean 初始化标记，判断是否已完成初始化；
         */
        protected static $_initialized = false;

        /**
         * @var array 数据源配置信息；
         */
        protected static $_config = array();

        /**
         * @var array 数据库实例对象集；
         */
        protected static $_databases = array();
    }