<?php
    /**
     * validators.inc 验证器方法声明文件，提供常规验证器的方法声明。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.validations
     * @version     1.0 beta
     *
     * @create      2012-11-11
     * @modify      2014-04-28
     */

    /**
     * 非空验证器，验证字段的值为非空值，成功返回true，失败返回false。
     *
     * @param ArrayAccess $input   验证对象，必须支持ArrayAccess访问
     * @param string      $field   验证字段
     * @param array       $options 验证选项
     * @return boolean
     */
    function requiredValidator ($input, $field, $options = array()) {
        return empty($input[$field]) == false;
    }

    /**
     * 正则表达式验证，验证字段的值是否满足正则模式规则，验证成功返回true，失败返回false。
     *
     * @param ArrayAccess $input   验证对象，必须支持ArrayAccess访问
     * @param string      $field   验证字段
     * @param string      $pattern 正则模式
     * @return boolean
     */
    function regexValidator ($input, $field, $pattern = '*') {
        # 如果验证值为空，返回true；
        if (empty($input[$field])) return true;
        else return preg_match ($pattern, $input[$field]) != false;
    }

    /**
     * 数字验证，验证字段的值是否为数字，成功返回true，失败返回false。
     *
     * @param ArrayAccess $input   验证对象，必须支持ArrayAccess访问
     * @param string      $field   验证字段
     * @param array       $options 验证选项
     * @return boolean
     */
    function numberValidator ($input, $field, $options = array()) {
        # 如果验证值为空，返回true
        if (empty($input[$field])) return true;
        else return preg_match ('/^\d*$/', $input[$field]) != false;
    }

    /**
     * 电话号码验证器，验证字段的值是否为正确的电话号码，验证成功返回true，失败返回false。
     *
     * 支持坐机(xxx-xxxxxxxx/xxxx-xxxxxxxx）、手机(xxxxxxxxxxx)、400免费电话(400-xxx-xxxxx）。
     *
     * @param ArrayAccess $input   验证对象，必须支持ArrayAccess访问
     * @param string      $field   验证字段
     * @param array       $options 验证选项
     * @return boolean
     */
    function telephoneValidator ($input, $field, $options = array()) {
        # 如果验证值为空，返回true;
        if (empty($input[$field])) return true;
        else return preg_match ('/^(\d{11})|(\d{3,4}-\d{7,8})|(400-(\d{3})-(\d{5}))$/', $input[$field]) != false;
    }

    /**
     * 比较验证器，验证比较字段的值是否与验证字段的值相同，验证成功返回true，否则返回false。
     *
     * @param ArrayAccess $input        验证字段，必须支持ArrayAccess访问
     * @param string      $field        验证字段
     * @param string      $compareField 比较字段
     * @return boolean
     */
    function compareValidator ($input, $field, $compareField = '') {
        # 如果比较值为空，则返回true；
        if (empty($input[$field])) return true;
        # 如果被比较字段为空，则返回false；
        if (empty($input[$compareField])) return false;

        # 返回比较结果；
        return $input[$field] == $input[$compareField];
    }

    /**
     * 数值范围验证器，验证字段的值是否在$range指定的范围内，验证成功返回true，失败返回false。
     *
     * @param ArrayAccess $input 验证对象，必须支持ArrayAccess访问
     * @param string      $field 验证字段
     * @param array       $range 值范围，array(最小值，最大值）
     * @return boolean
     */
    function rangeValidator ($input, $field, $range = array()) {
        # 如果验证字段的值为空值，则返回true值；
        if (empty($input[$field])) return true;

        list($min, $max) = $range;
        return $input[$field] >= $min && $input[$field] <= $max;
    }

    /**
     * 最小长度验证器，验证字段的值长度大于或等于$length，验证成功返回true，失败返回false。
     *
     * @param ArrayAccess $input  验证对象，必须支持ArrayAccess访问
     * @param string      $field  验证字段
     * @param int         $length 最小长度
     * @return boolean
     */
    function minlenValidator ($input, $field, $length = 10) {
        # 如果录入值为空，则返回true；
        if (empty($input[$field])) return true;
        else return mb_strlen ($input[$field]) >= $length;
    }

    /**
     * 最大长度验证器，验证字段的值长度小于或等于$length，验证成功返回true，失败返回false。
     *
     * @param ArrayAccess $input   验证对象，必须支持ArrayAccess访问
     * @param string      $field   验证字段
     * @param int         $length  最大长度
     * @return boolean
     */
    function maxlenValidator ($input, $field, $length = 15) {
        # 如果录入值为空，则返回true；
        if (empty($input[$field])) return true;
        else return mb_strlen ($input[$field]) < $length;
    }

    /**
     * 枚举验证器，验证字段的值必须为$options之一，验证成功返回true，失败返回false。
     *
     * @param ArrayAccess $input   验证对象，必须支持ArrayAccess访问
     * @param string      $field   验证字段
     * @param array       $options 枚举值，示例：array('a','b','c',12)
     * @return boolean
     */
    function enumValidator ($input, $field, $options = array()) {
        # 如果录入字段的值为空值，则直接返回true；
        if (empty($input[$field])) return true;
        else return in_array ($input[$field], $options);
    }

    /**
     * 邮件验证器，验证字段的值是否为合法的邮件地址，验证成功返回true，失败返回false；
     *
     * @param ArrayAccess $input   验证对象，必须支持ArrayAccess访问
     * @param string      $field   验证字段
     * @param array       $options 验证选项
     * @return boolean
     */
    function emailValidator ($input, $field, $options = array()) {
        # 如果验证值为空值，直接返回true；
        if (empty($input[$field])) return true;
        else return filter_var ($input[$field], FILTER_VALIDATE_EMAIL) != false;
    }

    /**
     * URL验证器，验证字段的值是否为正确的URL地址，验证成功返回true,失败返回false；
     *
     * @param ArrayAccess $input   验证对象，必须支持ArrayAccess访问
     * @param string      $field   验证字段
     * @param array       $options 验证选项
     * @return boolean
     */
    function urlValidator ($input, $field, $options = array()) {
        # 如果验证值为空，则直接返回true；
        if (empty($input[$field])) return true;
        else return filter_var ($input[$field], FILTER_VALIDATE_URL) != false;
    }

    /**
     * IP 验证器，验证字段的值是否为合法IP地址，验证成功返回true，失败返回false；
     *
     * @param ArrayAccess $input   验证对象，必须支持ArrayAccess访问
     * @param String      $field   验证字段
     * @param array       $options 验证选项
     * @return boolean
     */
    function ipValidator ($input, $field, $options = array()) {
        # 如果验证值为空值，则返回true；
        if (empty($input[$field])) return true;
        else return filter_var ($input[$field], FILTER_VALIDATE_IP) != false;
    }