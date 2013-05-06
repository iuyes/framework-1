<?php
    /**
     * model_finder.inc 模型查找类声明文件
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.models
     * @version     1.0 beta
     *
     * @create      2012-07-15
     * @modify      2013-04-28
     */

    /**
     * 模型查找器，实现模型内容的级联查询和数据检索
     *
     * @package jtita.models
     */
    class ModelFinder {
        /**
         * 使用$modelName、$syntax、$options初始化ModelFinder实例
         *
         * @param string|Model $modelName 模型名字
         * @param array        $syntax    查找语法
         * @param array        $options   查找选项
         * @return ModelFinder
         */
        public function __construct ($modelName, $syntax = array(), $options = array()) {
            # 初始化数据库语法；
            $this->_dbSyntax = $modelName::database ()->findSyntax ();

            $this->_modelName = $modelName;
            $this->_options = $options;
            $this->_syntax = $syntax;
        }

        /**
         * PHP 魔术方法，用于执行自定义find操作
         *
         * @param string $method 方法名称
         * @param array  $args   方法参数
         * @return array|ModelFinder
         *
         * @throws MethodNotFoundException 如果指定语法不存在。
         */
        public function __call ($method, $args) {
            # 如果指定操作数据语法中不存在；
            if (! isset($this->_dbSyntax[$method])) {
                throw new MethodNotFoundException('Not found method:' . $method);
            }

            # 获取语法名称
            $syntaxName = $this->_dbSyntax[$method];

            # 判断语法参数
            if (isset($args[0])) {
                $this->_syntax[$syntaxName] = $args[0];
                # 如果设置joins
                if ($method == 'joins') {
                    # 如果设置值为empty，则去除relation
                    if (empty($args[0])) {
                        unset($this->_options['relation']);
                    }
                    else {
                        $modelName = $this->_modelName;
                        $this->_options['relation'] = $modelName::relationSchemas ();
                    }
                }
                return $this;
            }
            else if (isset($this->_syntax[$syntaxName])) {
                return $this->_syntax[$syntaxName];
            }
            else return null;
        }

        /**
         * 获取全部模型信息；
         */
        public function models () {
            $modelName = $this->_modelName;
            $this->_options['format'] = Database::OPT_FORMAT_MODEL;

            $rows = $modelName::database ()->find ($this->_syntax, $this->_options);
            return $modelName::createModels ($rows, $this->_options);
        }

        /**
         * 返回当前查询数据的二维矩阵格式数据；
         */
        public function rows () {
            $modelName = $this->_modelName;
            $options['format'] = Database::OPT_FORMAT_NORMAL;

            return $modelName::database ()->find ($this->_syntax, $this->_options);
        }

        /**
         * 返回当前查询数据的list形式；
         */
        public function klist () {
            $modelName = $this->_modelName;
            $this->_options['format'] = Database::OPT_FORMAT_LIST;

            return $modelName::database ()->find ($this->_syntax, $this->_options);
        }

        /**
         * 返回当前查询数据的json格式数据；
         */
        public function json () {
            $modelName = $this->_modelName;

            $options = $this->_options;
            $options['format'] = 'json';

            return $modelName::database ()->find ($this->_syntax, $options);
        }

        /**
         * 返回当前查询数据的xml格式数据；
         */
        /*public function xml() {

        }*/

        /**
         * 获取当前查找对象的数量；
         */
        public function count () {
            $modelName = $this->_modelName;
            $rows = $modelName::findFields ('COUNT(*) AS count', $this->_syntax['where'], $this->_syntax, $this->_options);

            # 返回数量
            return $rows['count'];
        }

        /**
         * 返回当前查找模型名字
         *
         * @return string
         */
        public function modelName () {
            return $this->_modelName;
        }

        /**
         * 返回当前查找器语法说明
         */
        public function syntax () {
            return $this->_syntax;
        }

        /**
         * 返回当前查找选项
         */
        public function options () {
            return $this->_options;
        }

        /**
         * 根据$name获取额外信息，信息不存在返回$default。
         *
         * @param string $name    额外名字
         * @param mixed  $default 默认值
         * @return mixed
         */
        public function extra ($name, $default = false) {
            return isset($this->_extras[$name]) ? $this->_extras[$name] : $default;
        }

        /**
         * 使用$name和$value附加额外信息。
         *
         * @param string $name  名字
         * @param mixed  $value 值
         */
        public function addExtra ($name, $value) {
            $this->_extras[$name] = $value;
        }

        /**
         * 移除$name指定的额外附加信息。
         *
         * @param string $name 名字
         */
        public function removeExtra ($name) {
            unset($this->_extras[$name]);
        }


        /**
         * @var array 额外信息。
         */
        protected $_extras = array();

        /**
         * @var string|Model 模型名称；
         */
        protected $_modelName = null;

        /**
         * @var 数据库语法，数据库允许使用的语法；
         */
        protected $_dbSyntax = array();

        /**
         * @var array 查找语法
         */
        protected $_syntax = array();

        /**
         * @var array 查找选项
         */
        protected $_options = array();

        /**
         * @var array 分页信息
         */
        protected $_paginate = array();
    }