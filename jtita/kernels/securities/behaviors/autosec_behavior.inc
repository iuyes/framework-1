<?php
    /**
     * security.inc 提供架构安全服务。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.securities
     * @version     1.0 beta
     *
     * @create      2012-03-27
     * @modify      2013-04-28
     */

    /**
     * 自动安全检查行为，为对象提供额外的安全检查。
     *
     * @package jtita.kernels.securities.behaviors
     */
    class AutosecBehavior extends AbstractBehavior{
        /**
         * @var string 采用的授权方法。
         */
        public $authorize = 'default';

        /**
         * @var string 安全资源对象。
         */
        public $sro = '*';

        /**
         * @override _invoke
         */
        protected function _invoke ($target, $args) {
            return $this->_next->invoke ($target, $args);
        }
    }