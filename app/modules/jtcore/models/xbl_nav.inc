<?php
    /**
     * xbl_nav.php 导航模型声明类。
     *
     * @copyright   Copyright 2012 http://www.xbutu.com
     * @author      Zheng Guoping<zhenggp2004@163.com>
     *              (http://weibo.com/zhenggp)
     * @license     http://www.xbutu.com/license.txt
     *
     * @package     xbutu.xblives
     * @version     0.9
     *
     * @create      2013-03-08
     * @modify      2013-03-08
     */

    /**
     * 导航模型。
     *
     * @package xbutu.xblives
     */
    class XblNav extends ApplicationModel {

        /**
         * 数据关系
         *
         * @return array|void
         */
        public static function dataRelations () {
            return array('links' => array('className' => 'xblives.models.xbl_nav_link', 'type' => 'hasMany',
                'foreignKey' => 'nav_id', 'order' => 'sort'));
        }
    }

