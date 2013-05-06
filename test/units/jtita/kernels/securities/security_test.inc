<?php
    /**
     * security_test.inc 提供安全架构单元测试服务。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.securities
     * @version     1.0 beta
     *
     * @create      2013-04-03
     * @modify      2013-04-28
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

    /**
     * 导入测试事务类；
     */
    require_once(JTPATH_ROOT . '/jtita/kernels/securities/security.inc');

    /**
     * 安全服务单元测试。
     *
     * @package jtita.kernels.securites
     */
    class SecurityTest extends PHPUnit_Framework_TestCase {

        public function testHello() {

        }
    }