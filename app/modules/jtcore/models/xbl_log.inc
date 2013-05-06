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
     * 日志记录模型，提供日志记录服务。
     *
     * @package xbutu.xblives
     */
    class XblLog extends ApplicationModel {

        /**
         * 使用$title、$category、$startDate和$endDate搜索日志记录。
         *
         * @param null|string $title     标题
         * @param null|string $category  分类
         * @param null|string $startDate 开始日期
         * @param null|string $endDate   结束日期
         * @return ModelFinder
         */
        public static function query ($title = null, $category = null, $startDate = null, $endDate = null) {
            return self::findAll (self::_conditions ($title, $category, $startDate, $endDate));
        }


        /**
         * 根据$title、$category、$startDate和$endDate清空日志信息。
         *
         * @param null|string $title     标题
         * @param null|string $category  分类
         * @param null|string $startDate 开始时间
         * @param null|string $endDate   结束时间
         * @return int 响应的行。
         */
        public static function clear ($title = null, $category = null, $startDate = null, $endDate = null) {
            return self::deleteAll (self::_conditions ($title, $category, $startDate, $endDate));
        }

        /**
         * 获取日志分类信息。
         *
         * return array
         */
        public static function categories () {
            return self::findAll()->fields('distinct XblLog.category,XblLog.category')->klist();
        }

        /**
         * 根据$title、$category、$startDate和$endDate生成查询条件。
         *
         * @param null|string $title     标题
         * @param null|string $category  分类
         * @param null|string $startDate 开始日期
         * @param null|string $endDate   结束日期
         * @return array
         */
        protected static function _conditions ($title, $category, $startDate, $endDate) {
            $conditions = array();

            if (! empty ($title)) {
                $conditions['XblLog.subject like '] = addslashes ("%$title%");
            }

            if (! empty ($category)) {
                $conditions['XblLog.category'] = addslashes ($category);
            }

            if (! empty ($startDate)) {
                $conditions['XblLog.created > '] = strtotime ($startDate);
            }
            if (! empty ($endDate)) {
                $conditions['XblLog.created < '] = $startDate($endDate);
                return $conditions;
            }

            return $conditions;
        }
    }