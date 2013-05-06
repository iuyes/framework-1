<?php
    /**
     * xbl_user.php 管理员模型，提供管理员的业务模型。
     *
     * @copyright   Copyright 2012 http://www.xbutu.com
     * @author      Zheng Guoping<zhenggp2004@163.com>
     *              (http://weibo.com/zhenggp)
     * @license     http://www.xbutu.com/license.txt
     *
     * @package     xbutu.xblives
     * @version     0.9
     *
     * @create      2012-12-11
     * @modify      2013-01-24
     */

    /**
     * 用户模型，提供xblives的管理账户；
     */
    class XblUser extends ApplicationModel {
        /**
         * @const 允许访问
         */
        const ACCESS_ALLOW = 1;

        /**
         * @const 禁止访问。
         */
        const ACCESS_DENY = 0;

        /**
         * 根据$conditions开启管理员访问，并返回影响到的行；
         *
         * @param array $conditions 条件
         *
         * @return integer
         */
        public static function allow ($conditions) {
            return self::updateAll (array('allow' => self::ACCESS_ALLOW), $conditions);
        }

        /**
         * 根据$conditions禁止管理员的访问，并返回影响到的行；
         *
         * @param array $conditions 条件
         *
         * @return integer
         */
        public static function deny ($conditions) {
            return self::updateAll (array('allow' => self::ACCESS_DENY), $conditions);
        }

        /**
         * 为管理员分配角色$roles。
         *
         * @param array $roles 角色集
         *
         * @return boolean
         */
        public function assignRoles ($roles) {
            return self::assignMultiple ('roles', $this->key (), $roles);
        }

        /**
         * 重写创建回调方法，加入密码key值；
         */
        protected function _createBefore ($syntax, $options) {
            $this->passkey = 'Hello world';
            $this->full_name = 'hello World';
        }

        /**
         * 重写更新回调方法。
         *
         * @see Model::_updateBefore
         */
        protected function _updateBefore ($syntax, $options) {
            # 如果没有更新密码。
            if (empty($this->_currentProps['password'])) {
                unset($this->_currentProps['password']);
            }

            return true;
        }

        protected static function _validateRules ($on) {
            static $validators = array('create' => array('name' => array('required' => true, 'minlen' => 6),
                'email' => array('required' => true, 'email' => true),
                'password' => array('required' => true, 'minlen' => 6)),
                'update' => array('name' => array('required' => true, 'minlen' => 6),
                    'email' => array('required' => true, 'email' => true)));

            return $validators[$on];
        }

        protected static function _validateMessages ($on) {
            static $messages = array('name' => '用户名不能为空，并且必须大于6个字符。', 'email' => '邮件格式不正确，请录入正确的邮件地址。',
                'password' => '为了保证安全，录入的密码值请大于6个字符');

            return $messages;
        }

        public static function dataRelations () {
            return array('group' => array('type' => 'belongsTo', 'alias' => 'Group',
                'className' => 'xblives.models.xbl_user_group', 'foreign' => 'group_id'),
                'roles' => array('type' => 'hasAndBelongsTo', 'className' => 'xblives.models.xbl_role',
                    'relationTable' => 'xbl_user_role', 'primaryKey' => 'user_id', 'foreignKey' => 'role_id'));
        }
    }