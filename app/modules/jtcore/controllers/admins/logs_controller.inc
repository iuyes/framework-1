<?php
    /**
     * logs_controller.php 声明网站日志管理控制器；
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.jtita.com/license.txt
     *
     * @package     jtita.jtcore
     * @version     1.0 beta
     *
     * @create      2012-01-29
     * @modify      2013-04-15
     */

    Jtita::import ('jtcore.models.xbl_log');

    /**
     * 网站日志管理控制器。
     *
     * @package jtita.jtcore
     */
    class LogsController extends AdminsController {
        /**
         * 默认活动；
         */
        public function indexAction () {
            #条件过滤
            $this->logs = XblLog::query ($this->query ('title'),
                                         $this->query ('category'),
                                         $this->query ('startDate'),
                                         $this->query ('endDate'));
            $this->logFiles = file_names (JTPATH_RTIME_LOGS);
            $this->categories = XblLog::categories ();
        }

        /**
         * 查看日志文件Action
         */
        public function fileAction () {
            $this->fileName = $this->query ('fileName', 'run.log');
            $this->logFiles = file_names (JTPATH_RTIME_LOGS);

            if (in_array ($this->fileName, $this->logFiles)) {
                $this->fileContent = file_get_contents (JTPATH_RTIME_LOGS . '/' . $this->fileName);
            }
            else {
                $this->message ('对不起，指定文件未找到', 'fail');
                $this->redirect ('index');
            }
        }

        /**
         * 删除
         */
        public function deleteAction () {
            if ($this->isPost () && $this->forms ('METHOD') == 'DELETE') {
                $rows = XblLog::deleteAll (array('XblLog.id' => $this->forms ('id', array())));
                $this->message ("删除日志信息成功，共删除{$rows}条记录", 'success');
            }

            $this->redirect (array('action' => 'index', '_fullPath' => true));
        }

        /**
         * 清空活动。
         */
        public function clearAction () {
            # 如果是POST方法并且指定方法为DELETE
            if ($this->isPost () && $this->forms ('METHOD') == 'DELETE') {
                $rows = XblLog::clear ($this->query ('title'), $this->query ('category'), $this->query ('startDate'), $this->query ('endDate'));
                $this->message ("清空日志信息成功，共清空{$rows}条记录", 'success');
            }

            $this->redirect ('index');
        }
    }