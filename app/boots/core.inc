<?php
    /**
     * core.php 应用启动的核心文件
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     app.boots
     * @version     1.0 beta
     *
     * @create      2012-07-22
     * @modify      2013-04-28
     */

    /**
     * @const boolean 声明Jtita是否为调试状态。
     */
    define('JTDEBUG', true);

    /**
     * @const string 声明根路径,可以根据实际需要重新调整,结尾不带'/'；
     */
    define('JTPATH_ROOT', dirname (dirname (dirname (__FILE__))));

    /**
     * @const string 声明App路径，可以根据实际需要重新设置,结尾不带'/'；
     */
    define('JTPATH_APP', JTPATH_ROOT . '/app');

    /**
     * @const string 声明module路径，可以根据实际需要重新设置,结尾不带'/'；
     */
    define('JTPATH_MODULE', JTPATH_APP . '/modules');

    /**
     * @const string 声明Jtita路径，可以根据实际需要重新设置,结尾不带'/'；
     */
    define('JTPATH_JTITA', JTPATH_ROOT . '/jtita');

    /**
     * @const string 声明rtime路径，可以根据实际需要重新设置,结尾不带'/'；
     */
    define('JTPATH_RTIME', JTPATH_ROOT . '/rtime');

    /**
     * @const string 声明运行时代码存储路径，可根据实际需要重新设置，结尾不带’/’；
     */
    define('JTPATH_RTIME_CODES', JTPATH_RTIME . '/codes');

    /**
     * @const string 日志文件目录，可根据实际需要重新设置，结尾不带’/’；
     */
    define('JTPATH_RTIME_LOGS', JTPATH_RTIME . '/logs');

    /**
     * @const string 声明vendor路径，可以根据实际需要重新设置,结尾不带'/'；
     */
    define('JTPATH_VENDOR', JTPATH_ROOT . '/vendor');

    /**
     * @const string 声明默认日志文件
     */
    define('JTLOGGER_DEFAULT_FILE', JTPATH_RTIME_LOGS . '/run.log');

    /**
     * @const string 声明错误文件
     */
    define('JTERROR_FILE', JTPATH_RTIME_LOGS . '/error.log');

    # 引用基本函数
    require(JTPATH_JTITA . '/kernels/funs.inc');
    # 引用请求处理类；
    require(JTPATH_JTITA . '/kernels/renv.inc');
    # 引用Jtita类，启动标准内核服务；
    require(JTPATH_JTITA . '/kernels/jtita.inc');
    # 引用标准行为服务；
    require(JTPATH_JTITA . '/kernels/behaviors.inc');

    /**
     * 设置Jtita寻址路径，可以根据实际需要重新设置；
     */
    Jtita::paths (array(JTPATH_MODULE, JTPATH_APP, JTPATH_JTITA, JTPATH_VENDOR));

    /**
     * @const string 声明路由常量，默认域名
     */
    define('JTROUTER_DEFAULT_DOMAIN', Renv::domain ());

    /**
     * @const jtita 格式化日期字符串
     */
    define('JTFORMAT_DATE', 'Y-m-d');

    /**
     * @const jtita 格式化日期时间字符串
     */
    define('JTFORMAT_DATETIME', 'Y-m-d H:i:s');

    /**
     * @const jtita 格式化周字符串。
     */
    define('JTFORMAT_WEEK', 'l');

    /**
     * @const jtita 格式化时间字符串。
     */
    define('JTFORMAT_TIME', 'H:i:s');

    # 如果是测试模式。
    if (JTDEBUG) {
        # 设置错误标记。
        error_reporting (E_ALL);
        # 导入调试组件。
        Jtita::import ('kernels.debuggers.debugger');
    }

    # 设置错误日志文件。
    ini_set ('error_log', JTERROR_FILE);