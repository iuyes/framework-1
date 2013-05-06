<?php
    /**
     * sign_controller.php 登陆控制器
     *
     * @copyright   Copyright 2012 http://www.xbutu.com
     * @author      Zheng Guoping<zhenggp2004@163.com>
     *              (http://weibo.com/zhenggp)
     * @license     http://www.xbutu.com/license.txt
     *
     * @package     xbutu.xblives
     * @version     0.9
     *
     * @create      2013-03-05
     * @modify      2013-03-05
     */

    /**
     * 登陆控制器，提供后台登陆服务。
     *
     * @package xbutu.xblives
     */
    class SignController extends AdminsController {
        /**
         * 登入行为
         */
        public function indexAction () {
            if ($this->isPost ()) {
                $this->redirect (array('controller' => 'index', 'action' => 'index'));
            }
        }

        /**
         * 找回密码。
         */
        public function passwordAction () {

        }

        /**
         * 登出行为
         */
        public function outAction () {
            $this->redirect ('index');
        }
    }

