<?php
    /**
     * model.inc 模型类声明文件
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.models
     * @version     1.0 beta
     *
     * @create      2012-10-07
     * @modify      2013-04-28
     */

    /**
     * 模型关系异常，提供对模型关系的异常信息
     *
     * @package jtita.models
     */
    class ModelRelationException extends ModelException {
    }

    /**
     * 模型关系接口，提供模型的schema处理和模型懒加载服务
     *
     * @package jtita.models
     */
    interface ModelRelation {
        /**
         * @const string 关系外键常量选项
         */
        const OPT_FOREIGN = 'foreignKey';

        /**
         * @const string 关系外键类名选项
         */
        const OPT_CLASS = 'className';

        /**
         * @const string 关系表名选项
         */
        const OPT_TABLE = 'table';

        /**
         * @const string 多对于关系表；
         */
        const OPT_RELATION_TABLE = 'relationTable';

        /**
         * @const string 关系表别名
         */
        const OPT_ALIAS = 'alias';

        /**
         * @const string 主键
         */
        const OPT_PRIMARY = 'primaryKey';

        /**
         * 根据配置信息返回同步加载SCHEMA信息
         *
         * @param string $modelName    模型名称
         * @param array  $options      关系选项
         *
         * @return array
         */
        function schema ($modelName, $options);

        /**
         * 根据$model和$options懒加载关系对象
         *
         * @param Model $model   实例模型
         * @param array $options 配设选项
         *
         * @return Model|ModelFinder
         */
        function load ($model, $options);
    }

    /**
     * 抽象模型关系对象，提供模型关系中的一些基本服务；
     */
    abstract class AbstractModelRelation implements ModelRelation {

        /**
         * 根据配置信息返回同步加载选项SCHEMA信息
         *
         * @param string $modelName    模型名称
         * @param array  $options      选项
         *
         * @return array
         *
         * @throws ModelRelationException 指定关系不存在。
         */
        public function schema ($modelName, $options) {
            # 如果没有提供类名选项；
            if (empty($options[self::OPT_CLASS])) {
                throw new ModelRelationException('Model relation not support className options');
            }

            return $this->_schema ($modelName, $options);
        }

        /**
         * 创建一对一关系对象；
         *
         * @throws ModelRelationException 关系配置信息不正确。
         */
        public function load ($model, $options) {
            # 如果没有提供类名选项；
            if (empty($options[self::OPT_CLASS])) {
                throw new ModelRelationException('Model relation not support className options');
            }

            return $this->_load ($model, $options);
        }


        /**
         * 默认将为$options增加table和alias选项，可在子类中重写
         *
         * @param string $modelName    模型名称
         * @param array  $options      配置选项
         *
         * @return array
         */
        protected function _schema ($modelName, $options) {
            # 获取关系类的文件名和类名
            $relModelName = Jtita::importInterface ($options[self::OPT_CLASS]);
            $options[self::OPT_TABLE] = $relModelName::dataName ();

            # 如果没有指定别名
            if (! isset($options[self::OPT_ALIAS])) {
                $options[self::OPT_ALIAS] = $relModelName::dataAlias ();
            }

            return $options;
        }

        /**
         * 抽象关系对象创建方法，提供创建具体关系对象的服务；
         */
        protected abstract function _load ($model, $options);

        /**
         * 根据$model、$relModelName和$options获取查询条件
         *
         * @param Model        $model        模型实例
         * @param string|Model $relModelName 关系模型名字
         * @param array        $options      模型选项
         *
         * @return string
         */
        protected function _where ($model, $relModelName, &$options) {
            # 如果没有设置查询条件，返回默认条件；
            if (empty($options['where'])) {
                return $this->_defaultWhere ($model, $relModelName, $options);
            }

            # 获取条件并移除选项中的条件；
            $where = array_kpop ($options, 'where', array());
            if (is_array ($where)) {
                return array_merge ($where, $this->_defaultWhere ($model, $relModelName, $options));
            }
            else {
                return array_merge (array($where), $this->_defaultWhereArgs ($model, $relModelName, $options));
            }
        }

        /**
         * 根据$model、$relModelName和$options返回默认查询条件
         *
         * @param Model  $model        模型实例
         * @param string $relModelName 关系模型名字
         * @param array  $options      关系选项
         *
         * @return array 查询条件
         */
        protected function _defaultWhere ($model, $relModelName, $options) {
        }

        /**
         * 根据$model、$relModelName和$options返回默认查询参数
         * ；
         * @param Model  $model        模型实例
         * @param string $relModelName 关系模型名字
         * @param array  $options      关系选项
         *
         * @return array 查询条件
         */
        protected function _defaultWhereArgs ($model, $relModelName, $options) {
        }

        /**
         * 根据$modelName和$options返回外值名字
         *
         * @param string $dataName 模型名字
         * @param array  $options  关系选项
         *
         * @return string
         */
        protected function _foreignKeyName ($dataName, $options) {
            return isset($options[self::OPT_FOREIGN]) ? $options[self::OPT_FOREIGN] : $dataName . '_id';
        }
    }

    /**
     * 一对一关模型关系；
     *
     * @说明
     *         在实际应用中呈现出一对一关系的主要包括两种方式：
     *        一、以两个对象主键为统一键值的真实一对一关系如：用户基础信息 + 用户配置信息；
     *        二、以一对多关系呈现出的虚拟一对一关系如：论坛分类 + 最新主题
     */
    class HasOneModelRelation extends AbstractModelRelation implements ModelRelation {
        /**
         * 对象创建方法，提供创建具体关系对象的服务
         *
         * @param Model $model   模型实例
         * @param Array $options 关系选项
         *
         * @return mixed
         */
        protected function _load ($model, $options) {
            # 从类选项中分隔出引用文件和类名称；
            $relationModelName = Jtita::importInterface ($options[self::OPT_CLASS]);
            # 生成关系模型
            $relationModel = false;

            # 如果模型的外键值为空，则返回空白关系对象；
            if (($key = $model->key ()) != null) {
                $where = $this->_where ($model, $relationModelName, $options);
                $relationModel = $relationModelName::find ($where, $options);
            }

            # 如果指定模型未找到，返回空白对象；
            return $relationModel ? $relationModel : new $relationModelName(array(), true);
        }

        /**
         * @see AbstractModelRelation::_defaultWhere
         */
        protected function _defaultWhere ($model, $relModelName, $options) {
            $foreignKey = $this->_foreignKeyName ($model::dataName (), $options);
            return array($foreignKey => $model->key ());
        }

        /**
         * 获取当前默认条件参数值；
         */
        protected function _defaultWhereArgs ($model, $relModelName, $options) {
            return array(':foreignKey' => $model->key ());
        }
    }

    /**
     * 一对多模型关系，提供模型的一对多处理
     *
     * @package jtita.models
     */
    class HasManyModelRelation extends HasOneModelRelation implements ModelRelation {
        /**
         * 对象创建方法，提供创建具体关系对象的服务；
         */
        protected function _load ($model, $options) {
            # 从类选项中分隔出引用文件和类名称；
            $relationModelName = Jtita::importInterface ($options[self::OPT_CLASS]);
            # 处理完整的查询条件；
            $where = $this->_where ($model, $relationModelName, $options);
            # 返回模型查找对象；
            return $relationModelName::findAll ($where, $options);
        }
    }

    /**
     * 多对一模型关系；
     */
    class BelongsToModelRelation extends AbstractModelRelation {
        /**
         * 对象创建方法，提供创建具体关系对象的服务；
         */
        protected function _load ($model, $options) {
            # 从类选项中分隔出引用文件和类名称；
            $modelName = Jtita::importInterface ($options[self::OPT_CLASS]);

            # 获取外键名称
            $foreignKey = $this->_foreignKeyName ($modelName::dataName (), $options);

            # 加载关联模型对象
            $relationModel = false;
            # 如果模型的外键值为空，则返回空白关系对象；
            if ($model->$foreignKey != null) {
                $relationModel = $modelName::find ($model->$foreignKey, $options);
            }

            # 如果指定模型未找到，返回空白对象；
            return $relationModel ? $relationModel : new $modelName(array(), true);
        }
    }

    /**
     * 多对多模型关系；
     */
    class HasAndBelongsToModelRelation extends AbstractModelRelation implements ModelRelation {
        /**
         * 重写，根据$relationName和$options获取关系SCHEMA。
         *
         * @param string $modelName    模型名称
         * @param array  $options      选项
         * @return array
         */
        protected function _schema ($modelName, $options) {
            $relModelName = Jtita::importInterface ($options[self::OPT_CLASS]);
            $options = parent::_schema ($modelName, $options);

            if (empty($options[self::OPT_RELATION_TABLE])) {
                $tables = array($modelName::dataName (), $relModelName::dataName ());
                sort ($tables);
                $options[self::OPT_RELATION_TABLE] = camel_lower ($tables[0] . '_' . $tables[1]);
            }

            if (empty($options[self::OPT_PRIMARY])) {
                $options[self::OPT_PRIMARY] = camel_lower ($modelName . 'Id');
            }

            if (empty($options[self::OPT_FOREIGN])) {
                $options[self::OPT_FOREIGN] = camel_lower ($relModelName . 'Id');
            }

            unset($options[self::OPT_ALIAS]);

            return $options;
        }

        /**
         * 对象创建方法，提供创建具体关系对象的服务；
         */
        protected function _load ($model, $options) {
            # 从类选项中分隔出引用文件和类名称；
            $modelName = Jtita::importInterface ($options[self::OPT_CLASS]);
            return $modelName::findAll (array("{$modelName::dataKey()} in " => new NormalParameter("(SELECT {$options[self::OPT_FOREIGN]} FROM {$options[self::OPT_RELATION_TABLE]} WHERE {$options[self::OPT_PRIMARY]} = {$model->key()})")), $options);
        }
    }
	
	
	
	