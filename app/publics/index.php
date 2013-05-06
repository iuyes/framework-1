<?php
    /**
     * index.php 应用全局入口文件
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     app.publics
     * @version     1.0 beta
     *
     * @create      2012-07-22
     * @modify      2013-04-28
     */

    # 开启SESSION服务；
    session_start ();

    # 导入启动文件
    require(dirname (dirname (__FILE__)) . '/boots/core.inc');

    # 导入配置文件和前端应用控制器。
    Jtita::import ('boots.behaviors');
    Jtita::import ('appservs.application');

    # 创建前端控制器
    $app = Application::instance ();

    try {
        $app->initialize ();
        $app->dispatching ();
    } catch (Exception $ex) {
        $app->error ($ex);
    }

    $app->end ();