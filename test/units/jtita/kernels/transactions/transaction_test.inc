<?php

    /**
     * transaction_test.inc 测试事务接口对象服务。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.transactions
     * @version     1.0 beta
     *
     * @create      2012-07-09
     * @modify      2013-04-02
     */

    /**
     * 初始化开始位置；
     */
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (dirname (__FILE__)))))));

    /**
     * 导入基础服务类；
     */
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/behaviors.inc');

    /**
     * 导入测试事务类；
     */
    require_once(JTPATH_ROOT . '/jtita/kernels/transactions/transaction.inc');

    /**
     * 事务测试模拟对象；
     */
    class TransObMock {
        public $begin;
        public $complete;
        public $rollback;
        public $beginResult;

        function __construct ($beginResult = true) {
            $this->begin = false;
            $this->complete = false;
            $this->rollback = false;

            $this->beginResult = $beginResult;
        }

        function begin () {
            $this->begin = true;
            return $this->beginResult;
        }

        function complete () {
            $this->begin = false;
            $this->complete = true;
        }

        function rollback () {
            $this->begin = false;
            $this->rollback = true;
        }
    }

    class TransObMock1 {

    }

    /**
     * 事务测试类，提供事务功能的测试；
     */
    class TransactionTest extends PHPUnit_Framework_TestCase {

        /**
         * 测试注册；
         */
        public function testRegister () {
            $mock = new TransObMock();
            $mock1 = new TransObMock();

            $this->assertTrue (Transaction::register ($mock));
            $this->assertFalse (Transaction::hasRegister ($mock1));
            $this->assertTrue (Transaction::hasRegister ($mock));
        }

        /**
         * 测试注册指定事务方法不存在；
         *
         * @expectedException TransactionException
         */
        public function testRegisterForMethodNotExists () {
            $mock = new TransObMock1();

            Transaction::register ($mock);
        }

        /**
         * 测试重复注册
         *
         * @expectedException TransactionException
         */
        #public function testRegisterForRepeat () {
        #    $mock = new TransObMock();

        #    Transaction::register ($mock);
        #    Transaction::register ($mock);
        #}

        /**
         * 测试取消注册；
         */
        public function testUnRegister () {
            $mock = new TransObMock();

            $this->assertTrue (Transaction::register ($mock));
            $this->assertTrue (Transaction::unRegister ($mock));
            $this->assertFalse (Transaction::hasRegister ($mock));
        }

        /**
         * 测试注销不存在的实例；
         */
        public function testUnRegisterForNotExists () {
            $mock = new TransObMock();

            $this->assertFalse (Transaction::unRegister ($mock));
        }

        /**
         * 测试开始事务；
         *
         * @depends testRegister
         */
        public function testBegin () {
            $mock = new TransObMock();
            $mock1 = new TransObMock();
            $mock2 = new TransObMock();

            Transaction::register ($mock);
            Transaction::register ($mock1);

            Transaction::begin ();

            $this->assertTrue ($mock->begin);
            $this->assertTrue ($mock1->begin);
            $this->assertFalse ($mock2->begin);

            $this->assertTrue (Transaction::isBegin ());
        }

        /**
         * 测试重复开始事务；
         *
         * @expectedException TransactionException
         */
        #public function testBeginForRepeatBegin () {
        #    Transaction::begin ();
        #    Transaction::begin ();
        #}

        /**
         * 测试完成功能；
         *
         * @depends testRegister
         * @depends testBegin
         */
        public function testComplete () {
            $mock = new TransObMock();

            Transaction::register ($mock);
            Transaction::complete ();

            $this->assertTrue ($mock->complete);
            $this->assertFalse (Transaction::isBegin ());
        }

        /**
         * 测试完成还未开始的事务；
         *
         * @depends testComplete
         * @expectedException TransactionException
         */
        #public function testCompleteForNotBegin () {

        #    $this->assertFalse (Transaction::isBegin ());

        #    Transaction::complete ();
        #}

        /**
         * 测试事务开始时已经注册；
         *
         * @depends testBegin
         * @depends testComplete
         */
        public function testRegisterForIsBegin () {
            Transaction::begin ();

            $mock = new TransObMock();

            Transaction::register ($mock);

            $this->assertTrue ($mock->begin);

            Transaction::complete ($mock);
        }

        /**
         * 测试事务回滚；
         *
         * @depends testBegin
         * @depends testRegisterForIsBegin
         */
        public function testRollback () {
            $mock = new TransObMock();

            Transaction::register ($mock);

            Transaction::begin ();
            Transaction::rollback ();

            $this->assertTrue ($mock->rollback);
            $this->assertFalse (Transaction::isBegin ());
        }

        /**
         * 测试回滚还未开始的事务；
         *
         * @depends testRollback
         * @expectedException TransactionException
         */
        #public function testRollbackForNotBegin () {
        #    Transaction::rollback ();
        #}
    }