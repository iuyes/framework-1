<?php
    /**
     * router_styles.inc 声明路由样式服务
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
     * @modify      2013-04-28
     */

    /**
     * 路由器风格，提供生成URL的不同风格；
     *
     * @package jtita.routers
     */
    interface RouterStyle {
        /**
         * 根据$domain和$params生成URL地址。
         *
         * @param string $domain 域名信息
         * @param array  $params 参数
         *
         * @return string
         */
        function url ($domain, $params = array());
    }

    /**
     * 普通路由风格样式，提供生成一般的URL；
     *
     * @package jtita.routers
     */
    class NormalRouterStyle implements RouterStyle {
        /**
         * @var string 指定样式的脚本名称。
         */
        public $scriptName = 'index.php';

        /**
         * 根据$domain和$params生成完整的URL地址。
         *
         * @implements RouterStyle::url
         */
        public function url ($domain, $params = array()) {
            # 如果参数为空，并且脚本名称为默认index.php
            if (empty($params) && ($this->scriptName == 'index.php')) return $domain;

            $fullUrl = $domain . '/';
            # 如果脚本名称不是默认的index.php(http://www.jtita.com/admin.php)
            if ($this->scriptName != 'index.php') {
                $fullUrl .= $this->scriptName;
            }

            # 处理anchor
            if ($anchor = array_kpop ($params, Router::ANCHOR)) {
                $anchor = ('#' . urlencode ($anchor));
            }

            # http://www.jtita.com/index.php?module=orders&controllers=index&action=index
            if (! empty($params)) {
                $fullUrl .= ('?' . http_build_query ($params));
            }

            return $fullUrl . $anchor;
        }
    }

    /**
     * Pathinfo 路由风格样式，提供对PHP的Pathinfo路径支持；
     *
     * @package jtita.routers
     */
    class PathinfoRouterStyle implements RouterStyle {
        /**
         * @var string 脚本名称
         */
        public $scriptName = 'index.php';

        /**
         * 根据$domain和$params生成完整的URL地址。
         *
         * @implements RouterStyle::url
         */
        public function url ($domain, $params = array()) {
            # 如果参数为空，并且脚本名称为默认index.php
            if (empty($params) && ($this->scriptName == 'index.php')) return $domain;

            # http://www.jtita.com/index.php
            $fullUrl = $domain . '/' . $this->scriptName;

            # http://www.jtita.com/index.php/members/orders/index
            if ($rewrite = array_kpop ($params, Router::REWRITE)) {
                $fullUrl .= ('/' . $rewrite);
            }

            # 处理anchor
            if ($anchor = array_kpop ($params, Router::ANCHOR)) {
                $anchor = ('#' . urlencode ($anchor));
            }

            # http://www.jtita.com/index.php/members/orders/index/id-12/name-hello
            if (! empty($params)) {
                $fullUrl .= ('?' . http_build_query ($params));
            }

            return $fullUrl . $anchor;
        }
    }

    /**
     * 重写路由规则样式，提供对于重写规则后URL生成支持；
     *
     * @package jtita.routers
     */
    class RewriteRouterStyle implements RouterStyle {
        /**
         * 根据$domain和$params生成完整的URL地址。
         *
         * @implements RouterStyle::url
         */
        public function url ($domain, $params = array()) {
            # 如果参数为空，并且脚本名称为默认index.php
            if (empty($params)) return $domain;

            $fullUrl = $domain;
            # http://www.jtita.com/members/orders/index
            if ($rewrite = array_kpop ($params, Router::REWRITE)) {
                $fullUrl .= ('/' . $rewrite);
            }

            # 处理anchor
            if ($anchor = array_kpop ($params, Router::ANCHOR)) {
                if ($fullUrl == $domain && empty($params)) {
                    $anchor = ('/#' . urlencode ($anchor));
                } else {
                    $anchor = ('#' . urlencode ($anchor));
                }
            }

            # http://www.jtita.com/index.php/members/orders/index/id-12/name-hello
            if (! empty($params)) {
                $fullUrl .= ('?' . http_build_query ($params));
            }

            return $fullUrl . $anchor;
        }
    }

/**
 * 名字路由规则样式，提供对于命名重写规则URL生成支持；
 *
 * @package jtita.routers
 *
class NamedRouterStyle extends AbstractRouterStyle {
/**
 * 参数分隔选项；
 *
protected $_separator = JTROUTER_DEFAULT_SEPARATOR;

/**
 * 重写初始化选项；
 *
public function initialize ($options = array()) {
parent::initialize ($options);

if (isset($options[JTROUTER_SEPARATOR])) {
$this->_separator = $options[JTROUTER_SEPARATOR];
}
}

/**
 * 根据$params生成完整URL；
 *
 * @param string $params 可选项参数；
 * @return string
 *
protected function _fullURL ($params) {
# http://www.jtita.com
$url = $this->_domainName;

# http://www.jtita.com/members/orders
if (isset($params[JTPARAM_RULE])) {
$url .= $params[JTPARAM_RULE];
unset($params[JTPARAM_RULE]);
}

# http://www.jtita.com/members/orders/id/12/name/hello
if (empty($params) == false) {
$purl = http_build_query ($params);
$url .= '/' . str_replace (array('&', '='), $this->_separator, $purl);
}

return $url;
}
} */