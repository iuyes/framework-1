<?php
    /**
     * validators_test.inc 验证方法单元测试服务。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.validations
     * @version     1.0 beta
     *
     * @create      2012-11-11
     * @modify      2014-04-28
     */

    /**
     * 声明根节点路径；
     */
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (dirname (__FILE__)))))));

    /**
     * 导入被测试类。
     */
    require_once(JTPATH_ROOT . '/jtita/kernels/validations/validators.inc');

    /**
     * 验证器测试类，提供验证器的单位元测试服务；
     */
    class ValidatorsTest extends PHPUnit_Framework_TestCase {
        /**
         * 测试非空验证
         */
        public function testRequiredValidator () {
            $input = array('name' => false, 'fullName' => 'zhenggp');

            $this->assertTrue (requiredValidator ($input, 'fullName'));
            $this->assertFalse (requiredValidator ($input, 'name'));
        }

        /**
         * 正则表达式测试；
         */
        public function testRegexValidator () {
            $input = array('name' => 'zhenggp', 'age' => 'two');

            $this->assertTrue (regexValidator ($input, 'name', '/^[a-z]{7}$/'));
            $this->assertFalse (regexValidator ($input, 'name', '/^[0-9]*$/'));
        }

        /**
         * 测试验证正则表达式，空值验证；
         */
        public function testRegexValidatorForEmptyValue () {
            $input = array('name' => '', 'age' => 0, 'sex' => false);

            $this->assertTrue (regexValidator ($input, 'name', '^[a-z]*$'));
            $this->assertTrue (regexValidator ($input, 'age', '^[a-z]*$'));
            $this->assertTrue (regexValidator ($input, 'sex', '^[a-z]*$'));
        }

        /**
         * 测试数字验证；
         */
        public function testNumberValidator () {
            $input = array('age' => 18, 'age1' => '18', 'age2' => '12ab');

            $this->assertTrue (numberValidator ($input, 'age'));
            $this->assertTrue (numberValidator ($input, 'age1'));
            $this->assertFalse (numberValidator ($input, 'age2'));
        }

        /**
         * 测试验证数字空值；
         */
        public function testNumberValidatorForEmptyValue () {
            $input = array('age' => 0, 'name' => '', 'sex' => false);

            $this->assertTrue (numberValidator ($input, 'age'));
            $this->assertTrue (numberValidator ($input, 'sex'));
            $this->assertTrue (numberValidator ($input, 'name'));
        }

        /**
         * 测试电话验证器；
         */
        public function testTelephoneValidator () {
            $input = array('tel' => '023-65730981', 'tel1' => '1871-6918235', 'tel4' => '400-023-31921',
                'tel41' => '410-102-09123', 'telm' => '18812312311');

            $this->assertTrue (telephoneValidator ($input, 'tel'));
            $this->assertTrue (telephoneValidator ($input, 'tel1'));
            $this->assertTrue (telephoneValidator ($input, 'tel4'));
            $this->assertFalse (telephoneValidator ($input, 'tel41'));
            $this->assertTrue (telephoneValidator ($input, 'telm'));
        }

        /**
         * 测试验证电话，空值验证；
         */
        public function  testTelephoneValidatorForEmptyValue () {
            $input = array('tel' => '', 'tel1' => false, 'tel2' => 0);

            $this->assertTrue (telephoneValidator ($input, 'tel'));
            $this->assertTrue (telephoneValidator ($input, 'tel1'));
            $this->assertTrue (telephoneValidator ($input, 'tel2'));
        }

        /**
         * 测试范围验证器；
         */
        public function testRangeValidator () {
            $input = array('name' => 'c', 'age' => 18);

            $this->assertTrue (rangeValidator ($input, 'name', array('a', 'd')));
            $this->assertTrue (rangeValidator ($input, 'name', array('c', 'd')));
            $this->assertTrue (rangeValidator ($input, 'name', array('a', 'c')));
            $this->assertFalse (rangeValidator ($input, 'name', array('e', 'g')));

            $this->assertTrue (rangeValidator ($input, 'age', array(16, 24)));
            $this->assertTrue (rangeValidator ($input, 'age', array(18, 24)));
            $this->assertTrue (rangeValidator ($input, 'age', array(16, 18)));

            $this->assertFalse (rangeValidator ($input, 'age', array(25, 30)));
        }

        /**
         * 测试验证范围验证器，验证空值；
         */
        public function testRangeValidatorForEmptyValue () {
            $input = array('name' => '', 'age' => 0, 'sex' => false);

            $this->assertTrue (rangeValidator ($input, 'name', array('a', 'd')));
            $this->assertTrue (rangeValidator ($input, 'age', array(- 10, 10)));
            $this->assertTrue (rangeValidator ($input, 'sex', array('a', 'd')));
        }

        /**
         * 验证最小长度验证器；
         */
        public function testMinlenValidator () {
            $input = array('name' => 'zhenggp');

            $this->assertTrue (minlenValidator ($input, 'name', 6));
            $this->assertFalse (minlenValidator ($input, 'name', 10));
        }

        /**
         * 验证最小值空值验证。
         */
        public function testMinlenValidatorForEmptyValue () {
            $input = array('name' => '', 'age' => 0, 'sex' => false);

            $this->assertTrue (minlenValidator ($input, 'name', 10));
            $this->assertTrue (minlenValidator ($input, 'age', 10));
            $this->assertTrue (minlenValidator ($input, 'sex', 10));
        }

        /**
         * 验证最小长度验证器；
         */
        public function testMaxlenValidator () {
            $input = array('name' => 'zhenggp');

            $this->assertTrue (maxlenValidator ($input, 'name', 10));
            $this->assertFalse (maxlenValidator ($input, 'name', 2));
        }

        /**
         * 验证最小值空值验证。
         */
        public function testMaxlenValidatorForEmptyValue () {
            $input = array('name' => '', 'age' => 0, 'sex' => false);

            $this->assertTrue (maxlenValidator ($input, 'name', 10));
            $this->assertTrue (maxlenValidator ($input, 'age', 10));
            $this->assertTrue (maxlenValidator ($input, 'sex', 10));
        }

        /**
         * 测试枚举验证器；
         */
        public function testEnumValidator () {
            $input = array('sex' => 'm', 'name' => 'zhenggp', 'age' => 12);

            $this->assertTrue (enumValidator ($input, 'sex', array('m', 'w', 12)));
            $this->assertFalse (enumValidator ($input, 'name', array('m', 'w', 12)));
            $this->assertTrue (enumValidator ($input, 'age', array('m', 'w', 12)));
        }

        /**
         * 测试验证枚举验证器，空值验证；
         */
        public function testEnumValidatorForEmptyValue () {
            $input = array('sex' => false, 'name' => '', 'age' => 0);

            $this->assertTrue (enumValidator ($input, 'sex', array('m', 'w', 12)));
            $this->assertTrue (enumValidator ($input, 'name', array('m', 'w', 12)));
            $this->assertTrue (enumValidator ($input, 'age', array('m', 'w', 12)));
        }

        /**
         * 测试比较验证器；
         */
        public function testCompareValidator () {
            $input = array('name' => 'zhenggp', 'age' => 18, 'name1' => 'zhenggp', 'age1' => '18', 'age2' => 20);

            $this->assertTrue (compareValidator ($input, 'name', 'name1'));
            $this->assertTrue (compareValidator ($input, 'age', 'age1'));
            $this->assertFalse (compareValidator ($input, 'age', 'age2'));
        }

        /**
         * 测试比较验证器，选项值是空值；
         */
        public function testCompareValidatorForOptionsEmptyValue () {
            $input = array('name' => 'zhengg');

            $this->assertFalse (compareValidator ($input, 'name', 'name1'));
        }

        /**
         * 测试比较验证器，验证空值；
         */
        public function testCompareValidatorForEmptyValue () {
            $input = array('name' => '', 'age' => 0, 'sex' => false);

            $this->assertTrue (compareValidator ($input, 'name', 'age'));
            $this->assertTrue (compareValidator ($input, 'age', 'sex'));
            $this->assertTrue (compareValidator ($input, 'sex', 'age'));
        }

        /**
         * 测试邮件验证；
         */
        public function testEmailValidator () {
            $input = array('normal' => 'zhenggp@164.com', 'vip' => 'vip.zhenggp@163.com', 'number' => '40532@qq.com',
                'nums' => '292.2910@163.com', 'cndomain' => 'zheng@164.com.cn', 'fail' => 'failldkxxx',
                'fail1' => 'fail@153');

            $this->assertTrue (emailValidator ($input, 'normal'));
            $this->assertTrue (emailValidator ($input, 'vip'));
            $this->assertTrue (emailValidator ($input, 'number'));
            $this->assertTrue (emailValidator ($input, 'nums'));
            $this->assertTrue (emailValidator ($input, 'cndomain'));
            $this->assertFalse (emailValidator ($input, 'fail'));
            $this->assertFalse (emailValidator ($input, 'fail'));
        }

        /**
         * 测试邮件验证，验证空值；
         */
        public function testEmailValidatorForEmptyValue () {
            $input = array('email' => '', 'email1' => 0, 'email2' => false, 'email3' => true);

            $this->assertTrue (emailValidator ($input, 'email'));
            $this->assertTrue (emailValidator ($input, 'email1'));
            $this->assertTrue (emailValidator ($input, 'email2'));
            $this->assertTrue (emailValidator ($input, 'email'));
        }

        /**
         * 验证URL地址；
         */
        public function testUrlValidator () {
            $input = array('sina' => 'http://www.sina.com.cn', '163' => 'http://www.163.com/index.php?a=12&19d',
                'https' => 'https://www.163.com/index.php&a=1', 'qq' => 'qq.com/index.php');

            $this->assertTrue (urlValidator ($input, 'sina'));
            $this->assertTrue (urlValidator ($input, '163'));
            $this->assertTrue (urlValidator ($input, 'https'));
            $this->assertFalse (urlValidator ($input, 'qq'));
        }

        /**
         * 测试URL验证，空值；
         */
        public function testUrlValidatorForEmptyValue () {
            $input = array('sina' => false, '163' => '', 'qq' => 0);

            $this->assertTrue (urlValidator ($input, 'sina'));
            $this->assertTrue (urlValidator ($input, '163'));
            $this->assertTrue (urlValidator ($input, 'qq'));
        }

        /**
         * 测试IP验证器；
         */
        public function testIpValidator () {
            $input = array('ip' => '127.0.0.1', 'ip1' => '258.0.0.1', 'ip3' => '127.0.0.1,108.0.1');

            $this->assertTrue (ipValidator ($input, 'ip'));
            $this->assertFalse (ipValidator ($input, 'ip1'));
            $this->assertFalse (ipValidator ($input, 'ip3'));
        }

        /**
         * 测试验证IP验证器，空白值；
         */
        public function testIpValidatorForEmptyValue () {
            $input = array('ip' => false, 'ip1' => 0, 'ip2' => '');

            $this->assertTrue (ipValidator ($input, 'ip'));
            $this->assertTrue (ipValidator ($input, 'ip1'));
            $this->assertTrue (ipValidator ($input, 'ip2'));
        }
    }