<?php
    /**
     * navs_controller.php 声明网站导航管理控制器；
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.jtita.com/license.txt
     *
     * @package     jtita.jtcore
     * @version     1.0 beta
     *
     * @create      2013-01-29
     * @modify      2013-04-15
     */
    Jtita::import ('jtcore.models.xbl_nav');
    Jtita::import ('jtcore.models.xbl_nav_link');

    /**
     * 网站导航管理控制器。
     *
     * @package xbutu.xblives
     */
    class NavsController extends AdminsController {
        /**
         * 默认活动；
         */
        public function indexAction () {
            # 如果当前还未指向任何的导航菜单
            if ($nid = $this->query ('nid')) $this->nav = XblNav::find ($nid);
            else $this->nav = XblNav::find ();

            if (! $this->nav) {
                $this->message ('当前还未定义任何导航菜单，请首先增加导航菜单.', 'info');
                $this->redirect ('new');
            }

            $this->navs = XblNav::findAll ();
        }


        public function saveAction () {
            if ($this->isPost () == false) {
                return $this->redirect ('index');
            }

            $this->nav = new XblNav($this->forms ('XblNav'), true);

            if ($this->nav->save ()) {
                $this->navs = $this->forms ('links');
                foreach ($this->navs as $nav) {
                    if ($nav['method'] == 'delete') {
                        XblNavLink::deleteAll ($nav['id']);
                    }
                    else if ($nav['method'] == 'update' || $nav['method'] == 'insert') {
                        $link = new XblNavLink($nav, $nav['method'] == 'update');
                        $link->nav_id = $this->nav->id;
                        $link->save ();
                    }
                }

                $this->message ('更新导航信息成功', 'success');
                $this->redirect (array('action' => 'index', 'nid' => $this->nav->id));
            }
        }

        /**
         * 增加活动。
         */
        public function newAction () {
            if ($this->isPost ()) {
                $this->nav = new XblNav($this->forms ('XblNav'));

                if ($this->nav->save ()) {
                    $this->message ("创建导航\"{$this->nav->name}\"信息成功", 'success');
                    $this->redirect (array('action' => 'index', 'nav_id' => $this->nav->key ()));
                }
                else {
                    $this->message ("创建导航信息失败");
                }
            }

            $this->navs = XblNav::findAll ();
        }
    }