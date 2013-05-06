<?php
    /**
     * config.php 声明全局应用配置声明，与setting相比config.inc中的内容仅允许直接修改。
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
    # 数据库配置信息
    Jtita::write ('kernels/datasources', array(
        'default' => array (
            'class' => 'kernels.datasources.databases.mysql_fast',
            'server' => '127.0.0.1',
            'dbname' => 'lives',
            'dbuser' => 'root',
            'dbpass' => 'killer',
            'charset' => 'utf8',
            'initialize' => true
        )
    ));

    # 日志配置
    Jtita::write('kernels/loggers',array(
        'writers' => array(
            'default' => array(
                'class' => 'kernels.loggers.writers.file_log_writer',
                'database' => 'default',
                'table' => 'xbl_log'
            )
        )
    ));
