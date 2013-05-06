<?php
    /**
     * roles_controller.php 角色控制器，提供角色处理所需的服务。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.jtita.com/license.txt
     *
     * @package     jtita.jtcore
     * @version     1.0 beta
     *
     * @create      2012-01-27
     * @modify      2013-04-15
     */

    # 导入角色模型；
    Jtita::import ('jtcore.models.xbl_role');

    /**
     * 角色集控制器，提供角色信息的后台管理服务。
     */
    class RolesController extends AdminsController {

        /**
         * 默认活动；
         */
        public function indexAction () {
            # 获取查询条件
            $conditions = array();
            # 判断是否包含角色名字
            if ($name = $this->query ('name')) {
                $conditions['XblRole.name like '] = "%$name%";
            }

            $this->roles = XblRole::findAll ($conditions);
        }

        /**
         * 增加角色活动；
         */
        public function newAction () {
            # 如果是提交方法
            if ($this->isPost ()) {
                $this->role = new XblRole($this->forms ('XblRole'));

                if ($this->role->save ()) {
                    $this->message ('创建角色"' . $this->role->name . '"信息成功', 'success');
                    $this->redirect ('index');
                }
                else {
                    $this->message ('创建角色信息失败，请检查角色字段', 'fail');
                }
            }
        }

        /**
         * 编辑活动；
         */
        public function editAction () {
            if ($this->isPost ()) {
                $this->role = new XblRole($this->forms ('XblRole'), true);

                if ($this->role->save ()) {
                    $this->message ('更新角色"' . $this->role->name . '"信息成功.', 'success');
                    $this->redirect ('index');
                }
            }
            else {
                $this->role = XblRole::find ($this->query ('id'));
            }
        }

        /**
         * 删除活动；
         */
        public function deleteAction () {
            # 如果方法为删除方法并且包含id值。
            if ($this->isPost () && $this->forms ('method') == 'DELETE' && $this->forms ('id')) {
                if ($rows = XblRole::deleteAll (array('XblRole.id' => $this->forms ('id')))) {
                    $this->message ('删除角色信息成功，共删除' . $rows . '个角色.', 'success');
                }
            }

            $this->redirect ('index');
        }
    }