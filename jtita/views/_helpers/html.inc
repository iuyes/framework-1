<?php
    /**
     * html.inc 提供基本的HTML生成助手方法
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.views
     * @version     1.0 beta
     *
     * @create      2012-07-23
     * @modify      2013-04-28
     */

    /**
     * 使用htmlspecialchars方法处理$text参数
     *
     * @param  string $text 录入文本
     * @return string
     */
    function _h ($text) {
        return htmlspecialchars ($text);
    }

    /**
     * 使用htmlspecialchars格式化$text文本，并且指定为$len，从$start开始
     *
     * @param string  $text  录入文本
     * @param int     $len   文件长度
     * @param int     $start 开始索引
     * @return string
     */
    function _hs ($text, $len, $start = 0) {
        return mb_substr (htmlspecialchars ($text), $start, $len);
    }

    /**
     * Html element generate 根据$elementName、$options、$addition和$inner生成HTML标签。
     *
     * @param string $elementName  元素名称
     * @param array  $options      元素选项
     * @param string $addition     附加属性
     * @param string $inner        标签内容
     * @return string
     */
    function _heg ($elementName, $options = array(), $addition = '', $inner = '') {
        # 如果$options为数组属性，则生成HTML Attribute
        if (is_array ($options)) {
            $options = array_htmlattr ($options);
        }

        return "<$elementName $options $addition>$inner</$elementName>";
    }

    /**
     * 根据$url生成URL地址。
     *
     * @param mixed       $url   URL生成选项
     * @param string|bool $rule  规则名字
     * @return string
     */
    function _url ($url = array(), $rule = false) {
        return Router::url ($url, $rule);
    }

    /**
     * 根据$input生成满足js要求的obj字符串。
     *
     * @param $input 生成格式
     * @return string
     */
    function _jsobj ($input) {
        return array_js ($input);
    }

    /**
     * 根据$text、$url和$options生成A标签HTML
     *
     * @param string      $text     超连接文本。
     * @param mixed       $url      超连接地址。
     * @param mixed       $options  选项。
     * @param string|bool $rule     路由规则。
     * @return string
     */
    function _linkTo ($text, $url = array(), $options = array(), $rule = false) {
        # 如果$url为数组，使用Router生成URL地址信息。
        if (is_array ($url)) $url = Router::url ($url, $rule);
        # 使用_heg生成HTML标签。
        return _heg ('a', $options, "href=\"$url\"", $text);
    }

    /**
     * 使用$image、$url和$options生成带图片的A标签HTML。
     *
     * @param array       $image   图片选项
     * @param array       $url     URL地址选项
     * @param array       $options 选项
     * @param string|bool $rule    规则
     * @return string
     */
    function _linkImage ($image, $url = array(), $options = array(), $rule = false) {
        return _linkTo (_heg ('img', $image), $url, $options, $rule);
    }

    /**
     * 使用$email生成带mailto的A标签HTML
     *
     * @param string $email   邮件地址
     * @param array  $options 选项
     * @return string
     */
    function _mailTo ($email, $options = array()) {
        $options['href'] = "mailto:$email";
        # 使用heg生成A标签
        return _heg ('a', $options, '', $email);
    }

    /**
     * 根据$date返回格式化日期字符串。
     *
     * @param integer $date   时间戳
     * @param string  $format 默认日期格式化
     * @return string
     */
    function _date ($date, $format = JTFORMAT_DATE) {
        echo date ($format, $date);
    }

    /**
     * 根据$date返回格式化时间字符串。
     *
     * @param integer $date   时间戳
     * @param string  $format 格式器
     * @return string
     */
    function _datetime ($date, $format = JTFORMAT_DATETIME) {
        return date ($format, $date);
    }

    /**
     * 根据$time返回格式化时间字符串。
     *
     * @param integer $time   时间戳
     * @param string  $format 格式
     * @return string
     */
    function _time ($time, $format = JTFORMAT_TIME) {
        return date ($format, $time);
    }

    /**
     * 根据$date返回时间的周信息。
     *
     * @param integer $date   时间
     * @param string  $format 格式
     * @return string
     */
    function _week ($date, $format = JTFORMAT_WEEK) {
        return date ($format, $date);
    }

    /**
     * 根据$date返回过去的时间字符串。
     *
     *
     * @param int $date 时间戳
     * @return string
     */
    function _lasttime ($date) {
        # 获取时间差
        $time = time () - $date;

        if ($time < 60) return $time . '秒前';
        if ($time < 3600) return floor ($time / 60) . '分前';
        if ($time < 86400) return floor ($time / 3600) . '小时前';
        if ($time < 4320000) return floor ($time / 86400) . '天前';
        else return '3个月前';
    }
