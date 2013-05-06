<?php
    /**
     *  database.php 数据库访问类声明文件
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.datasources
     * @version     1.0 beta
     *
     * @create      2012-09-10
     * @modify      2013-04-28
     */

    /**
     * 数据类型类，提供数据库类型常量声明
     *
     * <b>说明</b>
     * <p>这里的常量声明未声明为全大写字符，这时为了看起来更像是在使用类型。</p>
     *
     * @package jtita.kernels.datasources
     */
    class DT {
        /**
         * @const int 布尔类型常量声明
         */
        const Boolean = 1;

        /**
         * @const int 整型类型常量声明
         */
        const Integer = 2;

        /**
         * @const int 浮点类型常量声明
         */
        const Float = 3;

        /**
         * @const int 货币类型常量声明
         */
        const Decimal = 4;

        /**
         * @const int 日期类型常量声明
         */
        const Date = 5;

        /**
         * @const int 时间类型常量声明
         */
        const Time = 6;

        /**
         * @const int 日期时间类型常量声明
         */
        const DateTime = 7;

        /**
         * @const int 字符串类型常量声明
         */
        const String = 8;

        /**
         * @const int 文本类型常量声明
         */
        const Text = 9;

        /**
         * @const int 大数据类型常量声明
         */
        const Bold = 10;

        /**
         * @const int 其它类型常量声明
         */
        const Other = 11;
    }

    /**
     * 数据库接口，提供数据库的访问能力
     *
     * @package jtita.kernels.datasources
     */
    interface Database {
        /**
         * @const string SCHEMA字段名称常量
         */
        const SCHEMA_FIELD = 'field';

        /**
         * @const string SCHEMA字段数据类型
         */
        const SCHEMA_DTTYPE = 'dtname';

        /**
         * @const string SCHEMA数据库字段类型
         */
        const SCHEMA_DBTYPE = 'dbtype';

        /**
         * @const string SCHEMA默认值
         */
        const SCHEMA_DEFAULT = 'default';

        /**
         * @const string SCHEMA数据长度
         */
        const SCHEMA_SIZE = 'size';

        /**
         * @const string SCHEMA 空值
         */
        const SCHEMA_NULL = 'null';

        /**
         * @const string SCHEMA 额外
         */
        const SCHEMA_EXTRA = 'extra';

        /**
         * @const string SCHEMA关键字常量
         */
        const SCHEMA_KEY = 'key';

        /**
         * @const string SCHEMA 格式器
         */
        const SCHEMA_FORMATER = 'formater';

        /**
         * @const string SCHEMA 格式字符串
         */
        const SCHEMA_FORMAT_STRING = 'formatString'; #

        /**
         * @const string 数据库表语法
         */
        const SYN_TABLE = 'table';

        /**
         * @const string 数据库表别名语法
         */
        const SYN_ALIAS = 'alias';

        /**
         * @const string 数据库字段语法
         */
        const SYN_FIELDS = 'fields';

        /**
         * @const string 数据库多表语法
         */
        const SYN_JOINS = 'joins';

        /**
         * @const string 数据库条件语法
         */
        const SYN_WHERE = 'where';

        /**
         * @const string 数据库排序语法
         */
        const SYN_ORDER = 'order';

        /**
         * @const string 数据库语法限制
         */
        const SYN_LIMIT = 'limit';

        /**
         * @const string 数据库选项，多记录选项
         */
        const OPT_MULTIPLE = 'multiple';

        /**
         * @const string 数据库选项，字段别名选项
         */
        const OPT_ASFIELD = 'asfield';

        /**
         * @const string 数据库选项，例外字段选项
         */
        const OPT_EXCEPT = 'except';

        /**
         * @const string 数据库关系选项
         */
        const OPT_RELATION = 'relation';

        /**
         * @const string 数据库关系选项，连接方式选项
         */
        const OPT_RELATION_JOIN = 'join';

        /**
         * @const string 数据库关系选项，ON选项
         */
        const OPT_RELATION_ON = 'on';

        /**
         * @const string 数据库关系选项，外值选项
         */
        const OPT_RELATION_FOREIGN = 'foreign';

        /**
         * @const string 数据库关系选项，主键选项
         */
        const OPT_RELATION_PRIMARY = 'primary';

        /**
         * @const string 数据库选项，返回格式类型选项
         */
        const OPT_FORMAT = 'format';

        /**
         * @const string 数据库格式选项，普通格式
         */
        const OPT_FORMAT_NORMAL = 'normal';

        /**
         * @const string 数据库格式选项，模型格式
         */
        const OPT_FORMAT_MODEL = 'model';

        /**
         * @const string 数据库格式选项，列表格式
         */
        const OPT_FORMAT_LIST = 'list';

        /**
         * @const string 数据库格式选项，json格式
         */
        const OPT_FORMAT_JSON = 'json';

        /**
         * @const string 数据库格式选项，xml格式
         */
        const OPT_FORMAT_XML = 'xml';

        /**
         * 使用$config初始化数据库
         *
         * @param array $config 初始化配置信息
         * @return void
         */
        function initialize ($config = array());

        /**
         * 返回一个布求值，表示是否完成初始化
         *
         * @return boolean
         */
        function initialized ();

        /**
         * 根据$dataName获取数据的schemas信息
         *
         * @example
         *  <pre>
         *        array (
         *            'id' => array(
         *                'type' => 'int',
         *                'size' => 12,
         *                'default' => '1',
         *                'null' => false,'extra' => ''),
         *            'name' => array(
         *                'type' => 'varchar',
         *                'size' => 32,
         *                'default' => false,
         *                'null' => false,'extra' => '')
         *        );
         *  </pre>
         *
         * @param string $dataName 数据名称
         * @return array()
         *
         * @throws DatasourceException 如果指定$schema在数据库中不存在，引发本异常
         */
        function schema ($dataName);

        /**
         * 根据$data、$syntax和$options创建数据信息到数据库，返回增加的行数量
         *
         * @param array $data    保存数据
         * @param array $syntax  命令语法
         * @param array $options 语法参数
         * @return int
         *
         * @throws DatasourceException 保存失败时引发本异常
         */
        function create ($data, $syntax, $options = array());

        /**
         * 如果数据库支持，返回最近创建数据的ID编号
         *
         * @return mixed
         */
        function createID ();

        /**
         * 根据$data、$syntax和$options更新已有的数据，并返回影响的行
         *
         * @param array $data    需要更新的数据
         * @param array $syntax  更新语法
         * @param array $options 更新选项
         * @return int
         *
         * @throws DatasourceException 更新失败时引发本异常
         */
        function update ($data, $syntax, $options = array());

        /**
         * 根据$syntax和$options删除数据库已有数据，并返回删除响应的行数量
         *
         * @param array $syntax  删除语法
         * @param array $options 删除选项
         * @return int
         *
         * @throws DatasourceException 如果删除过程中发生错误，引发本异常
         */
        function delete ($syntax, $options = array());

        /**
         * 根据$syntax和$options查找数据库中的数据，并返回
         *
         * @param array $syntax  查找语法
         * @param array $options 查找选项
         * @return mixed
         *
         * @throws DatasourceException 如果查找过程中发生错误，将引发本异常
         */
        function find ($syntax, $options = array());

        /**
         * 根据$syntax和$options获取查找数量
         *
         * @param array $syntax  查找语法
         * @param array $options 查找选项
         * @return int
         *
         * @throws DatasourceException 当查找过程中发生错误，将引发本异常
         */
        function findFields ($syntax, $options = array());

        /**
         * 返回当前数据库支持的查找语法
         *
         * @return array
         */
        function findSyntax ();

        /**
         * 根据$cmd、$args和$options执行自定义查询
         *
         * @param mixed $cmd     自定义命令
         * @param array $args    命令参数
         * @param array $options 命令选项
         * @return mixed
         *
         * @throws DatasourceException 如果查询发生错误，将引发本异常
         */
        function query ($cmd, $args = array(), $options = array());


        /**
         * 根据$cmd、$args和$options执行自定义命令
         *
         * @param mixed $cmd     自定义命令
         * @param array $args    命令参数
         * @param array $options 命令选项
         *
         * @throws DatasourceException 执行命令的过程中发生错误，将引发本异常
         */
        function execute ($cmd, $args = array(), $options = array());
    }