<?php
    /**
     * security_filter_test.inc  实现对安全过滤的单元测试。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.jtita.com/license.txt
     *
     * @package     jtita.controllers
     * @version     1.0 beta
     *
     * @create      2013-04-08
     * @modify      2013-04-08
     */

    # 声明测试根路径;
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (dirname (__FILE__)))))));

    # 引用被测试类
    require_once(JTPATH_ROOT . '/jtita/controllers/filters/security_filter.inc');

    /**
     * SecurityFilter 测试类，实现SecurityFilter类的单元测试。
     *
     * @package jtita.controllers.filters
     */
    class SecurityFilterTest extends PHPUnit_Framework_TestCase {

    }