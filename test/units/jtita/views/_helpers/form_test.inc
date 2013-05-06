<?php
    /**
     * form_test.php 提供对视图表单助手的单元测试服务；
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.views._helpers
     * @version     1.0 beta
     *
     * @create      2012-08-05
     * @modify      2013-04-28
     */

    /**
     * 定义顶级路径；
     */
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (dirname (__FILE__)))))));

    /**
     * 导入测试内容和依赖文件；
     */
    require_once(JTPATH_ROOT . '/jtita/kernels/funs.inc');
    require_once(JTPATH_ROOT . '/jtita/views/_helpers/form.inc');

    /**
     * 字段生成模拟器，提供字段生成类的测试模拟服务。
     *
     * @package jtita.views._helpers
     */
    class FieldBuilderMock extends FieldBuilder {
        public function modelName () {
            return $this->_modelName;
        }

        public function modelObj () {
            return $this->_modelObject;
        }

        public function builderOptions () {
            return $this->_options;
        }

        public function fieldID ($field) {
            return $this->_fieldID ($field);
        }

        public function  fieldName ($field) {
            return $this->_fieldName ($field);
        }
    }

    /**
     * 表单助手测试类，提供对表单中使用的方法和对象的测试服务。
     *
     * @package jtita.views._helpers
     */
    class FormTest extends PHPUnit_Framework_TestCase {

        /**
         * 测试字段生成器初始化。
         */
        public function testFieldBuilderForConstruct () {
            $builder = new FieldBuilderMock('User', 'Hello World', 'action');

            $this->assertEquals ('User', $builder->modelName ());
            $this->assertEquals ('Hello World', $builder->modelObj ());
            $this->assertEquals ('action', $builder->builderOptions ());
        }

        /**
         * 测试字段生成器，字段ID值。
         */
        public function testFieldBuilderForFieldID () {
            $builder = new FieldBuilderMock('UserGroup', null);

            $this->assertEquals ('user_group_id', $builder->fieldID ('id'));
            $this->assertEquals ('user_group__name', $builder->fieldID ('Name'));
        }

        /**
         * 测试字段生成器，字段名称。
         */
        public function testFieldBuilderForFieldName () {
            $builder = new FieldBuilderMock('UserGroup', null);

            $this->assertEquals ('UserGroup[id]', $builder->fieldName ('id'));
            $this->assertEquals ('UserGroup[Name]', $builder->fieldName ('Name'));
        }

        /*
         * 测试字段生成器，生成label HTML。
         */
        public function testFieldBuilderForLabel () {
            $builder = new FieldBuilderMock('UserGroup');

            $this->assertEquals ('<label for="user_group_id">id</label>', $builder->label ('id'));
        }

        /**
         * 测试字段生成器，生成label 字符串选项。
         */
        public function testFieldBuilderForLabelStringOptions () {
            $builder = new FieldBuilderMock('UserGroup');

            $this->assertEquals ('<label for="user_group_id">IDV</label>', $builder->label ('id', 'IDV'));
        }

        /**
         * 测试字段生成器，指定标签数组选项，但不包含标签选项。
         */
        public function testFieldBuilderForLabelArrayOptionsAndNotIncludeTitle () {
            $builder = new FieldBuilderMock('UserGroup');

            $this->assertEquals ('<label class="title" for="user_group_id">id</label>', $builder->label ('id', array('class' => 'title')));
        }

        /**
         * 测试字段生成器，指定标签数组选项，包含标题选项。
         */
        public function  testFieldBuilderForLabelArrayOptionsAndIncludeTitle () {
            $builder = new FieldBuilderMock('UserGroup');

            $this->assertEquals ('<label class="title" for="user_group_id">IDV</label>', $builder->label ('id', array(
                                                                                                                     'title' => 'IDV',
                                                                                                                     'class' => 'title'
                                                                                                                )));
        }

        /**
         * 测试字段生成器，生成普通文本录入框。
         */
        public function testFieldBuilderForText () {
            $builder = new FieldBuilderMock('UserGroup');

            $this->assertEquals ('<input id="user_group_id" name="UserGroup[id]" type="text" />', $builder->text ('id'));
        }

        /**
         * 测试字段生成器，指定非数组选项
         */
        public function testFieldBuilderForTextNotArrayOptions () {
            $builder = new FieldBuilderMock('UserGroup');

            $this->assertEquals ('<input value="error" id="user_group_id" name="UserGroup[id]" type="text" />', $builder->text ('id', 'error'));
            $this->assertEquals ('<input value="1" id="user_group_id" name="UserGroup[id]" type="text" />', $builder->text ('id', 1));
        }

        /**
         * 测试字段生成器，指定数组选项。
         */
        public function testFieldBuilderForTextArrayOptions () {
            $builder = new FieldBuilderMock('UserGroup');

            $this->assertEquals ('<input class="error" id="user_group_id" name="UserGroup[id]" type="text" />', $builder->text ('id', array('class' => 'error')));
        }

        /**
         * 测试字段生成器，指定模型实例包含值。
         */
        public function testFieldBuilderForTextForModelObjHasValue () {
            $builder = new FieldBuilderMock('UserGroup', array('name' => 'zhenggp'));

            $this->assertEquals ('<input id="user_group_name" name="UserGroup[name]" value="zhenggp" type="text" />', $builder->text ('name'));
        }

        /**
         * 测试字段生成器，指定字段包含值，但被重写。
         *
         * @depends testFieldBuilderForTextNotArrayOptions
         */
        public function testFieldBuilderForTextForModelObjHasValueAndSetValue () {
            $builder = new FieldBuilderMock('UserGroup', array('name' => 'zhenggp'));

            $this->assertEquals ('<input value="Hello World" id="user_group_name" name="UserGroup[name]" type="text" />', $builder->text ('name', 'Hello World'));
        }

        /**
         * 测试字段生成器，指定魔法值。
         *
         * @depends testFieldBuilderForTextNotArrayOptions
         */
        public function testFieldBuilderForTextForMagicValue () {
            $builder = new FieldBuilderMock('UserGroup');

            $this->assertEquals ('<input value="&lt;&gt;&lt;&gt;&amp;nbsp;&gt;&gt;" id="user_group_name" name="UserGroup[name]" type="text" />', $builder->text ('name', '<><>&nbsp;>>'));
        }

        /**
         * 测试生成文本域。
         */
        public function testFieldBuilderForTextarea () {
            $builder = new FieldBuilderMock('UserGroup');

            $this->assertEquals ('<textarea id="user_group_description" name="UserGroup[description]"></textarea>', $builder->textarea ('description'));
        }

        /**
         * 测试生成文本域，非数组选项
         */
        public function testFieldBuilderForTextareaNotArrayOptions () {
            $builder = new FieldBuilderMock('UserGroup');

            $this->assertEquals ('<textarea id="user_group_description" name="UserGroup[description]">Hello World</textarea>', $builder->textarea ('description', 'Hello World'));
            $this->assertEquals ('<textarea id="user_group_description" name="UserGroup[description]">123</textarea>', $builder->textarea ('description', 123));
        }

        /**
         * 测试生成文本域，数组选项。
         */
        public function testFieldBuilderForTextareaArrayOptions () {
            $builder = new FieldBuilderMock('UserGroup');

            $this->assertEquals ('<textarea class="error" id="user_group_description" name="UserGroup[description]">Hello World</textarea>', $builder->textarea ('description', array(
                                                                                                                                                                                     'value' => 'Hello World',
                                                                                                                                                                                     'class' => 'error'
                                                                                                                                                                                )));
        }

        /**
         * 测试生成文本域，设置模型值。
         */
        public function testFieldBuilderForTextareaModelValue () {
            $builder = new FieldBuilderMock('UserGroup', array('description' => 'Hello World'));

            $this->assertEquals ('<textarea id="user_group_description" name="UserGroup[description]">Hello World</textarea>', $builder->textarea ('description'));
        }

        /**
         * 测试生成文本域，设置模型值，但包含有值选项
         */
        public function testFieldBuilderForTextareaModelValueAndSetValue () {
            $builder = new FieldBuilderMock('UserGroup', array('description' => 'Hello World'));

            $this->assertEquals ('<textarea id="user_group_description" name="UserGroup[description]">VAL</textarea>', $builder->textarea ('description', 'VAL'));
        }

        /**
         * 测试字段生成，email 录入框。
         */
        public function testFieldBuilderForEmail () {
            $builder = new FieldBuilderMock('UserGroup', array('email' => 'zhenggp2004@163.com'));

            $this->assertEquals ('<input class="email" id="user_group_email" name="UserGroup[email]" value="zhenggp2004@163.com" type="email" />', $builder->email ('email', array('class' => 'email')));
        }

        /**
         * 测试字段生成，密码录入框。
         */
        public function testFieldBuilderForPassword () {
            $builder = new FieldBuilderMock('UserGroup', array('password' => '123321'));

            $this->assertEquals ('<input class="password" id="user_group_password" name="UserGroup[password]" value="123321" type="password" />', $builder->password ('password', array('class' => 'password')));
        }

        /**
         * 测试字段生成，密码录入框。
         */
        public function testFieldBuilderForPasswordValueSet () {
            $builder = new FieldBuilderMock('UserGroup', array('password' => '123321'));

            $this->assertEquals ('<input class="password" value="" id="user_group_password" name="UserGroup[password]" type="password" />', $builder->password ('password', array(
                                                                                                                                                                                 'class' => 'password',
                                                                                                                                                                                 'value' => ''
                                                                                                                                                                            )));
        }

        /**
         * 测试字段生成，select 录入框。
         */
        public function testFieldBuilderForSelect () {
            $builder = new FieldBuilderMock('User');

            $this->assertEquals ('<select id="user_group" name="User[group]"></select>', $builder->select ('group'));
        }

        /**
         * 测试字段生成，select 录入框，包含选项框
         */
        public function testFieldBuilderForSelectIncludeOptions () {
            $builder = new FieldBuilderMock('User');

            $this->assertEquals ('<select id="user_group" name="User[group]"><option value="1">ASP</option><option value="2">PHP</option></select>', $builder->select ('group', array(
                                                                                                                                                                                     'options' => array(
                                                                                                                                                                                         '1' => 'ASP',
                                                                                                                                                                                         '2' => 'PHP'
                                                                                                                                                                                     )
                                                                                                                                                                                )));
        }

        /**
         * 测试字段生成，select 录入框，包含empty选项。
         */
        public function testFieldBuilderForSelectIncludeEmpty () {
            $builder = new FieldBuilderMock('User');

            $this->assertEquals ('<select id="user_group" name="User[group]"><option>all</option><option value="1">ASP</option><option value="2">PHP</option></select>', $builder->select ('group', array(
                                                                                                                                                                                                         'options' => array(
                                                                                                                                                                                                             '1' => 'ASP',
                                                                                                                                                                                                             '2' => 'PHP'
                                                                                                                                                                                                         ),
                                                                                                                                                                                                         'empty' => 'all'
                                                                                                                                                                                                    )));
        }

        /**
         * 测试字段生成，select 录入框，包含attr选项。
         */
        public function  testFieldBuilderForSelectValueOptions () {
            $builder = new FieldBuilderMock('User');

            $this->assertEquals ('<select id="user_group" name="User[group]"><option value="1" selected>ASP</option><option value="2">PHP</option></select>', $builder->select ('group', array(
                                                                                                                                                                                              'options' => array(
                                                                                                                                                                                                  '1' => 'ASP',
                                                                                                                                                                                                  '2' => 'PHP'
                                                                                                                                                                                              ),
                                                                                                                                                                                              'value' => 1
                                                                                                                                                                                         )));
        }

        /**
         * 测试字段生成，select 录入框，包含attr选项。
         */
        public function  testFieldBuilderForSelectModelValue () {
            $builder = new FieldBuilderMock('User', array('group' => 1));

            $this->assertEquals ('<select id="user_group" name="User[group]"><option value="1" selected>ASP</option><option value="2">PHP</option></select>',
                $builder->select ('group', array(
                    'options' => array(
                        '1' => 'ASP',
                        '2' => 'PHP'
                    )
                )));
        }

        /**
         * 测试字段生成，包含单选框。
         */
        public function testFieldBuilderForRadio () {
            $builder = new FieldBuilderMock('User');

            $this->assertEquals ('<input id="user_allow" name="User[allow]" type="radio" />', $builder->radio ('allow'));
        }

        /**
         * 测试字段生成，单选框，包含值。
         */
        public function testFieldBuilderForRadioValue () {
            $builder = new FieldBuilderMock('User');

            $this->assertEquals ('<input value="1" id="user_allow_1" name="User[allow]" type="radio" />', $builder->radio ('allow', 1));
        }

        /**
         * 测试字段生成，单选框，数组选项。
         */
        public function testFieldBuilderForArrayOptions () {
            $builder = new FieldBuilderMock('User');

            $this->assertEquals ('<input value="1" class="checkbox" id="user_allow_1" name="User[allow]" type="radio" />', $builder->radio ('allow', array(
                                                                                                                                                          'value' => 1,
                                                                                                                                                          'class' => 'checkbox'
                                                                                                                                                     )));
        }

        /**
         * 测试字段值，包含模型字段值。
         */
        public function testFieldBuilderForRadioModelValue () {
            $builder = new FieldBuilderMock('User', array('allow' => 1));

            $this->assertEquals ('<input value="1" id="user_allow_1" name="User[allow]" type="radio" checked="1" />', $builder->radio ('allow', 1));
        }

        /**
         * 测试字段生成，只读对象。
         */
        public function testFieldBuilderForSpan () {
            $builder = new FieldBuilderMock('User');

            $this->assertEquals ('<span></span>', $builder->span ('allow'));
        }

        /**
         * 测试字段生成，Span对象。
         */
        public function testFieldBuilderForSpanAndModelValue () {
            $builder = new FieldBuilderMock('User', array('name' => 'zhenggp'));

            $this->assertEquals ('<span>zhenggp</span>', $builder->span ('name'));
        }

        /**
         * 测试字段生成，span attrs选项；
         */
        public function testFieldBuilderForSpanAttrsOptions () {
            $builder = new FieldBuilderMock('User', array('name' => 'zhenggp'));

            $this->assertEquals ('<span id="in" class="input">zhenggp</span>', $builder->span ('name', array(
                                                                                                            'id' => 'in',
                                                                                                            'class' => 'input'
                                                                                                       )));
        }

        /**
         * 测试字段生成器，span 并且指定格式器。
         */
        public function testFieldBuilderForSpanAndFormatter () {
            $builder = new FieldBuilderMock('User', array('created' => 60));

            $this->assertEquals ('<span>1970-01-01</span>', $builder->span ('created', array(
                                                                                            'formatter' => 'date',
                                                                                            'format' => 'Y-m-d'
                                                                                       )));
            $this->assertEquals ('<span>1970-01-01am</span>', $builder->span ('created', array(
                                                                                              'formatter' => 'date',
                                                                                              'format' => 'Y-m-da'
                                                                                         )));
        }

        /**
         * 测试表单生成器，开始模型。
         */
        public function testFormBuilderForBegin () {
            $builder = new FormBuilder('UserGroup');

            $this->assertEquals ('<form id="user_group_form" method="_post" charset="utf-8">', $builder->begin ());
        }

        /**
         * 测试表单生成器，开始标记，使用字符串选项。
         */
        public function testFormBuilderForBeginAndStringOptions () {
            $builder = new FormBuilder('UserGroup', null, '/users');

            $this->assertEquals ('<form id="user_group_form" method="_post" charset="utf-8" action="/users">', $builder->begin ());
        }

        /**
         * 测试表单生成，开始标记，使用数组选项。
         */
        public function  testFormBuilderForBeginAndArrayOptions () {
            $builder = new FormBuilder('UserGroup', null, array(
                                                               'action' => '/users',
                                                               'class' => 'error'
                                                          ));

            $this->assertEquals ('<form id="user_group_form" method="_post" charset="utf-8" action="/users" class="error">', $builder->begin ());
        }


        /**
         * 测试表单生成器，开始标记，使用数组action方法。

        public  function  testForBuilderForBeginAndArrayActionOptions() {
        $builder = new FormBuilder('UserGroup',null,array('action' => array('action' => 'edit'),'class' => 'error'));

        $this->assertEquals('<form id="user_group_form" method="_post" charset="utf-8" action="/users" class="error">',$builder->begin());
        }
         */

        /**
         * 测试生成结束标志。
         */
        public function  testFormBuilderForEnd () {
            $builder = new FormBuilder('UserGroup');

            $this->assertEquals ('</form>', $builder->end ());
        }

    }