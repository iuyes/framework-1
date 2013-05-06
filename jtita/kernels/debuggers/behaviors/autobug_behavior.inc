<?php
    /**
     * autobug_behavior.inc 自动调试行为类AutobugBehavior声明文件。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.debuggers.behaviors
     * @version     1.0 jtita
     *
     * @create      2012-07-15
     * @modify      2013-04-28
     */

    # 导入调试器
    if (!defined('JTITA_DEBUGGER')) {
        Jtita::import('kernels.debuggers.debugger');
    }

    /**
     * 自动调试行为，提供自动附加调试跟踪的行为。
     *
     * @package jtita.kernels.debuggers.behaviors
     *
     * @example
     *  <pre>
     *      jtita::behaviors('kernels.datasources.databases.mysql','query',
     *                 array('class' => 'kernels.debuggers.behavior',
     *                       'name' => 'mysql'));
     *  </pre>
     */
    class AutobugBehavior extends AbstractBehavior {
        /**
         * @var string 调试记录名称。
         */
        public $name = 'default';

        /**
         * 执行调试跟踪行为；
         */
        protected function _invoke ($target, $args) {
            # 记录到调试器；
            Debugger::write ($this->name, $args);

            # 调用下一个行为。
            return $this->_next->invoke ($target, $args);
        }
    }