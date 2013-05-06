<?php
    /**
     * application_model.inc 应用模型类声明文件。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.jtita.com/license.txt
     *
     * @package     jtita.appservs
     * @version     1.0 beta
     *
     * @create      2012-07-23
     * @modify      2013-04-07
     */

    /**
     * 判断是否已导入应用模型基础类。
     */
    Jtita::importInterface ('models.model');

    /**
     * 应用模型类，允许在app/appservs目录下重写本文件提供自定义模型类。
     *
     * @package jtita.appservs
     */
    class ApplicationModel extends Model {

    }
	