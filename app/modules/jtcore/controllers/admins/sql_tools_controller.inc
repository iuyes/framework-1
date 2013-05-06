<?php
    /**
     * sql_tools_controller.php 声明网站SQL工具控制器；
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

    /**
     * 网站后台SQL工具控制器。
     *
     * @package jtita.jtcore
     */
    class SqlToolsController extends AdminsController{

        /**
         * 默认活动；
         */
        public function indexAction () {
            if ($this->isPost ()){
                $this->sql = $this->forms ('SQL');
                $this->dsName = $this->forms ('datasource');

                if ($this->sql && $this->dsName){
                    Jtita::import ('kernels.datasources.datasource');
                    $datasource = Datasource::database ($this->dsName);

                    try{
                        $this->rows = $datasource->query ($this->sql);
                    } catch (Exception $ex){
                        $this->ex = $ex;
                    }
                }
            }

            $this->dsNames = array_keys (Jtita::read ('kernels/datasources'));
            $this->dsName = $this->forms ('datasource', $this->dsNames[0]);
        }
    }