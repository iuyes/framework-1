<?php
    /**
     * model_relations_test.inc 模型关系单元测试类声明文件
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.models
     * @version     1.0 beta
     *
     * @create      2012-10-07
     * @modify      2013-04-28
     */

    # 引用测试根路径
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (__FILE__))))));

    # 引用测试关联类
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');

    # 设置导入路径
    Jtita::paths (JTPATH_ROOT . '/jtita');

    # 写入配置信息
    Jtita::write ('kernels/datasources', array(
                                              'default' => array(
                                                  'class' => 'kernels.datasources.databases.mysql_fast',
                                                  'server' => '127.0.0.1',
                                                  'user' => 'root',
                                                  'dbpass' => 'killer',
                                                  'initialize' => true,
                                                  'dbname' => 'jtfix'
                                              )
                                         ));

    # 引用测试内容
    require_once(JTPATH_ROOT . '/jtita/models/model.inc');

    /**
     * 用户类，提供测试的模拟服务
     */
    class User extends Model {

    }

    /**
     * 用户组类，提供测试的模拟服务
     */
    class UserGroup extends Model {
        public static function dataName () {
            return 'user_group';
        }
    }

    class ModelRelationSchema extends Model {
        public static function dataName () {
            return 'user_group';
        }
    }

    /**
     * 模型关系测试类，提供模型关系的单元测试服务
     *
     * @package jtita.models
     */
    class ModelRelationTest extends PHPUnit_Framework_TestCase {

        /**
         * 测试抽象类，schema未提供类名选项
         *
         * @expectedException ModelRelationException
         */
        public function testAbstractRelationForSchemaNotSupportClassNameOption () {
            $hasOne = new HasOneModelRelation();
            $hasOne->schema ('group', array());
        }

        /**
         * 测试抽象类，schema 表名选项
         */
        public function testAbstractRelationForSchema () {
            $hasOne = new HasOneModelRelation();
            $schemas = $hasOne->schema ('group', array('className' => 'ModelRelationSchema'));

            $this->assertEquals ('user_group', $schemas['table']);
            $this->assertEquals ('ModelRelationSchema', $schemas['alias']);
        }

        /**
         * 测试抽象类，schema 表别名选项
         */
        public function testAbstractRelationForSchemaAlias () {
            $hasOne = new HasOneModelRelation();
            $schemas = $hasOne->schema ('group', array(
                                                      'className' => 'ModelRelationSchema',
                                                      'alias' => 'Group'
                                                 ));

            $this->assertEquals ('user_group', $schemas['table']);
            $this->assertEquals ('Group', $schemas['alias']);
        }

        /**
         * 测试抽象类，load未提供类名选项。
         *
         * @expectedException ModelRelationException
         */
        public function testAbstractRelationForLoadNotSupportClassNameOption () {
            $hasOne = new HasOneModelRelation();
            $user = new User();
            $hasOne->load ($user, array());
        }

        /**
         * 测试hasOne懒加载方法
         */
        public function testHasOneForLoad () {
            $hasOne = new HasOneModelRelation();
            $group = new UserGroup(array(
                                        'id' => 2,
                                        'name' => 'guests'
                                   ), true);
            $lazyGroup = $hasOne->load ($group, array(
                                                     'foreignKey' => 'id',
                                                     'className' => 'UserGroup'
                                                ));

            $this->assertEquals ('UserGroup', get_class ($lazyGroup));
            $this->assertEquals ('guest', $lazyGroup->name);
        }

        /**
         * 测试加载一对一对象，参数where选项
         */
        public function testHasOneForLoadWhereArgs () {
            User::database ()->execute ('DELETE FROM `user`');
            User::database ()->execute ('INSERT INTO `user`(`name`,`user_group_id`,`age`) VALUES(\'zhenggp\',1,18),(\'chenJ\',1,18)');

            $hasOne = new HasOneModelRelation();
            $group = new UserGroup(array(
                                        'id' => 1,
                                        'name' => 'admins'
                                   ), true);
            $user = $hasOne->load ($group, array(
                                                'className' => 'user',
                                                'where' => 'user_group_id = :foreignKey AND User.name=\'chenJ\''
                                           ));

            $this->assertEquals ('User', get_class ($user));
            $this->assertEquals ('chenJ', $user->name);
        }

        /**
         * 测试加载一对一对象，数组where选项
         */
        public function testHasOneForWhereArray () {
            User::database ()->execute ('DELETE FROM `user`');
            User::database ()->execute ('INSERT INTO `user`(`name`,`user_group_id`,`age`) VALUES(\'zhenggp\',1,18),(\'chenJ\',1,18)');

            $hasOne = new HasOneModelRelation();
            $group = new UserGroup(array(
                                        'id' => 1,
                                        'name' => 'admins'
                                   ), true);
            $user = $hasOne->load ($group, array(
                                                'className' => 'user',
                                                'where' => array('User.name' => 'chenJ')
                                           ));

            $this->assertEquals ('User', get_class ($user));
            $this->assertEquals ('chenJ', $user->name);
        }

        /**
         * 测试加载一对一对象，外键选项未提供
         */
        public function testHasOneForLoadForeignKeyOptionNotSupport () {
            User::database ()->execute ('DELETE FROM `user`');
            User::database ()->execute ('INSERT INTO `user`(`name`,`user_group_id`,`age`) VALUES(\'zhenggp\',1,18),(\'chenJ\',1,18)');

            $hasOne = new HasOneModelRelation();
            $group = new UserGroup(array('id' => 1));
            $user = $hasOne->load ($group, array('className' => 'user'));

            $this->assertNotEmpty ($user);
            $this->assertEquals ('zhenggp', $user->name);
        }

        /**
         * 测试加载一对一对象，主键值为空值
         */
        public function testHasOneToForLoadKeyIsEmpty () {
            $hasOne = new HasOneModelRelation();
            $group = new UserGroup(array('name' => 'admins'));
            $user = $hasOne->load ($group, array('className' => 'user'));

            $this->assertNotEmpty ($user);
            $this->assertNull ($user->name);
        }

        /**
         * 测试加载一对一，对象不存在
         */
        public function testHasOneForModelNotExists () {
            User::database ()->execute ('DELETE FROM `user`');

            $hasOne = new HasOneModelRelation();
            $group = new UserGroup(array(
                                        'name' => 'zhenggp',
                                        'id' => 1
                                   ));
            $user = $hasOne->load ($group, array('className' => 'user'));

            $this->assertNotEmpty ($user);
            $this->assertNull ($user->name);
        }

        /**
         * 测试加载一对一，使用额外的语法选项
         */
        public function testHasOneForLoadSyntax () {
            User::database ()->execute ('DELETE FROM `user`');
            User::database ()->execute ('INSERT INTO `user`(`name`,`user_group_id`,`age`) VALUES(\'zhenggp\',1,18),(\'chenJ\',1,18)');

            $hasOne = new HasOneModelRelation();
            $group = new UserGroup(array(
                                        'id' => 1,
                                        'name' => 'admins'
                                   ), true);
            $user = $hasOne->load ($group, array(
                                                'foreignKey' => 'user_group_id',
                                                'className' => 'user',
                                                'order' => 'User.id desc'
                                           ));

            $this->assertEquals ('User', get_class ($user));
            $this->assertEquals ('chenJ', $user->name);
        }

        /**
         * 测试加载一对多
         */
        public function testHasManyForLoad () {
            User::database ()->execute ('DELETE FROM `user`');
            User::database ()->execute ('INSERT INTO `user`(`name`,`user_group_id`,`age`) VALUES(\'zhenggp\',1,18),(\'chenJ\',1,18)');

            $hasMany = new HasManyModelRelation();
            $group = new UserGroup(array(
                                        'id' => 1,
                                        'name' => 'admins'
                                   ), true);
            $users = $hasMany->load ($group, array(
                                                  'foreignKey' => 'user_group_id',
                                                  'className' => 'user'
                                             ));
            $models = $users->models ();

            $this->assertEquals ('ModelFinder', get_class ($users));
            $this->assertEquals (2, count ($models));
        }


        /**
         * 测试加载一对多，指定where参数
         */
        public function testHasManyForWhereArgs () {
            User::database ()->execute ('DELETE FROM `user`');
            User::database ()->execute ('INSERT INTO `user`(`name`,`user_group_id`,`age`) VALUES(\'zhenggp\',1,18),(\'chenJ\',1,18)');

            $hasMany = new HasManyModelRelation();
            $group = new UserGroup(array('id' => 1));

            $users = $hasMany->load ($group, array(
                                                  'className' => 'user',
                                                  'where' => 'User.user_group_id = :foreignKey AND User.name = \'chenJ\''
                                             ));
            $users = $users->models ();

            $this->assertEquals (1, count ($users));
            $this->assertEquals ('chenJ', $users[0]->name);
        }

        /**
         * 测试加载一对多，指定where数组
         */
        public function testHasManyForWhereArray () {
            User::database ()->execute ('DELETE FROM `user`');
            User::database ()->execute ('INSERT INTO `user`(`name`,`user_group_id`,`age`) VALUES(\'zhenggp\',1,18),(\'chenJ\',1,18)');

            $hasOne = new HasManyModelRelation();
            $group = new UserGroup(array('id' => 1));

            $users = $hasOne->load ($group, array(
                                                 'className' => 'user',
                                                 'where' => array('User.name' => 'chenJ')
                                            ));
            $users = $users->models ();

            $this->assertEquals (1, count ($users));
            $this->assertEquals ('chenJ', $users[0]->name);
        }

        /**
         * 测试加载一对多，外键未指定
         */
        public function testHasManyForLoadForeignKeyOptionNotSupport () {
            User::database ()->execute ('DELETE FROM `user`');
            User::database ()->execute ('INSERT INTO `user`(`name`,`user_group_id`,`age`) VALUES(\'zhenggp\',1,18),(\'chenJ\',1,18)');

            $hasMany = new HasManyModelRelation();
            $group = new UserGroup(array(
                                        'id' => 1,
                                        'name' => 'admins'
                                   ), true);
            $users = $hasMany->load ($group, array('className' => 'user'));
            $models = $users->models ();

            $this->assertEquals ('ModelFinder', get_class ($users));
            $this->assertEquals (2, count ($models));
        }

        /**
         * 测试加载一对多，指定菩额外语法
         */
        public function testHasManyForLoadSyntax () {
            User::database ()->execute ('DELETE FROM `user`');
            User::database ()->execute ('INSERT INTO `user`(`name`,`user_group_id`,`age`) VALUES(\'zhenggp\',1,18),(\'chenJ\',1,18)');

            $hasMany = new HasManyModelRelation();
            $group = new UserGroup(array(
                                        'id' => 1,
                                        'name' => 'admins'
                                   ), true);
            $users = $hasMany->load ($group, array(
                                                  'className' => 'user',
                                                  'fields' => array(
                                                      'id',
                                                      'name'
                                                  )
                                             ));
            $models = $users->models ();

            $this->assertEquals ('ModelFinder', get_class ($users));
            $this->assertEquals (2, count ($models));

            foreach ($models as $model) {
                $this->assertNotNull ($model->id);
                $this->assertNotNull ($model->name);

                $this->assertNull ($model->age);
                $this->assertNull ($model->desc);
            }
        }


        /**
         * 测试加载一对多关系
         */
        public function testBelongsTo () {
            $belongs = new BelongsToModelRelation();
            $user = new User(array(
                                  'group_id' => 1,
                                  'name' => 'zhenggp'
                             ));
            $group = $belongs->load ($user, array(
                                                 'foreignKey' => 'group_id',
                                                 'className' => 'user_group'
                                            ));

            $this->assertNotEmpty ($group);
            $this->assertEquals ('admins', $group->name);
        }

        /**
         * 测试多对一关系，外键未指定
         */
        public function testBelongsToForForeignKeyOptionNotSupport () {
            $belongs = new BelongsToModelRelation();
            $user = new User(array(
                                  'user_group_id' => 1,
                                  'name' => 'zhenggp'
                             ));
            $group = $belongs->load ($user, array('className' => 'user_group'));

            $this->assertNotEmpty ($group);
            $this->assertEquals ('admins', $group->name);
        }

        /**
         * 测试多对一关系，外键值为空
         */
        public function testBelongsToForForeignKeyIsEmpty () {
            $belongs = new BelongsToModelRelation();
            $user = new User(array('name' => 'zhenggp'));
            $group = $belongs->load ($user, array('className' => 'user_group'));

            $this->assertNotEmpty ($group);
            $this->assertNull ($group->name);
        }

        /**
         * 测试多对一，外键模型不存在
         *
         * @depends testBelongsToForForeignKeyIsEmpty
         */
        public function testBelongsToForModelNotExists () {
            $belongs = new BelongsToModelRelation();
            $user = new User(array(
                                  'name' => 'zhenggp',
                                  'user_group_id' => 3
                             ));
            $group = $belongs->load ($user, array('className' => 'user_group'));

            $this->assertNotEmpty ($group);
            $this->assertNull ($group->name);
        }

        /**
         * 测试多对一，提供额外的语法支持
         */
        public function testBelongsToForSyntax () {
            $belongs = new BelongsToModelRelation();
            $user = new User(array(
                                  'name' => 'zhenggp',
                                  'user_group_id' => 1
                             ));
            $group = $belongs->load ($user, array(
                                                 'className' => 'user_group',
                                                 'fields' => array('id')
                                            ));

            $this->assertNotEmpty ($group);
            $this->assertEquals (1, $group->key ());
            $this->assertNull ($group->name);
            $this->assertTrue ($group->isUpdate ());
        }


    }
	