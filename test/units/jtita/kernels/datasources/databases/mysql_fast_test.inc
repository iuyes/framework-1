<?php
    /**
     * mysql_fast_test.php MysqlFast单元测试类声明
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

    # 引用测试根目录
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (dirname (dirname (__FILE__))))))));

    # 引用测试依赖类
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');

    require_once(JTPATH_ROOT . '/jtita/kernels/datasources/sql_builder.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/datasources/database.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/datasources/datasource.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/datasources/databases/mysql_fast.inc');

    # 声明数据库信息
    define('DBNAME', 'jtfix');
    define('DBPASS', 'killer');

    /**
    CREATE DATABASE jtfix;
    use jtfix;

    CREATE TABLE `user`(
        `id`	INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
        `name`	VARCHAR(16) NOT NULL,
        `age`	INT	NOT NULL,
        `desc`	VARCHAR(256) NULL,
        `user_group_id`	int null
    );

    CREATE TABLE `user_group` (
        `id`  INT NOT NULL PRIMARY KEY,
        `name`	varchar(16) NOT NULL
    );

    CREATE TABLE `user_rights` (
        `rid`	INT NOT NULL,
        `name`	VARCHAR(10) NOT NULL
    );

    INSERT INTO user_group (id,name) VALUES(1,'admins'),(2,'guest');

    CREATE TABLE `dtschema`	(
        `id`			INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
        `fbit`			BIT(1) NULL,
        `ftinyint`		TINYINT  NULL,
        `fsmallint`		SMALLINT  NULL,
        `fmediumint`	MEDIUMINT  NULL,
        `fint`			INT  NULL,
        `finteger`		INTEGER  NULL,
        `fbigint`		BIGINT  NULL,
        `freal`			REAL(10,3) NULL,
        `fdouble`		DOUBLE(10,3) NULL,
        `ffloat`		FLOAT(10,3) NULL,
        `fdecimal`		DECIMAL(10,3) NULL,
        `fnumeric`		NUMERIC(10,3) NULL,
        `fdate`			DATE NULL,
        `ftime`			TIME NULL,
        `ftimestamp`	TIMESTAMP NULL,
        `fdatetime`		DATETIME NULL,
        `fyear`			YEAR	NULL,
        `fchar`			CHAR(10) NULL,
        `fvarchar`		VARCHAR(128) NULL,
        `fbinary`		BINARY(10) NULL,
        `fvarbinary`	VARBINARY(128) NULL,
        `ftinyblob`		TINYBLOB NULL,
        `fblob`			BLOB NULL,
        `fmediumblob`	MEDIUMBLOB NULL,
        `flongblob`		LONGBLOB NULL,
        `ftinytext`		TINYTEXT NULL,
        `ftext`			TEXT NULL,
        `fmediumtext`	MEDIUMTEXT NULL,
        `flongtext`		LONGTEXT NULL,
        `fenum`			ENUM('1','2','3','4') NULL,
        `fset`			SET('a','b','c') NULL
        );
        BIT[(length)]
        TINYINT[(length)] [UNSIGNED] [ZEROFILL],SMALLINT[(length)] [UNSIGNED] [ZEROFILL]
        MEDIUMINT[(length)] [UNSIGNED] [ZEROFILL],INT[(length)] [UNSIGNED] [ZEROFILL]
        INTEGER[(length)] [UNSIGNED] [ZEROFILL],BIGINT[(length)] [UNSIGNED] [ZEROFILL]
        REAL[(length,decimals)] [UNSIGNED] [ZEROFILL],DOUBLE[(length,decimals)] [UNSIGNED] [ZEROFILL]
        FLOAT[(length,decimals)] [UNSIGNED] [ZEROFILL],DECIMAL[(length[,decimals])] [UNSIGNED] [ZEROFILL]
        NUMERIC[(length[,decimals])] [UNSIGNED] [ZEROFILL],DATE,TIME,TIMESTAMP,DATETIME,YEAR,CHAR[(length)]
        VARCHAR(length),BINARY[(length)],VARBINARY(length),TINYBLOB,BLOB,MEDIUMBLOB,LONGBLOB
        TINYTEXT [BINARY],TEXT [BINARY],MEDIUMTEXT [BINARY],LONGTEXT [BINARY],ENUM(value1,value2,value3,...),
        SET(value1,value2,value3,...)
     */

    /**
     * MysqlFast测试类，提供对MysqlFast的单元测试服务
     *
     * @package jtita.kernels.datasources.databases
     */
    class MysqlFastTest extends PHPUnit_Framework_TestCase{
        /**
         * 测试初始化
         */
        public function testInitialize () {
            $mysql = Jtita::ioc(array('class' => 'mysql_fast',
                                      'dbpass' => DBPASS,
                                      'dbname' => DBNAME,
                                      'initialize' => true));

            $this->assertTrue ($mysql->initialized ());
        }

        /**
         * 测试初始化，发生错误
         *
         * @expectedException DatasourceException
         */
        public function testInitializeFail () {
            $mysql = Jtita::ioc(array('class' => 'mysql_fast',
                                      'dbpass' => 'error_pass',
                                      'dbname' => DBNAME,
                                      'initialize' => true));
        }

        /**
         * 测试执行命令
         */
        public function testExecute () {
            $mysql = $this->_mysql ();

            $row = $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`) VALUES(\'zhenggp\',18,\'Hello world\')');
            $this->assertEquals (1, $row);
        }

        /**
         * 测试执行命令，发生错误
         *
         * @expectedException DatasourceException
         */
        public function testExecuteForError () {
            $mysql = $this->_mysql ();

            $mysql->execute ('insert into DD');
        }

        /**
         * 测试执行命令，带参数
         */
        public function testExecuteForArgs () {
            $mysql = $this->_mysql ();

            $row = $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`) VALUE(:name,:age,:desc)', array(':name' => 'zhenggp',
                                                                                                             ':age' => 18,
                                                                                                             ':desc' => 'Hello world'));

            $this->assertEquals (1, $row);
        }

        /**
         * 测试执行命令，关键字参数
         */
        public function testExecuteForKeyArgs () {
            $mysql = $this->_mysql ();

            $row = $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`) VALUE(:name,:age,:desc)', array(':name' => 'chenjuan',
                                                                                                             ':age' => 18,
                                                                                                             ':desc' => ";DELETE FROM user;'"));

            $this->assertEquals (1, $row);
        }

        /**
         * 测试执行命令，参数未提供
         *
         * @expectedException DatasourceException
         */
        public function testExecuteForArgsNotSupportArgs () {
            $mysql = $this->_mysql ();

            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`) VALUE(:name,:age,:desc)');
        }

        /**
         * 测试执行查询
         *
         * @depends testExecute
         */
        public function testQuery () {
            $mysql = $this->_mysql ();

            $mysql->execute ('DELETE FROM `user`');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`) VALUES(\'zhenggp\',30,\'heo\')');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`) VALUES(\'chengjuan\',30,\'heo\')');

            $rows = $mysql->query ('SELECT `name`,`age`,`desc` FROM `user`');

            $this->assertEquals (2, count ($rows));
            $this->assertEquals ('zhenggp', $rows[0]['name']);
            $this->assertEquals ('chengjuan', $rows[1]['name']);
        }

        /**
         * 测试查询，带参数
         *
         * @depends testExecute
         */
        public function testQueryForArgs () {
            $mysql = $this->_mysql ();

            $mysql->execute ('DELETE FROM `user`');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`) VALUES(\'zhenggp\',30,\'heo\'),(\'chengjuan\',26,\'heo\')');

            $rows = $mysql->query ('SELECT `name`,`age`,`desc` FROM `user` WHERE `age` = :age', array(':age' => 26));

            $this->assertEquals (1, count ($rows));
            $this->assertEquals ('chengjuan', $rows[0]['name']);
        }

        /**
         * 测试查询，带自定义参数
         */
        public function testQueryForKeyArgs () {
            $mysql = $this->_mysql ();

            $mysql->execute ('DELETE FROM `user`');
            $row = $mysql->query ('SELECT `name`,`age`,`desc` FROM `user` WHERE `desc` = :desc', array(':desc' => ";DELETE FROM user;'"));

            $this->assertEquals (0, count ($row));
        }


        /**
         * 测试查询，参数未提供
         *
         * @depends testExecute
         * @expectedException DatasourceException
         */
        public function testQueryForArgsNotSupport () {
            $mysql = $this->_mysql ();

            $rows = $mysql->query ('SELECT `name`,`age`,`desc` FROM `user` WHERE where age = :age');
        }

        /**
         * 测试查询，模型格式选项
         *
         * @depends testExecute
         */
        public function testQueryForModelFormatOptions () {
            $mysql = $this->_mysql ();

            $mysql->execute ('DELETE FROM `user`');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`,`user_group_id`) VALUES(\'zhenggp\',30,\'heo\',1),(\'chenjuan\',26,\'heo\',2)');

            $rows = $mysql->query ('SELECT `user`.`name`,`user`.`age`,`user`.`desc`,`group`.`name` as `gname` FROM `user` LEFT JOIN `user_group` as `group` ON `user`.`user_group_id` = `group`.`id`', array(), array(Database::OPT_FORMAT => Database::OPT_FORMAT_MODEL));

            # 声明期待数组
            $actualRows = array(array('user' => array('name' => 'zhenggp', 'age' => 30, 'desc' => 'heo'),
                                      'group' => array('gname' => 'admins')),
                                array('user' => array('name' => 'chenjuan', 'age' => 26, 'desc' => 'heo'),
                                      'group' => array('gname' => 'guest')));

            $this->assertEquals (2, count ($rows));
            $this->assertEquals (count ($actualRows), count ($rows));
            $this->assertEquals (count ($actualRows[0]), count ($rows[0]));
            $this->assertEquals (count ($actualRows[0]['user']), count ($rows[0]['user']));
            $this->assertEquals (count ($actualRows[0]['group']), count ($rows[0]['group']));

            $this->assertEquals (count ($actualRows[1]), count ($rows[1]));
            $this->assertEquals (count ($actualRows[1]['user']), count ($rows[1]['user']));
            $this->assertEquals (count ($actualRows[1]['group']), count ($rows[1]['group']));
        }

        /**
         * 测试查询，使用列表格式选项
         *
         * @depends testExecute
         */
        public function testQueryForListFormatOptions () {
            $mysql = $this->_mysql ();

            $mysql->execute ('DELETE FROM `user`');
            $rows = $mysql->query ('SELECT `group`.`id`,`group`.`name` FROM `user_group` as `group`', array(), array(Database::OPT_FORMAT => Database::OPT_FORMAT_LIST));

            #$actualRows = array(1 => 'admins', 2 => 'guest');

            $this->assertEquals('admins',$rows[1]);
            $this->assertEquals('guest',$rows[2]);
        }

        /**
         * 测试查询，使用json格式选项
         *
         * @depends testExecute
         */
        public function testQueryForJsonFormatOptions () {
            $mysql = $this->_mysql ();

            $mysql->execute ('DELETE FROM `user`');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`,`user_group_id`) VALUES(\'zhenggp\',30,\'heo\',1),(\'chenjuan\',26,\'heo\',2)');
            $rows = $mysql->query ('SELECT `user`.`name`,`user`.`age`,`user`.`desc`,`group`.`name` as `gname` FROM `user` LEFT JOIN `user_group` as `group` ON `user`.`user_group_id` = `group`.`id`', array(), array(Database::OPT_FORMAT => Database::OPT_FORMAT_JSON));

            $this->assertEquals ('[{"name":"zhenggp","age":"30","desc":"heo","gname":"admins"},{"name":"chenjuan","age":"26","desc":"heo","gname":"guest"}]', $rows);
        }

        /**
         * 测试查询，使用Xml格式选项
         */
        public function testQueryForXmlFormatOptions () {
            # $this->fail();
        }

        /**
         * 测试查询，发生错误
         *
         * @depends testExecute
         * @expectedException DatasourceException
         */
        public function testQueryForError () {
            $mysql = $this->_mysql ();

            $rows = $mysql->query ('SELECT `name`,`age`,`desc` FROM `user` WHERE ', array(':age' => 26));
        }

        /**
         * 测试数据schema
         */
        public function testDataSchema () {
            $mysql = $this->_mysql ();
            $schema = $mysql->schema ('dtschema');

            $this->assertEquals (32, count ($schema));

            $this->assertEquals (DT::Integer, $schema['id'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Boolean, $schema['fbit'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Integer, $schema['ftinyint'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Integer, $schema['fsmallint'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Integer, $schema['fmediumint'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Integer, $schema['fint'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Integer, $schema['finteger'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Integer, $schema['fbigint'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Float, $schema['freal'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Float, $schema['fdouble'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Decimal, $schema['fdecimal'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Decimal, $schema['fnumeric'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Date, $schema['fdate'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Time, $schema['ftime'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::DateTime, $schema['ftimestamp'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::DateTime, $schema['fdatetime'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Integer, $schema['fyear'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::String, $schema['fchar'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::String, $schema['fvarchar'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Bold, $schema['fbinary'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Bold, $schema['fvarbinary'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Bold, $schema['ftinyblob'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Bold, $schema['fmediumblob'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Bold, $schema['flongblob'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Text, $schema['ftinytext'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Text, $schema['ftext'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Text, $schema['fmediumtext'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Text, $schema['flongtext'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Other, $schema['fenum'][Database::SCHEMA_DTTYPE]);
            $this->assertEquals (DT::Other, $schema['fset'][Database::SCHEMA_DTTYPE]);
        }

        /**
         * 测试schema，数据表不存在
         *
         * @expectedException DatasourceException
         */
        public function testDataSchemaForDataNameNotExists () {
            $mysql = $this->_mysql ();
            $mysql->schema ('not_exists');
        }

        /**
         * 测试获取查询语法集
         */
        public function testFindSyntax () {
            $mysql = $this->_mysql ();
            $syntax = $mysql->findSyntax ();

            $this->assertTrue (is_array ($syntax));
        }

        /**
         * 测试查找数据
         *
         * @depends testExecute
         */
        public function testFindForAll () {
            $mysql = $this->_mysql ();

            # 初始化数据
            $mysql->execute ('DELETE FROM `user`');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`) VALUES(\'zhenggp\',18,\'programer\'),(\'ChenJuan\',18,\'programer\');');

            $rows = $mysql->find (array('table' => 'user'));

            $this->assertEquals (2, count ($rows));
            $this->assertEquals (5, count ($rows[0]));
            $this->assertEquals (5, count ($rows[1]));

            $this->assertEquals ('zhenggp', $rows[0]['name']);
            $this->assertEquals (18, $rows[0]['age']);
            $this->assertEquals ('programer', $rows[0]['desc']);

            $this->assertEquals ('ChenJuan', $rows[1]['name']);
            $this->assertEquals (18, $rows[1]['age']);
            $this->assertEquals ('programer', $rows[1]['desc']);
        }

        /**
         * 测试查找，未提供表语法
         *
         * @expectedException DatasourceException
         */
        public function testFindForNotExistTable () {
            $mysql = $this->_mysql ();
            $mysql->find (array(), array());
        }

        /**
         * 测试查找，指定数组字段
         *
         * @depends testExecute
         */
        public function testFindForArrayFields () {
            $mysql = $this->_mysql ();

            # 执行测试初始化
            $mysql->execute ('DELETE FROM `user`');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`) VALUES(\'zhenggp\',18,\'programer\'),(\'Chen juan\',18,\'programer\');');

            $rows = $mysql->find (array('fields' => array('name', 'desc'), 'table' => 'user'));

            $this->assertEquals (2, count ($rows));
            $this->assertEquals (2, count ($rows[0]));
            $this->assertEquals (2, count ($rows[1]));
        }

        /**
         * 执行测试，指定表别名
         *
         * @depends testExecute
         * @depends testFindForArrayFields
         */
        public function testFindForTableAlias () {
            $mysql = $this->_mysql ();

            # 初始化测试数据
            $mysql->execute ('DELETE FROM `user`');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`,`user_group_id`) VALUES(\'zhenggp\',18,\'programer\',1),(\'Chen juan\',18,\'programer\',2);');

            $rows = $mysql->find (array('fields' => array('User.name', 'User.desc'), 'table' => 'user',
                                        'alias' => 'User'));

            $this->assertEquals (2, count ($rows));
            $this->assertEquals (2, count ($rows[0]));
            $this->assertEquals (2, count ($rows[1]));
        }

        /**
         * 测试查找，指定别名字段选项
         *
         * @depends testExecute
         */
        public function testFindForAsFieldOptions () {
            $mysql = $this->_mysql ();
            # 初始化测试数据
            $mysql->execute ('DELETE FROM `user`');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`,`user_group_id`) VALUES(\'zhenggp\',18,\'programmer\',1),(\'Chen juan\',18,\'programmer\',2);');

            $rows = $mysql->find (array('fields' => array('name' => 'user:name', 'desc' => 'user:desc'),
                                        'table' => 'user'), array('asfield' => true));

            $this->assertEquals (2, count ($rows));
            $this->assertEquals (2, count ($rows[0]));
            $this->assertEquals (2, count ($rows[1]));

            $this->assertEquals ('zhenggp', $rows[0]['user:name']);
            $this->assertEquals ('programmer', $rows[0]['user:desc']);

            $this->assertEquals ('Chen juan', $rows[1]['user:name']);
            $this->assertEquals ('programmer', $rows[1]['user:desc']);
        }

        /**
         * 测试查找，使用字符串字段
         *
         * @depends testExecute
         */
        public function testFindForStringFields () {
            $mysql = $this->_mysql ();

            # 初始化字段
            $mysql->execute ('DELETE FROM `user`');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`) VALUES(\'zhenggp\',18,\'programmer\'),(\'Chen juan\',18,\'programmer\');');

            $rows = $mysql->find (array('fields' => '`name`,`desc`', 'table' => 'user'));

            $this->assertEquals (2, count ($rows));
            $this->assertEquals (2, count ($rows[0]));
            $this->assertEquals (2, count ($rows[1]));
        }

        /**
         * 测试查找，指定多张表
         *
         * @depends testExecute
         * @depends testQueryForModelFormatOptions
         * @depends testFindForArrayFields
         */
        public function testFindForJoins () {
            $mysql = $this->_mysql ();

            # 初始化数据关系
            $mysql->execute ('DELETE FROM `user`');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`,`user_group_id`) VALUES(\'zhenggp\',18,\'programer\',1),(\'Chen juan\',18,\'programer\',2);');

            $rows = $mysql->find (array('joins' => array('group'),
                                        'fields' => array('User.name', 'User.age', 'User.desc', 'user_group.name'),
                                        'table' => 'user',
                                        'alias' => 'User'), array(Database::OPT_FORMAT => Database::OPT_FORMAT_MODEL,
                                                                  'relation' => array('group' => array('table' => 'user_group'))));

            $actualRows = array(array('User' => array('name' => 'zhenggp', 'age' => 18, 'desc' => 'programer'),
                                      'user_group' => array('name' => 'admins')),
                                array('user' => array('name' => 'Chen juan', 'age' => 18, 'desc' => 'programer'),
                                      'user_group' => array('name' => 'guest')));

            $this->assertEquals (count ($actualRows), count ($rows));
            $this->assertEquals (count ($actualRows[0]), count ($rows[0]));
            $this->assertEquals (count ($actualRows[0]['User']), count ($rows[0]['User']));
            $this->assertEquals (count ($actualRows[0]['user_group']), count ($rows[0]['user_group']));
        }

        /**
         * 测试查询，默认连接字段
         *
         * @depends testExecute
         * @depends testFindForJoins
         */
        public function testFindForJoinsDefaultFields () {
            $mysql = $this->_mysql ();

            # 初始化测试数据
            $mysql->execute ('DELETE FROM `user`');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`,`user_group_id`) VALUES(\'zhenggp\',18,\'programer\',1),(\'Chen juan\',18,\'programer\',2);');

            $rows = $mysql->find (array('joins' => array('group'), 'table' => 'user',
                                        'alias' => 'User'), array(Database::OPT_FORMAT => Database::OPT_FORMAT_MODEL,
                                                                  'relation' => array('group' => array('table' => 'user_group'))));

            $this->assertEquals (2, count ($rows));
            $this->assertEquals (2, count ($rows[0]));
            $this->assertEquals (5, count ($rows[0]['User']));
            $this->assertEquals (2, count ($rows[0]['user_group']));
        }

        /**
         * 测试查找，连接表别名
         *
         * @depends testExecute
         */
        public function testFindForJoinsAlias () {
            $mysql = $this->_mysql ();

            # 初始化测试数据
            $mysql->execute ('DELETE FROM `user`');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`,`user_group_id`) VALUES(\'zhenggp\',18,\'programer\',1),(\'Chen juan\',18,\'programer\',2);');

            $rows = $mysql->find (array('joins' => array('group'), 'table' => 'user',
                                        'alias' => 'User'), array(Database::OPT_FORMAT => Database::OPT_FORMAT_MODEL,
                                                                  'relation' => array('group' => array('table' => 'user_group',
                                                                                                       'alias' => 'Group'))));

            $this->assertEquals (2, count ($rows));
            $this->assertEquals (2, count ($rows[0]));
            $this->assertEquals (5, count ($rows[0]['User']));
            $this->assertEquals (2, count ($rows[0]['Group']));
        }

        /**
         * 测试查询，指定数组外键字段
         *
         * @depends testExecute
         * @depends testFindForJoins
         */
        public function testFindForJoinsArrayFields () {
            $mysql = $this->_mysql ();

            # 初始化测试数据
            $mysql->execute ('DELETE FROM `user`');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`,`user_group_id`) VALUES(\'zhenggp\',18,\'programer\',1),(\'Chen juan\',18,\'programer\',2);');

            $rows = $mysql->find (array('joins' => array('group'), 'table' => 'user',
                                        'alias' => 'User'), array(Database::OPT_FORMAT => Database::OPT_FORMAT_MODEL,
                                                                  'relation' => array('group' => array('table' => 'user_group',
                                                                                                       'fields' => array('user_group.name')))));

            $this->assertEquals (2, count ($rows));
            $this->assertEquals (2, count ($rows[0]));
            $this->assertEquals (5, count ($rows[0]['User']));
            $this->assertEquals (1, count ($rows[0]['user_group']));
        }


        /**
         * 测试查询，字符串连接字段
         *
         * @depends testExecute
         * @depends testFindForJoins
         */
        public function testFindForJoinsStringFields () {
            $mysql = $this->_mysql ();

            # 初始化测试数据
            $mysql->execute ('DELETE FROM `user`');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`,`user_group_id`) VALUES(\'zhenggp\',18,\'programer\',1),(\'Chen juan\',18,\'programer\',2);');

            $rows = $mysql->find (array('joins' => array('group'), 'table' => 'user',
                                        'alias' => 'User'), array(Database::OPT_FORMAT => Database::OPT_FORMAT_MODEL,
                                                                  'relation' => array('group' => array('table' => 'user_group',
                                                                                                       'fields' => '`user_group`.`name`'))));

            $this->assertEquals (2, count ($rows));
            $this->assertEquals (2, count ($rows[0]));
            $this->assertEquals (5, count ($rows[0]['User']));
            $this->assertEquals (1, count ($rows[0]['user_group']));
        }

        /**
         * 测试查询，使用别外字段选项
         *
         * @depends testExecute
         * @depends testFindForJoins
         */
        public function testFindForJoinsAliasFields () {
            $mysql = $this->_mysql ();

            # 初始化测试数据
            $mysql->execute ('DELETE FROM `user`');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`,`user_group_id`) VALUES(\'zhenggp\',18,\'programer\',1),(\'Chen juan\',18,\'programer\',2);');

            $rows = $mysql->find (array('joins' => array('group'), 'table' => 'user',
                                        'alias' => 'User'), array('relation' => array('group' => array('table' => 'user_group',
                                                                                                       'fields' => array('user_group.name' => 'gname'),
                                                                                                       'asfield' => true))));

            # 测试验证数据
            $this->assertEquals (2, count ($rows));
            $this->assertEquals (6, count ($rows[0]));
        }

        /**
         * 测试查询，指定查询type
         *
         * @depends testExecute
         * @depends testFindForJoins
         */
        public function testFindForJoinsType () {
            $mysql = $this->_mysql ();

            # 初始化测试数据
            $mysql->execute ('DELETE FROM `user`');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`,`user_group_id`) VALUES(\'zhenggp\',18,\'programer\',1),(\'Chen juan\',18,\'programer\',null);');

            $rows = $mysql->find (array('joins' => array('group'), 'table' => 'user',
                                        'alias' => 'User'), array(Database::OPT_FORMAT => Database::OPT_FORMAT_MODEL,
                                                                  'relation' => array('group' => array('table' => 'user_group',
                                                                                                       'join' => 'inner'))));

            # 执行测试验证
            $this->assertEquals (1, count ($rows));
        }

        /**
         * 测试查找，使用joinOn选项
         *
         * @depends testExecute
         * @depends testFindForJoins
         */
        public function testFindForJoinsOn () {
            $mysql = $this->_mysql ();

            # 初始化测试数据
            $mysql->execute ('DELETE FROM `user`');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`,`user_group_id`) VALUES(\'zhenggp\',18,\'programer\',1),(\'Chen juan\',18,\'programer\',null);');

            $rows = $mysql->find (array('joins' => array('group'), 'table' => 'user',
                                        'alias' => 'User'), array(Database::OPT_FORMAT => Database::OPT_FORMAT_MODEL,
                                                                  'relation' => array('group' => array('table' => 'user_group',
                                                                                                       'on' => '`User`.`user_group_id` = `user_group`.`id`'))));

            # 验证查询结果
            $this->assertEquals (2, count ($rows));
        }

        /**
         * 测试查找指定字符串条件
         *
         * @depends testExecute
         */
        public function testFindForStringWhere () {
            $mysql = $this->_mysql ();

            $mysql->execute ('DELETE FROM `user`');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`,`user_group_id`) VALUES(\'zhenggp\',18,\'programer\',1),(\'Chen juan\',18,\'programer\',null);');

            $rows = $mysql->find (array('where' => 'user.name = \'zhenggp\'', 'table' => 'user'));

            $this->assertEquals (1, count ($rows));
            $this->assertEquals ('zhenggp', $rows[0]['name']);
            $this->assertEquals (18, $rows[0]['age']);
        }

        /**
         * 测试查找，指定参数条件
         *
         * @depends testExecute
         */
        public function testFindForStringArgsWhere () {
            $mysql = $this->_mysql ();

            $mysql->execute ('DELETE FROM `user`');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`,`user_group_id`) VALUES(\'zhenggp\',18,\'programer\',1),(\'Chen juan\',18,\'programer\',null);');

            $rows = $mysql->find (array('where' => array('user.name = :name', ':name' => 'zhenggp'),
                                        'table' => 'user'));

            $this->assertEquals (1, count ($rows));
            $this->assertEquals ('zhenggp', $rows[0]['name']);
            $this->assertEquals (18, $rows[0]['age']);
        }

        /**
         * 测试查找，使用数组条件
         *
         * @depends testExecute
         */
        public function testFindForArrayWhere () {
            $mysql = $this->_mysql ();

            $mysql->execute ('DELETE FROM `user`');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`,`user_group_id`) VALUES(\'zhenggp\',18,\'programer\',1),(\'Chen juan\',18,\'programer\',null);');

            $rows = $mysql->find (array('where' => array('user.name' => 'zhenggp'), 'table' => 'user'));

            $this->assertEquals (1, count ($rows));
            $this->assertEquals ('zhenggp', $rows[0]['name']);
            $this->assertEquals (18, $rows[0]['age']);
        }

        /**
         * 测试创建
         *
         * @depends testExecute
         * @depends testQuery
         */
        public function testCreate () {
            $mysql = $this->_mysql ();
            $mysql->execute ('DELETE FROM `user`');

            $mysql->create (array('name' => 'zhenggp', 'age' => 18, 'desc' => 'programmer',
                                  'user_group_id' => 1), array('table' => 'user'));
            $rows = $mysql->query ('SELECT `name`,`age`,`desc`,`user_group_id` FROM `user`');

            $this->assertEquals (1, count ($rows));
        }

        /**
         * 测试更新
         */
        public function testUpdate () {
            $mysql = $this->_mysql ();
            $mysql->execute ('DELETE FROM `user`');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`,`user_group_id`) VALUES(\'zhenggp\',18,\'programer\',1),(\'Chen juan\',18,\'programer\',null);');
            $count = $mysql->update (array('name' => 'test'), array('table' => 'user', 'alias' => 'User',
                                                                    'where' => array('User.name' => 'zhenggp')));

            $rows = $mysql->query ('SELECT * FROM `user` WHERE `name` = \'test\'');
            $this->assertEquals (1, $count);
            $this->assertEquals (1, count ($rows));
        }

        /**
         * 测试删除
         *
         * @depends testExecute
         * @depends testQuery
         */
        public function testDelete () {
            $mysql = $this->_mysql ();
            $mysql->execute ('DELETE FROM `user`');
            $mysql->execute ('INSERT INTO `user`(`name`,`age`,`desc`,`user_group_id`) VALUES(\'zhenggp\',18,\'programer\',1),(\'Chen juan\',18,\'programer\',null);');
            $count = $mysql->delete (array('table' => 'user'));
            $rows = $mysql->query ('SELECT * FROM `user`');

            $this->assertEquals (2, $count);
            $this->assertEmpty ($rows);
        }

        /**
         * 初始化MysqlFast测试对象
         */
        private function _mysql () {
            $mysql = Jtita::ioc(array('class' => 'mysql_fast','initialize' => true,
                                      'dbpass' => DBPASS, 'dbname' => DBNAME));

            return $mysql;
        }
    }