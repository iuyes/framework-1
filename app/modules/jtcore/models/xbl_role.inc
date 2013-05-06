<?php
/**
 * xbl_role.php 角色模型，提供角色的业务模型声明。
 *
 * @copyright   Copyright 2012 http://www.xbutu.com
 * @author      Zheng Guoping<zhenggp2004@163.com>
 *              (http://weibo.com/zhenggp)
 * @license     http://www.xbutu.com/license.txt
 *
 * @package     xbutu.xblives.models
 * @version     0.9
 *
 * @create      2012-12-11
 * @modify      2013-01-28
 */

/**
 * 角色模型对象，提供对管理员的间接权限支持；
 *
 * @package xbutu.xblives.models
 */
class XblRole extends ApplicationModel {
    protected  static function _validateRules($on) {
        return array('name' => 'required');
    }

    protected  static function _validateMessages($on) {
        return array('name' => '角色名称不能为空.');
    }
}