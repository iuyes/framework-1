<?php
    /**
     * db_log_writer.inc 数据库日志记录DbLogWriter类声明文件
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.logs.writers
     * @version     1.0 beta
     *
     * @create      2012-11-20
     * @modify      2013-04-28
     */

    # 如果还未导入数据源。
    if (defined ('JTITA_DATASOURCE')) {
        Jtita::import ('kernels.datasources.datasource');
    }

    /**
     * 数据库日志记录器，提供将日志信息记录到数据库的服务。
     *
     * @package jtita.kernels.loggers.writers
     *
     * @example
     *  <pre>
     *      CREATE TABLE logs (
     *          `id`       INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
     *          `subject`  VARCHAR(32)  NOT NULL,
     *          `category` VARCHAR(16)  NOT NULL,
     *          `message`  VARCHAR(1024) NOT NULL DEFAULT '',
     *          `created`  INT NOT NULL,
     *          `user`     VARCHAR(32)   NOT NULL,
     *          `clientIP` VARCHAR(32)   NOT NULL
     *      );
     *  </pre>
     */
    class DbLogWriter implements LogWriter {
        /**
         * @var string 使用的数据库名字。
         */
        public $database = 'default';

        /**
         * @var string 数据表名字。
         */
        public $table = 'log';

        /**
         * $see LogWriter::write
         */
        public function write ($subject, $message, $category, $user) {
            $fields = array('subject' => $subject, 'message' => $message,
                            'category' => $category, 'created' => time (),
                            'user' => $user, 'clientIP' => Renv::ip ());

            $db = Datasource::database ($this->database);
            $db->create ($fields, array(Database::SYN_TABLE => $this->table));
        }
    }