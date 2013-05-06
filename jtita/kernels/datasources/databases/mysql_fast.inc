<?php
    /**
     *  database.inc 数据库访问类声明文件
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.datasources
     * @version     1.0 beta
     *
     * @create      2012-07-19
     * @modify      2013-04-28
     */

    /*
     * 如果未引用SqlBuilder；
     */
     Jtita::importInterface ('kernels.datasources.sql_builder');

    /**
     * MySqlBuilder 提供生成MySql SQL指定服务
     *
     * @package jtita.kernels.datasources.databases
     */
    class MySqlBuilder extends SqlBuilder{

        /**
         * 继承SqlBuilder方法，生成安全名称定义
         *
         * @see SqlBuilder::safeName
         */
        public function safeName ($name) {
            return str_replace ('.', '`.`', "`$name`");
        }

        /**
         * 根据$name生成数据表访问描述命令
         *
         * @param string $name 数据表名称
         *
         * @return string
         */
        public function describe ($name) {
            return ' DESCRIBE ' . $this->safeName ($name);
        }

        /**
         * 继承SqlBuilder方法，过滤$value参数值
         *
         * @param string $value 参数值
         *
         * @return mixed
         */
        protected function _pfilter ($value) {
            $mysql = $this->_database->mysqli ();

            # 如果值为字符串，使用escape_string格式化值
            if (is_string ($value)){
                return '\'' . $mysql->real_escape_string ($value) . '\'';
            } # 如果值为数组值
            else if (is_array ($value)){
                return '(' . implode (',', $value) . ')';
            } else return $value;
        }
    }

    # TODO: 先通过难是否read与write分开有利于执行效率；

    /**
     * MySql 数据库对象，提供对Mysql数据库的访问服务
     *
     * @package jtita.kernels.datasources
     */
    class MysqlFast implements Database {
        /**
         * @var string 服务器
         */
        public $server = '127.0.0.1';

        /**
         * @var string 数据库名称；
         */
        public $dbname  = 'test';

        /**
         * @var string 数据库用户名
         */
        public $dbuser  = 'root';

        /**
         * @var string 数据库密码
         */
        public $dbpass  = '123456';

        /**
         * @var string 字符集；
         */
        public $charset = 'utf8';

        /**
         * 析构方法，执行数据的关闭
         */
        public function __destruct () {
            # 如果连接对象已打开，关闭连接对象；
            if ($this->_mysqliWriter){
                $this->_mysqliWriter->close ();
            }
        }

        /**
         * 使用$config初始化MySqli数据服务对象
         *
         * @param array $config初始化配置
         *
         * @return void
         */
        public function initialize ($config = array()) {
            # 如果当前还未进行初始化；
            if ($this->_initialized == false){
                $this->_initializeBuilder ();
                $this->_initializeLink ();

                # 设置初始化变量；
                $this->_initialized = true;
            }
        }

        /**
         * 返回一个布求值，判断是否已经完成初始化；
         */
        public function initialized () {
            return $this->_initialized;
        }

        /**
         * 返回原始的MySql连接对象；
         */
        public function mysqli() {
            return $this->_mysqliWriter;
        }

        /**
         * 根据$dataName获取schema的声明类型定义
         *
         * @see Database::schema
         */
        public function schema ($dataName) {
            # 如果指定数据模式已存在；
            if (! isset($this->_schemas[$dataName])){
                $this->_schemas[$dataName] = $this->_dataSchema ($dataName);
            }

            return $this->_schemas[$dataName];
        }

        /**
         * 根据$data、$syntax和$options执行创建命令，并返回响应行的数量
         *
         * @see Database::create
         */
        public function create ($data, $syntax, $options = array()) {
            $sql = $this->_sqlBuilder->create ($data, $syntax, $options);
            return $this->execute ($sql);
        }

        /**
         * 返回最后一次创建数据的自动编号
         *
         * @see Database::createID
         */
        public function createID () {
            return $this->_mysqliWriter->insert_id;
        }

        /**
         * 根据$data、$syntax和$options执行更新命令，并返回更新响应行的数量
         *
         * @see Database::update
         */
        public function update ($data, $syntax, $options = array()) {
            $sql = $this->_sqlBuilder->update ($data, $syntax, $options);
            return $this->execute ($sql);
        }

        /**
         * 根据$syntax和$options执行删除命令，并返回删除响应行的数量
         *
         * @see Database::delete
         */
        public function delete ($syntax, $options = array()) {
            $sql = $this->_sqlBuilder->delete ($syntax, $options);
            return $this->execute ($sql);
        }

        /**
         * 根据$syntax和$options获取数据
         *
         * @see Database::find
         */
        public function find ($syntax, $options = array()) {
            # 如果没有指定查找的数据表；
            if (! isset($syntax[Database::SYN_TABLE])){
                throw new DatasourceException('Mysql Not support find table');
            }

            # 返回查询结果；
            return $this->query ($this->_sqlBuilder->find ($syntax, $options), array(), $options);
        }

        /**
         * 根据$syntax和$options返回查询字段的行一行内容。
         *
         * @see Database::findFields
         */
        public function findFields ($syntax, $options = array()) {
            # 默认格式器；
            $options[Database::OPT_FORMAT] = Database::OPT_FORMAT_NORMAL;
            $sql = $this->_sqlBuilder->find($syntax, $options);
            $rows = $this->query ($sql, $options);
            # array(0 => array('count' => 10));
            return $rows[0];
        }

        /**
         * 返回查找语法关键字
         *
         * @see Database::findSyntax
         */
        public function findSyntax () {
            return self::$_findSyntax;
        }


        /**
         * 执行自定义命令查询，返回查询的结果
         *
         * @see Database::query
         */
        public function query ($cmd, $args = array(), $options = array()) {
            # 执行查询
            $cmd = $this->_sqlBuilder->prepare ($cmd, $args);
            $result = $this->_mysqliWriter->query ($cmd);

            # 如果查询发生错误，处理错误信息；
            if ($result === false) return $this->_error ($cmd);

            # 格式化结果行；
            $rows = $this->_formatRows ($result, $options);

            $result->free ();
            return $rows;
        }

        /**
         * 执行自定义命令，返回影响到的行
         *
         * @see Database::execute
         */
        public function execute ($cmd, $args = array(), $options = array()) {
            # 执行查询操作；
            $cmd = $this->_sqlBuilder->prepare ($cmd, $args);
            $row = $this->_mysqliWriter->query ($cmd);

            # 如果查询成功返回影响到的行；
            if ($row === false) return $this->_error (); else return $this->_mysqliWriter->affected_rows;
        }

        /**
         * 根据$dataName获取数据模式
         *
         * @param string $dataName数据表名字
         *
         * @return array
         */
        protected function _dataSchema ($dataName) {
            $rows = $this->query ($this->_sqlBuilder->describe ($dataName));
            $schema = array();

            foreach ($rows as $row){
                $schema[$row['Field']] = $this->_fieldSchema ($row);
            }

            return $schema;
        }

        /**
         * 当执行发生错误时，处理错误操作；
         */
        protected function _error ($cmd = '') {
            # 返回值false表示执行发生错误；
            $msg = $this->_mysqliWriter->error . '-' . $cmd;
            throw new DatasourceException($msg);
        }

        /**
         * 根据$result和$options格式化显示结果行
         *
         * @param Resource $result  查询结果
         * @param Array    $options 命令选项
         *
         * @return mixed
         */
        protected function _formatRows ($result, $options) {
            # 获取所需的格式；
            $format = Database::OPT_FORMAT_NORMAL;
            if (! empty($options[Database::OPT_FORMAT])){
                $format = $options[Database::OPT_FORMAT];
            }

            # 处理并返回结果；
            switch ($format){
                case Database::OPT_FORMAT_NORMAL:
                    return $this->_formatNormalRows ($result);
                case Database::OPT_FORMAT_LIST:
                    return $this->_formatListRows ($result);
                case Database::OPT_FORMAT_MODEL:
                    return $this->_formatModelRows ($result);
                case Database::OPT_FORMAT_JSON:
                    return $this->_formatJsonRows ($result);
                default:
                    return $this->_formatNormalRows ($result);
            }
        }

        /**
         * 根据$result返回模型列记录结果
         *
         * @param Resource $result 效果结果
         *
         * @return array
         */
        protected function _formatModelRows ($result) {
            # 获取字段属性，并且生成返回记录集；
            $fields = $result->fetch_fields ();
            $fieldCount = count ($fields);
            $rows = array();

            # 执行数据获取；
            while ($fieldRow = $result->fetch_row ()) {
                $row = array();
                for ($i = 0; $i < $fieldCount; ++$i){
                    $field = $fields[$i];
                    $table = empty($field->table) ? 'other' : $field->table;

                    $row[$table][$field->name] = $fieldRow[$i];
                }

                array_push ($rows, $row);
            }

            return $rows;
        }

        /**
         * 根据$result返回普通普通行记录
         *
         * @param Resource $result 效果结果
         *
         * @return array
         */
        protected function _formatNormalRows ($result) {
            # 处理结果集
            $rows = array();
            while ($row = $result->fetch_assoc ()) {
                array_push ($rows, $row);
            }

            return $rows;
        }

        /**
         * 根据$result格式化列表行记录
         *
         * @param Resource $result 查询结果
         *
         * @return array
         */
        protected function _formatListRows ($result) {
            $rows = array();
            # 处理结果集；
            while ($fieldRow = $result->fetch_row ()) {
                $rows[$fieldRow[0]] = $fieldRow[1];
            }

            return $rows;
        }

        /**
         * 根据$result返回结果的json格式化记录
         *
         * @param Resource $result 查询结果
         *
         * @return string
         */
        protected function _formatJsonRows ($result) {
            $rows = $this->_formatNormalRows ($result);
            return json_encode ($rows);
        }

        /**
         * 初始化mysql连接对象
         */
        protected function _initializeLink () {
            try{
                # 初始化连接对象；
                $this->_mysqliWriter = new mysqli($this->server, $this->dbuser, $this->dbpass,$this->dbname);
                # 初始化连接广本格式；
                if (! $this->_mysqliWriter->query ('set names ' . $this->charset)){
                    $this->_error ();
                }
            } catch (Exception $ex){
                throw new DatasourceException($ex->getMessage ());
            }
        }

        /**
         * 初始化SQLBuilder生成器
         */
        protected function _initializeBuilder () {
            $this->_sqlBuilder = new MySqlBuilder($this);
        }

        /**
         * 根据$field查询的字段信息，生成Schema信息
         *
         * @param string $field 字段名字
         *
         * @return array
         */
        protected function _fieldSchema ($field) {
            # 获取类型信息；
            list($dbtype, $dbsize) = $this->_fieldType ($field['Type']);

            # 生成常规Schema选项；
            $fieldSchema = array(Database::SCHEMA_FIELD => $field['Field'], Database::SCHEMA_NULL => $field['Null'],
                                 Database::SCHEMA_DEFAULT => $field['Default'],
                                 Database::SCHEMA_EXTRA => $field['Extra'], Database::SCHEMA_KEY => $field['Key'],);

            # 生成size，并且合并默认值；
            if ($dbsize) $fieldSchema[Database::SCHEMA_SIZE] = $dbsize;
            return array_merge (self::$_dtMappings[$dbtype], $fieldSchema);
        }

        /**
         * 根据$type获取列表类型定义
         *
         * @param string $type 类型声明
         *
         * @return array(string,int)
         */
        protected function _fieldType ($type) {
            $dbtype = $type;
            $dbsize = false;

            # int(1) number(1,10)
            if ($pos = strpos ($type, '(')){
                $dbtype = substr ($type, 0, $pos);
                $dbsize = substr ($type, $pos + 1, - 1);
            }

            return array($dbtype, $dbsize);
        }

        /**
         * @var array 数据类型映射关系；
         */
        protected static $_dtMappings = array('bit' => array(Database::SCHEMA_DBTYPE => 'bit',
                                                             Database::SCHEMA_DTTYPE => DT::Boolean),
                                              'tinyint' => array(Database::SCHEMA_DBTYPE => 'tinyint',
                                                                 Database::SCHEMA_DTTYPE => DT::Integer,
                                                                 Database::SCHEMA_FORMATER => 'intval'),
                                              'smallint' => array(Database::SCHEMA_DBTYPE => 'smallint',
                                                                  Database::SCHEMA_DTTYPE => DT::Integer,
                                                                  Database::SCHEMA_FORMATER => 'intval'),
                                              'mediumint' => array(Database::SCHEMA_DBTYPE => 'mediumint',
                                                                   Database::SCHEMA_DTTYPE => DT::Integer,
                                                                   Database::SCHEMA_FORMATER => 'intval'),
                                              'int' => array(Database::SCHEMA_DBTYPE => 'int',
                                                             Database::SCHEMA_DTTYPE => DT::Integer,
                                                             Database::SCHEMA_FORMATER => 'intval'

                                              ), 'bigint' => array(Database::SCHEMA_DBTYPE => 'bigint',
                                                                   Database::SCHEMA_DTTYPE => DT::Integer,
                                                                   Database::SCHEMA_FORMATER => 'intval'),
                                              'real' => array(Database::SCHEMA_DBTYPE => 'real',
                                                              Database::SCHEMA_DTTYPE => DT::Float,
                                                              Database::SCHEMA_FORMATER => 'floatval'),
                                              'double' => array(Database::SCHEMA_DBTYPE => 'double',
                                                                Database::SCHEMA_DTTYPE => DT::Float,
                                                                Database::SCHEMA_FORMATER => 'floatval'

                                              ), 'float' => array(Database::SCHEMA_DBTYPE => 'float',
                                                                  Database::SCHEMA_DTTYPE => DT::Float,
                                                                  Database::SCHEMA_FORMATER => 'floatval'),
                                              'decimal' => array(Database::SCHEMA_DBTYPE => 'decimal',
                                                                 Database::SCHEMA_DTTYPE => DT::Decimal,
                                                                 Database::SCHEMA_FORMATER => 'floatval'),
                                              'numeric' => array(Database::SCHEMA_DBTYPE => 'numeric',
                                                                 Database::SCHEMA_DTTYPE => DT::Decimal,
                                                                 Database::SCHEMA_FORMATER => 'floatval'),
                                              'date' => array(Database::SCHEMA_DBTYPE => 'date',
                                                              Database::SCHEMA_DTTYPE => DT::Date),
                                              'time' => array(Database::SCHEMA_DBTYPE => 'time',
                                                              Database::SCHEMA_DTTYPE => DT::Time),
                                              'timestamp' => array(Database::SCHEMA_DBTYPE => 'timestamp',
                                                                   Database::SCHEMA_DTTYPE => DT::DateTime),
                                              'datetime' => array(Database::SCHEMA_DBTYPE => 'datetime',
                                                                  Database::SCHEMA_DTTYPE => DT::DateTime),
                                              'year' => array(Database::SCHEMA_DBTYPE => 'year',
                                                              Database::SCHEMA_DTTYPE => DT::Integer),
                                              'char' => array(Database::SCHEMA_DBTYPE => 'char',
                                                              Database::SCHEMA_DTTYPE => DT::String),
                                              'varchar' => array(Database::SCHEMA_DBTYPE => 'varchar',
                                                                 Database::SCHEMA_DTTYPE => DT::String),
                                              'binary' => array(Database::SCHEMA_DBTYPE => 'binary',
                                                                Database::SCHEMA_DTTYPE => DT::Bold),
                                              'varbinary' => array(Database::SCHEMA_DBTYPE => 'varbinary',
                                                                   Database::SCHEMA_DTTYPE => DT::Bold),
                                              'tinyblob' => array(Database::SCHEMA_DBTYPE => 'tinyblob',
                                                                  Database::SCHEMA_DTTYPE => DT::Bold),
                                              'mediumblob' => array(Database::SCHEMA_DBTYPE => 'mediumblob',
                                                                    Database::SCHEMA_DTTYPE => DT::Bold),
                                              'blob' => array(Database::SCHEMA_DBTYPE => 'blob',
                                                              Database::SCHEMA_DTTYPE => DT::Bold),
                                              'longblob' => array(Database::SCHEMA_DBTYPE => 'blob',
                                                                  Database::SCHEMA_DTTYPE => DT::Bold),
                                              'tinytext' => array(Database::SCHEMA_DBTYPE => 'tinytext',
                                                                  Database::SCHEMA_DTTYPE => DT::Text),
                                              'text' => array(Database::SCHEMA_DBTYPE => 'text',
                                                              Database::SCHEMA_DTTYPE => DT::Text),
                                              'mediumtext' => array(Database::SCHEMA_DBTYPE => 'text',
                                                                    Database::SCHEMA_DTTYPE => DT::Text),
                                              'longtext' => array(Database::SCHEMA_DBTYPE => 'longtext',
                                                                  Database::SCHEMA_DTTYPE => DT::Text),
                                              'enum' => array(Database::SCHEMA_DBTYPE => 'enum',
                                                              Database::SCHEMA_DTTYPE => DT::Other),
                                              'set' => array(Database::SCHEMA_DBTYPE => 'set',
                                                             Database::SCHEMA_DTTYPE => DT::Other));

        /**
         * @var array find查询访问命令
         */
        protected static $_findSyntax = array('fields' => 'fields', 'joins' => 'joins', 'where' => 'where',
                                              'orderBy' => 'order', 'limit' => 'limit', 'distinct' => 'distinct');

        /**
         * @var array 数据库配置信息
         */
        protected $_config = array();

        /**
         * @var boolean 初始化标记
         */
        protected $_initialized = false;

        /**
         * @var SqlBuilder 提供生成SQL指定的服务
         */
        protected $_sqlBuilder = null;

        /**
         * @var array schemas提供数据模型存储
         */
        protected $_schemas = array();


        /**
         * @var MySqli 数据库连接对象
         */
        protected $_mysqliWriter = null;

        /**
         * @var MySqli 数据库读取连接
         */
        protected $_mysqliReader = null;
    }