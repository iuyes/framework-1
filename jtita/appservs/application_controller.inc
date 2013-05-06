<?php
    /**
     * application_controller.inc 应用控制器类声明文件
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.jtita.com/license.txt
     *
     * @package     jtita.appservs
     * @version     1.0 beta
     *
     * @create      2012-07-23
     * @modify      2013-04-07
     */

    /**
     * 判断是否包含基础类，未包含则执行导入服务
     */
    Jtita::importInterface ('controllers.controllers');

    /**
     * 应用控制器，如果需要提供自定义服务，可以在app/appservs中重写本文件，提供自定义控制器。
     *
     * @package jtita.appservs
     */
    class ApplicationController extends Controller {

    }
	