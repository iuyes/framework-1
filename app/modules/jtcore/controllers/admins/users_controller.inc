<?php
    /**
     * users_controller.php 用户控制器，提供用户管理交互功能。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.jtcore
     * @version     1.0 beta
     *
     * @create      2012-11-05
     * @modify      2013-04-15
     */

    /**
     * 引入模型对象；
     */
    Jtita::import ('jtcore.models.xbl_user');
    Jtita::import ('jtcore.models.xbl_role');
    Jtita::import ('jtcore.models.xbl_user_group');

    Jtita::import ('kernels.loggers.logger');

    /**
     * 用户管理控制器，提供用户逻辑控制服务；
     */
    class UsersController extends AdminsController {
        # 引用分页组件
        public $filters = array('users' => array('class' => 'controllers.filters.paginate_filter',
            'finder' => 'users'));

        /**
         * 用户主页活动；
         */
        public function indexAction () {
            # 条件过滤参数
            $conditions = array();

            if ($name = $this->query ('name')) {
                $conditions['XblUser.name like '] = "%$name%";
            }
            if ($group = $this->query ('group')) {
                $conditions['XblUser.group_id'] = $group;
            }

            $this->users = XblUser::findAll ($conditions);
            $this->groups = XblUserGroup::findAll ();
        }

        /**
         * 执行搜索功能。
         */
        public function searchAction () {
            $this->redirect (array('action' => 'index',
                                   'name' => $this->forms ('name',null),
                                   'group' => $this->forms('group',null)));
        }

        /**
         * 用户创建活动；
         */
        public function newAction () {
            # 如果当前请求为提交请求；
            if ($this->isPost ()) {
                $this->user = new XblUser($this->forms ('XblUser'));
                # 如果保存数据成功；
                if ($this->user->save () && $this->user->assignRoles ($this->forms ('roles', array()))) {
                    Logger::write ('创建新的管理员"' . $this->user->name . '"成功', 'xblives', '郑国平');
                    $this->message ('增加管理员信息成功');
                    return $this->redirect ('index');
                }
            }

            $this->roles = XblRole::findAll ();
            $this->groups = XblUserGroup::findAll ();
        }

        /**
         * 用户更新活动；
         */
        public function editAction () {
            # 如果当前请求是更新请求；
            if ($this->isGet ()) {
                $this->user = XblUser::find ($this->query ('id'));
            }
            else {
                $this->user = new XblUser($this->forms ('XblUser'), true, true);

                if ($this->user->save () && $this->user->assignRoles ($this->forms ('roles', array())) > - 1) {
                    $this->message ('更新管理员"' . $this->user->name . '"成功', 'success');
                    $this->redirect ('index');
                }
            }

            $this->groups = XblUserGroup::findAll ();
            $this->roles = XblRole::findAll ();
        }

        /**
         * 用户删除活动；
         */
        public function deleteAction () {
            # 判断是否删除
            if ($this->isPost () && $this->forms ('method') == 'DELETE' && $this->forms ('id')) {
                $rows = XblUser::deleteAll (array('XblUser.id' => $this->forms ('id')));
                $this->message ("删除管理员信息成功，共删除{$rows}个管理员", 'success');
            }
            else {
                $this->message ("删除管理员信息失败");
            }

            $this->redirect (array('action' => 'index', 'fullUrl' => true));
        }

        /**
         * 开启管理员访问权限。
         */
        public function allowAction () {
            // 判断删除
            if ($this->isPost () && $this->forms ('method') == 'PUT' && $this->forms ('id')) {
                if ($row = XblUser::allow (array('XblUser.id' => $this->forms ('id')))) {
                    $this->message ("重新开启管理员访问权限，共重新开启{$row}个管理员访问权限", 'success');
                }
                else {
                    $this->message ('选择的管理员已经是开启访问状态.', 'info');
                }
            }

            $this->redirect (array('action' => 'index', '_fullPath' => true));
        }

        /**
         * 禁止管理员访问权限。
         */
        public function denyAction () {
            // 判断删除
            if ($this->isPost () && $this->forms ('method') == 'PUT' && $this->forms ('id')) {
                if ($row = XblUser::deny (array('XblUser.id' => $this->forms ('id')))) {
                    $this->message ("禁止管理员的访问，共禁止{$row}个管理员访问权限", 'success');
                }
                else {
                    $this->message ('选择的管理员已经禁止访问.', 'info');
                }
            }

            $this->redirect (array('action' => 'index', '_fullPath' => true));
        }
    }