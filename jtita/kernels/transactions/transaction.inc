<?php
    /**
     * transaction.inc 事务组件接口服务Transaction类声明文件
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
     * @modify      2013-04-28
     */

    /**
     * @const bool 定义Jtita事务开启标识，方便其它对象创建时进行判断；
     *
     * @example
     *      <pre>
     *          class TransactionObserver {
     *              public function __construct() {
     *                  if (defined('JTITA_TRANSACTION')) {
     *                      Transaction::register($this);
     *                  }
     *              }
     *
     *              public function __destruct() {
     *                  if (defined('JTITA_TRANSACTION')) {
     *                      Transaction::unRegister($this);
     *                  }
     *              }
     *          }
     *  </pre>
     */
    define('JTITA_TRANSACTION', true);

    /**
     * 事务异常处理类，当发生事务异常时引发本异常；
     *
     * @package jtita.kernels.transactions
     */
    class TransactionException extends JtitaException{
    }

    /**
     * 事务组件接口服务类，提供Jtita的事务控制服务。
     *
     * 实现事务支持的对象必须实现以下方法：
     * <ul>
     *        <li>function begin() { }</li>
     *        <li>function rollback() { }</li>
     *        <li>function complete() { }</li>
     * </ul>
     *
     * <b>说明：</b>Transaction 是Jtita中事务控制的管理类，具有以下特性：
     *      <ul>
     *          <li>-仅只支持单一事务提交；</li>
     *          <li>-不支持分支事务提交；</li>
     *          <li>-仅支持单事务一步式提交非二步式提交；</li>
     *          <li>-事务应由应用层的service对象或controller对象使用；</li>
     *      </ul>
     *
     * @example
     *  <code>
     *        try {
     *            Transaction::begin();
     *
     *            $member = Member::find(1);
     *            $member->name = 'new name';
     *            $member->update();
     *
     *            Transaction::complete();
     *        } catch (Exception $ex) {
     *            Transaction::rollback();
     *        }
     *  </code>
     *
     * @package   jtita.kernels.transactions
     * @abstract
     */
    abstract class Transaction{
        /**
         * @const string 开始事务方法
         */
        const METHOD_BEGIN = 'begin';

        /**
         * @const string 完成事务方法
         */
        const METHOD_COMPLETE = 'complete';

        /**
         * @const string 回滚事务方法
         */
        const METHOD_ROLLBACK = 'rollback';

        /**
         * 注册一个事务支持对象；
         *
         * @param mixed $observer 注册的事务观察者；
         *
         * @return bool 注册成功返回true，返回返回false；
         *
         * @throws TransactionException 注册$observer未包含事务所需的广播方法
         */
        public static function register ($observer) {
            # 如果是调试状态。
            if (defined('JTITA_DEBUGGER')) {
                Debugger::write('transaction',array('method' => 'register',$observer));
            }

            # 检查事务所需的方法是否存在；
            if (! method_exists ($observer, self::METHOD_BEGIN) ||
                ! method_exists ($observer, self::METHOD_COMPLETE) ||
                ! method_exists ($observer, self::METHOD_ROLLBACK)) {
                throw new TransactionException('JTK0401:Register transaction observer not support method.');
            } else if (self::hasRegister ($observer)){
                trigger_error ('JTK0402:Register repeat observer');
            }

            # 将观察者加入到观察者集合中；
            array_push (self::$_observers, $observer);
            # 如果事务已经开始，执行观察者开始事务；
            if (self::$_isBegin) {
                $observer->begin ();
            }

            return true;
        }

        /**
         * 根据$observer注销一个事务观查者对象。
         *
         * @param mixed $observer 观查者
         *
         * @return bool
         */
        public static function unRegister ($observer) {
            # 如果是调试状态。
            if (defined('JTITA_DEBUGGER')) {
                Debugger::write('transaction',array('method' => 'unRegister',$observer));
            }

            # 查找事务观察者，并删除；
            foreach (self::$_observers as $k => $ob){
                if ($ob === $observer){
                    unset(self::$_observers[$k]);
                    return true;
                }
            }

            return false;
        }

        /**
         * 返回一个布尔值，判断是否指定对象已注册
         *
         * @param mixed $observer 注册的事务观察者
         *
         * @return boolean
         */
        public static function hasRegister ($observer) {
            return in_array ($observer, self::$_observers, true);
        }

        /**
         * 执行开启事务
         */
        public static function begin () {
            # 如果是调试状态。
            if (defined('JTITA_DEBUGGER')) {
                Debugger::write('transaction',array('method' => 'begin'));
            }

            # 如果事务已经开启
            if (self::$_isBegin){
                trigger_error ('JTK0403:Repeat begin transaction.');
                return;
            }
            # 开始执行事务。
            foreach (self::$_observers as $observer){
                $observer->begin ();
            }

            self::$_isBegin = true;
        }

        /**
         * 执行回滚事务
         */
        public static function rollback () {
            # 如果是调试状态。
            if (defined('JTITA_DEBUGGER')) {
                Debugger::write('transaction',array('method' => 'rollback'));
            }

            if (! self::$_isBegin){
                trigger_error ('JTK0405:Transaction not begin.');
                return;
            }

            foreach (self::$_observers as $observer){
                $observer->rollback ();
            }

            self::$_isBegin = false;
        }

        /**
         * 执行完成事务
         */
        public static function complete () {
            # 如果是调试状态。
            if (defined('JTITA_DEBUGGER')) {
                Debugger::write('transaction',array('method' => 'complete'));
            }

            if (! self::$_isBegin){
                trigger_error ('JTK0406:Transaction not begin.');
                return;
            }

            foreach (self::$_observers as $observer){
                $observer->complete ();
            }

            self::$_isBegin = false;
        }

        /**
         * 返回一个布求值，表示是否正处在事务过程中；
         */
        public static function isBegin () {
            return self::$_isBegin;
        }

        /**
         * @var array 监控的事务支持对象；
         */
        private static $_observers = array();

        /**
         * @var boolean 是否开始事务；
         */
        private static $_isBegin = false;
    }