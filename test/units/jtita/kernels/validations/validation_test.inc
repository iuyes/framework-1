<?php
    /**
     * validation_test.inc 声明架构验证测试接口测试。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.validations
     * @version     1.0 beta
     *
     * @create      2012-06-14
     * @modify      2013-04-28
     */

    /**
     * 声明根节点路径；
     */
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (dirname (__FILE__)))))));

    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');

    # 初始化导入路径；
    Jtita::paths (JTPATH_ROOT . '/jtita');
    # 导入被测试类；
    require_once(JTPATH_ROOT . '/jtita/kernels/validations/validation.inc');

    /**
     * validation 单位元测试类，提供Validation单位元测试服务；
     */
    class ValidationTest extends PHPUnit_Framework_TestCase {

        /**
         * 测试验证，使用字符串规则；
         */
        public function testValidateForStringRules () {
            $rules = array('name' => 'required');
            $messages = array('name' => 'name is required');

            $inputSuccess = array('name' => 'zhenggp');
            $inputFail = array('name' => '');

            $this->assertTrue (Validation::validate ($inputSuccess, $rules, $messages));
            $errs = Validation::validate ($inputFail, $rules, $messages);

            $this->assertTrue (is_array ($errs));
            $this->assertTrue (isset($errs['name']));
            $this->assertEquals ('name is required', $errs['name']);
        }

        /**
         * 测试验证，使用数组规则；
         */
        public function testValidateForArrayRules () {
            $rules = array('name' => array('required' => true));
            $messages = array('name' => 'name is required');

            $inputSuccess = array('name' => 'zhenggp');
            $inputFail = array('name' => '');

            $this->assertTrue (Validation::validate ($inputSuccess, $rules, $messages));
            $errs = Validation::validate ($inputFail, $rules, $messages);

            $this->assertTrue (is_array ($errs));
            $this->assertTrue (isset($errs['name']));
            $this->assertEquals ('name is required', $errs['name']);
        }

        /**
         * 测试验证，使用规则选项；
         */
        public function testValidateForRuleOptions () {
            $rules = array('age' => array('range' => array(18, 60)));
            $messages = array('age' => 'age is error');

            $inputSuccess = array('age' => '30');
            $inputFail = array('age' => '16');

            $this->assertTrue (Validation::validate ($inputSuccess, $rules, $messages));
            $errs = Validation::validate ($inputFail, $rules, $messages);

            $this->assertTrue (is_array ($errs));
            $this->assertTrue (isset($errs['age']));
            $this->assertEquals ('age is error', $errs['age']);
        }

        /**
         * 测试验证，多个验证器；
         */
        public function testValidateForManyValidator () {
            $rules = array('age' => array('required' => true, 'range' => array(18, 60)),
                'name' => array('required' => true, 'minlen' => 6));
            $messages = array('age' => 'age required and between 18 60',
                'name' => array('required' => 'name required', 'minlen' => 'name > 6'));

            $inputSuccess = array('age' => 18, 'name' => 'zhenggp');
            $inputFail = array('age' => 16, 'name' => '');
            $inputFail1 = array('name' => 'kk', 'age' => 20);

            $this->assertTrue (Validation::validate ($inputSuccess, $rules, $messages));
            $errs = Validation::validate ($inputFail, $rules, $messages);

            $this->assertTrue (is_array ($errs));
            $this->assertEquals (2, count ($errs));

            $this->assertEquals ('age required and between 18 60', $errs['age']);
            $this->assertEquals ('name required', $errs['name']);

            $errs1 = Validation::validate ($inputFail1, $rules, $messages);

            $this->assertEquals ('name > 6', $errs1['name']);
        }
    }