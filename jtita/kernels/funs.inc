<?php
    /**
     * funs.php 声明jtita中使用到的核心函数.
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels
     * @version     1.0 beta
     *
     * @create      2012-08-07
     * @modify      2013-04-26
     */

    /**
     * camel_upper camel大写命名规范，第一个单词首字小写，其它单词首字母大写。
     *
     * @example
     *  <pre>
     *      camel_upper('hello_world');  - helloWorld
     *      camel_upper('HelloWorld');   - helloWorld
     *      camel_upper('_hello_world'); - HelloWorld
     *  </pre>
     *
     * @param $word string 单词组
     *
     * @return string
     */
    function camel_upper ($word) {
        $word = preg_replace ('/^[A-Z]/e', 'strtolower(\'\\0\')', $word);
        $word = preg_replace ('/_[A-Za-z]/e', 'strtoupper(\'\\0\')', $word);
        return str_replace ('_', '', $word);
    }

    /**
     * camel_lower camel小写命名规范，所有单词小写，单词之间以"_"分隔。
     *
     * @example
     *  <pre>
     *      camel_lower('helloWorld');    - hello_world
     *      camel_lower('HelloWorld');    - hello_world
     *  </pre>
     *
     * @param $word 单词组
     *
     * @return string
     */
    function camel_lower ($word) {
        $word = preg_replace ('/^[A-Z]/e', 'strtolower(\'\\0\')', $word);
        return preg_replace ('/[A-Z]/e', '\'_\' . strtolower(\'\\0\')', $word);
    }

    /**
     * pascal 命名规范，所有单词首字母大写。
     *
     * @example
     *  <pre>
     *      pascal('hello_world');   - HelloWorld
     *      pascal('helloWorld');    - HelloWorld
     *  </pre>
     *
     * @param string $word 单词组
     *
     * @return mixed
     */
    function pascal ($word) {
        $word = preg_replace ('/(^[a-z]{1})|(_[a-z]{1})/e', 'strtoupper(\'\\0\')', $word);
        return str_replace ('_', '', $word);
    }

    /**
     * 根据主键调用回调函数$callback过滤数组，回调函数返回true表示继续保存在数组中。
     *
     * <b>$callback函数签名：</b>
     * <ul>
     *      <li>array_kfilter($arr,function($key,$value) {});</li>
     *      <li>array_kfilter($arr,function($key) {});</li>
     * </ul>
     *
     * @param array $array    需要过滤的数组
     * @param mixed $callback 回调函数，返回true表示继续保存
     *
     * @return array 过滤后的数组结果
     */
    function array_kfilter ($array, $callback) {
        $result = array();

        # 执行回调用函数，判断是否过滤；
        foreach ($array as $key => $val){
            if (call_user_func ($callback, $key, $val)){
                $result[$key] = $val;
            }
        }

        return $result;
    }

    /**
     * 根据主键弹出数组元素，如果关键字不存在则返回$def值。
     *
     * @example
     *  <pre>
     *      $arr = array('name' => 'jtita');
     *      array_kpop('no-exists','hello world');   # hello world $arr = ['name' => 'jtita'];
     *      array_kpop('name','hello world');       # jtita $arr = [];
     *  </pre>
     *
     * @param array      $array 数组
     * @param mixed      $key   主键值
     * @param bool|mixed $def   默认值
     *
     * @return mixed
     */
    function array_kpop (&$array, $key, $def = false) {
        # 判断指定键值是否存在，存在则注销键值；
        if (isset($array[$key])){
            $def = $array[$key];
            unset($array[$key]);
        }

        return $def;
    }

    /**
     * 将KEY与VALUE与数组连接在一起形成新的字符串。
     *
     * @example
     *  <pre>
     *      $fields = array('user' => 'USER','name' => 'NAME');
     *      $fields = array_kimplode(' AS ',',',$fields);
     *      #user AS USER,name AS NAME,
     *  </pre>
     *
     * @param string $kglue KEY与VALUE之间的连接符号；
     * @param string $glue  数组元素之间的连接符号；
     * @param array  $array 操作对象；
     *
     * @return string
     */
    function array_kimplode ($kglue, $glue, $array) {
        $karray = array();
        foreach ($array as $key => $value){
            array_push ($karray, $key . $kglue . $value);
        }

        return implode ($glue, $karray);
    }

    /**
     * 使用回调用函数将$input值合并成单一值并返回。
     *
     * <b>回调用函数签名</b>
     * <ul>
     *     <li>array_kreduce($arr,function($key,$val,$initial) {})</li>
     *     <li>array_kreduce($arr,function($key,$val,$initial) {},0)</li>
     *  </ul>
     *
     * @example
     *  <pre>
     *  　　  $fields = array(‘user’ => ‘USER’,’name’ => ‘NAME’);
     *　      $sql = array_kreduce($fields,function($key,$val,$initial) {
     *　          return $initial .= ($key . ‘ AS ‘ . $val . ‘,’);
     *　      },’’);  # 结果 user AS USER,name AS NAME,
     *  </pre>
     *
     * @param array $array    输入数组
     * @param mixed $callback 回调函数；
     * @param mixed $initial  初始值，默认为null
     *
     * @return mixed
     */
    function array_kreduce ($array, $callback, $initial = null) {
        # 执行循环操作；
        foreach ($array as $key => $val){
            $initial = call_user_func_array ($callback, array($key, $val, $initial));
        }

        return $initial;
    }

    /**
     * 获取$arr指定$key的值，如果不存在则返回$def值。
     *
     * @example
     *  <pre>
     *      $query = array(‘name’ => ‘jtita’);
     *      $v = array_kvalue($query,‘name’);        # jtita
     *      $v = array_kvalue($query,’age’);        # false
     *      $v = array_kvalue($query,’age’,20);        # 20
     *  </pre>
     *
     * @param Array      $arr 输入数组
     * @param mixed      $key KEY值
     * @param bool|mixed $def 默认值
     *
     * @return bool|mixed 返回值
     */
    function array_kvalue ($arr, $key, $def = false) {
        return isset($arr[$key]) ? $arr[$key] : $def;
    }

    /**
     * 将$arr数组生成为html属性标签，KEY将生成为属性名称、VALUE生成为属性值。
     *
     * <b>注意:</b>属性值将会自动调用htmlspecialchars进行过滤处理。
     *
     * @example
     *  <pre>
     *      # name="emp_id" class="header"
     *      array_htmlattr(array('name' => 'emp_id','class' => 'header'));
     *  </pre>
     *
     * @param Array $arr 属性值
     *
     * @return string
     */
    function array_htmlattr ($arr) {
        # 如果指定属性不是数组参数，直接返回；
        if (! is_array ($arr)) return $arr;

        # 生成完整参数；
        $html = '';
        foreach ($arr as $name => $value){
            $value = htmlspecialchars ($value);
            $html = "$html $name=\"$value\"";
        }

        return $html;
    }

    /**
     * 根据$arr生成满足js需求的对象文本。
     *
     * @example
     *  <pre>
     *      array_jsobj(array('name' => 'jtita','age' => 1));
     *      # {name:'jtita',age:1}
     *  </pre>
     *
     * @param $arr 生成js object的数组对象。
     *
     * @return string
     */
    function array_js ($arr) {
        if (empty($arr)) return '{}';

        $text = '{';
        foreach ($arr as $k => $v){
            if (is_string ($v)) $text .= ("$k:'$v',"); else $text .= ("$k:$v,");
        }

        return substr ($text, 0, - 1) . '}';
    }

    /**
     * 为指定数组的值附加字符串，如果指定$key不存在则设置附加字符串为数组$key的值。
     *
     * @example
     *  <pre>
     *      $arr = array('name' => 'zhenggp);
     *      array_joinstr($arr,'name',' programmer'); # $arr值为array('name' => 'zhenggp programmer)
     *      array_joinstr($arr,'age','18');  # $arr 值为array('name' => 'zhenggp programmer','age' => '18');
     *  </pre>
     *
     * @param Array  $arr  录入数组
     * @param mixed  $key  关键字
     * @param string $join 附加数组值
     *
     * @return void
     */
    function array_joinstr (&$arr, $key, $join) {
        # 如果附加值为false返回；
        if (empty($join)) return;

        # 如果数组右眼包含指定KEY的值；
        if (isset($arr[$key])){
            $arr[$key] = $arr[$key] . $join;
        } else{
            $arr[$key] = $join;
        }
    }

    /**
     * 根据$express中指定的$arr索引序列生成字符串，在$express中允许指定表达式，数组的层级使用”.”分隔，同时允许执行格式器。
     *
     * <b>表达式格式：</b>{key[.key][,formatter:format_string]}
     *
     * @example
     *      <pre>
     *          array_express (array(array('key' => 'goods')), 'Hello {0.key}');   # Hello goods
     *          array_express (array(array('key' => 'goods')), 'Hello {0.key}');   # Hello goods
     *          array_express (array(array('key' => array('goods'))), 'Hello {0.key.0}');   Hello goods
     *          array_express (array(array('key' => array('key1' => array('goods')))), 'Hello {0.key.key1.0}'); # Hello goods
     *          array_express (array(array('key' => array('key1' => array(array('af' => 'goods'))))), 'Hello {0.key.key1.0.af}');   # Hello goods
     *          array_express (array(array('key' => array('key1' => array(array('af' => array('goods')))))), 'Hello {0.key.key1.0.af.0}'); Hello error 仅仅允许4层
     *      </pre>
     *
     * @param array  $arr     录入字符串
     * @param string $express 语法。
     *
     * @return string
     */
    function array_express ($arr, $express) {
        $syntaxTree = array();
        if (preg_match_all ('/\{([\w|\.|,|:|-]+)\}/', $express, $syntaxTree)){
            foreach ($syntaxTree[1] as $idx => $syntax){
                # Hello Hello {0.key.f2,date:Y-m-d}
                $expr = explode (',', $syntax);
                $keys = explode ('.', $expr[0]);
                # 获取结果值。
                $value = $arr;
                foreach ($keys as $key){
                    $value = $value[$key];
                }
                # 处理格式器
                if (isset($expr[1])){
                    list($formatter, $format) = explode (':', $expr[1]);
                    $value = $formatter($format, $value);
                }

                $express = str_replace ($syntaxTree[0][$idx], $value, $express);
            }
        }

        return $express;
    }

    /**
     * 将$array数组转换为声明字符串。
     *
     * @param array $array  数组参数
     * @param int   $level  层次
     * @return string
     */
    function array_declare ($array, $level = 0) {
        # 如果数组为空，直接返回空值
        if (empty($array)) return 'array()';

        $declare = "array(\r\n";
        foreach ($array as $key => $value) {
            if (is_string ($key)) $key = ("'" . addcslashes ($key,  "'") . "'");
            if (is_string ($value)) {
                $value = ("'" . addcslashes ($value, "'") . "'");
            }
            else if (is_array ($value)) {
                $value = array_declare ($value, $level + 1);
            }

            $declare .= (str_repeat ("\t", $level + 1) . "$key => $value,\r\n");
        }

        return (substr ($declare, 0, - 3) . "\r\n") . str_repeat ("\t", $level) . ")";
    }

    /**
     * 根据$path读取其中的文件名并返回。
     *
     * @param string $path 路径
     * @param string $ext  扩展名
     *
     * @return array
     */
    function file_names ($path, $ext = '*') {
        $fileNames = array();
        if (false != ($handle = opendir ($path))){
            while (false !== ($file = readdir ($handle))) {
                if ($file != "." && $file != ".." && strpos ($file, ".") && ($ext == '*' || (substr ($file, strrpos ($file, '.') + 1) == $ext))){
                    $fileNames[$file] = $file;
                }
            }
            closedir ($handle);
        }

        return $fileNames;
    }

    /**
     * URL客户端跳转，重置客户端访问到新的$url地址。
     *
     * @param string  $url   跳转位置；
     * @param int     $delay 延迟；
     * @param boolean $exit  退出标记；
     *
     * @return void
     */
    function redirect ($url, $delay = 0, $exit = true) {
        $delay = intval ($delay);
        if ($delay > 0){
            echo <<<EOT
<html>
<head>
<meta http-equiv="refresh" content="{$delay};URL={$url}" />
</head>
</html>
EOT;
        } else{
            header ("Location: {$url}");
        }

        # 如果退出标志为true，立即中止执行；
        if ($exit) exit;
    }