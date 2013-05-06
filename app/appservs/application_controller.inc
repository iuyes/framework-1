<?php
    /**
     * application_controller.inc 应用控制器声明文件
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     app.appservs
     * @version     1.0 beta
     *
     * @create      2012-10-25
     * @modify      2013-04-28
     */

    /**
     * 引用控制器类
     */
    Jtita::importInterface('controllers.controller');

    /**
     * 应用控制器类，提供全局的应用控制器服务
     *
     * @package app.appservs
     */
    class ApplicationController extends Controller {
        /**
         * 返回视图类名称
         *
         * @return string
         */
        protected function _viewClassName () {
            return array('class' => 'views.theme_view');
        }
    }

    /**
     * 管理员控制器，提供后台管理控制器服务。
     *
     * @package app.appservs
     */
    class AdminsController extends Controller {
        /**
         * 返回控制器类名
         *
         * @return string
         */
        public function view () {
            return array('class' => 'views.theme_view',
                         'theme' => Jtita::read ('settings.themes.admins', 'default'));
        }
    }