<?php
    /**
     * router_rules.inc 默认路由规则类声明文件
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.routers
     * @version     1.0 beta
     *
     * @create      2012-08-02
     * @modify      2013-04-09
     */

    /**
     * 路由规则接口，提供实现各种路由规则的接口定义。
     *
     * @package jtita.routers
     */
    interface RouterRule {
        /**
         * 使用$params转换参数，转换失败返回false；
         *
         * @param array $params 转换参数；
         *
         * @return mixed
         */
        function parse ($params = array());

        /**
         * 使用$params反向转换参数，转换失败返回false；
         *
         * @param array $params 转换参数；
         *
         * @return mixed
         */
        function reverse ($params = array());
    }

    /**
     * 抽象路由规则，提供路由规则生成所需的基础服务。
     *
     * @package jtita.routers
     */
    abstract class AbstractRouterRule implements RouterRule {

        /**
         * @param null $domain
         *
         * @return mixed|void
         */
        public function domain ($domain = null) {

        }

        /**
         * 使用$params转换参数，转换失败返回false；
         *
         * @param array $params 转换参数；
         *
         * @return mixed
         */
        public function parse ($params = array()) { }

        /**
         * 使用$params反向转换参数，转换失败返回false；
         *
         * @param array $params 转换参数；
         *
         * @return mixed
         */
        public function reverse ($params = array()) { }

        /**
         * 魔法方法，指定参数选项。
         *
         * @param string       $name   参数名称
         * @param string|array $value  参数选项
         *
         * @return void
         */
        public function __set ($name, $value) {
            # 如果是数组参数
            if (! is_array ($value)) {
                $value = array($value);
            }

            # 设置参数值
            if (! empty($value[0])) $this->_defaults[$name] = $value[0];
            if (! empty($value[1])) $this->_validators[$name] = $value[1];
            if (! empty($value[2])) $this->_requires[$name] = $name;
        }

        /**
         * 根据$params检查必须参数是否都存在，成功返回true，失败返回false。
         *
         * @param array $params 参数集
         *
         * @return bool
         */
        protected function _require (&$params) {
            # 判断必选参数。
            foreach ($this->_requires as $name) {
                if (! isset($params[$name])) return false;
            }

            return true;
        }

        /**
         * 根据$params执行参数验证，验证成功返回true，失败返回false。
         *
         * @param array $params 参数集
         *
         * @return bool
         */
        protected function _validate (&$params) {
            # 进行有效性验证检查。
            foreach ($this->_validators as $name => $validator) {
                if (isset($params[$name]) && ! preg_match ('/' . $validator . '/', $params[$name])) {
                    return false;
                }
            }

            return true;
        }

        /**
         * @var array 正则验证器。
         */
        protected $_validators = array();

        /**
         * @var array 默认参数。
         */
        protected $_defaults = array();

        /**
         * @var array 必选项参数
         */
        protected $_requires = array();
    }

    /**
     * 普通路由规则，支持默认的参数形式。
     *
     * @example
     *      <pre>
     *          Router::domain('www.example.com')
     *              ->rule(array('class'        => 'normal_router_rule',  # 规则类名称
     *                           # '参数选项'     => array('默认值','有效性检查(正则表达式)','必须参数');
     *                           'package'      => array('admins','admins|admins.reports',true),
     *                           # '参数选项'     => array('默认值','有效性检查（正则表达式)','非必必须参数');
     *                           'controller'   => array('index','^[a-z]+$')
     *                           # '参数选项'     => '默认值'
     *                           'action'       => array('index'),
     *                           'id'           => 10   # 指定默认参数
     *                    ));
     *      </pre>
     *
     * @package jtita.routers
     */
    class NormalRouterRule extends AbstractRouterRule {
        /**
         * 使用$params转换参数，转换失败返回false；
         *
         * @param array $params 转换参数；
         *
         * @return mixed
         */
        public function parse ($params = array()) {
            # 合并默认值。
            $params = array_merge ($this->_defaults, $params);

            if (! $this->_require ($params)) return false;
            if (! $this->_validate ($params)) return false;

            return $params;
        }

        /**
         * 使用$params反向转换参数，转换失败返回false；
         *
         * @param array $params 转换参数；
         *
         * @return mixed
         */
        public function reverse ($params = array()) {
            # 从参数中去除默认值。
            $params = array_merge ($this->_defaults, $params);

            if (! $this->_require ($params)) return false;
            if (! $this->_validate ($params)) return false;

            return array_diff_assoc ($params, $this->_defaults);
        }
    }

    /**
     * 正则表达式路由规则，支持正则参数形式。
     *
     * @package jtita.routers
     */
    class RegexRouterRule extends AbstractRouterRule {
        /**
         * @var string 正则匹配表达式。
         */
        public $express = '';

        /**
         * 使用$params转换参数，转换失败返回false；
         *
         * @param array $params 转换参数；
         *
         * @return mixed
         */
        public function parse ($params = array()) {
            # 如果验证表达式为空，则直接返回匹配失败
            if (empty($this->express)) return false;

            # 从参数中获取重写参数。
            if ($rewrite = array_kpop ($params, Router::REWRITE)) {
                if ($regexParams = $this->_regexParse ($rewrite)) {
                    $params = array_merge ($params, $regexParams);
                }
                else return false;
            }

            $params = array_merge ($this->_defaults, $params);
            return $this->_require ($params) && $this->_validate ($params) ? $params : false;
        }


        /**
         * 使用$params反向转换参数，转换失败返回false；
         *
         * @param array $params 转换参数；
         *
         * @return mixed
         */
        public function reverse ($params = array()) {
            # 如果没有设置正则表达式，直接返回false。
            if (empty($this->express)) return false;

            # 合并默认值。
            $params = array_merge ($this->_defaults, $params);

            if (! $this->_require ($params)) return false;
            if (! $this->_validate ($params)) return false;

            return $this->_regexReverse ($params);
        }


        /**
         * 从重写参数中转换出参数信息，成功返回参数数组，失败返回false。
         *
         * @param string $rewrite
         *
         * @return bool|array
         */
        protected function _regexParse ($rewrite) {
            # 初始化变量。
            $matches = array();
            $params = array();

            # 如果正则表达式匹配成功。
            if (preg_match_all ($this->_regexPattern (), $rewrite, $matches, PREG_SET_ORDER)) {
                foreach ($matches[0] as $name => $value) {
                    # 去除结果中的数字索引
                    if (! is_numeric ($name)) {
                        $params[$name] = $value;
                    }
                }

                # 处理更多参数（/id-12/cat-13/)
                if ($more = array_kpop ($params, '_more')) {
                    $moreParams = array();
                    if (preg_match_all ('/\/(?P<name>[a-z]+)-(?P<val>[^\/]+)/', $more, $moreParams, PREG_SET_ORDER)) {
                        foreach ($moreParams as $param) {
                            $params[urldecode ($param['name'])] = urldecode ($param['val']);
                        }
                    }
                }

                return $params;
            }

            return false;
        }


        /**
         * 使用$params生成返回转换正则参数。
         *
         * @param array $params 全部参数集
         *
         * @return array
         */
        protected function _regexReverse ($params) {
            # 根据RULE生成$rule信息；
            $defaults = $this->_defaults;
            # 生成URL参数。
            #$url = preg_replace_callback ('/(\[)?([^\:])?\:([a-z]+)(\])?/', function ($names) use (&$params) {
            $url = preg_replace_callback ('/(\[)?([^\:])?\:([a-z]+)(\])?/', function ($names) use (&$params, $defaults) {
                $value = array_kpop ($params, $names[3], false);
                $def = array_kvalue ($defaults, $names[3], false);

                # 如果有参数值和默认值，并且默认值与参数值相同，并且没有其它参数
                if ($value && $def && $def == $value) {
                    return '';
                }
                else {
                    return $names[2] . $value;
                }
            }, $this->express);

            # 如果验证失败，返回false；
            if (! preg_match ($this->_regexPattern (), $url)) return false;

            # 去除没有处理的默认值。
            $params = array_diff_assoc ($params, $this->_defaults);
            # 处理最后需要传递的参数。
            if (! empty($params)) {
                if (substr ($url, - 1, 1) == '/') $url = substr ($url, 0, - 1);
                foreach ($params as $name => $val) {
                    $url .= ('/' . urlencode ($name) . '-' . urlencode ($val));
                }
            }

            return array(Router::REWRITE => $url);
        }

        /**
         * 获取重写表达
         */
        protected function _regexPattern () {
            # 如果正则表达式已经生成，返回已生成的结果。
            if ($this->_regexPattern) return $this->_regexPattern;

            # 获取验正则验证集。
            $validators = $this->_validators;
            # 转换为正则表达式生成模式。
            $pattern = preg_replace_callback ('/\\\:([a-z]+)/', function ($names) use ($validators) {
                # 获取变量名称。
                $name = $names[1];
                # 设置变量过滤条件。
                if (isset($validators[$names[1]])) return "(?P<$name>" . $validators[$name] . ')';
                else return "(?P<$name>[a-z_]+)";
            }, preg_quote ($this->express, '/'));

            $pattern = str_replace (array('\\[', '\\]'), array('(', '){0,1}'), $pattern);

            # 将字符串转换为正则表达式；
            return ($this->_regexPattern = '/^' . $pattern . '(?P<_more>\S+)?$/');
        }

        /**
         * @var bool|string 正则表达式
         */
        private $_regexPattern = false;
    }