<?php
    /**
     * autotrans_behavior_test.inc 自动行为支持活动
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.transactions.behaviors
     * @version     1.0 beta
     *
     * @create      2012-07-12
     * @modify      2013-04-28
     */

    /**
     * 初始化开始位置；
     */
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (dirname (dirname (__FILE__))))))));

    /**
     * 导入基础服务类；
     */
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/behaviors.inc');

    require_once(JTPATH_ROOT . '/jtita/kernels/transactions/transaction.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/transactions/behaviors/autotrans_behavior.inc');

    /**
     * 数据库模拟对象；
     */
    class DbMock {
        static $begin = false;
        static $rollback = false;
        static $complete = false;

        function __construct () {
            if (defined ('JTITA_TRANSACTION')) {
                Transaction::register ($this);
            }
        }

        function __destruct () {
            if (defined ('JTITA_TRANSACTION')) {
                Transaction::unRegister ($this);
            }
        }

        function begin () {
            self::$begin = true;
        }

        function complete () {
            self::$complete = true;
        }

        function rollback () {
            self::$rollback = true;
        }

        function query ($sql, $error) {
            if ($error) {
                throw new Exception();
            }

            return 'Hello World';
        }
    }

    /**
     * 业务服务模拟对象；
     */
    class BizServiceMock {
        function submit ($error = false) {
            $db = Jtita::create ('db_mock');
            return $db->query ('Hello World', $error);
        }
    }

    /**
     * 自动事务活动测试；
     */
    class AutotransBehaviorTest extends PHPUnit_Framework_TestCase {

        /**
         * 设置开始；
         */
        function setUp () {
            Jtita::clsBehaviors ();
            Jtita::paths (array(JTPATH_ROOT . '/jtita'));

            DbMock::$begin = false;
            DbMock::$complete = false;
            DbMock::$rollback = false;
        }

        /**
         * 测试回滚事务；
         */
        function testRollback () {
            // 注册自动行为；
            Jtita::behaviors ('biz_service_mock', 'submit', 'autotrans_behavior');

            $service = Jtita::create ('biz_service_mock');
            try {
                $service->submit (true);
            } catch (Exception $ex) {
            }

            $this->assertTrue (DbMock::$rollback);
            $this->assertFalse (DbMock::$complete);
        }

        /**
         * 测试完成事务；
         */
        function testComplete () {
            // 注册自动行为；
            Jtita::behaviors ('biz_service_mock', 'submit', 'autotrans_behavior');

            $service = Jtita::create ('biz_service_mock');
            $result = false;
            try {
                $result = $service->submit ();
            } catch (Exception $ex) {

            }

            $this->assertTrue (DbMock::$complete);
            $this->assertFalse (DbMock::$rollback);
            $this->assertEquals ('Hello World', $result);
            $this->assertEquals ('Hello World', $result);
        }

    }