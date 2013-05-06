<?php
    /**
     * jtcore.inc 提供应用所需的基础助手。
     *
     * @copyright     2012-2013 Jtita soft foundation
     *                (http://www.jtita.com)
     * @author        Jtita team<jtita@outlook.com>
     * @license       http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package       app/views
     * @version       1.0 beta
     *
     * @create      2013-03-07
     * @modify      2013-04-15
     */

    /**
     * 根据$cmd、$args和$options执行查询服务。
     */
    function _query ($cmd, $args = array(), $dbName = 'default') {
        $db = Datasource::database ($dbName);
        return $db->query ($cmd, $args);
    }

    /**
     * 根据$key读取配置信息，配置不存在返回$default。
     *
     * @param string $key     设置关键字
     * @param string $default 默认值
     * @return mixed
     */
    function _cfg ($key, $default = 'error') {
        # 初始化设置值
        static $setting = array();

        # 获取读取值。
        list($module, $key) = explode (Jtita::PACKAGE_SEPARATOR, $key);
        if (empty($setting[$module])) {
            if ($cfgFile = Jtita::lookup ($module . '.boots.config', '.inc', JTPATH_MODULE)) {
                $setting[$module] = include($cfgFile);
            }
        }

        return isset($setting[$module][$key]) ? $setting[$module][$key] : $default;
    }