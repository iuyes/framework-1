<?php
    /**
     * model_test.inc 提供模型的单元测试类声明
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.models
     * @version     1.0 beta
     *
     * @create      2012-08-20
     * @modify      2013-04-28
     */

    # 声明测试根节点
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (__FILE__))))));

    # 声明测试引用类
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');

    # 注册引用路径
    Jtita::paths (JTPATH_ROOT . '/jtita');

    # 重写数据源配设置信息
    Jtita::write ('kernels/datasources', array(
                                              'default' => array(
                                                  'class' => 'kernels.datasources.databases.mysql_fast',
                                                  'server' => '127.0.0.1',
                                                  'user' => 'root',
                                                  'dbpass' => 'killer',
                                                  'initialize' => true,
                                                  'dbname' => 'jtfix'
                                              ),
                                              'ex_syntax' => array('class' => 'database_mock')
                                         ));

    # 引用被测试类
    require_once(JTPATH_ROOT . '/jtita/models/model.inc');

    /**
     * 用户类，提供测试所需的模型类。
     *
     * @package jtita.models
     */
    class User extends Model {
    }

    /**
     * 用户组类，提供测试所需的模型类。
     */
    class UserGroup extends Model {
        public static function databaseName () {
            return 'securities';
        }

        public static function autoKey () {
            return false;
        }
    }

    /**
     * 用户权限类，提供测试所需的模型类
     *
     * @package jtita.models
     */
    class UserRight extends Model {
        public static function dataName () {
            return 'user_rights';
        }

        public static function dataKey () {
            return 'rid';
        }

        public static function autoKey () {
            return false;
        }
    }

    /**
     * 会员类，提供测试所需的模型类。
     *
     * @package jtita.models
     */
    class Member extends Model {
    }

    class DatabaseMock implements Database {
        public static $config = array();

        public static $createData = array();
        public static $createSyntax = array();
        public static $createOptions = array();

        public static $deleteSyntax = array();
        public static $deleteOptions = array();

        public static $findSyntax = array();
        public static $findOptions = array();

        public static $findCountSyntax = array();
        public static $findCountOptions = array();

        public static $updateData = array();
        public static $updateSyntax = array();
        public static $updateOptions = array();

        function initialize ($config = array()) { self::$config = $config; }

        function initialized () { return true; }

        function schema ($dataName) { return array(); }

        function create ($data, $syntax, $options = array()) {
            self::$createData = $data;
            self::$createSyntax = $syntax;
            self::$createOptions = $options;
        }

        function createID () { }

        function update ($data, $syntax, $options = array()) {
            self::$updateData = $data;
            self::$updateSyntax = $syntax;
            self::$updateOptions = $options;
        }

        function delete ($syntax, $options = array()) {
            self::$deleteSyntax = $syntax;
            self::$deleteOptions = $options;
        }

        function find ($syntax, $options = array()) {
            self::$findSyntax = $syntax;
            self::$findOptions = $options;

            return array();
        }

        function findSyntax () {

        }

        function query ($cmd, $args = array(), $options = array()) { }

        function execute ($cmd, $args = array(), $options = array()) { }

        function findFields ($syntax, $options = array()) {}
    }

    /**
     * 扩展语法模型类，提供测试所需的模型类。
     *
     * @package jtita.models
     */
    class Exsyntax extends Model {
        public static function database () { return Datasource::database ('ex_syntax'); }
    }

    /**
     * 用户类，提供测试所需的模型类。
     *
     * @package jtita.models
     */
    class UserR extends Model {
        public static function dataName () { return 'user'; }

        public static function dataRelations () {
            return array(
                'group' => array(
                    'type' => 'belongsTo',
                    'className' => 'group_r',
                    'alias' => 'Group',
                    'foreignKey' => 'user_group_id'
                )
            );
        }

        public function getGroup () {
            return $this->_currentProps['group'];
        }
    }

    /**
     * 用户组类，提供测试所需的模型类
     *
     * @package jtita.models
     */
    class GroupR extends Model {
        public static function dataName () { return 'user_group'; }

        public static function dataRelations () {
            return array(
                'users' => array(
                    'type' => 'hasMany',
                    'className' => 'user_r',
                    'foreignKey' => 'user_group_id'
                ),
                'last' => array(
                    'type' => 'hasOne',
                    'className' => 'user_r',
                    'foreignKey' => 'user_group_id',
                    'order' => 'UserR.id desc'
                )
            );
        }
    }

    /**
     * 关系测试模拟类
     */

    class MaxGroup extends Model {

        public static function dataRelations () {
            return array(
                'users' => array(
                    'type' => 'hasMany',
                    'className' => 'user_r',
                    'foreignKey' => 'user_group_id'
                ),
                'first' => array(
                    'type' => 'hasOne',
                    'className' => 'user_r',
                    'foreignKey' => 'user_group_id'
                ),
                'last' => array(
                    'type' => 'hasOne',
                    'className' => 'user_r',
                    'foreignKey' => 'user_group_id',
                    'order' => 'UserR.id desc'
                )
            );
        }

    }


    /**
     * 回调用模拟测试服务类，提供测试所需的服务
     *
     * @package jtita.models
     */
    class CallbackMock extends Model {
        public $createBefore = false;
        public $createInvoke = false;
        public $createAfter = false;

        public $updateBefore = false;
        public $updateInvoke = false;
        public $updateAfter = false;

        public $deleteBefore = false;
        public $deleteInvoke = false;
        public $deleteAfter = false;

        public $createBeforeReturn = true;
        public $createInvokeReturn = true;
        public $createAfterReturn = true;

        public $updateBeforeReturn = true;
        public $updateInvokeReturn = true;
        public $updateAfterReturn = true;

        public $deleteBeforeReturn = true;
        public $deleteInvokeReturn = true;
        public $deleteAfterReturn = true;


        public function __construct ($new = false) {
            if ($new) $this->_status = self::STATUS_NEW;
            else $this->_status = self::STATUS_UPDATE;
        }

        public static function databaseName () {
            return 'exsyntax';
        }

        protected function _createBefore ($syntax, $options) {
            $this->createBefore = true;
            return $this->createBeforeReturn;
        }

        protected function _createInvoke ($syntax, $options) {
            $this->createInvoke = true;
            return $this->createInvokeReturn;
        }

        protected function _createAfter ($syntax, $options) {
            $this->createAfter = true;
            return $this->createAfterReturn;
        }

        protected function _updateBefore ($syntax, $options) {
            $this->updateBefore = true;
            return $this->updateBeforeReturn;
        }

        protected function _updateInvoke ($syntax, $options) {
            $this->updateInvoke = true;
            return $this->updateInvokeReturn;
        }

        protected function _updateAfter ($syntax, $options) {
            $this->updateAfter = true;
            return $this->updateAfterReturn;
        }

        protected function _deleteBefore ($syntax, $options) {
            $this->deleteBefore = true;
            return $this->deleteBeforeReturn;
        }

        protected function _deleteInvoke ($syntax, $options) {
            $this->deleteInvoke = true;
            return $this->deleteInvokeReturn;
        }

        protected function _deleteAfter ($syntax, $options) {
            $this->deleteAfter = true;
            return $this->deleteAfterReturn;
        }
    }

    /**
     * 模型验证模拟器，提供模型验证测试模拟服务
     *
     * @package jtita.models
     */
    class ModelValidateMock extends Model {

        /**
         * 执行模拟验证，白盒测试。
         */
        public static function validateMock ($on, $data) {
            return static::_validate ($on, $data);
        }

        /**
         * @see Model::_validateRules
         */
        public static function _validateRules ($on) {
            static $rules = array(
                'auth' => array(
                    'name' => 'required',
                    'password' => 'required'
                )
            );

            return isset($rules[$on]) ? $rules[$on] : array();
        }

        /**
         * @see Model::_validateMessages
         */
        public static function _validateMessages ($on) {
            static $errors = array('auth' => array('password' => 'Sorry password can\'t null'));

            return isset($errors[$on]) ? $errors[$on] : array();
        }

        /**
         * @see Model::_validateBefore
         */
        public static function _validateBefore ($on, $data) {
            if ($on == 'before') {
                if (! $data['before']) {
                    return array('errors');
                }
            }

            return true;
        }

        /**
         * @see Model::_validateAfter
         */
        public static function _validateAfter ($on, $data) {
            if ($on == 'after') {
                if (! $data['after']) {
                    return array('errors');
                }
            }

            return true;
        }
    }

    /**
     * 模型单元测试类，提供对Model类的单元测试服务
     *
     * @package jtita.models
     */
    class ModelTest extends PHPUnit_Framework_TestCase {
        /**
         * 测试获取数据名称
         */
        public function testDataName () {
            $this->assertEquals ('user', User::dataName ());
            $this->assertEquals ('User', User::dataAlias ());
        }

        /**
         * 测试数据命名重写
         */
        public function testDataNameForOverride () {
            $this->assertEquals ('user_rights', UserRight::dataName ());
            $this->assertEquals ('UserRight', UserRight::dataAlias ());
        }

        /**
         * 测试获取数据关键字
         */
        public function testDataKey () {
            $this->assertEquals ('id', User::dataKey ());
            $this->assertTrue (User::autoKey ());
        }

        /**
         * 测试重写DataKey方法。
         */
        public function testDataKeyForOverride () {
            $this->assertEquals ('rid', UserRight::dataKey ());
            $this->assertFalse (UserRight::autoKey ());
        }

        /**
         * 测试获取数据Schema值
         */
        public function testSchema () {
            $schemas = User::dataSchemas ();

            $this->assertTrue (is_array ($schemas));
            $this->assertEquals (5, count ($schemas));
        }

        /**
         * 测试获取数据$schema，数据在数据库中不存在
         *
         * @expectedException DatasourceException
         */
        public function testSchemaForNotExists () {
            Member::dataSchemas ();
        }

        /**
         * 测试获取字段schema值
         */
        public function testSchemaField () {
            $field = User::dataSchemas ('id');
            $this->assertTrue (is_array ($field));
        }

        /**
         * 测试获取字段不存在
         */
        public function testSchemaFieldForNotExists () {
            $this->assertFalse (User::dataSchemas ('not_exists'));
        }

        /**
         * 测试创建多个数据内容
         */
        public function testCreateAll () {
            User::database ()->execute ('delete FROM `user`');

            $count = User::createAll (array(
                                           'name' => 'zhenggp',
                                           'user_group_id' => 1,
                                           'desc' => 'hello jtita',
                                           'age' => 18
                                      ));
            $rows = User::database ()->query ('SELECT * FROM `user`');

            $this->assertEquals (1, $count);
            $this->assertEquals (1, count ($rows));
        }

        /**
         * 测试创建多个值
         */
        public function testCreateAllForMultiple () {
            User::database ()->execute ('delete FROM `user`');

            $count = User::createAll (array(
                                           array(
                                               'name' => 'zhenggp',
                                               'user_group_id' => 1,
                                               'desc' => 'hello jtita',
                                               'age' => 18
                                           ),
                                           array(
                                               'name' => 'Chen juan',
                                               'user_group_id' => 2,
                                               'desc' => 'hello jtita',
                                               'age' => 16
                                           )
                                      ), array(), array('multiple' => true));
            $rows = User::database ()->query ('SELECT * FROM `user`');

            $this->assertEquals (2, $count);
            $this->assertEquals (2, count ($rows));
        }

        /**
         * 测试创建多个值，使用额外语法和选项
         */
        public function testCreateAllForSyntaxAndOptions () {
            Exsyntax::createAll (array('name' => 'test'), array('exsy' => 'yes'), array('op' => 'opt'));

            $this->assertEmpty (array_diff_assoc (array('name' => 'test'), DatabaseMock::$createData));
            $this->assertEmpty (array_diff_assoc (array(
                                                       'exsy' => 'yes',
                                                       'table' => 'exsyntax',
                                                       'alias' => 'Exsyntax'
                                                  ), DatabaseMock::$createSyntax));
            $this->assertEmpty (array_diff_assoc (array('op' => 'opt'), DatabaseMock::$createOptions));
        }

        /**
         * 测试更新多个值
         */
        public function testUpdateAll () {
            $database = $this->_initUser ();

            $count = User::updateAll (array('name' => 'new test'), array());
            $rows = $database->query ('select * from `user` where `name` = \'new test\'');

            $this->assertEquals (2, $count);
            $this->assertEquals (2, count ($rows));
        }

        /**
         * 测试更新多个值，更新数组条件进行更新
         */
        public function testUpdateAllForArrayConditions () {
            $database = $this->_initUser ();

            $count = User::updateAll (array('name' => 'new test'), array('User.name' => 'zhenggp'));
            $rows = $database->query ('select * from `user` where `name` = \'new test\'');

            $this->assertEquals (1, $count);
            $this->assertEquals (1, count ($rows));
        }

        /**
         * 测试更新多个值，使用字符串条件
         */
        public function testUpdateAllForStringConditions () {
            $database = $this->_initUser ();

            $count = User::updateAll (array('name' => 'new test'), 'User.name = \'zhenggp\'');
            $rows = $database->query ('select * from `user` where `name` = \'new test\'');

            $this->assertEquals (1, $count);
            $this->assertEquals (1, count ($rows));
        }

        /**
         * 测试更新多个值，使用参数条件
         */
        public function testUpdateAllForArgsConditions () {
            $database = $this->_initUser ();

            $count = User::updateAll (array('name' => 'new test'), array(
                                                                        'User.name = :uname',
                                                                        ':uname' => 'zhenggp'
                                                                   ));
            $rows = $database->query ('select * from `user` where `name` = \'new test\'');

            $this->assertEquals (1, $count);
            $this->assertEquals (1, count ($rows));
        }

        /**
         * 测试更新多个值，使用KEY
         */
        public function testUpdateAllForKey () {
            $database = $this->_initUser ();

            # 查询出数据主键值
            $rows = $database->query ('SELECT id FROM `user` WHERE `name` = \'zhenggp\'');

            $count = User::updateAll (array('name' => 'new test'), $rows[0]['id']);
            $rows = $database->query ('select * from `user` where `name` = \'new test\'');

            $this->assertEquals (1, $count);
            $this->assertEquals (1, count ($rows));
        }

        /**
         * 测试更新多个值，使用已已重写的key命名
         */
        public function testUpdateAllForOverrideKey () {
            $database = $this->_initUserRight ();

            $count = UserRight::updateAll (array('name' => 'hello'), 1);
            $rows = $database->query ('SELECT * FROM `user_rights` WHERE `name` = \'hello\'');

            $this->assertEquals (1, $count);
            $this->assertEquals (1, count ($rows));
            $this->assertEquals (1, $rows[0]['rid']);
        }

        /**
         * 测试更新多什值，使用字符串数据
         *
         * @depends testUpdateAllForOverrideKey
         */
        public function testUpdateAllForStringData () {
            $database = $this->_initUser ();

            # 查询出关键字
            $rows = $database->query ('SELECT id FROM `user` WHERE `name` = \'zhenggp\'');

            $count = User::updateAll ('`name` = \'new test\'', $rows[0]['id']);
            $rows = $database->query ('select * from `user` where `name` = \'new test\'');

            $this->assertEquals (1, $count);
            $this->assertEquals (1, count ($rows));
        }

        /**
         * 测试更新多个值，使用参数参数数据
         *
         * @depends testUpdateAllForOverrideKey
         */
        public function testUpdateAllForArgsData () {
            $database = $this->_initUser ();

            # 查询获取KEY值
            $rows = $database->query ('SELECT id FROM `user` WHERE `name` = \'zhenggp\'');

            $count = User::updateAll (array(
                                           '`name` = :name',
                                           ':name' => 'new test'
                                      ), $rows[0]['id']);
            $rows = $database->query ('select * from `user` where `name` = \'new test\'');

            $this->assertEquals (1, $count);
            $this->assertEquals (1, count ($rows));
        }

        /**
         * 测试更新多个值，使用额外的语法和选项
         */
        public function testUpdateAllForSyntaxAndOptions () {
            Exsyntax::updateAll (array('name' => 'test'), array(), array('exsy' => 'yes'), array('op' => 'opt'));

            $this->assertEmpty (array_diff_assoc (array('name' => 'test'), DatabaseMock::$updateData));
            $this->assertEmpty (array_diff_assoc (array(
                                                       'exsy' => 'yes',
                                                       'table' => 'exsyntax',
                                                       'alias' => 'Exsyntax'
                                                  ), DatabaseMock::$updateSyntax));
            $this->assertEmpty (array_diff_assoc (array('op' => 'opt'), DatabaseMock::$updateOptions));
        }

        /**
         * 测试根据KEY删除多个数据
         */
        public function testDeleteAllForKey () {
            $database = $this->_initUser ();

            $rows = $database->query ('SELECT id FROM `user` where `name` = \'zhenggp\'');

            User::deleteAll ($rows[0]['id']);

            $rows = $database->query ('SELECT * FROM `user`');
            $this->assertEquals (1, count ($rows));
        }

        /**
         * 测试根据已重写的KEY删除数据
         */
        public function testDeleteAllForOverrideKey () {
            $database = $this->_initUserRight ();
            $count = UserRight::deleteAll (2);
            $rows = UserRight::database ()->query ('SELECT * FROM `user_rights`');

            $this->assertEquals (1, $count);
            $this->assertEquals (1, $rows[0]['rid']);
        }

        /**
         * 删除多个数据，根据数组条件
         */
        public function testDeleteAllForArrayConditions () {
            $database = $this->_initUser ();

            $count = User::deleteAll (array('User.name' => 'zhenggp'));
            $count1 = User::deleteAll (array(
                                            'User.name' => 'chen juan',
                                            'User.id' => 0
                                       ));
            $rows = $database->query ('SELECT * FROM `user`');

            $this->assertEquals (1, $count);
            $this->assertEquals (0, $count1);

            $this->assertEquals (1, count ($rows));
            $this->assertEquals ('Chen juan', $rows[0]['name']);
        }

        /**
         * 删除多个数据，根据字符串条件
         */
        public function testDeleteAllForStringConditions () {
            $database = $this->_initUser ();

            $count = User::deleteAll ('User.name = \'zhenggp\'');
            $count1 = User::deleteAll ('User.name = \'chenjuan\' AND User.id = 0');
            $rows = $database->query ('SELECT * FROM `user`');

            $this->assertEquals (1, $count);
            $this->assertEquals (0, $count1);

            $this->assertEquals (1, count ($rows));
            $this->assertEquals ('Chen juan', $rows[0]['name']);
        }

        /**
         * 删除多个数据，根据参数条件
         */
        public function testDeleteAllForArgsConditions () {
            $database = $this->_initUser ();

            $count = User::deleteAll (array(
                                           'User.name = :name',
                                           ':name' => 'zhenggp'
                                      ));
            $count1 = User::deleteAll (array(
                                            'User.name = :name AND User.id = :uid',
                                            ':name' => 'Chen juan',
                                            ':uid' => 0
                                       ));
            $rows = $database->query ('SELECT * FROM `user`');

            $this->assertEquals (1, $count);
            $this->assertEquals (0, $count1);

            $this->assertEquals (1, count ($rows));
            $this->assertEquals ('Chen juan', $rows[0]['name']);
        }

        /**
         * 删除多个数据，使用额外参数
         */
        public function testDeleteAllForExsyntaxAndOptions () {
            Exsyntax::deleteAll (array(), array('exsy' => 'yes'), array('op' => 'opt'));

            $this->assertEmpty (array_diff_assoc (array(
                                                       'exsy' => 'yes',
                                                       'table' => 'exsyntax',
                                                       'alias' => 'Exsyntax'
                                                  ), DatabaseMock::$deleteSyntax));
            $this->assertEmpty (array_diff_assoc (array('op' => 'opt'), DatabaseMock::$deleteOptions));
        }


        /**
         * 测试构造方法，使用数组参数构造实例
         */
        public function testConstructorForArrayProps () {
            $user = new User(array(
                                  'name' => 'zhenggp',
                                  'age' => 18,
                                  'desc' => 24,
                                  'id' => 12
                             ));

            $this->assertEquals ('zhenggp', $user->name);
            $this->assertEquals (18, $user->age);
            $this->assertEquals (24, $user->desc);

            $this->assertTrue ($user->isNew ());
        }

        /**
         * 使用_get获取不存在的属性值
         */
        public function testGetPropsForCtorNotExistsProps () {
            $user = new User();

            $this->assertNull ($user->notexists);
            $this->assertNull ($user->notex);

            $this->assertTrue ($user->isNew ());
        }

        /*
         * 使用构造方法，包含KEY值
         */
        public function testConstructorForKey () {
            $database = $this->_initUser ();

            # 查询数据主键
            $rows = $database->query ('SELECT id FROM `user` WHERE `name` = \'zhenggp\'');
            $user = new User($rows[0]['id']);

            $this->assertEquals ('zhenggp', $user->name);
            $this->assertEquals (18, $user->age);
            $this->assertEquals ('programmer', $user->desc);

            $this->assertTrue ($user->isUpdate ());
        }

        /**
         * 测试构造方法，使用重写KEY值
         */
        public function testConstructorForOverrideKey () {
            $database = $this->_initUserRight ();

            $right = new UserRight(1);

            $this->assertEquals ('zhenggp', $right->name);
            $this->assertEquals (1, $right->rid);
        }

        /**
         * 测试构造方法，使用对象未找到
         *
         * @expectedException ModelNotFoundException
         */
        public function testConstructorForKeyNotFound () {
            User::database ()->execute ('DELETE FROM `user`');

            new User(12);
        }

        /**
         * 测试构造方法，使用强制更新和加载功能
         */
        public function testConstructorForForceUpdateAndForceLoad () {
            $database = $this->_initUser ();

            # 查询数据主键
            $rows = $database->query ('SELECT id FROM `user` WHERE `name` = \'zhenggp\'');
            $user = new User(array(
                                  'name' => 'test',
                                  'age' => 30,
                                  'id' => $rows[0]['id']
                             ), true, true);

            $this->assertEquals ('programmer', $user->desc);
            $this->assertEquals (30, $user->age);
            $this->assertEquals ('test', $user->name);

            $this->assertEquals ('zhenggp', $user->originalProp ('name'));
            $this->assertEquals (18, $user->originalProp ('age'));

            $this->assertTrue ($user->isUpdate ());
        }

        /**
         * 测试构造方法，强制加载对象未找到。
         *
         * @expectedException ModelNotFoundException
         */
        public function testConstructorForForceLoadKeyNotFound () {
            User::database ()->execute ('DELETE FROM `user`');

            new User(array('id' => 12), true, true);
        }

        /**
         * 测试构造属性读取
         */
        public function testCtorProp () {
            $user = new User(array(
                                  'desc' => 'new desc',
                                  'name' => 'zhenggp'
                             ));

            $user->desc = 'new desc';
            $user->name = 'test';

            $this->assertEquals ('zhenggp', $user->originalProp ('name'));
            $this->assertEquals ('new desc', $user->originalProp ('desc'));
        }

        /**
         * 测试设置已存在的属性值
         */
        public function testSetForExistsProps () {
            $user = new User(array(
                                  'desc' => 'programmer',
                                  'name' => 'zhenggp'
                             ));

            $user->desc = 'manager';
            $user->name = 'Chen juan';

            $this->assertEquals ('manager', $user->desc);
            $this->assertEquals ('Chen juan', $user->name);
        }

        /**
         * 测试设置不存在的属性值
         */
        public function testSetForNotExistsProps () {
            $user = new User(array());

            $user->notexists = 'Hello world';
            $user->notexista = 12;

            $this->assertEquals ('Hello world', $user->notexists);
            $this->assertEquals (12, $user->notexista);
        }

        /**
         * 测试仪创建模型集
         */
        public function testCreateModels () {
            $users = User::createModels (array(
                                              array('User' => array('name' => 'zhenggp')),
                                              array('User' => array('name' => 'Chen juan'))
                                         ));

            $this->assertEquals (2, count ($users));
            $this->assertEquals ('zhenggp', $users[0]->name);
            $this->assertEquals ('Chen juan', $users[1]->name);

            $this->assertTrue ($users[0]->isUpdate ());
            $this->assertTrue ($users[1]->isUpdate ());
        }

        /**
         * 测试创建模型集，包含关系模型
         */
        public function testCreateModelsForRelation () {
            $users = User::createModels (array(
                                              array(
                                                  'User' => array('name' => 'zhenggp'),
                                                  'GroupR' => array('name' => 'admin')
                                              ),
                                              array(
                                                  'User' => array('name' => 'Chen juan'),
                                                  'GroupR' => array('name' => 'guest')
                                              )
                                         ), array('relation' => array('Group' => array('className' => 'GroupR'))));


            $this->assertEquals (2, count ($users));
            $this->assertEquals ('admin', $users[0]->Group->name);
            $this->assertEquals ('guest', $users[1]->Group->name);

            $this->assertTrue ($users[0]->Group->isUpdate ());
            $this->assertTrue ($users[1]->Group->isUpdate ());
        }

        /**
         * 测试查找方法，使用数组参数
         */
        public function testFindForArrayConditions () {
            $this->_initUser ();

            $user = User::find (array('name' => 'zhenggp'));

            $this->assertEquals ('zhenggp', $user->name);
            $this->assertEquals (18, $user->age);
        }

        /**
         * 测试查找方法，使用字符串条件
         */
        public function testFindForStringConditions () {
            $this->_initUser ();

            $user = User::find ('User.name = \'zhenggp\'');

            $this->assertEquals ('zhenggp', $user->name);
            $this->assertEquals (18, $user->age);
        }

        /**
         * 测试查找方法，使用参数条件
         */
        public function testFindForArgsConditions () {
            $this->_initUser ();

            $user = User::find (array(
                                     'User.name = :name',
                                     ':name' => 'zhenggp'
                                ));

            $this->assertEquals ('zhenggp', $user->name);
            $this->assertEquals (18, $user->age);
        }

        /**
         * 测试查找方法，使用KEY值条件
         */
        public function testFindForKeyConditions () {
            $database = $this->_initUser ();

            $rows = $database->query ('SELECT id FROM `user` WHERE `name` = \'zhenggp\'');
            $user = User::find ($rows[0]['id']);

            $this->assertEquals ('zhenggp', $user->name);
            $this->assertEquals (18, $user->age);
        }

        /**
         * 测试查找方法，结果有多个值
         */
        public function testFindForMany () {
            $database = $this->_initUser ();

            $user = User::find (array());

            $this->assertEquals ('zhenggp', $user->name);
            $this->assertEquals (18, $user->age);
        }

        /**
         * 测试查找方法，实例不存在
         */
        public function testFindForNotExists () {
            User::database ()->execute ('DELETE FROM `user`');

            $user = User::find (array());

            $this->assertFalse ($user);
        }

        /**
         * 测试查找，包含joins选项
         */
        public function testFindForSyncJoinsOptions () {
            $this->_initUser ();

            $user = UserR::find (array('UserR.name' => 'zhenggp'), array('joins' => array('group')));
            $group = $user->getGroup ();

            $this->assertNotEmpty ($group);
            $this->assertEquals ('GroupR', get_class ($group));
            $this->assertEquals ('admins', $group->name);
        }

        /**
         * 测试同步加载joins查找器
         */
        public function testFindForSyncJoinsFinder () {
            $this->_initUser ();

            $users = UserR::findAll ();

            $userModels = $users->joins ('group')->models ();

            $this->assertNotEmpty ($userModels[0]->getGroup ());
            $this->assertNotEmpty ($userModels[1]->getGroup ());
        }

        /**
         * 测试查找方法，使用额外的语法和选项
         */
        public function testFindForSyntaxAndOptions () {
            Exsyntax::find (array(), array('exs' => 'yes'), array('opt' => 'opv'));

            $this->assertEmpty (array_diff_assoc (array(
                                                       'exs' => 'yes',
                                                       'table' => 'exsyntax',
                                                       'alias' => 'Exsyntax'
                                                  ), DatabaseMock::$findSyntax));
            $this->assertEmpty (array_diff_assoc (array('opt' => 'opv'), DatabaseMock::$findOptions));
        }


        /**
         * 测试创建模型方法
         */
        public function testCreate () {
            User::database ()->execute ('DELETE FROM `user`');

            $user = new User(array(
                                  'name' => 'zhenggp',
                                  'age' => 30,
                                  'user_group_id' => 1
                             ));
            $this->assertTrue ($user->save ());

            $rows = User::database ()->query ('SELECT * FROM `user`');

            $this->assertEquals ('zhenggp', $rows[0]['name']);
            $this->assertEquals (30, $rows[0]['age']);
            $this->assertEquals (1, $rows[0]['user_group_id']);

            $this->assertEquals ($user->key (), $rows[0]['id']);
            $this->assertTrue ($user->isUpdate ());
        }

        /**
         * 测试更新模型方法，
         */
        public function testUpdate () {
            $database = $this->_initUser ();

            # 查询模型主键
            $rows = $database->query ('SELECT id FROM `user` WHERE `name` = \'zhenggp\'');
            $user = new User(array(
                                  'name' => 'test',
                                  'age' => 30,
                                  'id' => $rows[0]['id']
                             ), true);

            $this->assertTrue ($user->save ());
            $this->assertEmpty ($database->query ('SELECT * FROM `user` where `name` = \'zhenggp\''));

            $rows = $database->query ('SELECT * FROM `user` WHERE `id` = :id', array(':id' => $rows[0]['id']));

            $this->assertEquals ('test', $rows[0]['name']);
            $this->assertEquals (30, $rows[0]['age']);
        }

        /**
         * 测试删除模型方法
         */
        public function testDelete () {
            $database = $this->_initUser ();
            # 获取对象主键
            $rows = $database->query ('SELECT id FROM `user` WHERE `name` = \'zhenggp\'');
            $user = new User($rows[0]['id']);

            $this->assertTrue ($user->delete ());
            $this->assertEmpty ($database->query ('select * from `user` where id = :id', array(':id' => $rows[0]['id'])));
            $this->assertTrue ($user->isDelete ());
        }

        /**
         * 测试生成json数据
         */
        public function testJson () {
            $user = new User(array(
                                  'name' => 'zhenggp',
                                  'age' => 18
                             ));
            $user1 = new User(array(
                                   'name' => 'chen juan',
                                   'age' => 32
                              ));

            $this->assertEquals ('{"name":"zhenggp","age":18}', $user->json ());
            $this->assertEquals ('{"name":"chen juan","age":32}', $user1->json ());
        }

        /**
         * 测试生成原始属性的json值
         */
        public function testJsonForOriginal () {
            $user = new User(array(
                                  'name' => 'zhenggp',
                                  'age' => 18
                             ));
            $user1 = new User(array(
                                   'name' => 'chen juan',
                                   'age' => 32
                              ));

            $user->namge = 'Hello';
            $user->age = 20;

            $user1->name = 'test';
            $user1->age = 20;

            $this->assertEquals ('{"name":"zhenggp","age":18}', $user->json (true));
            $this->assertEquals ('{"name":"chen juan","age":32}', $user1->json (true));
        }

        /**
         * 测试创建回调方法
         */
        public function testCreateCallbackMethod () {
            $model = new CallbackMock(true);

            $this->assertTrue ($model->save ());
            $this->assertTrue ($model->createBefore);
            $this->assertTrue ($model->createInvoke);
            $this->assertTrue ($model->createAfter);
        }

        /**
         * 测试创建回调用方法false返回值
         */
        public function testCreateCallbackMethodForBeforeFalse () {
            $model = new CallbackMock(true);
            $model->createBeforeReturn = false;

            $this->assertFalse ($model->save ());
            $this->assertTrue ($model->createBefore);
            $this->assertFalse ($model->createInvoke);
            $this->assertFalse ($model->createAfter);
        }

        /**
         * 测试执行回调方法，返回false值
         */
        public function testCreateCallbackMethodForInvokeFalse () {
            $model = new CallbackMock(true);
            $model->createInvokeReturn = false;

            $this->assertFalse ($model->save ());
            $this->assertTrue ($model->createBefore);
            $this->assertTrue ($model->createInvoke);
            $this->assertFalse ($model->createAfter);
        }

        /**
         * 测试创建回调用方法，返回false值
         */
        public function testCreateCallbackMethodForAfterFalse () {
            $model = new CallbackMock(true);
            $model->createAfterReturn = false;

            $this->assertTrue ($model->save ());
            $this->assertTrue ($model->createBefore);
            $this->assertTrue ($model->createInvoke);
            $this->assertTrue ($model->createAfter);
        }

        /**
         * 测试更新回调用方法
         */
        public function testUpdateCallbackMethod () {
            $model = new CallbackMock(false);

            $this->assertTrue ($model->save ());
            $this->assertTrue ($model->updateBefore);
            $this->assertTrue ($model->updateInvoke);
            $this->assertTrue ($model->updateAfter);
        }

        /**
         * 测试更新回调用方法，返回false值
         */
        public function testUpdateCallbackMethodForBeforeFalse () {
            $model = new CallbackMock(false);
            $model->updateBeforeReturn = false;

            $this->assertFalse ($model->save ());
            $this->assertTrue ($model->updateBefore);
            $this->assertFalse ($model->updateInvoke);
            $this->assertFalse ($model->updateAfter);
        }

        /**
         * 测试更新执行回调方法，返回false值
         */
        public function testUpdateCallbackMethodForInvokeFalse () {
            $model = new CallbackMock(false);
            $model->updateInvokeReturn = false;

            $this->assertFalse ($model->save ());
            $this->assertTrue ($model->updateBefore);
            $this->assertTrue ($model->updateInvoke);
            $this->assertFalse ($model->updateAfter);
        }

        /**
         * 测试更新回调方法，返回false值
         */
        public function testUpdateCallbackMethodForAfterFalse () {
            $model = new CallbackMock(false);
            $model->updateAfterReturn = false;

            $this->assertTrue ($model->save ());
            $this->assertTrue ($model->updateBefore);
            $this->assertTrue ($model->updateInvoke);
            $this->assertTrue ($model->updateAfter);
        }

        /**
         * 测试删除回调方法
         */
        public function testDeleteCallbackMethod () {
            $model = new CallbackMock(false);

            $this->assertTrue ($model->delete ());
            $this->assertTrue ($model->deleteBefore);
            $this->assertTrue ($model->deleteInvoke);
            $this->assertTrue ($model->deleteAfter);
        }

        /**
         * 测试删除回调方法，返回false值
         */
        public function testDeleteCallbackMethodForBeforeFalse () {
            $model = new CallbackMock(false);
            $model->deleteBeforeReturn = false;

            $this->assertFalse ($model->delete ());
            $this->assertTrue ($model->deleteBefore);
            $this->assertFalse ($model->deleteInvoke);
            $this->assertFalse ($model->deleteAfter);
        }

        /**
         * ���Ե��ûص�����������ʧ�ܣ�

        public function testDeleteCallbackMethodForInvokeFalse() {
        $model = new CallbackMock(false);
        $model->deleteInvokeReturn = false;

        $this->assertFalse($model->delete());
        $this->assertTrue($model->deleteBefore);
        $this->assertTrue($model->deleteInvoke);
        $this->assertFalse($model->deleteAfter);
        }*/

        /**
         * 测试删除回调方法
         */
        public function testDeleteCallbackMethodForAfterFalse () {
            $model = new CallbackMock(false);
            $model->deleteAfterReturn = false;

            $this->assertTrue ($model->delete ());
            $this->assertTrue ($model->deleteBefore);
            $this->assertTrue ($model->deleteInvoke);
            $this->assertTrue ($model->deleteAfter);
        }

        /**
         * 测试错误方法，设置数组错误
         */
        public function testErrorsForArrayError () {
            $user = new User();

            $user->errors ('age', array('age < 18'));
            $user->errors ('age', array('age format error'));
            $user->errors ('name', array(
                                        'name repeat',
                                        'name size < 10'
                                   ));

            $errors = $user->errors ();
            $ageErrors = $user->errors ('age');
            $nameErrors = $user->errors ('name');

            # 错误值
            /* #errors = array('age' => array('age < 18','age format error'),
                               'name'=> array('name repeat','name size < 10'));
            */
            $this->assertTrue (is_array ($errors));
            $this->assertEquals (2, count ($errors));
            $this->assertEmpty (array_diff_assoc (array(
                                                       'age < 18',
                                                       'age format error'
                                                  ), $ageErrors));
            $this->assertEmpty (array_diff_assoc (array(
                                                       'name repeat',
                                                       'name size < 10'
                                                  ), $nameErrors));
        }

        /**
         * 测试错误，设置字符串错误
         */
        public function testErrorsForStringError () {
            $user = new User();

            $user->errors ('age', 'age < 18');
            $user->errors ('age', 'age format error');

            $errors = $user->errors ();
            $ageErrors = $user->errors ('age');

            /**
             * errors: array('age' => array('age < 18','age format error'))
             */

            $this->assertTrue (is_array ($errors));
            $this->assertEquals (1, count ($errors));
            $this->assertEmpty (array_diff (array(
                                                 'age < 18',
                                                 'age format error'
                                            ), $ageErrors));
        }

        /**
         * 测试错误返回，不存在的错误值
         *
         * @depends testErrorsForStringError
         */
        public function testErrorForNotExists () {
            $user = new User();
            $user->errors ('age', 'age > 10');

            $this->assertFalse ($user->errors ('name'));
            $this->assertFalse ($user->errors ('group_id'));
        }

        /**
         * 测试清除错误值
         *
         * @depends testErrorsForStringError
         */
        public function testClearError () {
            $user = new User();
            $user->errors ('age', 'age < 18');

            $user->clearErrors ();
            $errors = $user->errors ();

            $this->assertEmpty ($errors);
        }

        /**
         * 测试使用新的数据值，清除错误
         *
         * @depends testErrorsForStringError
         */
        public function testClearErrorForArray () {
            $user = new User();
            $user->errors ('age', 'age > 10');

            $user->clearErrors (array('name' => 'name size < 10'));
            $errors = $user->errors ();

            $this->assertEmpty (array_diff_assoc (array('name' => 'name size < 10'), $errors));
        }

        /**
         * 测试使用字符串清除错误值
         *
         * @depends testErrorsForStringError
         */
        public function testClearErrorForString () {
            $user = new User();
            $user->errors ('age', 'age > 10');

            $user->clearErrors ('name size < 10');
            $errors = $user->errors ();

            $this->assertTrue (is_array ($errors));
            $this->assertTrue (isset($errors['base'][0]));
            $this->assertEmpty (array_diff (array('name size < 10'), $errors['base']));
        }

        /**
         * 测试关系加载
         */
        public function testRelationsForHasOneLazyLoad () {
            $this->_initUser ();
            $group = new GroupR(1);

            $user = $group->last;

            $this->assertEquals ('UserR', get_class ($user));
            $this->assertEquals ('zhenggp', $user->name);
            $this->assertEquals ('zhenggp', $group->last->name);
        }

        /**
         * 测试多个关系，懒加载
         */
        public function testRelationForHasManyLazyLoad () {
            $database = $this->_initUser ();

            $group = new GroupR(1);
            $users = $group->users;

            $this->assertEquals ('ModelFinder', get_class ($users));
            $this->assertEquals (2, $users->count ());

            $users = $users->models ();
            $this->assertEquals (1, count ($users));
            $this->assertEquals ('zhenggp', $users[0]->name);
        }

        /**
         * 测试Belongs关系懒加载
         */
        public function testRelationForBelongsToLazyLoad () {
            $database = $this->_initUser ();
            $user = UserR::find (array('name' => 'zhenggp'));

            $groupName = $user->group->name;

            $this->assertEquals ('admins', $groupName);
        }

        /**
         * 测试joins语法
         *
        public function testFindForJoinsSyntax() {
        $user = UserR::find(array('UserR.name' => 'zhenggp'), array('joins' => array('group')));

        # TODO:
        #print_r($user);
        }*/

        /**
         * 测试验证，自定义验证方法
         */
        public function testValidateForOnAuthenticate () {
            $success = array(
                'name' => 'zhenggp',
                'password' => '123'
            );
            $fail = array('name' => 'error');

            $this->assertTrue (ModelValidateMock::validateMock ('auth', $success));
            $errors = ModelValidateMock::validateMock ('auth', $fail);

            $this->assertTrue (is_array ($errors));
            print_r ($errors);
            $this->assertEquals ('Sorry password can\'t null', $errors['password']);
        }

        /**
         * 测试验证，使用模型数据
         */
        public function testValidateForModelData () {
            $success = new ModelValidateMock(array(
                                                  'name' => 'zhenggp',
                                                  'password' => '123'
                                             ));
            $fail = new ModelValidateMock(array('name' => 'error'));

            $this->assertTrue (ModelValidateMock::validateMock ('auth', $success));
            $this->assertTrue (is_array (ModelValidateMock::validateMock ('auth', $fail)));
        }

        /**
         * 测试验证，测试After回调用方法
         */
        public function testValidateForBeforeCallback () {
            $success = array('before' => true);
            $fail = array('before' => false);

            $this->assertTrue (ModelValidateMock::validateMock ('before', $success));
            $this->assertTrue (is_array (ModelValidateMock::validateMock ('before', $fail)));
        }

        /**
         * 测试验证，测试After回调用方法
         */
        public function testValidateForAfterCallback () {
            $success = array('after' => true);
            $fail = array('after' => false);

            $this->assertTrue (ModelValidateMock::validateMock ('after', $success));
            $this->assertTrue (is_array (ModelValidateMock::validateMock ('after', $fail)));
        }

        /**
         * 初始化用户值，并且返回相关数据
         *
         * @return Database
         */
        private function _initUser () {
            $database = User::database ();

            $database->execute ('DELETE FROM `user`');
            $database->execute ('INSERT INTO `user`(`name`,`age`,`desc`,`user_group_id`) VALUES(\'zhenggp\',18,\'programmer\',1),(\'Chen juan\',18,\'programmer\',null);');

            $database->execute ('DELETE FROM `user_group');
            $database->execute ("INSERT INTO `user_group`(`id`,`name`) VALUES(1,'admins'),(2,'guest')");

            return $database;
        }

        /**
         * 初始化用户权限数据，并返回数据库
         *
         * @return Database
         */
        private function _initUserRight () {
            $database = UserRight::database ();

            $database->execute ('DELETE FROM `user_rights`');
            $database->execute ('INSERT INTO `user_rights`(`rid`,`name`) VALUES(1,\'zhenggp\'),(2,\'chenjuan\')');

            return $database;
        }
    }