<?php
/**
 * xbl_user_group.php 提供xblives管理组模型声明。
 *
 *  Copyright 2012 http://xbutu.com
 *
 * @package    xbutu/lives
 * @version    0.1
 *
 * @author        Zheng Guoping(http://weibo.com/zhenggp)
 * @create        2012-10-25
 * @modify        2013-1-25
 */

/**
 * xblives 管理组模型声明类。
 */
class XblUserGroup extends ApplicationModel {

    /**
     * 根据$on返回验证规则信息。
     *
     * @param string $on 验证目标。
     * @return array|void 验证规则
     */
    protected static function _validateRules($on) {
        # 返回验证规则
        return array('name' => array(
            'required' => true
        ));
    }

    /**
     * 根据$on返回验证错误消息。
     *
     * @param string $on 验证目标
     * @return array|void 验证消息
     */
    protected  static function _validateMessages($on) {
        return array('name' => '管理组名称不能为空');
    }

}