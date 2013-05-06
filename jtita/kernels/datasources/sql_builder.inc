<?php
    /**
     * sql_builder.php SQL生成器SqlBuilder类声明文件
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.datasources
     * @version     1.0 beta
     *
     * @create      2012-08-29
     * @modify      2013-04-28
     */

    /**
     * SQL 生成器，提供生成SQL访问所需的SQL Statements
     *
     * @package jtita.kernels.datasources
     */
    abstract class SqlBuilder {
        /**
         * @var array 表连接类型
         */
        static $joinTypes = array(
            'left' => 'LEFT JOIN',
            'right' => 'RIGHT JOIN',
            'inner' => 'INNER JOIN',
            'outer' => 'OUTER JOIN'
        );

        /**
         * @var Database 数据库引用
         */
        protected $_database = null;

        /**
         * 使用$database 初始化SqlBuilder实例 。
         *
         * @param Database 数据库实例
         * @return SqlBuilder
         */
        public function __construct ($database) {
            $this->_database = $database;
        }

        /**
         * 对$name进行安全命名，防止关键字冲突。
         *
         * @abstract
         *
         * @param string $name 原始名称
         * @return string
         */
        public abstract function safeName ($name);

        /**
         * 根据$cmd和$args生成准备执行的SQL命令。
         *
         * @param string $cmd  SQL命令
         * @param array  $args SQL 命令参数
         * @return string
         */
        public function prepare ($cmd, $args = array()) {
            # 如果参数为空，直接返回命令
            if (empty($args)) return $cmd;

            # 使用_pfilter方法处理参数集，并返回替换结果。
            $args = array_map (array($this,'_pfilter'), $args);
            return str_replace (array_keys ($args), $args, $cmd);
        }

        /**
         * 根据$data、$syntax和$options生成INSERT创建命令。
         *
         * @param array $data    创建数据
         * @param array $syntax  命令语法
         * @param array $options 选项参数
         * @return string
         */
        public function create ($data, $syntax, $options = array()) {
            # 处理语法别名
            $this->_prepareSyntax ($syntax);

            # 返回生成结果
            return $this->_insert ($syntax, $options) . $this->_insertFields ($data, $syntax, $options);
        }

        /**
         * 根据$data、$syntax和$options生成数据更新SQL命令
         *
         * @param array $data    更新数据
         * @param array $syntax  命令语法
         * @param array $options 更新选项参数
         * @return string
         */
        public function update ($data, $syntax, $options = array()) {
            # 处理语法别名
            $this->_prepareSyntax ($syntax);

            # 返回更新生成结果
            return $this->_update ($syntax, $options) . $this->_updateFields ($data, $syntax, $options) . $this->_where ($syntax, $options) . $this->_order ($syntax, $options) . $this->_limit ($syntax, $options);
        }

        /**
         * 根据$syntax和$options生成删除命令。
         *
         * @param array $syntax  语法参数
         * @param array $options 命令语法
         * @return string
         */
        public function delete ($syntax, $options = array()) {
            # 处理语法别名
            $this->_prepareSyntax ($syntax);

            # 返回删除更新结果,注意：MYSQL不支持DELETE FROM user order by limit 语法。
            return $this->_delete ($syntax, $options) . $this->_where ($syntax, $options) /* .
				   $this->_order($syntax,$options) . 
                   $this->_limit($syntax,$options)*/
                ;
        }

        /**
         * 根据$syntax和$options生成查找命令
         *
         * @param array $syntax  语法参数
         * @param array $options 选项参数
         * @return string
         */
        public function find ($syntax, $options = array()) {
            # 处理语法别名
            $this->_prepareSyntax ($syntax);

            # 返回查找生成的结果
            return $this->_select ($syntax, $options) . $this->_from ($syntax, $options) . $this->_where ($syntax, $options) . $this->_order ($syntax, $options) . $this->_limit ($syntax, $options);
        }

        /**
         * 预处理语法
         *
         * @param array $syntax 命令语法
         * @return void
         */
        protected function _prepareSyntax (&$syntax) {
            # 判断是否有别名
            if (empty($syntax[Database::SYN_ALIAS])) {
                $syntax[Database::SYN_ALIAS] = $syntax[Database::SYN_TABLE];
            }

            # 如果包含连接，但连接为非字符串
            if (! empty($syntax['joins']) && ! is_array ($syntax['joins'])) {
                $syntax['joins'] = array($syntax['joins']);
            }
        }

        /**
         * 根据$syntax生成INSERT INTO 命令
         *
         * @param array $syntax  命令语法
         * @param array $options 命令参数
         * @return string
         */
        protected function _insert ($syntax, $options) {
            return 'INSERT INTO ' . $this->safeName ($syntax[Database::SYN_TABLE]);
        }

        /**
         * 根据$data、$syntax和$options生成插入字段命令
         *
         * @param array $data    数据参数
         * @param array $syntax  命令语法
         * @param array $options 命令选项
         * @return string
         */
        protected function _insertFields ($data, $syntax, $options) {
            # 判断是否为一次插入多个值
            if (empty($options[Database::OPT_MULTIPLE])) {
                $data = array($data);
            }

            # 返回处理SCHEMA和字段
            list($schema, $dataFields) = $this->_dataFields ($data[0], $syntax);
            $fields = implode (',', array_map (array(
                                                    $this,
                                                    'safeName'
                                               ), $dataFields));

            # $values = implode(',',array_map(array($this,'_pfilter'),$rows));
            $values = 'VALUES';
            foreach ($data as $row) {
                $values .= '(';
                foreach ($dataFields as $field) {
                    $values .= ($this->_vfilter ($schema[$field], $row[$field]) . ',');
                }
                $values = substr ($values, 0, - 1) . '),';
            }

            $values = substr ($values, 0, - 1);
            return sprintf ('(%s) %s', $fields, $values);
        }

        /**
         * 根据$syntax和$options生成更新SQL命令
         *
         * @param array $syntax  命令语法
         * @param array $options 命令选项
         * @return string
         */
        protected function _update ($syntax, $options) {
            return 'UPDATE ' . $this->_tableAsName ($syntax) . $this->_joinsTable ($syntax, $options);
        }

        /**
         * 根据$data、$syntax和$options生成更新字段
         *
         * @param array $data    字段数据
         * @param array $syntax  命令语法
         * @param array $options 命令选项
         * @return string
         */
        protected function _updateFields ($data, $syntax, $options = array()) {
            # 如果数据为字符串，直接返回
            if (is_string ($data)) return (' SET ' . $data);
            if (! empty($data[0])) return (' SET ' . $this->prepare ($data[0], $data));

            # 获取表别名
            $alias = $syntax[Database::SYN_ALIAS];
            $fields = ' SET ';

            # SET [User].[id] = 12,[User].[name] = 'zhenggp',;
            list($schema, $dataFields) = $this->_dataFields ($data, $syntax, $options);

            # 处理数据字段
            foreach ($dataFields as $field) {
                $fields .= ($this->_fullField ($field, $alias) . ' = ' . $this->_vfilter ($schema[$field], $data[$field]) . ',');
            }

            # SET User].[id] = 12,[User].[name] = `zhenggp`;
            return substr ($fields, 0, - 1);
        }

        /**
         * 根据$fieldSchema和$value对值进行过滤
         *
         * @param array $fieldSchema 字段SCHEMA
         * @param mixed $value       字段值
         * @return mixed
         */
        protected function _vfilter ($fieldSchema, $value) {
            if (empty($fieldSchema[Database::SCHEMA_FORMATER])) {
                return $this->_pfilter ($value);
            }
            else if (empty($fieldSchema[Database::SCHEMA_FORMAT_STRING])) {
                return $this->_pfilter (call_user_func ($fieldSchema[Database::SCHEMA_FORMATER], $value));
            }
            else {
                return $this->_pfilter (call_user_func ($fieldSchema[Database::SCHEMA_FORMATER], $fieldSchema[Database::SCHEMA_FORMAT_STRING], $value));
            }
        }

        /**
         * 根据$data、$syntax和$options获取数据SCHEMA和字段信息。
         *
         * @param array $data    数据字段
         * @param array $syntax  数据语法
         * @param array $options 选项
         * @return array(schema,fields)
         */
        protected function _dataFields ($data, $syntax, $options = array()) {
            $schema = $this->_database->schema ($syntax[Database::SYN_TABLE]);
            $fields = array_intersect (array_keys ($data), array_keys ($schema));

            # 如果更新语法中包含字段，则处理字符集合。
            if (isset($syntax[Database::SYN_FIELDS])) {
                if (! empty($options[Database::OPT_EXCEPT])) {
                    $fields = array_diff ($fields, $syntax[Database::SYN_FIELDS]);
                }
                else {
                    $fields = array_intersect ($fields, $syntax[Database::SYN_FIELDS]);
                }
            }

            return array(
                $schema,
                $fields
            );
        }

        /**
         * 根据$data和$options生成删除命令
         *
         * @param array $syntax  命令语法
         * @param array $options 命令参数
         * @return string
         */
        protected function _delete ($syntax, $options) {
            return 'DELETE ' . $this->safeName ($syntax[Database::SYN_ALIAS]) . $this->_from ($syntax, $options);
        }

        /**
         * 根据$data和$options生成SELECT语法命令。
         *
         * @param array $syntax  命令语法
         * @param array $options 命令参数
         * @return string
         */
        protected function _select ($syntax, $options) {
            return 'SELECT ' . $this->_fields ($syntax, $options);
        }

        /**
         * 根据$data和$options生成命令字段集
         *
         * @param array $syntax  命令语法
         * @param array $options 命令选项
         * @return string
         */
        protected function _fields ($syntax, $options) {
            # 判断是否指定语法字段
            if (empty($syntax[Database::SYN_FIELDS])) {
                $fields = $this->_tableFields ($syntax[Database::SYN_TABLE], $syntax[Database::SYN_ALIAS]);

                # 判断是否包含连接表。
                if (! empty($syntax[Database::SYN_JOINS]) && ! empty($options[Database::OPT_RELATION])) {
                    $fields .= $this->_relationFields ($syntax[Database::SYN_JOINS], $options[Database::OPT_RELATION]);
                }

                return $fields;
            }
            else {
                return $this->_explicitFields ($syntax[Database::SYN_FIELDS], ! empty($options[Database::OPT_ASFIELD]));
            }
        }

        /**
         * 显示字段$fields处理，$aliasField指定是否为包含别名处理。
         *
         * @param array|string 显示字段参数
         * @param boolean $aliasField 别名判断字段
         * @return string
         */
        protected function _explicitFields ($fields, $aliasField) {
            # 如果显示字段为字符串。
            if (is_string ($fields)) {
                return $fields;
            }
            # 如果显示字段为数组并且指定采用别名方式
            else if (is_array ($fields) && $aliasField) {
                return $this->_aliasFields ($fields);
            }
            # 如果指定字段为数组。
            else if (is_array ($fields)) {
                return implode (',', array_map (array(
                                                     $this,
                                                     'safeName'
                                                ), $fields));
            }
            else {
                return '*';
            }
        }

        /**
         * 根据$table和$alias生成表的字段信息
         *
         * @param string  $table 表名
         * @param boolean $alias 是否采用别名字段形式
         * @return string
         */
        protected function _tableFields ($table, $alias) {
            $schema = $this->_database->schema ($table);
            return $this->_fullFields (array_keys ($schema), $alias);
        }

        /**
         * 根据$joins和$relations生成关系表字段
         *
         * @param array $joins     连接关系名称
         * @param array $relations 关系
         * @return string
         *
         * @throws DatasourceException
         */
        protected function _relationFields ($joins, $relations) {
            $fields = '';

            # 处理关系
            foreach ($joins as $relationName) {
                # 如果连接关系不存在。
                if (empty($relations[$relationName])) {
                    throw new  DatasourceException('JTK0801:SqlBuilder not support relations:' . $relationName);
                }

                # 获取关系表名称和别名
                list($table, $alias) = $this->_relationTableName ($relations[$relationName]);

                # 如果指定字段为空。
                if (empty($relations[$relationName][Database::SYN_FIELDS])) {
                    $fields = $fields . ',' . $this->_tableFields ($table, $alias);
                }
                else {
                    $fields = $fields . ',' . $this->_explicitFields ($relations[$relationName][Database::SYN_FIELDS], ! empty($relations[$relationName][Database::OPT_ASFIELD]));
                }
            }

            # ,[user_group].[id],[user_group].[name]
            return $fields;
        }

        /**
         * 根据$fields生成别名字段命令
         *
         * @param array $fields 字段集
         * @return string
         */
        protected function _aliasFields ($fields) {
            $safeFields = '';

            # `username`,`user_group`,
            foreach ($fields as $field => $fieldAlias) {
                $safeFields .= ($this->safeName ($field) . ' AS ' . $this->safeName ($fieldAlias) . ',');
            }

            # `username`,`user_group`
            return substr ($safeFields, 0, - 1);
        }

        /**
         * 根据$fields和$alias生成字段完整命令
         *
         * @param array   $fields 字段集
         * @param boolean $alias  别名
         * @return string
         */
        protected function _fullFields ($fields, $alias) {
            $safeFields = '';

            # `user`.`id`,`user`.`name`,
            foreach ($fields as $field) {
                $safeFields .= ($this->_fullField ($field, $alias) . ',');
            }

            # `user`.`id`,`user`.`name`
            return substr ($safeFields, 0, - 1);
        }

        /**
         * 根据$safeField和$safeAlias生成字段完整标识
         *
         * @param string $field 字段名称
         * @param string $alias 表别名
         * @return string
         */
        protected function _fullField ($field, $alias) {
            return $this->safeName ($alias . '.' . $field);
        }

        /**
         * 根据$syntax和$options生成WHERE条件命令
         *
         * @param array $syntax  语法命令
         * @param array $options 语法选项
         * @return string
         */
        protected function _where ($syntax, $options) {
            # 如果不包含WHERE条件。
            if (empty($syntax[Database::SYN_WHERE])) return '';

            # 获取WHERE语法条件
            $where = $syntax[Database::SYN_WHERE];
            if (is_string ($where)) {
                return ' WHERE ' . $where;
            }
            else if (is_array ($where) && ! empty($where[0])) {
                # array('User.id = :id',':id' => 12);
                return ' WHERE ' . $this->prepare ($where[0], $where);
            }
            else if (is_array ($where)) {
                $strWhere = '';
                # [User].[id] = 1 AND [User].[name] = 'zhenggp' AND ;
                foreach ($where as $field => $value) {
                    # 如果$value为数组，使用in运算符。
                    if (is_array ($value)) {
                        $strWhere .= ($this->safeName ($field) . ' in ' . $this->_pfilter ($value) . ' AND ');
                    }
                    # 如果在字段中找到空白，表示指定了查询运算符;
                    else if (strstr ($field, ' ')) {
                        $strWhere .= ($field . ' ' . $this->_pfilter ($value) . ' AND ');
                    }
                    else {
                        $strWhere .= ($this->safeName ($field) . ' = ' . $this->_pfilter ($value) . ' AND ');
                    }
                }

                # [User].[id] = 1 AND [User].[name] = 'zhenggp';
                return ' WHERE ' . substr ($strWhere, 0, - 5);
            }

            return '';
        }

        /**
         * 根据$syntax和$options生成LIMIT语法命令
         *
         * @param array $syntax  命令语法
         * @param array $options 命令选项
         * @return string
         */
        protected function _limit ($syntax, $options) {
            # 如果没有指定limit语法，直接返回空字符串。
            if (empty($syntax[Database::SYN_LIMIT])) return '';
            # 如果limit语法为数组。
            if (is_array ($syntax[Database::SYN_LIMIT])) {
                return ' LIMIT ' . implode (',', $syntax[Database::SYN_LIMIT]);
            }
            # 如果limit语法为字符串，则直接返回LIMIT语法结果 TODO: 缺乏测试。
            else  {
                return ' LIMIT ' . $syntax[Database::SYN_LIMIT];
            }
        }

        /**
         * 根据$syntax和$options生成SQL FROM 命令
         *
         * @param array $syntax  命令语法
         * @param array $options 命令选项
         * @return string
         */
        protected function _from ($syntax, $options) {
            return ' FROM ' . $this->_tableAsName ($syntax) . $this->_joinsTable ($syntax, $options);
        }

        /**
         * 根据$syntax生成SQL TABLE AS 命令
         *
         * @param array $syntax 语法命令
         * @return string
         */
        protected function _tableAsName ($syntax) {
            return $this->safeName ($syntax[Database::SYN_TABLE]) . ' AS ' . $this->safeName ($syntax[Database::SYN_ALIAS]);
        }

        /**
         * 根据$syntax生成SQL ORDER BY命令
         *
         * @param array $syntax  命令语法
         * @param array $options 命令选项
         * @return string
         */
        protected function _order ($syntax, $options) {
            # 如果语法未指定order，返回空字符串。
            if (empty($syntax[Database::SYN_ORDER])) return '';

            # 如果指定order语法为字符串。
            if (is_string ($syntax[Database::SYN_ORDER])) {
                return ' ORDER BY ' . $syntax[Database::SYN_ORDER];
            }
            # 如果指定order语法为数组
            else if (is_array ($syntax[Database::SYN_ORDER])) {
                $order = ' ORDER BY ';
                # ORDER BY [User].[id] ASC,[User].[name] DESC,
                foreach ($syntax[Database::SYN_ORDER] as $field => $dire) {
                    $order .= ($this->safeName ($field) . ' ' . strtoupper ($dire) . ',');
                }

                return substr ($order, 0, - 1);
            }
            else return '';
        }

        /**
         * 根据$syntax和$options生成连接表SQL命令
         *
         * @param array $syntax  命令语法
         * @param array $options 命令选项
         * @return string
         */
        protected function _joinsTable ($syntax, $options) {
            $joinTable = '';

            # 如果joins语法不为空，并且关系关系选项不为空。
            if (! empty($syntax[Database::SYN_JOINS]) && $options[Database::OPT_RELATION]) {
                $joinTable = $this->_relationTables ($syntax, $options[Database::OPT_RELATION]);
            }

            return $joinTable;
        }

        /**
         * 根据$syntax和$relations生成SQL JOIN命令
         *
         * @param array $syntax    命令语法
         * @param array $relations 命令选项
         * @return string
         *
         * @throws DatasourceException
         */
        protected function _relationTables ($syntax, $relations) {
            # joinTables 命令
            $joinTables = '';

            foreach ($syntax[Database::SYN_JOINS] as $relationName) {
                # 如果指定关系不存在，则引发异常信息
                if (empty($relations[$relationName])) {
                    throw new DatasourceException('SqlBuilder not support relations:' . $relationName);
                }

                # 获取关系表名称和表别名
                list($table, $alias) = $this->_relationTableName ($relations[$relationName]);
                $relation = $relations[$relationName];

                # 获取连接方式。
                $join = empty($relation[Database::OPT_RELATION_JOIN]) ? 'left' : $relation[Database::OPT_RELATION_JOIN];

                # 如果获取join on语法命令为空，自动生成ON语法
                if (empty($relation[Database::OPT_RELATION_ON])) {
                    # 获取关系外键
                    $foreign = empty($relation[Database::OPT_RELATION_FOREIGN]) ? $table . '_id' : $relation[Database::OPT_RELATION_FOREIGN];
                    # 获取关系主键
                    $primary = empty($relation[Database::OPT_RELATION_PRIMARY]) ? 'id' : $relation[Database::OPT_RELATION_PRIMARY];

                    # `user`.`user_group_id` = `user_group`.`id`;
                    $on = sprintf ('%s.%s = %s.%s', $this->safeName ($syntax[Database::SYN_ALIAS]), $this->safeName ($foreign), $this->safeName ($alias), $this->safeName ($primary));
                }
                # 如果包含ON命令，直接生成$on命令
                else {
                    $on = $relation[Database::OPT_RELATION_ON];
                }

                # LEFT JOIN `user_group` as `group`	ON `user`.`user_group_id` = `group`.`id`;
                $joinTables .= sprintf (' %s %s AS %s ON %s', self::$joinTypes[$join], $this->safeName ($table), $this->safeName ($alias), $on);
            }

            return $joinTables;
        }

        /**
         * 根据$relation生成关系表名称。
         *
         * @param array $relation 关系
         * @return array(table,alias)
         *
         * @throws DatasourceException
         */
        protected function _relationTableName ($relation) {
            # 如果未指定关系表名
            if (empty($relation[Database::SYN_TABLE])) {
                throw new DatasourceException('JTK0702:SqlBuilder relation not support table.');
            }

            $table = $relation[Database::SYN_TABLE];
            $alias = empty($relation[Database::SYN_ALIAS]) ? $table : $relation[Database::SYN_ALIAS];

            return array(
                $table,
                $alias
            );
        }

        /**
         * 根据$value过滤参数值并返回。
         *
         * @param mixed $value 参数值
         * @return mixed
         */
        protected abstract function _pfilter ($value);
    }
