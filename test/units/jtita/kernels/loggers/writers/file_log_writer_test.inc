<?php
    /**
     * file_log_writer_test.php 文件日志记录器，单元测试服务。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.logs.behaviors
     * @version     1.0 beta
     *
     * @create      2012-07-15
     * @modify      2013-04-28
     */

    # 初始化根节点路径
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (dirname (dirname (__FILE__))))))));

    /**
     * 导入基础服务类；
     */
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/jtita.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/behaviors.inc');

    /**
     * 导入被测试类；
     */
    require_once(JTPATH_ROOT . '/jtita/kernels/loggers/logger.inc');
    require_once(JTPATH_ROOT . '/jtita/kernels/loggers/writers/file_log_writer.inc');

    /**
     * 测试日志文件位置；
     */
    defined ('LOG_FILENAME') or define('LOG_FILENAME', dirname (__FILE__) . '/logs.txt');

    /**
     * 文件日志记录器；
     */
    class FileLogWriterTest extends PHPUnit_Framework_TestCase {
        /**
         * 初始化测试环境；
         */
        public function setUp () {
            // 如果测试文件存在、删除；
            if (file_exists (LOG_FILENAME)) {
                unlink (LOG_FILENAME);
            }
        }

        /**
         * 测试写入文件；
         */
        public function testWrite () {
            $writer = new FileLogWriter();
            $writer->file = LOG_FILENAME;
            $writer->write ('Hello world','normal','zgp','Hello world Zheng Guoping');

            $this->assertFileExists (LOG_FILENAME);
        }
    }
	