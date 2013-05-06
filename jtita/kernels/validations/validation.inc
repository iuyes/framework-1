<?php
    /**
     * validation.php 验证组件服务接口Validation类声明文件。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.validations
     * @version     1.0 beta
     *
     * @create      2012-11-07
     * @modify      2013-04-28
     */

    /**
     * 验证组件开启标志。
     */
    define('JTITA_VALIDATION', true);

    # 导入预定义验证器；
    Jtita::import ('kernels.validations.validators');

    /**
     * 验证组件接口对象，提供对象的多字段、多规则验证服务。
     *
     * <h3>默认Validation中支持以下验证器规则</h3>
     *  <ul>
     *      <li>非空验证器(required)，验证字段为非空值；</li>
     *      <li>正则表达式验证器(regex)，通过正则表达式验证字段值；</li>
     *      <li>数字验证器(number)，验证字段值是否为数字值；</li>
     *      <li>电话验证器(telephone)，验证电话是否为坐机(xxxx-xxxxxxxx｜xxx-xxxxxxxx)、
     *          移动电话(xxxxxxxxxxx)、400电话(400-xxx-xxxxx)；</li>
     *      <li>比较验证器(compare)，验证指定的字段值是否相同；</li>
     *      <li>数字范围验证器(range)，验证值是否在指定的范围内；</li>
     *      <li>枚举验证器(enum)，验证值是否属于指定的值之一；</li>
     *      <li>最小长度验证器(minlen)，验证字段值是否大于指定长度；</li>
     *      <li>邮件验证器(email)，验证字段值是否为正确的邮件格式；</li>
     *      <li>URL验证器(url)，验证字段值是否为正确的URL地址；</li>
     *      <li>IP验证器(ip)，验证字雄值是否为正确的IP地址；</li>
     *  </ul>
     *  </p>验证器与验证器方法之间的映射关系是：xxxxValidator中xxxx表示验证器的名字，例如：requiredValidator验证器名字为required。<p>
     *
     *  <h3>扩展验证器</h3>
     *  <p>开发者也可以根据自己的需要扩充验证器，验证器的方法签名必须为：</p>
     *  <code>
     *      xxxxValidator($input,$field,$options) {
     *           # do something
     *      }
     *  </code></p>
     *  <p>其中xxxx表示验证器的名字。</p>
     *  <b>注意：调用Validation::jquery.validate 之前必须已经自下定义验证器载入当前上下文件运行环境中。</b>
     *
     * @package jtita.kernels.validations
     */
    class Validation {

        /**
         * 对$input字段执行$rules验证，错误消息为$messages，验证成功返回true，失败返回错误数组。
         *
         * <h3>$rules验证规则</h3>
         * <p>在validate方法中支持以下三种规则方式定义：</p>
         * <ul>
         *      <li><b>方式一：直接指定字段验证器</b>：
         *          array('name' => 'required','email' => 'email')</li>
         *      <li><b>方式二：指定多个验证器</b>：
         *          array('name' => array('required' => true),'email' => array('email' => true))</li>
         *      <li><b>方式三：指定具有选项参数的验证器：</b>
         *           array('name' => array('required' => true,'minlen' => 6),
         *                'age' => array('range' => array(18,60)))</li>
         * </ul>
         * <p>前两种方式都是声明对象的name字段为非空字段，email字段为邮件规则字段。</p>
         * <p>第三种方式声明name字段必须为非空字段，并且最小长度大于等于6，age字段的范围为18至60之间。</p>
         * <p>注意：验证规则的执行过程与声明的过程一致，当字段验证失败时，将直接退出字段的验证，因此需要注意验证的顺序，减少不必要的验证操作</p>
         *
         * <h3>$messages错误消息</h3>
         * <p>在validate方法中支持以下两种方式声明字段的错误消息:</p>
         * <ul>
         *      <li><b>方式一：直接指定字段的整体错误消息</b>：
         *          array('name' => '名字不能空,并且大于10')</li>
         *      <li><b>方式二：为每种验证器分别提供错误消息：</b>
         *          array('name' => array('required' => '名字不能为空',
         *                                'minlen'    => '名字的长度必须大于10'))</li>
         * </ul>
         * <p>两种方式的区别在于，在验证字段验证失败时，
         *      <ul>
         *          <li><b>第一种方式</b>：返回统一字段错误消息；</li>
         *          <li><b>第二种方式</b>：则返回引起错误的具体验证器指定的错误消息；</li>
         *      </ul>
         *   在实际使用过程中，可根据需要选择使用方式。
         * </p>
         *
         * <h3>Examples</h3>
         * <code>
         *      Validation::jquery.validate(array('name' => 'zhenggp','sex' => 'm','age' => 18)),
         *                           array('name' => 'required',
         *                                 'sex'  => array('enum' => array('m','w')),
         *                                 'age'  => array('number' => true,'range' => array(16,60))),
         *                           array('name' => '用户名不能为空',
         *                                 'sex'  => '性别只能为W或M',
         *                               # 'age'  => array('number' => '年龄必须为数字','年龄范围必须为16-18周数之间'),
         *                                 'age'  => '年龄必须为数字并且范围必须为16-60周数之间'));
         *
         * </code>
         * <p>指定name字段的验证器为非空验证，sex字段的验证器为枚举验证，age字段的验证器为非空，并且范围为16致60之间。</p>
         * <p>为name和sex指定统一的错误消息，为age统一指定和分别指定不同的错误消息</p>
         *
         * @param ArrayAccess|array $input    验证规则
         * @param array                   $rules    验证规则集合
         * @param array                   $messages 验证错误消息
         * @return array|boolean 返回值为空，表示没有任何错误
         */
        public static function validate ($input, $rules, $messages) {
            $errors = array();

            # 获取验证字段和验证规则；
            foreach ($rules as $field => $validators) {
                # 如果验证器为字符串，表示直接设置为验证规则；
                if (is_string ($validators)) {
                    $validators = array($validators => true);
                }

                # 执行字段验证器；
                foreach ($validators as $validator => $options) {
                    $validatorMethod = self::_fullValidator ($validator);
                    if ($validatorMethod($input, $field, $options) == false) {
                        # 处理验证错误消息；
                        if (isset($messages[$field])) {
                            if (is_string ($messages[$field])) {
                                $errors[$field] = $messages[$field];
                            } else {
                                $errors[$field] = $messages[$field][$validator];
                            }
                        } else {
                            $errors[$field] = "{$validator} validate error";
                        }
                        break;
                    }
                }
            }

            return empty($errors) ? true : $errors;
        }

        /**
         * 返回验证器的完整命名，默认为“验证器"+Validator
         *
         * @param string $validator 验证器名字
         * @return string 完整命名
         */
        protected static function _fullValidator ($validator) {
            return "{$validator}Validator";
        }
    }