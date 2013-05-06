<?php
    /**
     * visitor.php 网站流量访问控制器，提供网站流量；
     *
     * @copyright   Copyright 2012 http://www.xbutu.com
     * @author      Zheng Guoping<zhenggp2004@163.com>
     *              (http://weibo.com/zhenggp)
     * @license     http://www.xbutu.com/license.txt
     *
     * @package     xbutu.xblives
     * @version     0.9
     *
     * @create      2013-03-19
     * @modify      2013-03-19
     */

    # 导入存储对象。
    Xbutu::import ('xblives.models.xbl_visitor');

    /**
     * 访问控制器，提供网站的访问统计服务。
     */
    class VisitorController extends ApplicationController {
        /**
         * 执行统计
         */
        public function indexAction () {
            $conditions = array();
            $group = intval ($this->query ('group', 0));
            $cat = intval ($this->query ('cat', 0));
            $sub = intval ($this->query ('sub', 0));

            if (! empty($group)) {
                $conditions['XblVisitor.group'] = $group;
            }
            if (! empty($cat)) {
                $conditions['XblVisitor.cat'] = $cat;
            }
            if (! empty($sub)) {
                $conditions['XblVisitor.sub_cat'] = $sub;
            }

            # 根据配置读取创建时间
            $created = strtotime (date (Xbutu::read ('settings.xblive.visitor', 'Y-m-d h:i', time ())));
            $conditions['XblVisitor.created'] = $created;

            # 判断当前是否已有内容
            $count = XblVisitor::findFields ('COUNT(*) as count');
            if (empty($count['count'])) {
                XblVisitor::createAll (array('group_id' => $group, 'cat' => $cat, 'sub_cat' => $sub, 'visitors' => 1,
                    'created' => $created));
            }
            else {
                XblVisitor::updateAll ('XblVisitor.visitors = XblVisitor.visitors + 1', $conditions);
            }

            $this->response->disableCache ();
            $this->_autoRender = false;
        }

    }