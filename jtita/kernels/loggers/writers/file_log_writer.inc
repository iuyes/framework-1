<?php
    /**
     * file_log_writer.inc 文件日志记录器FileLogWriter类声明文件。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.logs.writers
     * @version     1.0 beta
     *
     * @create      2012-07-12
     * @modify      2013-04-28
     */

    /**
     * 文件日志默认文件地址
     */
    defined ('JTLOGGER_DEFAULT_FILE') or define('JTLOGGER_DEFAULT_FILE', dirname (dirname (dirname (__FILE__) . '/log.log')));

    /**
     * 文件日志记录器，提供将日志信息持久化文件的服务。
     *
     * @package jtita.kernels.loggers.writers
     */
    class FileLogWriter implements LogWriter {
        /**
         * @const string 默认文件模板
         */
        const DEFAULT_TEMPLATE = "--------------------------\r\n subject:{:subject}\r\n category:{:category}\r\n user:{:user} message:{:message}\r\n time:{:time}\r\n----------------------------";

        /**
         * @var string 文件名
         */
        public $file = JTLOGGER_DEFAULT_FILE;

        /**
         * @var string 存储模板
         */
        public $template = self::DEFAULT_TEMPLATE;

        /**
         * $see LogWriter
         */
        public function write ($subject, $message, $category, $user) {
            $this->_writeToFile ($this->_message ($subject, $message, $category, $user));
        }

        /**
         * 根据$subject、$message、$category生成完整的记录消息。
         *
         * @param string     $subject  日志主题
         * @param string     $message  日志消息
         * @param int|string $category 日志类别
         * @param string     $user     用户
         * @return string
         */
        protected function _message ($subject, $message, $category, $user) {
            return str_replace (array('{:message}', '{:category}', '{:time}', '{:subject}', '{:user}'), array($message,
                $category, date ('Y-m-d H:m:s', time ()), $subject, $user), $this->template);
        }

        /**
         * 将完整内容记录到日志文件中
         *
         * @param string $msg 完整日志
         * @return void
         */
        protected function _writeToFile ($msg) {
            file_put_contents ($this->file, $msg, FILE_APPEND);
        }
    }