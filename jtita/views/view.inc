<?php
    /**
     * view.inc 提供视图接口声明
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
     * 视图异常类，当发生视图错误时引发本异常
     *
     * @package jtita.views
     */
    class ViewException extends JtitaException {
    }

    /**
     *  视图接口，提供展示视图的能力
     *
     * @package jtita.views
     */
    interface View extends ArrayAccess {
        /**
         * @const string 视图默认模板护名
         */
        const TEMPLATE_EXT = '.jtp';

        /**
         * @const string 视图模板选项
         */
        const OPT_TEMPLATE = 'template';

        /**
         * @const string 视图控制器选项
         */
        const OPT_CONTROLLER = 'controller';

        /**
         * @const string 视图模块选项
         */
        const OPT_MODULE = 'module';

        /**
         * @const string 视图包选项
         */
        const OPT_PACKAGE = 'package';

        /**
         * @const string 视图活动选项
         */
        const OPT_ACTION = 'action';

        /**
         * @const string 视图pass选项
         */
        const OPT_PASS = 'pass';

        /**
         * 使用$context和$options初始化视图对象。
         *
         * @param mixed      $context    控制上下文，视图上下文件对象必须有$request和$response字段。
         * @param Array      $options    选项
         */
        function initialize ($context, $options = array());

        /**
         * 读取或设置视图通知$message消息。
         *
         * @param array $message 通知消息.
         * @return array
         */
        function message ($message = null);

        /**
         * 获取或设置视图关联变量为$vars。
         *
         * @param array $vars 视图变量
         */
        function vars ($vars = null);

        /**
         * 输出模板内容
         *
         * @throws ViewException 当输出内容时发生错误，将引发异常
         */
        function render ();
    }