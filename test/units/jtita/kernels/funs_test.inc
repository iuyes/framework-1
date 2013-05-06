<?php
    /**
     * funs_test.php 提供核心函数的单元测试服务；
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels
     * @version     1.0 beta
     *
     * @create      2012-10-03
     * @modify      2013-04-26
     */

    # 声明应用根目录；
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (__FILE__))))));

    # 导入应用测试目录
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');

    /**
     * 内核工具单元测试；
     */
    class FunsTest extends PHPUnit_Framework_TestCase{
        /**
         * test camel upper
         */
        public function testCamelUpper () {
            $this->assertEquals ('helloWorld', camel_upper ('HelloWorld'));
            $this->assertEquals ('helloWorld', camel_upper ('hello_world'));
            $this->assertEquals ('helloWorldAll', camel_upper ('hello_world_all'));
            $this->assertEquals ('HelloWorld', camel_upper ('_hello_world'));
        }

        /**
         * test camel lower
         */
        public function testCamelLower () {
            $this->assertEquals ('hello_world', camel_lower ('helloWorld'));
            $this->assertEquals ('hello_world', camel_lower ('HelloWorld'));
            $this->assertEquals ('hello_world_all', camel_lower ('HelloWorldAll'));
        }

        /**
         * test Pascal
         */
        public function testPascal () {
            $this->assertEquals ('HelloWorld', pascal ('hello_world'));
            $this->assertEquals ('HelloWorld', pascal ('helloWorld'));
            $this->assertEquals ('HelloWorldAll', pascal ('hello_world_all'));
        }

        /**
         * 测试根据数组键值过滤数组；
         */
        public function testArrayKfilter () {
            $arr = array(2 => 'tow', 3 => 'three', 4 => 'four', 5 => 10);
            $arr = array_kfilter ($arr, function ($key, $val) {
                # 非偶数关键字，并且为字符串值；
                return ($key % 2 != 0) && is_string ($val);
            });

            $this->assertArrayHasKey(3,$arr);
            $this->assertArrayNotHasKey(2,$arr);
            $this->assertArrayNotHasKey(4,$arr);
            $this->assertArrayNotHasKey(5,$arr);
        }

        /**
         * 测试根据数组键值过滤数组，仅有主键参数；
         */
        public function testArrayKfilterForCallbackOnlyKeyArgument () {
            $arr = array(2 => 'tow', 3 => 'three', 4 => 'four', 5 => 10);
            $earr = array(3 => 'three', 5 => 10);
            $rarr = array_kfilter ($arr, function ($key) {
                # 非偶数关键字，并且为字符串值；
                return ($key % 2 != 0);
            });

            $this->assertEmpty (array_diff_assoc ($earr, $rarr));
        }

        /**
         * 测试根据数据键值弹出数据；
         */
        public function testArrayKpop () {
            $arr = array('one' => 1, 'tow' => 2);
            $earr = array('one' => 1);
            $resu = array_kpop ($arr, 'tow');

            $this->assertEquals (2, $resu);
            $this->assertEmpty (array_diff_assoc ($arr, $earr));
        }

        /**
         * 测试根据数据键值弹出数据，主键值不存在；
         */
        public function testArrayKpopForUndefKey () {
            $arr = array();

            $this->assertFalse (array_kpop ($arr, 'unexists'));
        }

        /**
         * 测试根据数组键值弹出数据，主键值不存在并且指定默认值；
         */
        public function testArrayKpopForUndefKeyAndDefaultVal () {
            $arr = array();

            $this->assertEquals ('Hello World', array_kpop ($arr, 'unexists', 'Hello World'));
        }

        /**
         * 测试数组字符串Key连接；
         */
        public function testArrayKimplode () {
            $arr = array('id' => 1, 'name' => 2);
            $str = array_kimplode ('=', ',', $arr);

            $this->assertEquals ('id=1,name=2', $str);
        }

        /**
         * 测试根据key和value合并生成单一值
         */
        public function testArrayKreduce () {
            $arr = array('id' => 1, 'name' => 2);
            $sql = array_kreduce ($arr, function ($key, $val, $initial) {
                return $initial . $key . '=' . $val . ',';
            });

            $this->assertEquals ('id=1,name=2,', $sql);
        }

        /**
         * 测试根据key和value合并生成单一值，指定初始值；
         */
        public function testArrayKreduceForInitial () {
            $arr = array(1 => 'one', 2 => 'tow', 3 => 'three');
            $sum = array_kreduce ($arr, function ($key, $val, $initial) {
                return $initial + $key;
            }, 0);

            $this->assertEquals (6, $sum);
        }

        /**
         * 测试获取文件名
         */
        public function testFiles () {
            $fileNames = file_names (dirname (__FILE__) . '/app_mock');

            $this->assertEquals (6, count ($fileNames));
            $this->assertArrayHasKey ('ctor_object.inc', $fileNames);
            $this->assertArrayHasKey ('simple1.inc', $fileNames);
            $this->assertArrayHasKey ('error.log', $fileNames);
        }

        /**
         * 测试获取文件名，使用扩展名。
         */
        public function testFilesForExt () {
            $logFiles = file_names (dirname (__FILE__) . '/app_mock', 'log');

            $this->assertArrayHasKey ('error.log', $logFiles);
            $this->assertEquals (1, count ($logFiles));

            $incFiles = file_names (dirname (__FILE__) . '/app_mock', 'inc');
            $this->assertEquals (5, count ($incFiles));
            $this->assertArrayHasKey ('ctor_object.inc', $incFiles);
            $this->assertArrayHasKey ('simple1.inc', $incFiles);
        }

        /**
         * 测试获取文件名，使用扩展名并且文件不存在。
         */
        public function  testFilesForExtNotExists () {
            $files = file_names (dirname (dirname (__FILE__)), 'not exists');
            $this->assertEquals (0, count ($files));
        }

        /**
         * 测试数组表达式。
         */
        public function testArrayExpress () {
            $this->assertEquals ('Hello goods', array_express (array('goods'), 'Hello {0}'));
            $this->assertEquals ('Hello goods', array_express (array('key' => 'goods'), 'Hello {key}'));
        }

        /**
         * 测试数组表达式，使用多层数组。
         */
        public function testArrayExpressForMultiLevel () {
            $this->assertEquals ('Hello goods', array_express (array(array('goods')), 'Hello {0.0}'));
            $this->assertEquals ('Hello goods', array_express (array(array('key' => 'goods')), 'Hello {0.key}'));
            $this->assertEquals ('Hello goods', array_express (array(array('key' => array('goods'))), 'Hello {0.key.0}'));
            $this->assertEquals ('Hello goods', array_express (array(array('key' => array('key1' => array('goods')))), 'Hello {0.key.key1.0}'));
            $this->assertEquals ('Hello goods', array_express (array(array('key' => array('key1' => array(array('af' => 'goods'))))), 'Hello {0.key.key1.0.af}'));
        }

        /**
         * 测试数组表达式，使用格式器。
         */
        public function testArrayExpressForFormater () {
            $this->assertEquals ('Hello 1970-01-01', array_express (array(0), 'Hello {0,date:Y-m-d}'));
            $this->assertEquals ('Hello 1970-01-01', array_express (array(array('key' => 0)), 'Hello {0.key,date:Y-m-d}'));
            $this->assertEquals ('Hello 1970-01-01', array_express (array(array('key' => array('f2' => 0))), 'Hello {0.key.f2,date:Y-m-d}'));
        }

        /**
         * 测试数组表达式，多个格式语法
         */
        public function testArrayExpressForMoreSyntax () {
            $this->assertEquals ('Hello 1970-01-01', array_express (array(0), 'Hello {0,date:Y}-{0,date:m}-{0,date:d}'));
            $this->assertEquals ('Hello 1970-01-01', array_express (array(array('key' => 0)), 'Hello {0.key,date:Y}-{0.key,date:m}-{0.key,date:d}'));
            $this->assertEquals ('Hello 1970-01-01', array_express (array(array('key' => array('f2' => 0))), 'Hello {0.key.f2,date:Y}-{0.key.f2,date:m}-{0.key.f2,date:d}'));
        }

        /**
         * 测试数组表达式，值不存在。
         */
        public function testArrayExpressForNotSupport () {
            $this->assertEquals ('Hello --', array_express (array(0), 'Hello {0.key}-{0.key}-{0.key}'));
            $this->assertEquals ('Hello --', array_express (array(array('key' => 0)), 'Hello {0.key.f2}-{0.key.f2}-{0.key.f2}'));
        }


        /**
         * 测试将数组转换成声明
         */
        public function testArrayToDeclareForStringKey () {
            $declare = array_declare (array('k1' => 1, 'k2' => 2, 'k3' => 'three'));
            #array(
            #    'k1' => 1,
            #    'k2' => 2,
            #    'k3' => 'three'
            #)
            $this->assertEquals ("array(\r\n\t'k1' => 1,\r\n\t'k2' => 2,\r\n\t'k3' => 'three'\r\n)", $declare);
        }

        /**
         * 测试将数组转换为声明，数字索引
         */
        public function testArrayToDeclareForIntKey () {
            $declare = array_declare (array(1, 2, 'k3' => 'three'));
            #array(
            #    0 => 1,
            #    1 => 2,
            #    'k3' => 'three'
            #)
            #echo $declare;
            $this->assertEquals ("array(\r\n\t0 => 1,\r\n\t1 => 2,\r\n\t'k3' => 'three'\r\n)", $declare);
        }

        /**
         * 测试将数组转换为声明，数组值。
         */
        public function testArrayToDeclareForArrayValue () {
            $declare = array_declare (array(array('php', 'one' => 1, 2 => array('k1' => 1, 'k2' => 'two')),
                                            'k3' => 'three'));
            #array(
            #    0 => array(
            #       0 => 'php',
            #       'one' => 1
            #       2 => array(
            #           'k1' => 1,
            #           'k2' => 'two'
            #       )
            #    ),
            #    'k3' => 'three'
            #)
            #echo $declare;
            $this->assertEquals ("array(\r\n" . "\t0 => array(\r\n" . "\t\t0 => 'php',\r\n" . "\t\t'one' => 1,\r\n" . "\t\t2 => array(\r\n" . "\t\t\t'k1' => 1,\r\n" . "\t\t\t'k2' => 'two'\r\n" . "\t\t)\r\n" . "\t),\r\n" . "\t'k3' => 'three'\r\n" . ")", $declare);
        }

        /**
         * 测试数组转声明，空数组。
         */
        public function testArrayToDeclareForEmptyArray () {
            $declare = array_declare (array());
            $this->assertEquals ('array()', $declare);

            $declare = array_declare (array(array()));
            $this->assertEquals ("array(\r\n" . "\t0 => array()\r\n" . ")", $declare);
        }

        /**
         * 测试数组转换，魔法字符串
         */
        public function testArrayToDeclareForMagicStringValue () {
            $declare = array_declare (array("Hello's"));
            $this->assertEquals ("array(\r\n\t0 => 'Hello\'s'\r\n)", $declare);
        }
    }