<?php
    /**
     * theme_view.inc 主题视图类声明文件
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.views
     * @version     1.0 beta
     *
     * @create      2012-08-15
     * @modify      2013-04-07
     */

    # 如果未导入默认视图对象
    Jtita::importInterface ('views.normal_view');

    /**
     * 主题视图对象，提供视图对于主题的支持
     *
     * @package jtita.views
     */
    class ThemeView extends NormalView {
        /**
         * @const string 默认主题
         */
        const DEFAULT_THEME = 'default';

        /**
         * @var string 主题名称
         */
        public $themeName = self::DEFAULT_THEME;
        
        /**
         * 获取布局文件路径集
         */
        protected function _layoutPaths () {
            # 获取已有的路径集合
            $paths = parent::_layoutPaths ();

            # app/themes/theme-name/_layouts;
            array_unshift ($paths, JTPATH_APP . '/themes/' . $this->themeName . '/_layouts');
            # app/themes/default/module-name/_elements;
            if ($module = $this->_options[self::OPT_MODULE]) {
                array_unshift ($paths, JTPATH_APP . '/themes/' . $this->themeName . '/' . $module . '/_layouts');
            }

            # 返回模板路径
            return $paths;
        }

        /**
         * 获取模板文件路径集
         */
        protected function _templatePaths () {
            # 获取原始路径
            $paths = parent::_templatePaths ();

            # 如果选项包含模块信息。
            if ($module = $this->_options[self::OPT_MODULE]) {
                # app/themes/default/module-name
                array_unshift ($paths, JTPATH_APP . '/themes/' . $this->themeName . '/' . $module);
            }
            else {
                # app/themes/default/module-name
                array_unshift ($paths, JTPATH_APP . '/themes/' . $this->themeName);
            }

            # 返回模板路径
            return $paths;
        }

        /**
         * 获取元素路径集
         */
        protected function _elementPaths () {
            # 原始已有的路径集；
            $paths = parent::_elementPaths ();

            # app/themes/default/_elements;
            array_unshift ($paths, JTPATH_APP . '/themes/' . $this->themeName . '/_elements');
            # app/themes/default/module-name/_elements;
            if ($module = $this->_options[self::OPT_MODULE]) {
                array_unshift ($paths, JTPATH_APP . '/themes/' . $this->themeName . '/' . $module . '/_elements');
            }

            # 返回助手路径
            return $paths;
        }

        /**
         * 获取助手路径集
         */
        protected function _helperPaths () {
            # 原始已有的路径集；
            $paths = parent::_helperPaths ();
            # app/themes/default/_helpers;
            array_unshift ($paths, JTPATH_APP . '/themes/' . $this->themeName . '/_helpers');
            # app/themes/default/module-name/_helpers;
            if ($module = $this->_options[self::OPT_MODULE]) {
                array_unshift ($paths, JTPATH_APP . '/themes/' . $this->themeName . '/' . $module . '/_helpers');
            }

            # 返回助手路径
            return $paths;
        }
    }