<?php
    /**
     * router.inc 声明路由组件服务接口对象；
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.routers
     * @version     1.0 beta
     *
     * @create      2012-07-23
     * @modify      2013-04-28
     */

    /**
     * @const 定义服务开启标志；
     */
    define('JTROUTER', true);

    # 导入路由规则和路由样式。
    Jtita::import ('routers.router_rules');
    Jtita::import ('routers.router_styles');

    /**
     * 路由异常，当路由引发异常时执行。
     *
     * @package jtita.routers
     */
    class RouterException extends JtitaException {
    }

    /**
     * 路由服务接口，提供路由相关服务。
     *
     * @package jtita.routers
     * @abstract
     */
     class Router {
        /**
         * @const 重写参数
         */
        const REWRITE = 'url';

        /**
         * @const 特殊变量名称，完整URL。
         */
        const FULLURL = 'fullUrl';

        /**
         * @const 特殊变量，锚。
         */
        const ANCHOR = '#';

        /**
         * 使用$domain和$config指定路由的域名信息，并返回域名相关的路由信息。
         *
         * @param string       $domain  域名标识
         * @param bool|array   $options 域名配置信息，指定false表示不设置域名信息
         *
         * @return Router
         */
        public static function domain ($domain, $options = false) {
            # 如果指定有域名的选项信息
            if ($options) self::$_domains[$domain] = $options;

            # 返回新的路由实例
            return new Router($domain);
        }

        /**
         * 从$params中匹配完整的参数信息，如果匹配失败返回false。
         *
         * @param array $params
         *
         * @return array|boolean
         */
        public static function match ($params) {
            # 根据当前域名获取规则链
            $rules = & self::_linkRules (JTROUTER_DEFAULT_DOMAIN);

            foreach ($rules as &$rule) {
                $objRule = self::_createRule ($rule);

                # 如果转换成功，则直接返回
                if ($ruleParams = $objRule->parse ($params)) {
                    return $ruleParams;
                }
            }

            return false;
        }

        /**
         * 使用$params和$ruleName生成URL地址,如果指定$ruleName将从$ruleName指定的规则集生成URL地址，
         * 未指定将使用当前域名规则生成URL地址。
         *
         * @param array|string $params   URL参数
         * @param bool         $ruleName 路由名称
         *
         * @return string
         *
         * @throws RouterException 域名未提供或规则反转失败。
         */
        public static function url ($params = array(), $ruleName = false) {
            # 如果指定参数不是数组
            if (! is_array ($params)) {
                $params = array(JTPARAM_ACTION => $params);
            }

            # 如果没有指定规则名称。
            if (! $ruleName) {
                if ($fullPath = array_kpop ($params, Router::FULLURL, false)) {
                    $params = array_merge (self::$_fullParams, $params);
                }
                else {
                    $params = array_merge (self::$_baseParams, $params);
                }

                $ruleName = JTROUTER_DEFAULT_DOMAIN;
            }

            # 根据当前域名获取规则链
            $rules = self::_linkRules ($ruleName);

            # 从规则集中生成规则。
            foreach ($rules as &$rule) {
                $objRule = self::_createRule ($rule);
                # 如果转换成功，则直接返回
                if (($ruleParams = $objRule->reverse ($params)) !== false) {
                    # 如果指定域名没有配置信息。
                    if (! isset(self::$_domains[$rule['domain']])) {
                        throw new RouterException('Not found domain config:' . $rule['domain']);
                    }

                    # 获取域名信息。
                    $domain = self::$_domains[$rule['domain']];
                    $style = self::_domainStyle ($domain);

                    return $style->url ($domain['domain'], $ruleParams);
                }
            }

            throw new RouterException('Not match rule');
        }

        /**
         * 使用$base和$all初始化完整路由器。
         *
         * @param array $base 基本参数。
         * @param array $all  完整参数。
         *
         * @return void
         */
        public static function defaultUrlParams ($base, $all) {
            self::$_baseParams = $base;
            self::$_fullParams = $all;
        }

        /**
         * 将$rule指定的规则注册到全局规则中。
         *
         * @param array $rule   规则IOC信息；
         * @param array $domain 规则所在域名；
         *
         * @return mixed 注册地址
         */
        protected static function & _addRule ($rule, $domain) {
            self::$_rules[] = array('domain' => $domain, 'rule' => $rule, 'objRule' => false);
            return self::$_rules[count (self::$_rules) - 1];
        }

        /**
         * 根据$rule创建规则实例。
         *
         * @param $rule 规则信息
         *
         * @return RouterRule
         */
        protected static function _createRule (&$rule) {
            # 判断是否已创建规则对象。
            if (empty($rule['objRule'])) {
                $rule['objRule'] = Jtita::ioc ($rule['rule']);
            }

            return $rule['objRule'];
        }

        /**
         * 将$rule指定的规则注册到$name职责链中。
         *
         * @param string $name 名称
         * @param mixed  $rule 规则信息
         *
         * @return void
         */
        protected static function _addLink ($name, &$rule) {
            # 如果指定的规则链不存在，则创建一个。
            if (! isset(self::$_links[$name])) {
                self::$_links[$name] = array();
            }

            self::$_links[$name][] = & $rule;
        }

        /**
         * 根据$name获取规则中的规则信息数组。
         *
         * @param $name 链名称
         *
         * @return array
         */
        protected static function & _linkRules ($name) {
            # 如果指定连接不存在，则初始化，防止只读引用。
            if (! isset(self::$_links[$name])) {
                self::$_links[$name] = array();
            }

            return self::$_links[$name];
        }

        /**
         * 根据领域信息生成样式对象。
         *
         * @param string $domain 领域对象
         *
         * @return RouterStyle
         */
        protected static function _domainStyle (&$domain) {
            # 判断域名中的样式对象是否存在。
            if (! isset($domain['objStyle'])) {
                $style = array_kvalue ($domain, 'style', 'normal');
                $domain['objStyle'] = Jtita::ioc ($style);
            }

            # 返回样式对象。
            return $domain['objStyle'];
        }

        /**
         * 使用$domain初始化路由实例。
         */
        protected function __construct ($domain) {
            $this->_currentDomain = $domain;
        }

        /**
         * 使用$rule和$options注册路由规则集。
         *
         * @param array       $rule    路由规则。
         * @param array|bool  $names   规则命名；
         *
         * @return Router
         */
        public function rule ($rule, $names = array()) {
            # 注册到全局规则中。
            $currRule = & self::_addRule ($rule, $this->_currentDomain);
            # 增加到当前域名的职责链中。
            self::_addLink ($this->_currentDomain, $currRule);

            # 如果指定有规则的命名。
            if (is_string ($names)) {
                self::_addLink ($names, $currRule);
            }
            else {
                foreach ($names as $name) {
                    self::_addLink ($name, $currRule);
                }
            }

            return $this;
        }

        /**
         * @var array 域名配置信息。
         */
        protected static $_domains = array();

        /**
         * @var array 全部规则信息
         */
        protected static $_rules = array();

        /**
         * @var array 路由规则链。
         */
        protected static $_links = array();

        /**
         * @var array 基本参数集
         */
        protected static $_baseParams = array();

        /**
         * @var array 完整参数集
         */
        protected static $_fullParams = array();

        /**
         * @var string 当前域名。
         */
        protected $_currentDomain = JTROUTER_DEFAULT_DOMAIN;
    }
