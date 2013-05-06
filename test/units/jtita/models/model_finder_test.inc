<?php
    /**
     * model_finder_test.inc 模型查找测试类声明文件
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.models
     * @version     1.0 beta
     *
     * @create      2012-08-15
     * @modify      2013-04-28
     */

    # 引入路径根节点
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (__FILE__))))));

    # 引用测试依赖文件
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');

    # 设置引入路径
    Jtita::paths (array(JTPATH_ROOT . '/jtita'));

    # 设置配置信息
    Jtita::write ('kernels/datasources', array(
                                              'default' => array(
                                                  'class' => 'kernels.datasources.databases.mysql_fast',
                                                  'server' => '127.0.0.1',
                                                  'user' => 'root',
                                                  'dbpass' => 'killer',
                                                  'initialize' => true,
                                                  'dbname' => 'jtfix'
                                              ),
                                              'mock' => array('class' => 'model_finder_database_mock')
                                         ));

    # 设置引用路径
    require_once(JTPATH_ROOT . '/jtita/models/model.inc');

    /**
     * 模型查找数据库模拟对象，提供模型查找所需的单元测试服务
     *
     * @package jtita.models
     */
    class ModelFinderDatabaseMock {
        public function initialize ($config = array()) {
        }

        public function findSyntax () {
            return array(
                'orderBy' => 'order',
                'fields' => 'fields',
                'where' => 'where',
                'limit' => 'limit'
            );
        }
    }

    /**
     * 用户模拟对象，提供模型模拟服务
     *
     * @package jtita.models
     */
    class UserMock extends Model {
        public static function databaseName () {
            return 'mock';
        }
    }

    /**
     * 用户模型
     *
     * @package jtita.models
     */
    class User extends Model {

    }

    /**
     * 会员模型
     *
     * @package jtita.models
     */
    class Member extends Model {

    }

    /**
     * 模型查找器测试类，提供模型查找类的查找服务
     *
     * @package jtita.models
     */
    class ModelFinderTest extends PHPUnit_Framework_TestCase {
        /**
         * 测试默认构造器
         */
        public function testConstructorForDefault () {
            $finder = new ModelFinder('User');

            $this->assertEquals ('User', $finder->modelName ());
            $this->assertEmpty ($finder->options ());
            $this->assertEmpty ($finder->syntax ());
        }

        /**
         * 测试构造器
         */
        public function testConstructor () {
            $finder = new ModelFinder('Member', 'Member.id=12', array('o1' => 'ov'));

            $this->assertEquals ('Member', $finder->modelName ());
            $this->assertEquals ('Member.id=12', $finder->syntax ());
            $this->assertTrue (is_array ($finder->options ()));

            $options = $finder->options ();

            $this->assertEquals ('ov', $options['o1']);
        }

        /**
         * 测试调用声明的语法
         */
        public function testCallForDeclareSyntax () {
            $finder = new ModelFinder('UserMock');

            $finder->where ('User.id = 12')->fields ('User.id,User.name,User.descr')->orderBy ('User.id');

            $this->assertEquals ('User.id = 12', $finder->where ());
            $this->assertEquals ('User.id,User.name,User.descr', $finder->fields ());
            $this->assertEquals ('User.id', $finder->orderBy ());
            $this->assertNull ($finder->limit ());
        }

        /**
         * 测试调用未声明的查找语法
         *
         * @expectedException MethodNotFoundException
         */
        public function testCallForNotFoundSyntax () {
            $finder = new ModelFinder('UserMock');

            $finder->notexists ();
        }

        /**
         * 测试查找数量
         */
        public function testCount () {
            $this->_initUser ();

            $users = new ModelFinder('User', array(
                                                  'table' => 'user',
                                                  'alias' => 'User'
                                             ));
            $this->assertEquals (2, $users->count ());
        }

        /**
         * 测试查找返回rows记录
         */
        public function testRows () {
            $this->_initUser ();

            $users = new ModelFinder('User', array(
                                                  'table' => 'user',
                                                  'alias' => 'User'
                                             ));

            $users->where (array('User.name' => 'zhenggp'))->fields (array(
                                                                          'User.name',
                                                                          'User.age',
                                                                          'User.user_group_id'
                                                                     ));
            $rows = $users->rows ();

            $this->assertTrue (is_array ($rows));
            $this->assertEquals (1, count ($rows));
            $this->assertEquals (3, count ($rows[0]));
            $this->assertEquals ('zhenggp', $rows[0]['name']);
            $this->assertEquals (18, $rows[0]['age']);
            $this->assertEquals (1, $rows[0]['user_group_id']);
        }

        /**
         * 测试查找返回模型记录，
         */
        public function testModels () {
            $this->_initUser ();

            $users = new ModelFinder('User', array(
                                                  'table' => 'user',
                                                  'alias' => 'User'
                                             ));

            $users->where (array('User.name' => 'zhenggp'))->fields (array(
                                                                          'User.name',
                                                                          'User.age',
                                                                          'User.user_group_id'
                                                                     ));
            $models = $users->models ();

            $this->assertTrue (is_array ($models));
            $this->assertEquals (1, count ($models));

            $this->assertEquals ('zhenggp', $models[0]->name);
            $this->assertEquals (18, $models[0]->age);
            $this->assertEquals (1, $models[0]->user_group_id);
        }

        /**
         * 测试查找全部记录
         */
        public function testModelFindAll () {
            $this->_initUser ();

            $users = User::findAll ('User.name = \'zhenggp\'');
            $models = $users->models ();

            $this->assertEquals ('User', $users->modelName ());
            $this->assertTrue (is_array ($models));
            $this->assertEquals (1, count ($models));
        }

        /**
         * 查找查找获取列表记录
         */
        public function testKlist () {
            $this->_initUser ();

            $users = new ModelFinder('User', array(
                                                  'table' => 'user',
                                                  'alias' => 'User'
                                             ));

            $users->fields (array(
                                 'User.id',
                                 'User.name'
                            ));
            $klist = $users->klist ();

            $this->assertTrue (is_array ($klist));
            $this->assertEquals (2, count ($klist));
        }

        /**
         * 测试查找获取json记录
         */
        public function testJson () {
            $this->_initUser ();
            $users = New ModelFinder('User', array(
                                                  'table' => 'user',
                                                  'alias' => 'User'
                                             ));
            $users->fields (array(
                                 'name',
                                 'age',
                                 'desc'
                            ));

            $this->assertEquals ('[{"name":"zhenggp","age":"18","desc":"programmer"},{"name":"Chen juan","age":"18","desc":"programmer"}]', $users->json ());
        }

        /**
         * 测试查找获取xml数据
         *
        public function testXml() {

        }*/

        /**
         * 初始化测试数据
         */
        private function _initUser () {
            $database = User::database ();

            $database->execute ('DELETE FROM `user`');
            $database->execute ('INSERT INTO `user`(`name`,`age`,`desc`,`user_group_id`) VALUES(\'zhenggp\',18,\'programmer\',1),(\'Chen juan\',18,\'programmer\',null);');

            return $database;
        }
    }