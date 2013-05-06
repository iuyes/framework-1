<?php
    /**
     * access_controller.php 网站流量访问控制器，提供网站流量的后台查阅功能；
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.jtita.com/license.txt
     *
     * @package     jtita.jtcore
     * @version     1.0 beta
     *
     * @create      2012-01-29
     * @modify      2013-04-15
     */

    # 导入依赖包
    Jtita::import ('jtcore.models.xbl_visitor');
    Jtita::import ('jtcore.models.xbl_visitor_group');

    /**
     * 网站流量后台管理控制器。
     *
     * @package xbutu.xblives
     */
    class AccessController extends AdminsController {

        /**
         * 默认活动；
         */
        public function indexAction () {
            $this->groups = XblVisitorGroup::findAll ();
        }

        /**
         * 增加组
         */
        public function newGroupAction () {
            if ($this->isPost ()) {
                $this->group = new XblVisitorGroup ($this->forms ('XblVisitorGroup'));
                # 保存内容
                if ($this->group->save ()) {
                    $this->message ('创建访问标记信息成功', 'success');
                }
                else {
                    $this->message ('创建访问标记失败', 'fail');
                }
            }

            $this->redirect ('index');
        }

        /**
         * 编辑组标记
         */
        public function editGroupAction () {
            if ($this->isPost ()) {
                $this->group = new XblVisitorGroup ($this->forms ('XblVisitorGroup'), true);
                # 保存内容
                if ($this->group->save ()) {
                    $this->message ('更新访问标记信息成功', 'success');
                }
                else {
                    $this->message ('更新访问标记失败', 'fail');
                }
            }

            $this->redirect ('index');
        }

        /**
         * 删除组
         */
        public function deleteGroupAction () {
            if ($this->isPost ()) {
                if (XblVisitorGroup::deleteAll ($this->forms ('id'))) {
                    $this->message ('删除访问标记信息成功', 'success');
                }
                else {
                    $this->message ('删除访问标记失败', 'fail');
                }
            }

            $this->redirect ('index');
        }
    }