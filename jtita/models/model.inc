<?php
    /**
     * model.inc 模型类声明文件
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.models
     * @version     1.0 beta
     *
     * @create      2012-07-23
     * @modify      2013-04-28
     */

    # 导入模型查找服务
    Jtita::importInterface ('models.model_finder');
    # 引用模型关系
    Jtita::importInterface ('models.model_relations');
    # 引用数据源服务
    Jtita::importInterface ('kernels.datasources.datasource');
    # 引用验证服务
    Jtita::importInterface ('kernels.validations.validation');

    /**
     * 模型异常，当模型发生错误时将引发本异常
     *
     * @package jtita.models
     */
    class ModelException extends JtitaException{
    }

    /**
     * 模型未打开异常，当指定的模型没有找到时引发本异常
     *
     * @package jtita.models
     */
    class ModelNotFoundException extends ModelException{
    }

    /**
     * 模型使用ActiveRecord模式设置的对象模型类
     *
     * @package jtita.models
     */
    abstract class Model implements ArrayAccess{
        /**
         * @const string 默认使用的数据库名称
         */
        const DEFAULT_DBNAME = 'default';

        /**
         * @const string 错误基础常量
         */
        const ERROR_BASE = 'base';

        /**
         * @const int 模型创建状态
         */
        const STATUS_NEW = 1;

        /**
         * @const int 模型更新状态
         */
        const STATUS_UPDATE = 2;

        /**
         * @const int 模型删除状态
         */
        const STATUS_DELETE = 3;

        /**
         * @const string 模型一对一关系
         */
        const HAS_ONE = 'hasOne';

        /**
         * @const string 模型一对多关系
         */
        const HAS_MANY = 'hasMany';

        /**
         * @const string 模型多对一关系
         */
        const BELONGS_TO = 'belongsTo';

        /**
         * @const string 模型多对多关系
         */
        const HAS_AND_BELONGS_TO = 'hasAndBelongsTo';

        /**
         * @var array 模型的原始属性（初始化属性），保存对象的原始属性值
         */
        protected $_originalProps = array();

        /**
         * @var array 模型的当前属性，保存对象的当前属性值
         */
        protected $_currentProps = array();

        /**
         * @var int 模型的当前状态，默认为更新状态
         */
        protected $_status = self::STATUS_NEW;

        /**
         * @var array 当前模型的错误信息
         */
        protected $_errorMessages = array();

        /**
         * @var array[ModelRelation] 模型关系对象集合，
         */
        protected static $_modelRelations = array();

        /**
         * @var array 关系SCHEMA 信息
         */
        protected static $_relationSchemas = array();

        /**
         * 根据$prop、$forceUpdate和$forceLoad初始化模型实例
         *
         * @param array|int $props       属性值
         * @param boolean   $forceUpdate 强制更新，默认为false。
         * @param boolean   $forceLoad   强制加载，默认为false。
         *
         * @return Model
         */
        public function __construct ($props = array(), $forceUpdate = false, $forceLoad = false) {
            # 如果不是数组，则直接表示根据KEY获取当前对象；
            if (! is_array ($props)){
                $this->_originalProps = $this->_read ($props);
                $this->_currentProps = $this->_originalProps;
                $this->_status = self::STATUS_UPDATE;
            } else if ($forceUpdate && $forceLoad && isset($props[static::dataKey ()])){
                $this->_originalProps = $this->_read ($props[static::dataKey ()]);
                $this->_currentProps = array_merge ($this->_originalProps, $props);
                $this->_status = self::STATUS_UPDATE;
            } else{
                $this->_currentProps = $props;
                $this->_originalProps = $props;

                # 如果是强制更新，将状态设置为更新状态；
                $this->_status = $forceUpdate ? self::STATUS_UPDATE : self::STATUS_NEW;
            }
        }

        /**
         * 实现PHP __get魔术方法，实现对当前属性的访问，如果属性值不存在则返回null值
         *
         * @param string $prop 属性名字
         *
         * @return mixed|null
         */
        public function __get ($prop) {
            if (isset($this->_currentProps[$prop])){
                return $this->_currentProps[$prop];
            } else if (array_key_exists ($prop, static::dataRelations ())){
                $this->_currentProps[$prop] = self::_createRelationProps ($this, self::relationSchemas ($prop));
                return $this->_currentProps[$prop];
            } else return null;
        }

        /**
         * 实现PHP __set魔术操作，实现对模型当前属性值的设置
         *
         * TODO: 未实现对关系属性对象的设置
         *
         * @param string $prop  属性名字
         * @param mixed  $value 属性值
         *
         * @return void
         */
        public function __set ($prop, $value) {
            $this->_currentProps[$prop] = $value;
        }

        /**
         * 使用ArrayAccess方式访问属性值，返回是否包含$offset
         *
         * @param string $offset 判断属性名称
         *
         * @return boolean
         */
        public function offsetExists ($offset) {
            return isset($this->_currentProps[$offset]);
        }

        /**
         * 使用ArrayAccess方式访问属性值，获取指定$offset指定值
         *
         * @param string $offset 属性名称
         *
         * @return mixed
         */
        public function offsetGet ($offset) {
            return isset($this->_currentProps[$offset]) ? $this->_currentProps[$offset] : null;
        }

        /**
         * 使用ArrayAccess方式访问属性值，为$offset指定值设置新的值
         *
         * @param string $offset 属性名称
         * @param mixed  $value  属性值
         *
         * @return void
         */
        public function offsetSet ($offset, $value) {
            $this->_currentProps[$offset] = $value;
        }

        /**
         * 使用ArrayAccess方式访问属性值，设置$offset指定值为空值
         *
         * @param string $offset 属性名称
         *
         * @return void
         */
        public function offsetUnset ($offset) {
            unset($this->_currentProps[$offset]);
        }

        /**
         * 返回当前模型的KEY值，如果KEY值不存将返回null
         *
         * @return mixed|null
         */
        public function key () {
            return $this->{static::dataKey ()};
        }

        /**
         * 获取$prop属性的初始值，$prop不存将返回null值
         *
         * @param string $prop 属性名字
         *
         * @return mixed
         */
        public function originalProp ($prop) {
            if (isset($this->_originalProps[$prop])){
                return $this->_originalProps[$prop];
            } else return null;
        }

        /**
         * 使用$props批量更新模型属性值
         *
         * @param array $props 更新属性值
         *
         * @return void
         */
        public function changeProps ($props) {
            $this->_currentProps = array($this->_currentProps, $props);
        }

        /**
         * 返回一个布尔值，表示当前状态是否为新建状态。
         *
         * @return boolean
         */
        public function isNew () {
            return $this->_status == self::STATUS_NEW;
        }

        /**
         * 返回一个布尔值，表示当前状态是否为更新状态
         *
         * @return boolean
         */
        public function isUpdate () {
            return $this->_status == self::STATUS_UPDATE;
        }

        /**
         * 返回一个布尔值，表示当前状态是否为删除状态
         *
         * @return boolean
         */
        public function isDelete () {
            return $this->_status == self::STATUS_DELETE;
        }


        /**
         * 如果$original为true将返回原始数据的json格式数据，否则返回当前数据的json格式数据。
         *
         * @param boolean $original 原始属性
         *
         * @return string
         */
        public function json ($original = false) {
            if ($original){
                return json_encode ($this->_originalProps);
            } else{
                return json_encode ($this->_currentProps);
            }
        }

        /**
         * 返回当前数据的xml格式数据；
         *
        public function xml() {

        } */


        /**
         * 执行验证，验证当前模型是否满足创建或更新条件，验证成功返回true，失败返回false。
         *
         * @return boolean
         */
        public function validate () {
            # 如果当前状态为删除，直接返回false值。
            if ($this->isDelete ()){
                $this->clearErrors ('Can\'t jquery.validate delete model');
                return false;
            }

            # 根据状态获取验证方法
            $on = $this->isNew () ? 'create' : 'update';

            # 执行当前对象的验证方法
            if (($errors = static::_validate ($on, $this)) !== true){
                $this->clearErrors ($errors);
                return false;
            }

            return true;
        }

        /**
         * 根据$syntax和$options将实例从数据源中删除，删除成功返回true，失败返回false。
         *
         * @param array $syntax   额外语法需求；
         * @param array $options  删除选项；
         *
         * @return boolean
         */
        public function delete ($syntax = array(), $options = array()) {
            # 如果当前对象不是更新状态，返回删除失败；
            if (! $this->isUpdate ()){
                $this->clearErrors ('Can\'t delete not exists model');
                return false;
            }

            # 生成语法；
            $syntax = self::_margeSyntax ($syntax);
            $options = self::_mergeOptions ($syntax, $options);

            # 执行删除操作
            return $this->_delete ($syntax, $options);
        }

        /**
         * 根据$syntax和$options将数据更新回数据库，更新成功返回true，失败返回false。
         *
         * @param array $syntax  额外的语法支持
         * @param array $options 更新选项
         *
         * @return boolean
         */
        public function save ($syntax = array(), $options = array()) {
            # 如果当前状态为删除状态
            if ($this->isDelete ()){
                $this->clearErrors ('Can\'t save delete model');
                return false;
            }

            # 如果验证失败返回false。
            if (! $this->validate ()) return false;

            # 声明语法结构；
            $syntax = self::_margeSyntax ($syntax);
            $options = self::_mergeOptions ($syntax, $options);

            # 执行更新方法；
            if ($this->isNew ()){
                return $this->_create ($syntax, $options);
            } else{
                return $this->_update ($syntax, $options);
            }
        }

        /**
         * 取消全部的更新操作，返回初始化状态或上一次save的状态
         *
         * @return void
         */
        public function cancel () {
            $this->_currentProps = $this->_originalProps;
        }

        /**
         * 设置或获取模型的错误信息
         *
         * <ul>
         *      <li>如果$propName为空，则直接返回所有错误消息集合。</li>
         *      <li>如果$errorMessage为空，则表示返回$propName指定的错误消息。</li>
         *      <li>如果$errorMessage为非空值，则增加$errorMessage的错误值到错误字段。</li>
         * </ul>
         *
         * @param string       $propName      错误属性；
         * @param string|array $errorMessages 错误信息；
         *
         * @return array|boolean 如果指定字段错误消息不存在则返回false。
         */
        public function errors ($propName = null, $errorMessages = null) {
            # 如果是读取错误信息；
            if (is_null ($propName)){
                return $this->_errorMessages;
            }

            # 如果设置有错误值
            if (! is_null ($errorMessages)){
                # 如果指定属性未已经包含错误信息；
                if (! isset($this->_errorMessages[$propName])){
                    $this->_errorMessages[$propName] = array();
                }

                # 如果是数组错误信息；
                if (is_array ($errorMessages)){
                    $this->_errorMessages[$propName] = array_merge ($this->_errorMessages[$propName], $errorMessages);
                } else{
                    array_push ($this->_errorMessages[$propName], $errorMessages);
                }

            }

            return isset($this->_errorMessages[$propName]) ? $this->_errorMessages[$propName] : false;
        }

        /**
         * 使用$errors重新清除模型的错误消息。
         *
         * @param mixed $errors 新的错误信息
         *
         * @return Model
         */
        public function clearErrors ($errors = array()) {
            # 如果初始化错误是为数组；
            if (is_array ($errors)){
                $this->_errorMessages = $errors;
            } else{
                $this->_errorMessages = array(self::ERROR_BASE => array($errors));
            }

            return $this;
        }

        /**
         * 如果有主键值表示更新，否则表示创建模型。
         *
         * @param array   $props     属性集;
         * @param boolean $forceLoad 强制加载,默认为false。
         *
         * @return Model
         */
        public static function createForUpdate ($props, $forceLoad = false) {
            # 如果主键的值为空值。
            $forceUpdate = true;
            if (empty($props[static::dataKey ()])){
                unset($props[static::dataKey ()]);
                $forceUpdate = false;
            }

            return new static($props, $forceUpdate, $forceLoad);
        }

        /**
         * 根据$rows和$options创建对象模型集合并返回
         *
         * @param array $rows    创建行记录
         * @param array $options 创建选项
         *
         * @return array
         */
        public static function createModels ($rows, $options = array()) {
            $relationModels = self::_importRelationModels ($options);
            $modelName = get_called_class ();
            $models = array();

            # 生成并添加记录到模型集合中；
            foreach ($rows as $row){
                # 处理关系模型
                foreach ($relationModels as $relationName => $relationModel){
                    # 获取关系类名和别名
                    list($relationModelName, $aliasName) = $relationModel;

                    if (isset($row[$aliasName])){
                        $row[$modelName][$relationName] = new $relationModelName($row[$aliasName], true);
                    }
                }

                array_push ($models, new $modelName($row[$modelName], true));
            }

            return $models;
        }

        /**
         * 根据$relationName和$modelRelation注册模型关系对象
         *
         * @param string        $relationName  模型名字
         * @param ModelRelation $modelRelation 模型关系对象
         *
         * @return void
         */
        public static function registerModelRelation ($relationName, $modelRelation) {
            self::$_modelRelations[$relationName] = $modelRelation;
        }

        /**
         * 根据$fields、$syntax和$options创建多个模型对象；
         *
         * @param mixed $fields  创建字段；
         * @param array $syntax  额外的语法支持；
         * @param array $options 选项；
         *
         * @return int 影响到的行；
         */
        public static function createAll ($fields, $syntax = array(), $options = array()) {
            # 合并生成的语法和选项。
            $syntax = self::_margeSyntax ($syntax);
            $options = self::_mergeOptions ($syntax, $options);

            # 执行数据库创建操作
            return static::database ()->create ($fields, $syntax, $options);
        }

        /**
         * 根据$fields、$condition、$syntax和$options更新多条数据模型；
         *
         * @param array $fields     更新字段
         * @param array $conditions 更新条件
         * @param array $syntax     额外的语法支持；
         * @param array $options    更新选项支持；
         *
         * @return int 影响到的行；
         */
        public static function updateAll ($fields, $conditions, $syntax = array(), $options = array()) {
            # 如果条件是数字；
            if (is_numeric ($conditions)){
                $conditions = array(static::dataKey () => $conditions);
            }

            # 合并生成的语法和选项
            $syntax = self::_margeSyntax (array('where' => $conditions), $syntax);
            $options = self::_mergeOptions ($syntax, $options);

            # 执行数据库更新操作
            return static::database ()->update ($fields, $syntax, $options);
        }

        /**
         * 根据$condition、$syntax和$options删除数据对象。
         *
         * @param mixed $conditions 删除条件
         * @param array $syntax     额外的语法
         * @param array $options    删除选项
         *
         * @return int 影响到的行数量
         */
        public static function deleteAll ($conditions = array(), $syntax = array(), $options = array()) {
            # 如果条件是数字；
            if (is_numeric ($conditions)){
                $conditions = array(static::dataKey () => $conditions);
            }

            # 合并执行语法和选项
            $syntax = self::_margeSyntax (array('where' => $conditions), $syntax);
            $options = self::_mergeOptions ($syntax, $options);

            # 执行数据库删除操作
            return static::database ()->delete ($syntax, $options);
        }

        /**
         * 根据$condition、$syntax和$options查找多个模型对象；
         *
         * @param array  $conditions 查询条件
         * @param array  $syntax     额外语法
         * @param  array $options    查询选项；
         *
         * @return ModelFinder
         */
        public static function findAll ($conditions = array(), $syntax = array(), $options = array()) {
            # 如果条件是数字；
            if (is_numeric ($conditions)){
                $conditions = array(static::dataKey () => $conditions);
            }

            # 合并执行语法和选项
            $syntax = self::_margeSyntax (array('where' => $conditions), $syntax);
            $options = self::_mergeOptions ($syntax, $options);

            # 返回模型查找服务
            return new ModelFinder(get_called_class (), $syntax, $options);
        }

        /**
         * 根据$condition、$syntax和$options查找单个数据模型对象实例，失败返回false。
         *
         * @param mixed $conditions 查找条件
         * @param array $syntax     额外的语法
         * @param array $options    查找选项
         *
         * @return Model|bool
         */
        public static function find ($conditions = array(), $syntax = array(), $options = array()) {
            # 如果条件是数字；
            if (is_numeric ($conditions)){
                $conditions = array(static::dataKey () => $conditions);
            }

            # 合并查找语法和选项
            $syntax = self::_margeSyntax (array('where' => $conditions, 'limit' => 1), $syntax);
            $options = self::_mergeOptions ($syntax, $options, array('format' => 'model'));

            # 执行数据库查找功能
            $rows = static::database ()->find ($syntax, $options);
            $models = self::createModels ($rows, $options);

            # 如果返回值为空，则返回false值。
            return empty($models) ? false : $models[0];
        }

        /**
         * 根据$conditions、$syntax和$options查找字段值。
         *
         * @param string $fields     字段信息
         * @param array  $conditions 条件
         * @param array  $syntax     语法
         * @param array  $options    选项
         *
         * @return mixed
         */
        public static function findFields ($fields, $conditions = array(), $syntax = array(), $options = array()) {
            $syntax = self::_margeSyntax (array('where' => $conditions, 'fields' => $fields), $syntax);
            return self::database ()->findFields ($syntax, $options);
        }

        /**
         * 根据$cmd、$args和$options执行自定义命令。
         *
         * @param string  $cmd        命令
         * @param array   $args       参数
         * @param array   $options    选项
         *
         * @return int
         */
        public static function execute ($cmd, $args = array(), $options = array()) {
            return static::database ()->execute ($cmd, $args, $options);
        }

        /**
         * 为多对多中间表分配和设置值。
         *
         * @param string $relationName       关系名
         * @param mixed  $key                主键值
         * @param array  $fkeys              外键主键值
         *
         * @return integer 影响的行。
         */
        public static function assignMultiple ($relationName, $key, $fkeys) {
            # 获取关系SCHEMA;
            $schema = self::relationSchemas ($relationName);

            # 执行移除角色和分配角色。
            if (empty($fkeys)){
                return self::execute ('DELETE FROM :relationTable WHERE :primaryKey = :key', array(':relationTable' => new NormalParameter($schema[ModelRelation::OPT_RELATION_TABLE]),
                                                                                                   ':primaryKey' => new NormalParameter($schema[ModelRelation::OPT_PRIMARY]),
                                                                                                   ':key' => new NormalParameter($key)));
            } else{
                $rows = self::execute ('DELETE FROM :relationTable WHERE :primaryKey = :key AND :foreignKey NOT IN(:fkey)', array(':relationTable' => new NormalParameter($schema[ModelRelation::OPT_RELATION_TABLE]),
                                                                                                                                  ':primaryKey' => new NormalParameter($schema[ModelRelation::OPT_PRIMARY]),
                                                                                                                                  ':foreignKey' => new NormalParameter($schema[ModelRelation::OPT_FOREIGN]),
                                                                                                                                  ':key' => $key,
                                                                                                                                  ':fkey' => new NormalParameter(implode (',', $fkeys))));

                $dbKeys = self::query ('SELECT :foreignKey,:primaryKey FROM :relationTable WHERE :primaryKey=:key', array(':foreignKey' => new NormalParameter($schema[ModelRelation::OPT_FOREIGN]),
                                                                                                                          ':primaryKey' => new NormalParameter($schema[ModelRelation::OPT_PRIMARY]),
                                                                                                                          ':relationTable' => new NormalParameter($schema[ModelRelation::OPT_RELATION_TABLE]),
                                                                                                                          ':key' => new NormalParameter($key)), array('format' => 'list'));

                $fkeys = array_diff ($fkeys, array_keys ($dbKeys));

                if (! empty($fkeys)){
                    # 生成SQL
                    $values = '';
                    foreach ($fkeys as $fkey){
                        $values .= "($key,$fkey),";
                    }

                    # 执行加入工作
                    $rows += self::execute ('INSERT INTO :relationTable(:primaryKey,:foreignKey) VALUES :values', array(':relationTable' => new NormalParameter($schema[ModelRelation::OPT_RELATION_TABLE]),
                                                                                                                        ':primaryKey' => new NormalParameter($schema[ModelRelation::OPT_PRIMARY]),
                                                                                                                        ':foreignKey' => new NormalParameter($schema[ModelRelation::OPT_FOREIGN]),
                                                                                                                        ':values' => new NormalParameter(substr ($values, 0, - 1))));
                }

                return $rows;
            }
        }

        /**
         * 根据$cmd、$args和$options执行自定义查询。
         *
         * @param string $cmd     命令
         * @param array  $args    参数
         * @param array  $options 选项
         *
         * @return mixed
         */
        public static function query ($cmd, $args = array(), $options = array()) {
            return static::database ()->query ($cmd, $args, $options);
        }

        /**
         * 根据$field返回数据模式，如果$field为空，返回全部schema信息
         *
         * @param string $field 字段名称
         *
         * @return array|boolean
         */
        public static function dataSchemas ($field = null) {
            # 从数据库中获取SCHEMA信息。
            $schemas = static::database ()->schema (static::dataName ());

            # 如果未指定字段，返回全部；
            if (! isset($field)) return $schemas;

            # 返回时指定字段不存在则返回false；
            return isset($schemas[$field]) ? $schemas[$field] : false;
        }

        /**
         * 根据$relation返回数据的关系模型，如果$relationName为空，返回全部schema信息
         *
         * @param string $relationName 关系名称
         *
         * @return array|boolean
         */
        public static function relationSchemas ($relationName = null) {
            # 获取模型信息
            $modelName = get_called_class ();

            # 如果关系SCHEMA为空
            if (! isset(self::$_relationSchemas[$modelName])){
                self::$_relationSchemas[$modelName] = self::_createRelationSchemas ();
            }

            # 获取当前模型关系schemas
            $schemas = self::$_relationSchemas[$modelName];

            # 如果获取全部的关系
            if (is_null ($relationName)) return $schemas;
            # 返回指定的关系
            return isset($schemas[$relationName]) ? $schemas[$relationName] : false;
        }

        /**
         * 获取数据关系定义集
         *
         * @return array
         */
        public static function dataRelations () {
            return array();
        }

        /**
         * 获取当前模型的数据库实例,可根据需要在子类中重写方法。
         *
         * @return Database
         */
        public static function database () {
            return Datasource::database (self::DEFAULT_DBNAME);
        }

        /**
         * 获取数据在数据源中的命名标识，可根据需要在子类中重写方法。
         *
         * @return string
         */
        public static function dataName () {
            return camel_lower (get_called_class ());
        }

        /**
         * 获取数据在数据源中的别名标识，可根据需要在子类中重写方法。
         *
         * @return string
         */
        public static function dataAlias () {
            return get_called_class ();
        }

        /**
         * 获取数据的主键标识，可根据需要在子类中重写方法。
         *
         * @return string 默认为id
         */
        public static function dataKey () {
            return 'id';
        }

        /**
         * 获取一个布值，表示主键标识是否为自动标识，可根据需要在子类中重写。
         *
         * @return boolean 默认返回true
         */
        public static function autoKey () {
            return true;
        }

        /**
         * 返回只读数据；
         */
        public static function readProps () {
            return array();
        }

        /*
         * 根据$syntax和$options添加数据到数据库中；
         *
         * @param array $syntax 额外的语法支持；
         * @param array $options 创建选项；
         * @return boolean
         */
        protected function _create ($syntax, $options) {
            # 去除ID设置
            unset($this->_currentProps[static::dataKey ()]);

            # 执行创建过程；
            if ($this->_createBefore ($syntax, $options) !== false){
                if ($this->_createInvoke ($syntax, $options) !== false){
                    $this->_createAfter ($syntax, $options);
                    $this->_status = self::STATUS_UPDATE;
                    return true;
                }
            }

            return false;
        }

        /*
         * 根据$syntax和$options更新数据到数据库中；
         *
         * @param array $syntax 额外的语法支持；
         * @param array $options 创建选项；
         * @return boolean
         */
        protected function _update ($syntax, $options) {
            # 执行更新过程；
            if ($this->_updateBefore ($syntax, $options) !== false){
                if ($this->_updateInvoke ($syntax, $options) !== false){
                    $this->_updateAfter ($syntax, $options);
                    # 更新原始值和状态符
                    $this->_originalProps = $this->_currentProps;
                    $this->_status = self::STATUS_UPDATE;

                    return true;
                }
            }

            return false;
        }

        /**
         * 根据$syntax和$options执行删除操作
         *
         * @param array $syntax  删除语法
         * @param array $options 删除选项
         *
         * @return boolean
         */
        protected function _delete ($syntax, $options) {
            # 执行删除工作；
            if ($this->_deleteBefore ($syntax, $options) !== false){
                if ($this->_deleteInvoke ($syntax, $options) !== false){
                    $this->_deleteAfter ($syntax, $options);
                    $this->_status = self::STATUS_DELETE;
                    return true;
                }
            }

            return false;
        }

        /**
         * 创建之前回调用方法，如果返回false则中止创建过程；
         *
         * @param array $syntax  创建语法
         * @param array $options 创建选项
         *
         * @return boolean
         */
        protected function _createBefore ($syntax, $options) {
            return true;
        }

        /**
         * 创建后调用方法
         *
         * @param array $syntax  创建语法
         * @param array $options 创建选项
         *
         * @return void
         */
        protected function _createAfter ($syntax, $options) {
        }

        /**
         * 执行创建过程，如果返回false则表示创建失败
         *
         * @param array $syntax  创建语法
         * @param array $options 创建选项
         *
         * @return boolean
         */
        protected function _createInvoke ($syntax, $options) {
            # 执行更新操作；
            static::database ()->create ($this->_currentProps, $syntax, $options);

            # 如果是自动KEY，则获取最新生成的ID值
            if (static::autoKey ()){
                $this->_currentProps[static::dataKey ()] = static::database ()->createID ();
            }

            return true;
        }

        /**
         * 更新之前回调用方法，返回false中止更新过程
         *
         * @param array $syntax  更新语法
         * @param array $options 更新选项
         *
         * @return boolean
         */
        protected function _updateBefore ($syntax, $options) {
            return true;
        }

        /**
         * 更新之后回调用方法；
         *
         * @param array $syntax  更新语法
         * @param array $options 更新选项
         *
         * @return void
         */
        protected function _updateAfter ($syntax, $options) {
        }

        /**
         * 根据$syntax和$options执行更新服务
         *
         * @param array $syntax  更新语法
         * @param array $options 更新选项
         *
         * @return boolean
         */
        protected function _updateInvoke ($syntax, $options) {
            # 如果有指定的额外条件，并且条件为数组参数
            $syntax['where'] = $this->_margeKeyWhere ($syntax);

            # 处理更新字段；
            $readProps = static::readProps ();
            if (! empty($readProps)){
                $syntax['fields'] = $readProps;
                $options['except'] = true;
            }

            return static::database ()->update ($this->_currentProps, $syntax, $options);
        }

        /**
         * 删除之前回调方法，返回false中止删除过程
         *
         * @param array $syntax  删除语法
         * @param array $options 删除选项
         *
         * @return boolean
         */
        protected function _deleteBefore ($syntax, $options) {
            return true;
        }

        /**
         * 删除之后调用方法
         *
         * @param array $syntax  删除语法
         * @param array $options 删除选项
         *
         * @return void
         */
        protected function _deleteAfter ($syntax, $options) {
        }

        /**
         * 根据$syntax和$options执行删除方法
         *
         * @param array $syntax  删除语法
         * @param array $options 删除选项
         *
         * @return boolean
         */
        protected function _deleteInvoke ($syntax, $options) {
            # 合并where条件
            $syntax['where'] = $this->_margeKeyWhere ($syntax);

            # 执行删除功能
            return static::database ()->delete ($syntax, $options);
        }

        /**
         * 根据$id读取模型的属性值
         *
         * @param int $id 模型编号
         *
         * @return array
         * @throws ModelNotFoundException
         */
        protected function _read ($id) {
            # 合并语法参数
            $syntax = self::_margeSyntax (array('where' => array(static::dataKey () => $id)));

            # 执行查找方法
            $rows = static::database ()->find ($syntax);
            if (! empty($rows)){
                return $rows[0];
            }

            throw new ModelNotFoundException('Not found ' . static::dataAlias () . ' model:' . $id);
        }

        /**
         * 根据$syntax合并主键WHERE条件语法，并返回合并后的where语法。
         *
         * @param array $syntax 命令语法
         *
         * @return array WHERE 条件
         */
        protected function _margeKeyWhere ($syntax) {
            # 如果语法中包含where条件，并且条件为数组条件
            if (isset($syntax['where']) && is_array ($syntax['where'])){
                return array_merge ($syntax['where'], array(static::dataKey () => $this->key ()));
            } else{
                return array(static::dataKey () => $this->key ());
            }
        }

        /**
         * 使用$syntax生成数据库语法；
         */
        protected static function _margeSyntax ($syntax = array(), $syntax1 = array()) {
            # 如果没有设置当前别名
            if (! isset($syntax['alias'])){
                $syntax['alias'] = static::dataAlias ();
            }
            # 合并表名
            $syntax['table'] = static::dataName ();

            return array_merge ($syntax1, $syntax);
        }

        /**
         * 根据$syntax、$options和$exOptions对选项进行常规处理；
         *
         * @param array $syntax    语法
         * @param array $options   语法选项
         * @param array $exOptions 扩展选项
         *
         * @return array
         */
        protected static function _mergeOptions ($syntax, $options, $exOptions = array()) {
            # 如果语法中包括联接表信息；
            if (isset($syntax['joins'])){
                $options['relation'] = static::relationSchemas ();
            }

            # 返回选项；
            return array_merge ($options, $exOptions);
        }

        /**
         * 根据$options导入关系模型对应关系
         *
         * @param array $options
         *
         * @return array
         */
        protected static function _importRelationModels ($options) {
            # 声明关系模型数组
            $relationModels = array();

            # 如果选项包含关系声明
            if (! empty($options['relation'])){
                # 处理模型中的关系
                foreach ($options['relation'] as $relationName => $relationOptions){
                    # 获取关联类的文件名和类名
                    $modelName = Jtita::importInterface ($relationOptions['className']);

                    $aliasName = array_kvalue ($relationOptions, 'alias', $modelName);
                    $relationModels[$relationName] = array($modelName, $aliasName);
                }
            }

            return $relationModels;
        }

        /**
         * 根据配置创建模型关系对象；
         */
        protected static function _createRelationProps ($model, $config) {
            # 获取模型关系
            $type = array_kvalue ($config, 'type', self::HAS_ONE);

            # 如果指定对象模型未找到
            if (empty(self::$_modelRelations[$type])){
                throw new ModelRelationException('Not support model relation:' . $type);
            }

            # 如果指定映射关系未创建，则创建指定映射对象；
            return self::$_modelRelations[$type]->load ($model, $config);
        }

        /**
         * 创建关系关系的schemas信息
         *
         * @return array
         * @throws ModelRelationException;
         */
        protected static function _createRelationSchemas () {
            # 获取数据关系
            $options = static::dataRelations ();
            $schemas = array();

            # 获取关系名称和选项
            foreach ($options as $name => $config){
                # 获取模型关系
                $type = array_kvalue ($config, 'type', self::HAS_ONE);

                # 如果指定对象模型未找到
                if (empty(self::$_modelRelations[$type])){
                    throw new ModelRelationException('Not support model relation:' . $type);
                }

                $schemas[$name] = self::$_modelRelations[$type]->schema (get_called_class (), $config);
            }

            return $schemas;
        }

        /**
         * 根据$on和$data执行验证服务，成功返回true，失败返回错误数组。
         *
         * @param string                  $on   验证目标方法
         * @param ArrayAccess|array|Model $data 需要验证的数据
         *
         * @return array|boolean
         */
        protected static function _validate ($on, $data) {
            # 执行验证前处理
            if (($messages = static::_validateBefore ($on, $data)) !== true){
                return $messages;
            }

            # 执行验证处理
            if (($messages = Validation::validate ($data, static::_validateRules ($on), static::_validateMessages ($on))) !== true){
                return $messages;
            }

            # 执行验证后处理
            if (($messages = static::_validateAfter ($on, $data)) !== true){
                return $messages;
            }

            return true;
        }

        /**
         * 根据$on返回验证规则集合，可根据实际需要在子类中重写
         *
         * @param string $on 验证目标方法
         *
         * @return array
         */
        protected static function _validateRules ($on) {
            return array();
        }

        /**
         * 根据$on返回验证错误消息，可根据实际需要在子类中重写
         *
         * @param string $on 验证目标方法
         *
         * @return array
         */
        protected static function _validateMessages ($on) {
            return array();
        }

        /**
         * 根据$on、$data和$message执行验证前的操作，验证成功返回true，失败返回错误数组,可根据需要重写本方法。
         *
         * @param string $on   验证目标方法
         * @param array  $data 验证目标数据
         *
         * @return boolean|array
         */
        protected static function _validateAfter ($on, $data) {
            return true;
        }

        /**
         * 根据$on、$data和$message执行验证后操作，验证成功返回true，失败返回错误数组，可根据需要重写本方法。
         *
         * @param string $on   验证目标方法
         * @param array  $data 验证目标数据
         *
         * @return boolean|array
         */
        protected static function _validateBefore ($on, $data) {
            return true;
        }
    }

    # 注册模型关系对象
    Model::registerModelRelation (Model::HAS_ONE, new HasOneModelRelation());
    Model::registerModelRelation (Model::HAS_MANY, new HasManyModelRelation());
    Model::registerModelRelation (Model::BELONGS_TO, new BelongsToModelRelation());
    Model::registerModelRelation (Model::HAS_AND_BELONGS_TO, new HasAndBelongsToModelRelation());