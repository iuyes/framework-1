<?php
    /**
     * autolog_behavior.inc 自动日志行为AutologBehavior类声明文件。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.loggers.behaviors
     * @version     1.0 beta
     *
     * @create      2012-07-10
     * @modify      2013-04-26
     */

    /**
     * 自动日志行为，提供日志记录服务
     *
     * @package   jtita.kernels.loggers.behaviors
     *
     * @example
     *      <pre>
     *            Jtita::behaviors('orders.services.order_service','submit',
     *            array('class'   => 'kernels.loggers.behaviors.autolog_behavior',
     *                  'subject' => 'oss - submit order'),
     *                  'message'  => 'submit order for ',
     *                  'category' => 1));
     *      </pre>
     */
    class AutologBehavior extends AbstractBehavior {
        /**
         * @var string 日志分类。
         */
        public $category = 'normal';

        /**
         * @var string 默认主题、array_express。
         */
        public $subject = 'Autolog subject: default';

        /**
         * @var string 自动消息，array_express。
         */
        public $message = 'Autolog message: default';

        /**
         * 执行自定义行为操作，并执行方法链。
         *
         * @param array $target 方法目标 array($sender,$method)
         * @param array $args   方法参数
         *
         * @return mixed
         */
        protected function _invoke ($target, $args) {
            Logger::write (array_express ($args, $this->subject),
                           $this->category,'none', array_express ($args, $this->message));

            return $this->_next->invoke ($target, $args);
        }
    }