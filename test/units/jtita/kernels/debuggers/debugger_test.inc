<?php
    /**
     * debugger_test.inc 提供Debugger类的测试；
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.debuggers
     * @version     1.0 beta
     *
     * @create      2012-07-15
     * @modify      2013-04-26
     */
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (dirname (__FILE__)))))));

    # 导入基础服务类；
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/behaviors.inc');

     # 导入测试事务类；
    require_once(JTPATH_ROOT . '/jtita/kernels/debuggers/debugger.inc');

    /**
     * 调试器测试类，实现Debugger的测试；
     */
    class DebuggerTest extends PHPUnit_Framework_TestCase {
        /**
         *  测试写入操作；
         */
        public function testWrite () {
            Debugger::write ('mysql', 'hello world');
            Debugger::write ('mysql', 'hello world1');
            Debugger::write ('oracle', 'Hello world');

            $vars = Debugger::read ('mysql');

            $this->assertTrue (is_array ($vars));
            $this->assertEquals (2, count ($vars));
            $this->assertEquals ('hello world', $vars[0]);
            $this->assertEquals ('hello world1', $vars[1]);
        }

        /**
         * 测试dump 不存在的name；
         */
        public function testReadNotExistsName () {
            $vars = Debugger::read ('not exists');

            $this->assertTrue (is_array ($vars));
            $this->assertTrue (empty($vars));
        }
    }