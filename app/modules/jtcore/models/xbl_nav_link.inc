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
     * 导航连接模块，提供导航中的连接信息。
     *
     * @package xbutu.xblives
     */
    class XblNavLink extends ApplicationModel {
        /**
         * @const 分隔符标记
         */
        const SEPARATOR = '-----';
        /**
         * @const 标题标记
         */
        const TITLE = '--title--';
    }