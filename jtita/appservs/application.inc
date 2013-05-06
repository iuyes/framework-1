<?php
    /**
     * application.inc 提供应用请求前端类声明
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
     * 引入应用前端基础类
     */
    Jtita::importInterface ('applications.application_base');

    /**
     * 应用前端控制器类，提供应用请求的相关服务
     *
     * 可以在app/appservs目录下重写本文件，提供自定义应用请求处理。
     *
     * @package jtita.appservs
     */
    class Application extends ApplicationBase {

    }