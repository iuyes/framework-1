<?php
    /**
     * autotrans_behavior.inc 自动行事务支持行为AutotransBehavior类声明文件
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.transactions.behaviors
     * @version     1.0 beta
     *
     * @create      2012-07-09
     * @modify      2013-04-02
     */

    if (!defined('JTITA_TRANSACTION')) {
        Jtita::import('kernels.transactions.transaction');
    }

    /**
     * 自动事务支持类，提供通过行为实现事务的自动支持服务。
     *
     * @example
     *  <pre>
     *      Jtita::behaviors('orders.service.submit_service','submit',
     *                       'kernels.transactions.behavior.autotrans_behavior' => 'read');
     *
     *     Jtita::behaviors('orders.service.submit_server','submit','transactions.behavior.autotrans_behavior');
     *  </pre>
     *
     * @package jtita.kernels.transactions.behaviors
     */
    class AutotransBehavior extends AbstractBehavior {
        /**
         * 执行自动事务管理；
         */
        protected function _invoke ($target, $args) {
            try {
                Transaction::begin ();
                $result = $this->_next->invoke ($target, $args);
                Transaction::complete ();

                return $result;
            } catch (Exception $ex) {
                Transaction::rollback ();
                throw $ex;
            }

        }
    }