<?php
    /**
     * jtita_test.inc 提供架构核心服务Jtita类单元测试服务。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels
     * @version     1.0 beta
     *
     * @create      2012-06-12
     * @modify      2013-04-26
     */

    # 根路径
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (__FILE__))))));
    # 测试导入模拟路径
    define ('JTMOCK_PATH', dirname (__FILE__) . '/app_mock');

    # 导入被测试类。
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/behaviors.inc');

    /**
     * jtita_test 单元测试服务。
     *
     * @package jtita.kernels
     */
    class JtitaTest extends PHPUnit_Framework_TestCase {
        /**
         * 测试路径默认设置。
         */
        public function testPathsForDefault () {
            $paths = Jtita::paths ();

            $this->assertTrue (is_array ($paths));
            $this->assertEmpty ($paths);
        }

        /**
         * 测试路径，设置为字符串。
         */
        public function testPathsForSetString () {
            $paths = Jtita::paths ('/home/jtita');

            $this->assertTrue (is_array ($paths));
            $this->assertEquals (1, count ($paths));
            $this->assertEquals ('/home/jtita', $paths[0]);
        }

        /**
         * 测试路径，设置数组值。
         */
        public function testPathsForSetArray () {
            $paths = Jtita::paths (array('/home/jtita', '/home/app'));

            $this->assertTrue (is_array ($paths));
            $this->assertEquals (2, count ($paths));
            $this->assertEquals ('/home/jtita', $paths[0]);
            $this->assertEquals ('/home/app', $paths[1]);
        }

        /**
         * 测试Lookup简单文件；
         */
        public function testLookupForSimpleFile () {
            Jtita::paths (JTMOCK_PATH);

            $this->assertEquals (path_replace (JTMOCK_PATH . '/simple.inc'), path_replace (Jtita::lookup ('simple')));
        }

        /**
         * 测试Lookup包内部文件；
         */
        public function testLookupForPackageFile () {
            Jtita::paths (JTMOCK_PATH);

            $this->assertEquals (path_replace (JTMOCK_PATH . '/jtita/kernels/package_file.inc'), path_replace (Jtita::lookup ('jtita.kernels.package_file')));
        }

        /**
         * 测试Lookup优先级文件；
         */
        public function testLookupForHighPriorityFile () {
            Jtita::paths (array(JTMOCK_PATH . '/app', JTMOCK_PATH . '/jtita'));

            $this->assertEquals (path_replace (JTMOCK_PATH . '/app/kernels/high_priority_file.inc'), path_replace (Jtita::lookup ('kernels.high_priority_file')));
        }

        /**
         * 测试Lookup低优先级文件；
         */
        public function testLookupForLowerPriorityFile () {
            Jtita::paths (array(JTMOCK_PATH . '/app', JTMOCK_PATH . '/jtita'));

            $this->assertEquals (path_replace (JTMOCK_PATH . '/jtita/kernels/lower_priority_file.inc'), path_replace (Jtita::lookup ('kernels.lower_priority_file')));
        }

        /**
         * 测试Lookup包含文件扩展名；
         */
        public function testLookupForFileExt () {
            Jtita::paths (JTMOCK_PATH);

            $this->assertEquals (path_replace (JTMOCK_PATH . '/app/file_ext.inc'), path_replace (Jtita::lookup ('app.file_ext', '.inc')));
            $this->assertEquals (path_replace (JTMOCK_PATH . '/app/file_ext.php'), path_replace (Jtita::lookup ('app.file_ext', '.php')));
        }

        /**
         * 测试查找文件，字符串路径；
         */
        public function testLookupForStringPaths () {
            Jtita::paths (array());

            $this->assertEquals (path_replace (JTMOCK_PATH . '/jtita/kernels/lower_priority_file.inc'), path_replace (Jtita::lookup ('kernels.lower_priority_file', '.inc', JTMOCK_PATH . '\jtita')));
        }

        /**
         * 测试查找文件，数组路径；
         */
        public function testLookupForArrayPaths () {
            Jtita::paths (array());

            $this->assertEquals (path_replace (JTMOCK_PATH . '/app/kernels/high_priority_file.inc'), path_replace (Jtita::lookup ('kernels.high_priority_file', '.inc', array(JTMOCK_PATH . '/app',
                                                                                                                                                                              JTMOCK_PATH . '/jtita'))));
        }

        /**
         * 测试Lookup不存在的文件；
         */
        public function testLookupForNotExistsFile () {
            Jtita::paths (array(JTMOCK_PATH . '/app', JTMOCK_PATH . '/jtita'));

            $this->assertFalse (Jtita::lookup ('not_exists_file'));
        }

        /**
         * 测试Import简单文件；
         */
        public function testImportForSimpleFile () {
            Jtita::paths (JTMOCK_PATH);

            $this->assertFalse (class_exists ('Simple'));

            $this->assertTrue (! ! Jtita::import ('simple'));
            $this->assertTrue (class_exists ('Simple'));
        }

        /**
         * 测试Import重复文件；
         */
        public function testImportForRepeatFile () {
            Jtita::paths (JTMOCK_PATH);

            $this->assertFalse (class_exists ('Simple1'));
            $this->assertTrue (! ! Jtita::import ('simple1'));

            $this->assertFalse (Jtita::import ('simple1'));
            $this->assertTrue (class_exists ('Simple1'));
        }

        /**
         * 测试Import包内部文件；
         */
        public function testImportForPackageFile () {
            Jtita::paths (JTMOCK_PATH);

            $this->assertTrue (! ! Jtita::import ('jtita.kernels.package_file'));
            $this->assertTrue (class_exists ('PackageFile'));
        }

        /**
         * 测试Import高优先文件；
         *
         * @depends testPathsForSetArray
         * @depends testImportForRepeatFile
         * @depends testImportForPackageFile
         */
        public function testImportForHighPriorityFile () {
            Jtita::paths (array(JTMOCK_PATH . '/app', JTMOCK_PATH . '/jtita'));

            $this->assertFalse (class_exists ('HighPriorityFile'));
            $this->assertTrue (! ! Jtita::import ('kernels.high_priority_file'));

            $this->assertTrue (class_exists ('HighPriorityFile'));
            $this->assertEquals ('app.kernels', HighPriorityFile::PATH);
        }

        /**
         * 测试Import低先文件；
         *
         * @depends testPathsForSetArray
         * @depends testImportForRepeatFile
         * @depends testImportForPackageFile
         */
        public function testImportForLowerPriorityFile () {
            Jtita::paths (array(JTMOCK_PATH . '/app', JTMOCK_PATH . '/jtita'));

            $this->assertFalse (class_exists ('LowerPriorityFile'));
            $this->assertTrue (! ! Jtita::import ('kernels.lower_priority_file'));

            $this->assertTrue (class_exists ('LowerPriorityFile'));
            $this->assertEquals ('jtita.kernels', LowerPriorityFile::PATH);
        }

        /**
         * 测试导入使用扩展名；
         *
         * @depends testPathsForSetString
         * @depends testImportForPackageFile
         * @depends testImportForRepeatFile
         */
        public function testImportForFileExt () {
            Jtita::paths (JTMOCK_PATH);

            $this->assertFalse (class_exists ('FileExt'));
            $this->assertTrue (! ! Jtita::import ('app.file_ext', '.inc'));

            $this->assertTrue (class_exists ('FileExt'));
            $this->assertFalse (Jtita::import ('app.file_ext', '.inc'));
            $this->assertTrue (! ! Jtita::import ('app.file_ext', '.php'));
        }

        /**
         * 测试import使用字条串路径；
         */
        public function testImportForStringPath () {
            $this->assertFalse (class_exists ('PathLowerPriorityFile'));
            $this->assertTrue (! ! Jtita::import ('kernels.path_lower_priority_file', '.inc', JTMOCK_PATH . '/jtita'));

            $this->assertTrue (class_exists ('PathLowerPriorityFile'));
            $this->assertEquals ('jtita.kernels', PathLowerPriorityFile::PATH);

            $this->assertFalse (Jtita::import ('kernels.path_lower_priority_file', '.inc', JTMOCK_PATH . '/jtita'));
        }

        /**
         * 测试import使用数组路径；
         */
        public function testImportForArrayPath () {
            $this->assertFalse (class_exists ('PathHighPriorityFile'));
            $this->assertTrue (! ! Jtita::import ('kernels.path_high_priority_file', '.inc', array(JTMOCK_PATH . '/app',
                                                                                                   JTMOCK_PATH . '/jtita')));

            $this->assertTrue (class_exists ('PathHighPriorityFile'));
            $this->assertEquals ('app.kernels', PathHighPriorityFile::PATH);

            $this->assertFalse (Jtita::import ('kernels.path_high_priority_file', '.inc', array(JTMOCK_PATH . '/app',
                                                                                                JTMOCK_PATH . '/jtita')));
        }

        /**
         * 测试import文件名相同，路径不同；
         */
        public function testImportForSameFileDifferPath () {
            $this->assertFalse (class_exists ('AppDefferPath'));
            $this->assertTrue (! ! Jtita::import ('kernels.deffer_path', '.inc', JTMOCK_PATH . '/app'));
            $this->assertFalse (Jtita::import ('kernels.deffer_path', '.inc', JTMOCK_PATH . '/jtita'));

            $this->assertTrue (class_exists ('AppDefferPath'));
            $this->assertFalse (class_exists ('JtitaDefferPath'));
        }

        /**
         * 导入具有返回值的文件。
         */
        public function testImportForReturnFile () {
            Jtita::paths (JTMOCK_PATH);
            $result = Jtita::import ('app.configs.settings');

            $this->assertTrue (is_array ($result));
            $this->assertEquals (3, count ($result));

            $this->assertEquals ('Hello World', $result[0]);
            $this->assertEquals ('Hello PHP', $result[1]);
            $this->assertEquals ('Hello JTITA', $result[2]);
        }

        /**
         * 测试导入不存在的路径；
         *
         * @expectedException FileNotFoundException
         */
        public function testImportForNotExistsFile () {
            Jtita::import ('not_exists');
        }

        /**
         * 测试导入接口文件。
         */
        public function testImportInterface () {
            Jtita::paths (JTMOCK_PATH);

            $this->assertFalse (class_exists ('ImportClass'));
            $this->assertFalse (interface_exists ('ImportInterface'));

            $className = Jtita::importInterface ('jtita.datasources.import_class');
            Jtita::importInterface ('jtita.datasources.import_interface#import_interface');

            $this->assertEquals ('ImportClass', $className);

            $this->assertTrue (class_exists ('ImportClass'));
            $this->assertTrue (interface_exists ('ImportInterface'));
        }

        /**
         * 测试导入接口文件，接口已存在。
         *
         * @depends testImportInterface
         */
        public function testImportInterfaceForInterfaceExists () {
            Jtita::paths (JTMOCK_PATH);

            $this->assertTrue (class_exists ('ImportClass'));
            $this->assertTrue (interface_exists ('ImportInterface'));

            Jtita::importInterface ('jtita.datasources.import_class');
            Jtita::importInterface ('jtita.datasources.import_interface#import_interface');
        }

        /**
         * 测试写入简单内容；
         */
        public function testWriteForSimple () {
            Jtita::write ('key', 'value');

            $this->assertEquals ('value', Jtita::read ('key'));
        }


        /**
         * 测试写入配置，来自文件。
         */
        public function testWriteForFile () {
            Jtita::paths (JTMOCK_PATH . '/boots');
            Jtita::writeFormFile ('file', 'setting');

            $setting = Jtita::read ('file');

            $this->assertEquals (2, count ($setting));
            $this->assertEquals ('Hello Jtita', $setting['title']);
            $this->assertEquals ('good', $setting['description']);
        }

        /**
         * 测试写入配置，文件不存在。
         *
         * @expectedException FileNotFoundException
         */
        public function testWriteForNotExists () {
            Jtita::paths (JTMOCK_PATH . '/boots');
            Jtita::writeFormFile ('file', 'not_exists');
        }

        /**
         * 测试写入重复值；
         *
         * @depends testWriteForSimple
         */
        public function testWriteForRepeatName () {
            Jtita::write ('repeat', 'sv1');

            $this->assertEquals ('sv1', Jtita::read ('repeat'));

            Jtita::write ('repeat', 'sv-news');
            $this->assertEquals ('sv-news', Jtita::read ('repeat'));
        }

        /**
         * 测试读取没有写入的值；
         *
         * @depends testWriteForSimple
         */
        public function testReadForNotWriteValue () {
            $this->assertFalse (Jtita::read ('notExists'));
            $this->assertEquals ('default', Jtita::read ('notExists', 'default'));
        }

        /**
         * 测试根据字符串删除配置;
         *
         * @depends testWriteForSimple
         */
        public function testDeleteForString () {
            Jtita::write ('delKEY', 'delVAL');

            Jtita::delete ('delKEY');
            $this->assertFalse (Jtita::read ('delKEY'));
        }

        /**
         * 测试写入对象；
         */
        public function testWriteForObject () {
            Jtita::write ('temp', new JtitaEmployee('t1id', 't1name'));

            $emp = Jtita::read ('temp');

            $this->assertTrue (is_object ($emp));
            $this->assertEquals ('t1id', $emp->id);
            $this->assertEquals ('t1name', $emp->name);
        }

        /**
         * 测试使用字符串参数注册动态行为；
         */
        public function testBehaviors () {
            Jtita::behaviors ('orders.services.order_service', 'submit', 'kernels.transactions.submit_behavior');

            $this->assertTrue (Jtita::hasBehaviors ('orders.services.order_service'));
            $this->assertTrue (Jtita::hasBehaviors ('orders.services.order_service', 'submit'));
        }

        /**
         * 测试使用字符串注销类行为；
         */
        public function testUnBehaviorsForMethod () {
            Jtita::behaviors ('orders.services.order_service1', 'submit', 'kernels.transactions.submit_behavior');
            Jtita::unBehaviors ('orders.services.order_service1', 'submit');

            $this->assertFalse (Jtita::hasBehaviors ('orders.services.order_service1', 'submit'));
            $this->assertFalse (Jtita::hasBehaviors ('orders.services.order_service1'));
        }

        /**
         * 测试使用字符串参数注销字符串方法；
         */
        public function testUnBehaviorsForClass () {
            Jtita::behaviors ('orders.services.order_service3', 'submit', 'kernels.transactions.submit_behavior');
            Jtita::behaviors ('orders.services.order_service3', 'submit', 'kernels.transactions.submit_behavior1');

            Jtita::unBehaviors ('orders.services.order_service3');
            $this->assertFalse (Jtita::hasBehaviors ('orders.services.order_service3'));
        }

        /*
         * 测试注销不存在的行为；
         */
        public function testUnBehaviorForNotExists () {
            Jtita::unBehaviors ('not_exists.not_class');
            Jtita::unBehaviors ('not_exists.not_class', 'not_method');
        }

        /**
         * 测试创建简单对象；
         *
         * @depends testPathsForSetString
         */
        public function testCreateForSimpleObject () {
            Jtita::paths (JTMOCK_PATH);

            $simpleObj = Jtita::create ('simple_object');

            $this->assertNotNull ($simpleObj);
            $this->assertInstanceOf ('SimpleObject', $simpleObj);
        }

        /**
         * 测试创建包对象；
         *
         * @depends testPathsForSetString
         */
        public function testCreateForPackageObject () {
            Jtita::paths (JTMOCK_PATH);

            $packageObj = Jtita::create ('app.package_object');

            $this->assertNotNull ($packageObj);
            $this->assertInstanceOf ('PackageObject', $packageObj);
        }

        /**
         * 测试创建对象包含在文件内部；
         *
         * @depends testPathsForSetString
         * @depends testCreateForPackageObject
         */
        public function testCreateForInnerFileObject () {
            Jtita::paths (JTMOCK_PATH);

            $innerObj = Jtita::create ('app.inner_object#inner_hi');

            $this->assertNotNull ($innerObj);
            $this->assertInstanceOf ('InnerHi', $innerObj);
        }

        /**
         * 测试创建具有相同类名的对象；
         *
         * @depends testPathsForSetString
         * @depends testCreateForInnerFileObject
         */
        public function testCreateForRepeatClassName () {
            Jtita::paths (JTMOCK_PATH);

            $obj = Jtita::create ('app.same_object');
            $obj1 = Jtita::create ('app.other_same_object#same_object');

            $this->assertNotNull ($obj);
            $this->assertNotNull ($obj1);

            $this->assertInstanceOf ('SameObject', $obj);
            $this->assertInstanceOf ('SameObject', $obj1);

            $this->assertEquals ('same_object', $obj->file ());
            $this->assertEquals ('same_object', $obj1->file ());
        }

        /**
         * 测试创建具有构造器选项；
         *
         * @depends testPathsForSetString
         * @depends testCreateForSimpleObject
         */
        public function testCreateForCtors () {
            Jtita::paths (JTMOCK_PATH);

            $ctorObj = Jtita::create ('ctor_object', array('id' => 'NE0000', 'name' => 'Zhenggp'));

            $this->assertNotNull ($ctorObj);
            $this->assertInstanceOf ('CtorObject', $ctorObj);
            $this->assertEquals ('NE0000', $ctorObj->id);
            $this->assertEquals ('Zhenggp', $ctorObj->name);
        }

        /**
         * 测试创建对象类名不存在；
         *
         * @depends testPathsForSetString
         * @depends testCreateForInnerFileObject
         * @expectedException ClassNotFoundException
         */
        public function testCreateForClassNameNotExists () {
            Jtita::paths (JTMOCK_PATH);

            Jtita::create ('simple_object#notexists');
        }

        /**
         * 测试ioc对象，配置参数为字符串。
         */
        public function testIocForString () {
            JTita::paths (JTMOCK_PATH);
            $obj = Jtita::ioc ('prop_object');

            $this->assertInstanceOf ('PropObject', $obj);
        }

        /**
         * 测试ioc方法，class未提供；
         *
         * @depends testCreateForSimpleObject
         * @expectedException JtitaException
         */
        public function testIocForClassNotSupport () {
            Jtita::ioc (array('ctors' => array(), 'props' => array()));
        }

        /**
         * 测试ioc方法，指定ctors参数；
         *
         * @depends testCreateForCtors
         */
        public function testIocForCtors () {
            Jtita::paths (JTMOCK_PATH);

            $ctorObj = Jtita::ioc (array('class' => 'ctor_object',
                                         'ctors' => array('id' => 'NE0000', 'name' => 'Zhenggp')));

            $this->assertNotNull ($ctorObj);
            $this->assertInstanceOf ('CtorObject', $ctorObj);
            $this->assertEquals ('NE0000', $ctorObj->id);
            $this->assertEquals ('Zhenggp', $ctorObj->name);
        }

        /**
         * 测试ioc方法，指定props参数；
         */
        public function testIocForProps () {
            Jtita::paths (JTMOCK_PATH);

            $propObj = Jtita::ioc (array('class' => 'prop_object', 'id' => 'NE0000', 'name' => 'Zhenggp'));

            $this->assertNotNull ($propObj);
            $this->assertInstanceOf ('PropObject', $propObj);
            $this->assertEquals ('NE0000', $propObj->id);
            $this->assertEquals ('Zhenggp', $propObj->name);
        }

        /**
         * 测试IOC方法，提供更多的字段。
         */
        public function testIocForMoreProps () {
            Jtita::paths (JTMOCK_PATH);

            $propObj = Jtita::ioc (array('class' => 'prop_object', 'id' => 'NE0000', 'name' => 'Zhenggp',
                                         'not_exists' => 'not_exists'));

            $this->assertNotNull ($propObj);
            $this->assertInstanceOf ('PropObject', $propObj);
            $this->assertEquals ('NE0000', $propObj->id);
            $this->assertEquals ('Zhenggp', $propObj->name);
        }

        /**
         * 测试IOC，自动初始化。
         */
        public function testIocForAutoInitialize () {
            Jtita::paths (JTMOCK_PATH);

            $autoInit = Jtita::ioc (array('class' => 'jtita.datasources.auto_initialize', 'initialize' => true));

            $this->assertTrue ($autoInit->initialized);
        }

        /**
         * 测试IOC，指定初始化不支持
         *
         * @expectedException MethodNotFoundException
         */
        public function testIocForAutoInitializeNotSupport () {
            Jtita::ioc (array('class' => 'jtita_employee', 'initialize' => true));
        }

        /**
         * 测试获取实例对象；
         *
         * @depends testPathsForSetString
         * @depends testCreateForSimpleObject
         */
        public function testInstance () {
            Jtita::paths (JTMOCK_PATH);

            $instance = Jtita::instance ('simple_object');
            $instance1 = Jtita::instance ('simple_object');

            $instance->field1 = 'Hello world';
            $instance->field2 = 'Goods';

            $this->assertEquals ('Hello world', $instance1->field1);
            $this->assertEquals ('Goods', $instance1->field2);
        }


        /**
         * 测试创建注册有行为的对象；
         */
        public function testCreateForBehavior () {
            $callback = false;

            Jtita::clsBehaviors ();
            Jtita::behaviors ('jtita_target_mock', 'setField',
                    array('class' => 'callback_behavior',
                          'callback' => function ($target, $args,ObjectBehavior $next) use (&$callback) {
                    $callback = true;
                    return $next->invoke ($target, $args);
                })
            );

            $obj = Jtita::create ('jtita_target_mock');

            $this->assertInstanceOf ('ObjectWrapper', $obj);
            $this->assertInstanceOf ('JtitaTargetMock', $obj->target);

            $obj->setField ('Hello World');
            $this->assertTrue ($callback);
            $this->assertEquals ('Hello World', $obj->field);
            $this->assertEquals ('Hello World', $obj->target->field);
        }

        /**
         * 测试创建注册行为模式；
         */
        public function testCreateForRegisterWrapper () {
            Jtita::clsBehaviors ();
            Jtita::behaviors ('jtita_target_mock', 'wrapper', 'JtitaBehaviorWrapperMock');

            $obj = Jtita::create ('jtita_target_mock');
            $this->assertInstanceOf ('JtitaBehaviorWrapperMock', $obj);
        }

        /**
         * 自动测试初始化。
         */
        public function setUp () {
            Jtita::paths (array());
            Jtita::clsBehaviors ();
            Jtita::clear ();
        }
    }

    # 路径导入方法
    function path_replace ($path) {
        return str_replace ('\\', '/', $path);
    }

    /**
     * 定义普通类，进行write object测试；
     */
    class JtitaEmployee{
        public $id;
        public $name;

        public function __construct ($id = 'EMP001', $name = 'EMPNAME') {
            $this->id = $id;
            $this->name = $name;
        }
    }

    /**
     * 目标模拟对象；
     */
    class JtitaTargetMock{
        public $field;

        public function setField ($field) {
            $this->field = $field;
        }
    }

    class JtitaBehaviorWrapperMock {
        function _construct ($target) {

        }

        function behaviors ($behaviors) {
        }
    }
