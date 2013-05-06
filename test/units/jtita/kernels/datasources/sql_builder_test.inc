<?php
    /**
     *  sql_builder_test.inc SqlBuilder单元测试类声明文件
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.datasources
     * @version     1.0 beta
     *
     * @create      2012-08-30
     * @modify      2013-04-28
     */

    # 声明测试根节点路径。
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (dirname (__FILE__)))))));

    # 引用测试关联类
    require_once(JTPATH_ROOT . '/jtita/kernels/datasources/database.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/datasources/sql_builder.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');

    /**
     *  SQL 生成模拟器，提供SqlBuilder测试所需的模拟服务
     *
     * @package jtita.kernels.datasources
     */
    class SqlBuilderMock extends SqlBuilder {
        public function safeName ($name) {
            return str_replace ('.', '].[', "[$name]");
        }

        protected function _pfilter ($value) {
            return "($value)";
        }
    }

    /**
     * 数据库模拟器，提供SqlBuilder测试所需要数据库模拟服务。
     */
    class SqlBuilderDatabaseMock implements Database {
        function initialize ($config = array()) { }

        function initialized () { }

        function findFields ($syntax, $options = array()) {}

        function schema ($dataName) {
            if ($dataName == 'user') {
                return array(
                    'id' => array(),
                    'name' => array(),
                    'age' => array(),
                    'desc' => array(),
                    'user_group_id' => array()
                );
            }
            else if ($dataName == 'user_group') {
                return array(
                    'id' => array(),
                    'name' => array()
                );
            }
            else if ($dataName == 'product') {
                return array(
                    'id' => array(),
                    'name' => array(),
                    'category' => array(),
                    'brand' => array()
                );
            }
            else if ($dataName == 'category') {
                return array(
                    'id' => array(),
                    'name' => array()
                );
            }
            else if ($dataName == 'brand') {
                return array(
                    'id' => array(Database::SCHEMA_FORMATER => 'intval'),
                    'name' => array(
                        Database::SCHEMA_FORMATER => 'date',
                        Database::SCHEMA_FORMAT_STRING => 'Y-m-d'
                    )
                );
            }
            else {
                return array();
            }
        }

        function create ($data, $syntax, $options = array()) { }

        function createID () { }

        function update ($data, $syntax, $options = array()) { }

        function delete ($syntax, $options = array()) { }

        function find ($syntax, $options = array()) { }

        function findCount ($syntax, $options = array()) { }

        function findSyntax () { }

        function query ($cmd, $args = array(), $options = array()) { }

        function execute ($cmd, $args = array(), $options = array()) { }
    }

    /**
     * SqlBuilder 单元测试类，提供SqlBuilder的单元测试服务。
     *
     * @package jtita.kernels.datasources
     */
    class SqlBuilderTest extends PHPUnit_Framework_TestCase {

        /**
         * 测试生成安全命名
         */
        public function testSafeName () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $name = $builder->safeName ('name');

            $this->assertEquals ('[name]', $name);
        }

        /**
         * 测试包含分隔符的安全命名
         */
        public function testSafeNameForSeparator () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $name = $builder->safeName ('user.name');

            $this->assertEquals ('[user].[name]', $name);
        }

        /**
         * 测试预处理命令
         */
        public function testPrepare () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->prepare ('SELECT [name],[age],[desc] FROM [user] WHERE [age] = :age', array(':age' => 26));

            $this->assertEquals ('SELECT [name],[age],[desc] FROM [user] WHERE [age] = (26)', $sql);
        }

        /**
         * 测试预处理命令，参数未提供
         */
        public function testPrepareForArgsNotSupport () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());

            $sql = $builder->prepare ('SELECT [name],[age],[desc] FROM [user] WHERE where age = :age', array(':name' => 'zhenggp'));

            $this->assertEquals ('SELECT [name],[age],[desc] FROM [user] WHERE where age = :age', $sql);
        }

        /**
         * 测试创建命令
         */
        public function testCreate () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->create (array(
                                          'id' => 12,
                                          'name' => 'admin'
                                     ), array('table' => 'user_group'));

            $this->assertEquals ('INSERT INTO [user_group]([id],[name]) VALUES((12),(admin))', $sql);
        }

        /**
         * 测试创建命令，使用格式器
         */
        public function testCreateForFormater () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->create (array(
                                          'name' => 0,
                                          'id' => 123
                                     ), array(
                                             'table' => 'brand',
                                             'alias' => 'Brand'
                                        ));

            $this->assertEquals ('INSERT INTO [brand]([name],[id]) VALUES((1970-01-01),(123))', $sql);
        }

        /**
         * 测试创建命令，创建多个数据
         */
        public function testCreateForMultiple () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->create (array(
                                          array(
                                              'id' => 12,
                                              'name' => 'admin'
                                          ),
                                          array(
                                              'id' => 13,
                                              'name' => 'guest'
                                          )
                                     ), array(
                                             'table' => 'user_group',
                                             'alias' => 'Group'
                                        ), array('multiple' => true));

            $this->assertEquals ('INSERT INTO [user_group]([id],[name]) VALUES((12),(admin)),((13),(guest))', $sql);
        }


        /**
         * 测试更新命令
         */
        public function testUpdate () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->update (array('name' => 'zhenggp'), array(
                                                                      'table' => 'user_group',
                                                                      'alias' => 'Group'
                                                                 ));

            $this->assertEquals ('UPDATE [user_group] AS [Group] SET [Group].[name] = (zhenggp)', $sql);
        }

        /**
         * 测试更新命令，使用where语法
         */
        public function testUpdateForArrayWhere () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->update (array('name' => 'zhenggp'), array(
                                                                      'table' => 'user_group',
                                                                      'where' => array('id' => 12)
                                                                 ));

            $this->assertEquals ('UPDATE [user_group] AS [user_group] SET [user_group].[name] = (zhenggp) WHERE [id] = (12)', $sql);
        }

        /**
         * 测试更新命令，使用where字符串语法
         */
        public function testUpdateForStringWhere () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->update (array('name' => 'zhenggp'), array(
                                                                      'table' => 'user_group',
                                                                      'where' => 'id = 12'
                                                                 ));

            $this->assertEquals ('UPDATE [user_group] AS [user_group] SET [user_group].[name] = (zhenggp) WHERE id = 12', $sql);
        }

        /**
         * 测试更新命令，使用where参数语法
         */
        public function testUpdateForArgsWhere () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->update (array('name' => 'zhenggp'), array(
                                                                      'table' => 'user_group',
                                                                      'where' => array(
                                                                          'id = :id',
                                                                          ':id' => 12
                                                                      )
                                                                 ));

            $this->assertEquals ('UPDATE [user_group] AS [user_group] SET [user_group].[name] = (zhenggp) WHERE id = (12)', $sql);
        }

        /**
         * 测试更新命令，使用fields语法
         */
        public function testUpdateForFields () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->update (array(
                                          'name' => 'zhenggp',
                                          'id' => '123'
                                     ), array(
                                             'table' => 'user',
                                             'alias' => 'User',
                                             'fields' => array('name')
                                        ));

            $this->assertEquals ('UPDATE [user] AS [User] SET [User].[name] = (zhenggp)', $sql);
        }

        /**
         * 测试更新命令，使用except语法
         */
        public function testUpdateForFieldsAndExcept () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->update (array(
                                          'name' => 'zhenggp',
                                          'id' => '123'
                                     ), array(
                                             'table' => 'user',
                                             'alias' => 'User',
                                             'fields' => array('name')
                                        ), array('except' => true));

            $this->assertEquals ('UPDATE [user] AS [User] SET [User].[id] = (123)', $sql);
        }

        /**
         * 测试更新命令，提供比表更多的字段
         */
        public function testUpdateForMoreFields () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->update (array(
                                          'name' => 'zhenggp',
                                          'desc' => 'programmer'
                                     ), array(
                                             'table' => 'user_group',
                                             'alias' => 'Group'
                                        ));

            $this->assertEquals ('UPDATE [user_group] AS [Group] SET [Group].[name] = (zhenggp)', $sql);
        }

        /**
         * 测试更新命令，字符串数据
         */
        public function testUpdateForStringData () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->update ('User.name = User.name + User.desc', array(
                                                                               'table' => 'user',
                                                                               'alias' => 'User',
                                                                               'where' => 'User.id = 12'
                                                                          ));

            $this->assertEquals ('UPDATE [user] AS [User] SET User.name = User.name + User.desc WHERE User.id = 12', $sql);
        }

        /**
         * 测试更新命令，使用参数类型数据
         */
        public function testUpdateForArgsData () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->update (array(
                                          'User.name = :name',
                                          ':name' => 'zhenggp'
                                     ), array(
                                             'table' => 'user',
                                             'alias' => 'User',
                                             'where' => 'User.id = 12'
                                        ));

            $this->assertEquals ('UPDATE [user] AS [User] SET User.name = (zhenggp) WHERE User.id = 12', $sql);
        }

        /**
         * 测试更新命令，使用order limit语法
         *
        public function testUpdateForOrderLimit() {
        $builder = new SqlBuilderMock(new DatabaseMock());
        $sql = $builder->update(array('Group' => array('name' => 'zhenggp','desc' => 'programer')),
        array('table' => 'user_group','alias' => 'Group',
        'order' => array('Group.id' => 'asc','Group.name' => 'desc'),
        'limit' => '1'));

        $this->assertEquals('UPDATE [user_group] AS [Group] SET [Group].[name] = (zhenggp) ORDER BY [Group].[id] ASC,[Group].[name] DESC LIMIT 1',$sql);

        }*/

        /**
         * 测试更新命令，使用joins语法
         */
        public function testUpdateForJoins () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->update (array('name' => 'zhenggp'), array(
                                                                      'table' => 'user',
                                                                      'alias' => 'User',
                                                                      'joins' => array('group')
                                                                 ), array(
                                                                         'relation' => array(
                                                                             'group' => array(
                                                                                 'table' => 'user_group',
                                                                                 'alias' => 'UserGroup'
                                                                             )
                                                                         )
                                                                    ));

            $this->assertEquals ('UPDATE [user] AS [User] LEFT JOIN [user_group] AS [UserGroup] ON [User].[user_group_id] = [UserGroup].[id] SET [User].[name] = (zhenggp)', $sql);
        }

        /**
         * 测试更新命令，使用参数格式器
         */
        public function testUpdateForFormater () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->update (array(
                                          'name' => '0',
                                          'id' => '123'
                                     ), array(
                                             'table' => 'brand',
                                             'alias' => 'Brand'
                                        ));

            $this->assertEquals ('UPDATE [brand] AS [Brand] SET [Brand].[name] = (1970-01-01),[Brand].[id] = (123)', $sql);
        }

        /**
         * 测试删除命令
         */
        public function testDelete () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->delete (array(
                                          'table' => 'user',
                                          'alias' => 'User'
                                     ));

            $this->assertEquals ('DELETE [User] FROM [user] AS [User]', $sql);
        }

        /**
         * 测试删除命令，使用where语法
         */
        public function testDeleteForArrayWhere () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->delete (array(
                                          Database::SYN_WHERE => array(
                                              'User.id' => 12,
                                              'User.name' => 'zhenggp'
                                          ),
                                          'table' => 'user',
                                          'alias' => 'User'
                                     ));

            $this->assertEquals ('DELETE [User] FROM [user] AS [User] WHERE [User].[id] = (12) AND [User].[name] = (zhenggp)', $sql);
        }

        /**
         * 测试删除命令，使用字符串删除命令
         */
        public function testDeleteForStringWhere () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->delete (array(
                                          Database::SYN_WHERE => '[user].[id] = 12',
                                          'table' => 'user'
                                     ));

            $this->assertEquals ('DELETE [user] FROM [user] AS [user] WHERE [user].[id] = 12', $sql);
        }

        /**
         * 测试删除命令，使用参数条件
         */
        public function testDeleteForArgsWhere () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->delete (array(
                                          Database::SYN_WHERE => array(
                                              '[User].[id] = :id',
                                              ':id' => 12
                                          ),
                                          'table' => 'user',
                                          'alias' => 'User'
                                     ));

            $this->assertEquals ('DELETE [User] FROM [user] AS [User] WHERE [User].[id] = (12)', $sql);
        }

        /**
         * 测试删除命令，使用order和limit语法。
         */
        public function testDeleteForOrderLimit () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->delete (array(
                                          'where' => array('User.id' => 12),
                                          'order' => array(
                                              'User.id' => 'desc',
                                              'User.name' => 'asc'
                                          ),
                                          'limit' => '1,10',
                                          'table' => 'user',
                                          'alias' => 'User'
                                     ));

            # MySQL 不支持本语法。
            # $this->assertEquals('DELETE [User] FROM [user] AS [User] WHERE [User].[id] = (12) ORDER BY [User].[id] DESC,[User].[name] ASC LIMIT 1,10',$sql);
            $this->assertEquals ('DELETE [User] FROM [user] AS [User] WHERE [User].[id] = (12)', $sql);
        }

        /**
         * 测试删除命令，使用joins语法
         */
        public function testDeleteForJoins () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->delete (array(
                                          'where' => array(
                                              '[User].[id] = :id',
                                              ':id' => 12
                                          ),
                                          'table' => 'user',
                                          'alias' => 'User',
                                          'joins' => array('group')
                                     ), array(
                                             'relation' => array(
                                                 'group' => array(
                                                     'table' => 'user_group',
                                                     'alias' => 'UserGroup'
                                                 )
                                             )
                                        ));

            $this->assertEquals ('DELETE [User] FROM [user] AS [User] LEFT JOIN [user_group] AS [UserGroup] ON [User].[user_group_id] = [UserGroup].[id] WHERE [User].[id] = (12)', $sql);
        }

        /**
         * 测试生成查找命令
         */
        public function testFind () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->find (array(Database::SYN_TABLE => 'user'));

            $this->assertEquals ('SELECT [user].[id],[user].[name],[user].[age],[user].[desc],[user].[user_group_id] FROM [user] AS [user]', $sql);
        }

        /**
         * 测试生成查找命令，包含别名语法
         */
        public function testFindForAlias () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->find (array(
                                        Database::SYN_TABLE => 'user',
                                        Database::SYN_ALIAS => 'User'
                                   ));

            $this->assertEquals ('SELECT [User].[id],[User].[name],[User].[age],[User].[desc],[User].[user_group_id] FROM [user] AS [User]', $sql);
        }

        /**
         * 测试查找命令，数组字段语法
         */
        public function testFindForArrayFields () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->find (array(
                                        Database::SYN_FIELDS => array(
                                            'user.name',
                                            'user.id',
                                            'user.age'
                                        ),
                                        Database::SYN_TABLE => 'user'
                                   ));

            $this->assertEquals ('SELECT [user].[name],[user].[id],[user].[age] FROM [user] AS [user]', $sql);
        }

        /**
         * 测试查找命令，数组as字段语法
         */
        public function testFindForArrayAsFields () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->find (array(
                                        Database::SYN_FIELDS => array(
                                            'user.name' => 'user_name',
                                            'user.id' => 'uid',
                                            'user.age' => 'user_age'
                                        ),
                                        Database::SYN_TABLE => 'user'
                                   ), array(Database::OPT_ASFIELD => true));

            $this->assertEquals ('SELECT [user].[name] AS [user_name],[user].[id] AS [uid],[user].[age] AS [user_age] FROM [user] AS [user]', $sql);
        }

        /**
         * 测试查找命令，字符串字段语法
         */
        public function testFindForStringFields () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->find (array(
                                        Database::SYN_FIELDS => '[user].[id],[user].[name],[user].[age]',
                                        Database::SYN_TABLE => 'user'
                                   ));

            $this->assertEquals ('SELECT [user].[id],[user].[name],[user].[age] FROM [user] AS [user]', $sql);
        }

        /**
         * 测试查找命令，字符串条件
         */
        public function testFindForStringWhere () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->find (array(
                                        Database::SYN_WHERE => '[user].[id] = 1',
                                        Database::SYN_TABLE => 'user'
                                   ));

            $this->assertEquals ('SELECT [user].[id],[user].[name],[user].[age],[user].[desc],[user].[user_group_id] FROM [user] AS [user] WHERE [user].[id] = 1', $sql);
        }

        /**
         * 测试查找命令，数组条件
         */
        public function testFindForArrayWhere () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->find (array(
                                        Database::SYN_WHERE => array('user.id' => 1),
                                        Database::SYN_TABLE => 'user'
                                   ));

            $this->assertEquals ('SELECT [user].[id],[user].[name],[user].[age],[user].[desc],[user].[user_group_id] FROM [user] AS [user] WHERE [user].[id] = (1)', $sql);
        }

        /**
         *  测试查找命令，joins语法
         */
        public function testFindForJoins () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->find (array(
                                        Database::SYN_JOINS => array('group'),
                                        Database::SYN_TABLE => 'user'
                                   ), array(Database::OPT_RELATION => array('group' => array('table' => 'user_group'))));

            $this->assertEquals ('SELECT [user].[id],[user].[name],[user].[age],[user].[desc],[user].[user_group_id],[user_group].[id],[user_group].[name] FROM [user] AS [user] LEFT JOIN [user_group] AS [user_group] ON [user].[user_group_id] = [user_group].[id]', $sql);
        }

        /**
         * 测试查找命令，joins alias语法
         */
        public function testFindForJoinsAlias () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->find (array(
                                        Database::SYN_JOINS => array('group'),
                                        Database::SYN_TABLE => 'user',
                                        Database::SYN_ALIAS => 'User'
                                   ), array(
                                           Database::OPT_RELATION => array(
                                               'group' => array(
                                                   'table' => 'user_group',
                                                   'alias' => 'Group'
                                               )
                                           )
                                      ));

            $this->assertEquals ('SELECT [User].[id],[User].[name],[User].[age],[User].[desc],[User].[user_group_id],[Group].[id],[Group].[name] FROM [user] AS [User] LEFT JOIN [user_group] AS [Group] ON [User].[user_group_id] = [Group].[id]', $sql);
        }

        /**
         * 测试查找命令，join on 语法
         */
        public function testFindForJoinsOn () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->find (array(
                                        Database::SYN_JOINS => array('group'),
                                        Database::SYN_TABLE => 'user'
                                   ), array(
                                           Database::OPT_RELATION => array(
                                               'group' => array(
                                                   'table' => 'user_group',
                                                   'on' => '[user].[group] = [group].[id]'
                                               )
                                           )
                                      ));

            $this->assertEquals ('SELECT [user].[id],[user].[name],[user].[age],[user].[desc],[user].[user_group_id],[user_group].[id],[user_group].[name] FROM [user] AS [user] LEFT JOIN [user_group] AS [user_group] ON [user].[group] = [group].[id]', $sql);
        }

        /**
         * 测试查找命令，joins fields语法
         */
        public function testFindForJoinsStringFields () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->find (array(
                                        Database::SYN_JOINS => array('group'),
                                        Database::SYN_TABLE => 'user'
                                   ), array(
                                           Database::OPT_RELATION => array(
                                               'group' => array(
                                                   'table' => 'user_group',
                                                   'fields' => '[user_group].[id]'
                                               )
                                           )
                                      ));

            $this->assertEquals ('SELECT [user].[id],[user].[name],[user].[age],[user].[desc],[user].[user_group_id],[user_group].[id] FROM [user] AS [user] LEFT JOIN [user_group] AS [user_group] ON [user].[user_group_id] = [user_group].[id]', $sql);
        }

        /**
         * 测试查找命令，joins 数组字段
         */
        public function testFindForJoinsArrayFields () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->find (array(
                                        Database::SYN_JOINS => array('group'),
                                        Database::SYN_TABLE => 'user'
                                   ), array(
                                           Database::OPT_RELATION => array(
                                               'group' => array(
                                                   'table' => 'user_group',
                                                   'fields' => array('user_group.id')
                                               )
                                           )
                                      ));

            $this->assertEquals ('SELECT [user].[id],[user].[name],[user].[age],[user].[desc],[user].[user_group_id],[user_group].[id] FROM [user] AS [user] LEFT JOIN [user_group] AS [user_group] ON [user].[user_group_id] = [user_group].[id]', $sql);
        }

        /**
         * 测试查找命令，joins数组别名字段
         */
        public function testFindForJoinsArrayAliasFields () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->find (array(
                                        Database::SYN_JOINS => array('group'),
                                        Database::SYN_TABLE => 'user'
                                   ), array(
                                           Database::OPT_RELATION => array(
                                               'group' => array(
                                                   'table' => 'user_group',
                                                   'fields' => array('user_group.id' => 'gid'),
                                                   Database::OPT_ASFIELD => true
                                               )
                                           )
                                      ));

            $this->assertEquals ('SELECT [user].[id],[user].[name],[user].[age],[user].[desc],[user].[user_group_id],[user_group].[id] AS [gid] FROM [user] AS [user] LEFT JOIN [user_group] AS [user_group] ON [user].[user_group_id] = [user_group].[id]', $sql);
        }

        /**
         * 测试查找命令，joins type语法
         */
        public function testFindForJoinsType () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->find (array(
                                        Database::SYN_TABLE => 'user',
                                        Database::SYN_JOINS => array('group')
                                   ), array(
                                           Database::OPT_RELATION => array(
                                               'group' => array(
                                                   'table' => 'user_group',
                                                   'join' => 'right'
                                               )
                                           )
                                      ));

            $this->assertEquals ('SELECT [user].[id],[user].[name],[user].[age],[user].[desc],[user].[user_group_id],[user_group].[id],[user_group].[name] FROM [user] AS [user] RIGHT JOIN [user_group] AS [user_group] ON [user].[user_group_id] = [user_group].[id]', $sql);
        }

        /**
         * 测试查找命令，joins foreign语法
         */
        public function testFindForJoinsForeign () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->find (array(
                                        Database::SYN_JOINS => array('group'),
                                        Database::SYN_TABLE => 'user'
                                   ), array(
                                           Database::OPT_RELATION => array(
                                               'group' => array(
                                                   'table' => 'user_group',
                                                   'foreign' => 'group'
                                               )
                                           )
                                      ));

            $this->assertEquals ('SELECT [user].[id],[user].[name],[user].[age],[user].[desc],[user].[user_group_id],[user_group].[id],[user_group].[name] FROM [user] AS [user] LEFT JOIN [user_group] AS [user_group] ON [user].[group] = [user_group].[id]', $sql);
        }

        /**
         * 测试查找命令，joins primary语法
         */
        public function testFindForJoinsPrimary () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->find (array(
                                        Database::SYN_JOINS => array('group'),
                                        Database::SYN_TABLE => 'user'
                                   ), array(
                                           Database::OPT_RELATION => array(
                                               'group' => array(
                                                   'table' => 'user_group',
                                                   'primary' => 'ids'
                                               )
                                           )
                                      ));

            $this->assertEquals ('SELECT [user].[id],[user].[name],[user].[age],[user].[desc],[user].[user_group_id],[user_group].[id],[user_group].[name] FROM [user] AS [user] LEFT JOIN [user_group] AS [user_group] ON [user].[user_group_id] = [user_group].[ids]', $sql);
        }

        /**
         * 测试查找命令，joins 多个内容
         */
        public function testFindJoinsMany () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->find (array(
                                        Database::SYN_JOINS => array(
                                            'category',
                                            'brand'
                                        ),
                                        Database::SYN_TABLE => 'product',
                                        Database::SYN_ALIAS => 'Product'
                                   ), array(
                                           'relation' => array(
                                               'category' => array(
                                                   'table' => 'category',
                                                   'foreign' => 'category'
                                               ),
                                               'brand' => array(
                                                   'table' => 'brand',
                                                   'foreign' => 'brand'
                                               )
                                           )
                                      ));

            $this->assertEquals ('SELECT [Product].[id],[Product].[name],[Product].[category],[Product].[brand],[category].[id],[category].[name],[brand].[id],[brand].[name] FROM [product] AS [Product] LEFT JOIN [category] AS [category] ON [Product].[category] = [category].[id] LEFT JOIN [brand] AS [brand] ON [Product].[brand] = [brand].[id]', $sql);
        }

        /**
         * 测试查找命令，order 语法
         */
        public function testFindForStringOrder () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->find (array(
                                        Database::SYN_ORDER => '[user].[id] ASC',
                                        'table' => 'user'
                                   ));

            $this->assertEquals ('SELECT [user].[id],[user].[name],[user].[age],[user].[desc],[user].[user_group_id] FROM [user] AS [user] ORDER BY [user].[id] ASC', $sql);
        }

        /**
         * 测试查找命令，order 数组语法
         */
        public function testFindForArrayOrder () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->find (array(
                                        Database::SYN_ORDER => array(
                                            'user.id' => 'ASC',
                                            'user.name' => 'DESC'
                                        ),
                                        'table' => 'user'
                                   ));

            $this->assertEquals ('SELECT [user].[id],[user].[name],[user].[age],[user].[desc],[user].[user_group_id] FROM [user] AS [user] ORDER BY [user].[id] ASC,[user].[name] DESC', $sql);
        }

        /**
         * 测试查找命令，limit 字符串语法
         */
        public function testFindForStringLimit () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->find (array(
                                        Database::SYN_LIMIT => '10,20',
                                        'table' => 'user'
                                   ));

            $this->assertEquals ('SELECT [user].[id],[user].[name],[user].[age],[user].[desc],[user].[user_group_id] FROM [user] AS [user] LIMIT 10,20', $sql);
        }

        /**
         * 测试查找命令，limit 数组语法
         */
        public function testFindForArrayLimit () {
            $builder = new SqlBuilderMock(new SqlBuilderDatabaseMock());
            $sql = $builder->find (array(
                                        Database::SYN_LIMIT => array(
                                            10,
                                            20
                                        ),
                                        'table' => 'user'
                                   ));

            $this->assertEquals ('SELECT [user].[id],[user].[name],[user].[age],[user].[desc],[user].[user_group_id] FROM [user] AS [user] LIMIT 10,20', $sql);
        }
    }
