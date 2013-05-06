<?php
    /**
     * user_groups_controller.php 用户组控制器，提供用户组管理交互功能。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.jtita.com/license.txt
     *
     * @package     jtita.jtcore
     * @version     1.0 beta
     *
     * @create      2012-12-11
     * @modify      2013-04-15
     */

    /**
     * 导入相关模型
     */
    Jtita::import ('jtcore.models.xbl_user_group');

    /**
     * 用户组控制器，提供用户组的后台管理服务。
     */
    class UserGroupsController extends AdminsController {
        /**
         * 用户组默认活动，提供默认的管理。
         */
        public function indexAction () {
            # 查询当前编号
            if ($id = $this->query ('id', false)) {
                $this->group = XblUserGroup::find ($id);
            }

            $this->groups = XblUserGroup::findAll ();
        }

        /**
         * 编辑活动，提供编辑用户组的功能。
         */
        public function editAction () {
            # 使用创建或更新对象，创建管理组
            $this->group = XblUserGroup::createForUpdate ($this->forms ('UserGroup'));

            if ($this->group->save ()) {
                $this->message ('保存管理组"' . $this->group->name . '"信息成功。', 'success');
                $this->redirect ('index');
            }
            else {
                $this->message ('保存管理组信息失败，请检查字段值是否有效。', 'fail');
                $this->groups = XblUserGroup::findAll ();

                $this->render ('index');
            }
        }

        /**
         * 删除活动，提供删除用户组的功能。
         */
        public function deleteAction () {
            # 如果方法为删除方法，并且包含ID值。
            if ($this->forms ('method') == 'DELETE' && $this->forms ('id')) {
                $rows = XblUserGroup::deleteAll (array('XblUserGroup.id' => $this->forms ('id')));
                $this->message ('删除管理组信息成功共删除' . $rows . '个', 'success');
            }
            else {
                $this->message ('删除管理组信息失败');
            }

            $this->redirect ('index');
        }

        public function dialogAction () {

        }

    }