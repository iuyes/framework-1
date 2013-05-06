<?php
    /**
     * ghtml_test.inc 实现对ghtml服务的单元测试；
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.ghtmls
     * @version     1.0 beta
     *
     * @create      2013-04-08
     * @modify      2013-04-08
     */

    # 声明测试根路径;
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (__FILE__))))));

    # 引用被测试类
    require_once(JTPATH_ROOT . '/jtita/ghtmls/ghtml.inc');

    /**
     * Ghtml 测试类，实现对Ghtml类的测试。
     */
    class GhtmlTest extends PHPUnit_Framework_TestCase {

    }