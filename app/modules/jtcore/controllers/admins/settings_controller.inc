<?php
    /**
     * settings_controller.php 声明网站配置控制器；
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.jtcore
     * @version     1.0 beta
     *
     * @create      2012-01-29
     * @modify      2013-04-15
     */

    /**
     * 网站后台配置管理控制器。
     *
     * @package xbutu.xblives
     */
    class SettingsController extends AdminsController{

        /**
         * 默认活动；
         */
        public function indexAction () {
            # 配置文件地址。
            $fileName = Jtita::lookup ('jtcore.boots.config', '.inc', JTPATH_MODULE);

            if ($this->isPost ()){
                $this->jtcore = $this->forms ('jtcore');
                if (file_put_contents ($fileName, '<?php return ' . array_declare ($this->jtcore) . ';')){
                    $this->message ('成功保存设置信息', 'success');
                    return $this->redirect ('index');
                }

                $this->message ('保存设置信息失败，请检查权限设置', 'fail');
            } else{
                $this->jtcore = include($fileName);
            }
        }


    }