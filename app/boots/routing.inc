<?php
    /**
     * routing.inc 路由全局配置文件
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.jtita.com/license.txt
     *
     * @package     app.boots
     * @version     1.0 beta
     *
     * @create      2012-07-22
     * @modify      2013-04-12
     */

    # 配置默认风格
    Router::domain ('www.example.com', array('domain' => 'http://www.example.com','style' => 'normal_router_style'))
           ->rule(array('class'         => 'normal_router_rule',
                        'controller'    => 'index',
                        'action'        => 'index',
                        'package'       => array(null,'[^admins]')));

    Router::domain ('admin.example.com', array('domain' => 'http://admin.example.com','style' => 'normal_router_style'))
            ->rule(array('class'      => 'normal_router_rule',
                         #'express'    => ':controller/:action',
                         'module'     => 'jtcore',
                         'controller' => 'index',
                         'action'     => 'index',
                         'package'    => array('admins','admins|admins.reports')),'admins');
    /*
            ->rule(array('class'      => 'regex_router_rule',
                         'express'    => ':controller',
                         'module'     => 'jtcore',
                         'controller' => 'index',
                         'action'     => 'index',
                         'package'    => array('admins','admins|admins.reports')),'admins');

    */