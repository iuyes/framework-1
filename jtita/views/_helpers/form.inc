<?php
    /**
     * form.inc 表单帮助类，提供表单的HTML生成函数。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.views
     * @version     1.0 beta
     *
     * @create      2012-07-31
     * @modify      2013-04-28
     */

    /**
     * 字段生成器，提供表单中单中字段的HTML生成。
     */
    class FieldBuilder {

        /**
         * @const 格式化选项；
         */
        const OPTION_FORMAT = 'format';

        /**
         * @const 格式器
         */
        const OPTION_FORMATTER = 'formatter';

        /**
         * @const 默认格式器；
         */
        const DEFAULT_FORMATTER = 'date';

        /**
         * @var array 默认错误选项
         */
        public static $defaultErrorsOptions = array('class' => 'error-summary');

        /**
         * 使用$modelName、$modelObj和$options初始化FormBuilder实例。
         *
         * @param string                    $modelName 模型名称
         * @param array|\ArrayAccess|\Model $modelObj  模型实例对象
         * @param array|string              $options   选项
         */
        public function __construct ($modelName, $modelObj = array(), $options = array()) {
            $this->_modelName = $modelName;
            $this->_modelObject = $modelObj;
            $this->_options = $options;

            # 如果模型对象不为空。
            if (! empty($modelObj) && method_exists ($modelObj, 'errors')) {
                $this->_modelErrors = $modelObj->errors ();
            }
        }

        /**
         * 根据$propName返回模型的数组值，不存在则返回$default。
         *
         * @param string $propName 属性名称
         * @param mixed  $default  默认值；
         * @return mixed
         */
        public function prop ($propName, $default = false) {
            # 如果包含模型对象，并且指定的属性值存在。
            if (! empty($this->_modelObject) && isset($this->_modelObject[$propName])) {
                $default = $this->_modelObject[$propName];
            }

            return $default;
        }

        /**
         * 根据$field和$options生成label标签。
         *
         * @param string       $field   字段名称
         * @param string|array $options 标签选项
         *
         * @return string
         */
        public function label ($field, $options = array()) {
            # 如果选项为非数组选项。
            if (! is_array ($options)) {
                $options = array('title' => $options);
            }

            $title = array_kpop ($options, 'title', $field);
            $options['for'] = $this->_fieldID ($field);

            return '<label' . array_htmlattr ($options) . '>' . $title . '</label>';
        }

        /**
         * 根据地$field和$options生成隐藏域字端。
         *
         * @param string       $field   字段名称
         * @param string|array $options 选项
         *
         * @return string
         */
        public function hidden ($field, $options = array()) {
            # 如果选项为非数组选项。
            if (! is_array ($options)) {
                $options = array('value' => $options);
            }

            return $this->_input ($field, 'hidden', $options);
        }

        /**
         * 根据$field和$options生成input text标签。
         *
         * @param string       $field   字段名称
         * @param array|string $options 选项
         *
         * @return string
         */
        public function text ($field, $options = array()) {
            return $this->_input ($field, 'text', $options);
        }

        /**
         * 根据$field和$options生成input email标签。
         *
         * @param string       $field   字段名称
         * @param array|string $options 选项
         *
         * @return string
         */
        public function email ($field, $options = array()) {
            return $this->_input ($field, 'email', $options);
        }

        /**
         * 根据$field和$options生成input password标签。
         *
         * @param string       $field   字段名称
         * @param array|string $options 选项
         *
         * @return string
         */
        public function password ($field, $options) {
            return $this->_input ($field, 'password', $options);
        }

        /**
         * 根据$field和$options生成SELECT HTML标签
         *
         * @param string $field   字段名称
         * @param array  $options 选项
         *
         * @return string
         */
        public function select ($field, $options = array()) {
            # 初始化默认选项。
            $options['id'] = $this->_fieldID ($field);
            $options['name'] = $this->_fieldName ($field);

            # 获取列表选项。
            $value = array_kpop ($options, 'value', $this->_fieldValue ($field));
            $lists = array_kpop ($options, 'options', array());
            $empty = array_kpop ($options, 'empty');

            # 生成HTML。
            $html = '<select' . array_htmlattr ($options) . '>';
            # 如果包含空白选项。
            $html .= $this->options ($lists, $value, $empty);

            return $html . '</select>';
        }

        /**
         * 根据$options和$value生成列表选项
         *
         * @param array       $list    选项
         * @param mixed       $value   选定值
         * @param bool|string $empty   空白选项
         * @return string
         */
        public function options ($list, $value = false, $empty = false) {
            $html = '';

            if ($empty) $html .= "<option>$empty</option>";
            foreach ($list as $key => $title) {
                if ($key == $value) {
                    $html .= "<option value=\"$key\" selected>$title</option>";
                }
                else {
                    $html .= "<option value=\"$key\">$title</option>";
                }
            }

            return $html;
        }

        /**
         * 根据$field和$options生成checkbox HTML标签。
         *
         * @param string      $field   字段名称
         * @param array       $options 选项
         * @return string
         */
        public function checkbox ($field, $options = array()) {
            return $this->_input ($field, 'checkbox', $options);
        }

        /**
         * 根据$field和$options生成radio HTML标签。
         *
         * @param string $field   字段名称
         * @param mixed  $options 选项
         *
         * @return string
         */
        public function radio ($field, $options = array()) {
            # 如果选项不是数组
            if (! is_array ($options)) {
                $options = array('value' => $options);
            }

            # 从选项中获取值信息。
            $optionValue = array_kvalue ($options, 'value');
            $modelValue = $this->_fieldValue ($field);

            # 设置radio的默认值。
            if ($optionValue) {
                $options['id'] = $this->_fieldID ($field) . '_' . $optionValue;
            }
            else {
                $options['id'] = $this->_fieldID ($field);
            }

            $options = $this->_fieldError ($field, $options);
            $options['name'] = $this->_fieldName ($field);
            $options['type'] = 'radio';

            # 如果当前值与模型值相同，则设置选取标志。
            if (! is_null ($modelValue) && $options && $modelValue == $optionValue) {
                $options['checked'] = 1;
            }

            return '<input' . array_htmlattr ($options) . ' />';
        }

        /**
         * 根据$field和$options生成字段span。
         *
         * @param string      $field   字段名称
         * @param array|mixed $options 选项
         *
         * @return string
         */
        public function span ($field, $options = array()) {
            # 获取字段值。
            $value = $this->_fieldValue ($field);

            # 如果指定有value值;
            if (! is_null ($value) && isset($options[self::OPTION_FORMAT])) {
                $format = array_kpop ($options, self::OPTION_FORMAT);
                $formatter = array_kpop ($options, self::OPTION_FORMATTER, self::DEFAULT_FORMATTER);
                $value = call_user_func ($formatter, $format, $value);
            }

            return "<span" . array_htmlattr ($options) . ">$value</span>";
        }

        /**
         * 根据$field和$options生成textarea标签。
         *
         * @param string       $field   字段名称
         * @param array|string $options 选项
         *
         * @return string
         */
        public function textarea ($field, $options = array()) {
            # 如果选项为非数组选项
            if (! is_array ($options)) {
                $options = array('value' => $options);
            }

            # 设置默认选项值。
            $options = $this->_defaultFieldOptions ($field, $options);
            $value = array_kpop ($options, 'value', '');

            return '<textarea' . array_htmlattr ($options) . '>' . $value . '</textarea>';
        }

        /**
         * 显示字段错误消息。
         */
        public function errors ($options = array()) {
            # 如果错误消息不为空。
            if (empty($this->_modelErrors)) return '';

            # 合并默认选项。
            $options = array_merge (self::$defaultErrorsOptions, $options);

            # 生成HTML标签。
            $html = '<div ' . array_htmlattr ($options) . '>';
            $html .= '<a href="javascript:void(0);" onclick="this.parentNode.style.display=\'none\';" class="close">x</a><ul>';
            foreach ($this->_modelErrors as $field => $message) {
                $fieldID = $this->_fieldID ($field);
                $html .= "<li id=\"$field-error\"><a href='javascript:void(0);' onclick='document.getElementById(\"$fieldID\").focus();'>$message</a></li>";
            }

            return $html . '</ul></div>';
        }

        /**
         * 根据$field、$type和$options生成录入框。
         *
         * @param string      $field   字段名称
         * @param string      $type    字段类型
         * @param array|mixed $options 选项
         *
         * @return string
         */
        protected function _input ($field, $type, $options = array()) {
            # 如果选项为非数组选项。
            if (! is_array ($options)) {
                $options = array('value' => $options);
            }

            # 设置默认选项值。
            $options = $this->_defaultFieldOptions ($field, $options);
            $options['type'] = $type;

            return '<input' . array_htmlattr ($options) . ' />';
        }

        /**
         * 为字段增加默认的选项参数，并且返回。
         *
         * @param string $field   字段名称
         * @param array  $options 当前选项
         *
         * @return array
         */
        protected function _defaultFieldOptions ($field, $options) {
            # 生成字段的ID和name属性。
            $options['id'] = $this->_fieldID ($field);
            $options['name'] = $this->_fieldName ($field);

            # 如果未直接指定值，并且模型实例包含指定值。
            if (! isset($options['value']) && ! is_null ($fieldValue = $this->_fieldValue ($field))) {
                $options['value'] = $fieldValue;
            }

            # 返回错误处理后的字段。
            return $this->_fieldError ($field, $options);
        }

        /**
         * 根据$field返回字段错误消息。
         *
         * @param string $field   字段名称
         * @param array  $options 选项
         *
         * @return string
         */
        protected function _fieldError ($field, $options) {
            # 如果模型错误不为空。
            if (! empty($this->_modelErrors) && ! empty($this->_modelErrors[$field])) {
                $options['title'] = $this->_modelErrors[$field];
                array_joinstr ($options, 'class', ' input-error');
            }

            return $options;
        }

        /**
         * 根据$field生成字段ID值
         *
         * @param string $field 字段名字
         *
         * @return string
         */
        protected function _fieldID ($field) {
            return camel_lower ($this->_modelName . '_' . $field);
        }

        /**
         * 根据$field生成字段名称。
         *
         * @param string $field 字段名称
         *
         * @return string
         */
        protected function _fieldName ($field) {
            return $this->_modelName . "[$field]";
        }

        /**
         * 根据$field获取字段值。
         *
         * @param string $field 字段名称
         *
         * @return mixed
         */
        protected function _fieldValue ($field) {
            # 如果模型实例为空，直接返回值。
            if (empty($this->_modelObject)) return null;
            # 如果模型指定字段为空值。
            if (! isset($this->_modelObject[$field])) return null;

            # 返回模型值。
            return $this->_modelObject[$field];
        }


        /**
         * @var string 模型名字
         */
        protected $_modelName;

        /**
         * @var Model|ArrayAccess 模型实例
         */
        protected $_modelObject;

        /**
         * @var bool|array 判断是否允许错误显示。
         */
        protected $_modelErrors = false;

        /**
         * @var array 模型选项
         */
        protected $_options = array();
    }

    /**
     * 表单生成器，提供表单的HTML生成。
     *
     * @package jtita.views.helpers
     */
    class FormBuilder extends FieldBuilder {

        /**
         * 使用$modelName、$modelObj和$options初始化FormBuilder实例。
         *
         * @param string                    $modelName 模型名称
         * @param array|\ArrayAccess|\Model $modelObj  模型实例对象
         * @param array|string              $options   如果选项为字符串，表示指定action属性；
         */
        public function __construct ($modelName, $modelObj = null, $options = array()) {
            # 如果选项不是数组，则指定action属性为$options值。
            if (! is_array ($options)) {
                $options = array('action' => $options);
            }

            parent::__construct ($modelName, $modelObj, $options);
        }

        /**
         * 返回FORM HTML开始标签。
         */
        public function begin () {
            # 合并默认选项。
            $options = array_merge ($this->_defaultFormOptions (), $this->_options);

            return '<form' . array_htmlattr ($options) . '>';
        }

        /**
         * 返回FROM 环验证码。
         */
        public function token () {

        }

        /**
         * 返回FROM HTML结束标签。
         */
        public function end () {
            return '</form>';
        }

        /**
         * 返回表单默认选项。
         */
        protected function _defaultFormOptions () {
            return array('id' => $this->_formID (), 'method' => 'post', 'charset' => 'utf-8',
                'action' => Router::url (array()));
        }

        /**
         * 返回表单默认ID。
         *
         * @return string
         */
        protected function _formID () {
            return camel_lower ($this->_modelName . '_form');
        }

    }

    /**
     * 字段集合生成器，提供fieldset的HTML生成。
     */
    class FieldSetBuilder {

    }


    /**
     * 根据$modelName、$modelObj、$builder和$options生成表单HTML。
     *
     * @param string       $modelName 模型名称
     * @param Model|array  $modelObj  模型对象
     * @param Closure      $builder   生成回调
     * @param array|string $options   选项
     *
     * @return string
     */
    function _formBuilder ($modelName, $modelObj, $builder, $options = array()) {
        $formBuilder = new FormBuilder($modelName, $modelObj, $options);

        echo $formBuilder->begin ();
        echo $builder($formBuilder);
        echo $formBuilder->end ();
    }