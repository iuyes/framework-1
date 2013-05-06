<?php
    /**
     * jtita.inc 提供架构核心服务Jtita类声明。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team <jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels
     * @version     1.0 beta
     *
     * @create      2012-06-12
     * @modify      2013-04-26
     */

    /*
     * @const boolean 如果没有定义调试，自动设置为调试环境。
     */
    defined ('JTDEBUG') or define('JTDEBUG', true);

    /**
     * Jtita 架构基本异常，所有的Jtita架构异常都应当继承自本异常。
     *
     * @package jtita.kernels
     */
    class JtitaException extends Exception{
    }

    /**
     * 文件未找到异常，导入文件不存在时引发本异常。
     *
     * @package jtita.kernels
     */
    class FileNotFoundException extends JtitaException{
    }

    /**
     * 类未找到异常，创建对象时类未找到将引发的本异常。
     *
     * @package jtita.kernels
     */
    class ClassNotFoundException extends JtitaException{
    }

    /**
     * 方法未找到异常，类中的指定方法未找到将引发本异常
     *
     * @package jtita.kernels
     */
    class MethodNotFoundException extends JtitaException{
    }

    /**
     * 字段未找到异常，类中的指定字段未找到将引发本异常
     *
     * @package jtita.kernels
     */
    class FieldNotFoundException extends JtitaException{
    }

    /**
     * Jtita 提供架构的核心服务(配置、文件导入、对象创建、IOC)
     *
     * @package jtita.kernels
     * @abstract
     */
    abstract class Jtita{
        /**
         * @const string 默认文件扩展名。
         */
        const DEFAULT_FILE_EXT = '.inc';

        /**
         * @const string 导入包分隔符。
         */
        const PACKAGE_SEPARATOR = '.';

        /**
         * @const string 导入类分隔符。
         */
        const CLASS_SEPARATOR = '#';

        /**
         * 设置或获取文件导入路径，如果$paths不为空，则表示获取之前先设置路径；
         *
         * <b>注意：路径后面不要包含文件夹分隔符。</b>
         * <ul>
         *     <li>正确：e:/jtita、/var/www/site/jtita。</li>
         *     <li>错误：e:/jtita/、/var/www/site/jtita/。</li>
         * </ul>
         *
         * @see   Jtita::lookup()
         *
         * @example
         *      <pre>
         *        Jtita::paths(array(JTPATH_RTIME,JTPATH_APP,JTPATH_JTITA,JTPATH_VENDOR));
         *        Jtita::paths(APP_PATH);
         *        Jtita::paths();
         *      </pre>
         *
         * @param  array|string $paths 设置导入路径，null值表示不设置。
         *
         * @return array
         */
        public static function paths ($paths = null) {
            # 非空值，设置路径。
            if (! is_null ($paths)){
                if (is_string ($paths)){
                    $paths = array($paths);
                }

                self::$_paths = $paths;
            }

            return self::$_paths;
        }

        /**
         * 从文件导入路径中查找$fileName指定的文件，成功并且返回完整路径，未找到文件则返回false。
         *
         * <b>Jtita 优先文件</b>
         * <p>在Jtita中所有导入的文件都是通过Jtita::lookup方法进行定位的，而在Lookup内部会根据
         *    Jtita::paths()设置的路径集依次进行查找，在查找到对应的文件时就会中止查找，返回查找
         *    匹配后的完整路径，这就实现了优先文件功能，例如在jtita和app目录下都有文件a.php，目录
         *    结构如下：
         *    <ul>
         *          <li>/var/www/site/app/a.php。</li>
         *          <li>/var/www/site/jtita/a.php。</li>
         *    </ul>
         *    设置设置Jtita::paths(array('/var/www/site/app','/var/www/site/jtita'))后，在
         *    执行Jtita::lookup('a')时，将优先查找到/var/www/site/app目录下的a.php文件。
         *  </p>
         *  <p>如果希望禁止优先文件功能，可以在lookup时指定$paths，文件夹将中$paths指定的路径中查找。</p>
         *
         * @see   Jtita::paths()
         *
         * @example
         *  <pre>
         *        Jtita::paths(JTPATH_JTITA);
         *        # 实际查找结果为JTPATH_JTITA/kernels/caches/cache_engine.inc。
         *        Jtita::lookup('kernels.caches.cache_engine');
         *  </pre>
         *
         * @param string       $file    文件名，如果包含在包之中使用'.'代替目录分隔符，不包含扩展名。
         * @param string       $ext     文件扩展名，默认为.inc；
         * @param string|array $paths   文件查找路径，默认为Jtita导入路径；
         *
         * @return string|boolean
         */
        public static function lookup ($file, $ext = '.inc', $paths = null) {
            # kernels.caches.cache_engine -> kernels/caches/cache_engine.php
            $file = str_replace (self::PACKAGE_SEPARATOR, DIRECTORY_SEPARATOR, $file) . $ext;

            # 设置查找路径；
            if (is_null ($paths)) $paths = self::$_paths; else if (is_string ($paths)) $paths = array($paths);

            # 从路径中查找文件。
            foreach ($paths as $path){
                # 合并生成完整路径
                $fullFile = $path . DIRECTORY_SEPARATOR . $file;
                # 如果文件存在，则返回文件路径；
                if (file_exists ($fullFile)) return $fullFile;
            }

            return false;
        }

        /**
         * 将$fileName指定的文件导入当前运行环境中，如果文件已导入将返回false。
         *
         * 注意：导入时只以文件名和文件扩展名为识别标准，不以路径作用识别标准。
         * <pre>
         *     Jtita::import('kernels.caches.cache_engine','.php','e:/jtita');
         *     # return false; kernels.caches.cache_engine 已加载；
         *     Jtita::import('kernels.caches.cache_engine','.php','e:/app');
         * </pre>
         *
         * @see   Jtita::lookup()
         *
         * @example
         *  <pre>
         *        Jtita::import('kernels.caches.cache_engine');
         *        Jtita::import('kernels.caches.cache_engine','.php');
         *        Jtita::import('kernels.caches.cache_engine','.php',JTPATH_APP);
         *  </pre>
         *
         * @param string       $fileName 文件名
         * @param string       $ext      文件扩展名
         * @param string|array $paths    查找路径，默认为注册路径
         *
         * @return mixed 导入成功返回1或文件的return值，重复导入返回false
         *
         * @throws FileNotFoundException 如果导入文件在路径中未找到将引发异常。
         */
        public static function import ($fileName, $ext = '.inc', $paths = null) {
            # 生成文件KEY
            $fileKey = $fileName . $ext;

            # 判断是否已导入，如果未导入查找文件并导入。
            if (isset(self::$_imports[$fileKey])) return false;
            # 查找文件，并导入文件。
            if ($fullFile = self::lookup ($fileName, $ext, $paths)){
                self::$_imports[$fileKey] = $fullFile;
                # 返回include值，默认为1。
                return include($fullFile);
            }

            # 如果文件未找到，引发FileNotFoundException
            throw new FileNotFoundException('JTK-0101:file not found - ' . $fileName . ' in import.');
        }

        /**
         * 根据$className、$ext和$paths导入接口声明文件，如果接口已声明将不进行任何导入。
         *
         * @see self::import
         *
         * @param string        $className 类名的完整名称，可以使用类名分隔符“#”。
         * @param string        $ext       文件扩展名。
         * @param array|string  $paths     指定路径。
         *
         * @return string       返回导入类的名字
         */
        public static function importInterface ($className, $ext = '.inc', $paths = null) {
            # 从类中分隔出文件名和类名
            list($fileName, $className) = self::_split ($className);

            # 判断指定类是否存在，不存在则导入指定的文件。
            if (! class_exists ($className) && ! interface_exists ($className)){
                self::import ($fileName, $ext, $paths);
            }

            return $className;
        }

        /**
         * 写入运行时配置信息，如果已有$name已存在，则覆盖。
         *
         * @param string|array $name  配置名字
         * @param mixed        $value 写入值
         *
         * @return void
         */
        public static function write ($name, $value = null) {
            self::$_cfgs[$name] = $value;
        }

        /**
         * 从$paths中导入配置文件$file，并写入$name配置中，如果已存在相同的$name将覆盖。
         *
         * @example
         *  <pre>
         *      在$file文件中，必须返回配置的内容，例如：setting.inc
         *        return array('title' => 'Hello world','ok');
         *
         *      Jtita::writeForFile('global','setting');
         *      Jtita::read('global')   # array('title' => 'Hello world','ok');
         *  </pre>
         *
         * @param              $name  string 配置名称。
         * @param              $file  string 文件名，可以包含路径值。
         * @param              $ext   string 文件扩展名，默认为.inc。
         * @param array|string $paths 文件路径，默认为Jtita全局路径。
         *
         * @throws FileNotFoundException 如果指定文件不存在，将引发本异常。
         */
        public static function writeFormFile ($name, $file, $ext = '.inc', $paths = null) {
            # 如果从路径中找到文件。
            if ($fullFile = self::lookup ($file, $ext, $paths)){
                self::write ($name, include($fullFile));
            } else{
                echo $fullFile;
                throw new FileNotFoundException('JTK0102:file not found - ' . $file . ' in write for file.');
            }
        }

        /**
         * 读取运行时配置节点值，如果值不存在返回$default
         *
         * @see Jtita::write()
         *
         * @param string $name    配置名字
         * @param mixed  $default 默认值
         *
         * @return array|bool|mixed 配置值
         */
        public static function read ($name, $default = false) {
            return isset(self::$_cfgs[$name]) ? self::$_cfgs[$name] : $default;
        }

        /**
         * 删除$name指定的配置节点值，如果$name为数组表示删除多个值。
         *
         * @param mixed $name 配置名称；
         *
         * @return void
         */
        public static function delete ($name) {
            unset(self::$_cfgs[$name]);
        }

        /**
         * 清除所有运行时配置值
         *
         * @return void
         */
        public static function clear () {
            self::$_cfgs = array();
        }

        /**
         * 注册$className类的$method方法的动态行为。
         *
         * @example
         *  <pre>
         *      # 为order_service类的submit方法增加动态行为submit_behavior；
         *      Jtita::behaviors('orders.services.order_service','submit','kernels.transactions.submit_behavior');
         *      Jtita::behaviors('orders.services.order_service','submit','kernels.debuggers.debug_behavior');
         *  </pre>
         *
         * @param string $className  类名称
         * @param string $method     方法名称
         * @param mixed  $behavior   行为IOC配置
         *
         * @return void
         */
        public static function behaviors ($className, $method, $behavior) {
            # 如果指定类从未注册任何行为。
            if (! isset(self::$_behaviors[$className])){
                self::$_behaviors[$className] = array();
            }

            # 如果指定方法从未注册任何行为。
            if (! isset(self::$_behaviors[$className][$method])){
                self::$_behaviors[$className][$method] = array();
            }

            self::$_behaviors[$className][$method][] = $behavior;
        }

        /**
         * 注销$className类$method方法的动态行为。
         *
         * @see Jtita::behaviors()
         *
         * @example
         *      #注销order_service全部动态行为；
         *      Jtita::unBehavior('orders.services.order_service');
         *
         *      # 注销order_service类的submit方法的全部动态行为；
         *      Jtita::unBehavior('orders.services.order_service','submit');
         *
         * @param string     $className   类名称
         * @param string     $method      方法名称
         *
         * @return void
         */
        public static function unBehaviors ($className, $method = null) {
            # 判断是否为注销类的全部行为。
            if (is_null ($method)){
                unset(self::$_behaviors[$className]);
            } else{
                unset(self::$_behaviors[$className][$method]);
                # 如果指定类的参数为空
                if (empty(self::$_behaviors[$className])){
                    unset(self::$_behaviors[$className]);
                }
            }
        }

        /**
         * 返回一个布尔值，注册有动态行为返回true，否则返回false
         *
         * @example
         *      # 判断order_service是否注册有动态行为；
         *      Jtita::hasBehavior('orders.services.order_service');
         *
         *      # 判断order_service是否注册有submit方法的动态行为；
         *      Jtita::hasBehavior('orders.services.order_service','submit');
         *
         * @param string $className 类名称
         * @param string $method    方法名称
         *
         * @return boolean
         */
        public static function hasBehaviors ($className, $method = null) {
            return is_null ($method) ? isset(self::$_behaviors[$className]) : isset(self::$_behaviors[$className][$method]);
        }

        /**
         * 清除全部动态行为
         */
        public static function clsBehaviors () {
            self::$_behaviors = array();
        }

        /**
         * 使用$ctors作为构造参数创建$clsName指定的类实例。
         *
         * <b>注意：使用构造参数创建的类只允许有一个参数</b>
         * <pre>
         *      class A { function __contruct($options) {} }
         *      class B { function __contruct($p1,$p2) {} }
         *
         *      Jtita::create('a',array('id' => 'xxx','name' => 'xxx')); 调用正确；
         *      Jtita::create('b',array('xxx','xxx')); # 调用错误，类B有两个方法。
         *  </pre>
         *
         * @see Jtita::import()
         * @see Jtita::behaviors()
         *
         * @example
         *      Jtita::create('kernels.caches.engines.memcached');
         *      Jtita::create('kernels.caches.engines.memcached#memcached');
         *
         * @param string $className 类名，满足Jtita需求的类名。
         * @param mixed  $ctors     构造参数。
         *
         * @return mixed 类实例
         *
         * @throws FileNotFoundException 指定类文件未找到
         * @throws ClassNotFoundException 指定类位找到
         */
        public static function create ($className, $ctors = array()) {
            # 导入类接口文件。
            $classOnly = self::importInterface ($className);

            # 如果还是未找到类的定义；
            if (! class_exists ($classOnly)){
                throw new ClassNotFoundException('JTK0103: class not found - ' . $classOnly);
            }

            # 创建对象并且附加行为；
            $instance = new $classOnly($ctors);
            if (self::hasBehaviors ($className)){
                $instance = self::_createObjectWrapper ($className, $instance);
            }

            return $instance;
        }

        /**
         * 使用数组$cfg 创建和反转对象实例，$cfg允许为字符串或数组。
         *
         * <ul>
         *      <li>字符串：表示直接创建实例，不进行任何初始化。</li>
         *      <li>数组：由数组class key指定类名称，ctors指定初始参数、initialize指定调用initialize()方法，其它参数初始化字段属性</li>
         * </ul>
         *
         * @see     self::create
         *
         * @example
         *      <pre>
         *          # 使用属性初始化，$cfg中除了class、ctors和initialize之外的key、value值都会自动设置为字段属性。
         *          Jtita::ioc(array('class'  => 'kernels.datasources.databases.mysql_fast',
         *                           'server' => '127.0.0.1',
         *                           'dbname' => 'jtita',
         *                           'user'   => 'root',
         *                           'pass'   => '123321'));
         *          # 使用构造器初始化
         *          Jtita::ioc(array('class' => 'kernels.datasources.databases.mysql_fast',
         *                           'initialize' => true,      # 调用initialize方法。
         *                           'ctors' => array('server' => '127.0.0.1',
         *                                            'dbname' => 'jtita',
         *                                            'user'   => 'root',
         *                                            'pass'   => '123321')));
         *          # 使用类名，不带任何参数
         *          Jtita::ioc('kernels.datasource.databases.mysql_fast');
         *      </pre>
         *
         * @param array|string $cfg 类的ioc配设置信息；
         *
         * @return mixed
         *
         * @throws JtitaException 如果配置未提供类名。
         * @throws MethodNotFoundException 指定类的initialize方法未找到。
         */
        public static function ioc ($cfg) {
            # 如果指定$cfg为字符串。
            if (is_string ($cfg)){
                return self::create ($cfg);
            } # 判断是否指定配设类名。
            else if (! isset($cfg['class'])){
                throw new JtitaException('JTK0104:ioc cfg not support class.');
            }

            # 获取类名和构造参数。
            $class = array_kpop ($cfg, 'class');
            $ctors = array_kpop ($cfg, 'ctors', array());
            $initial = array_kpop ($cfg, 'initialize');

            # 创建具体实例，并设置属性值。
            $obj = self::create ($class, $ctors);
            foreach ($cfg as $name => $value){
                $obj->{$name} = $value;
            }

            # 初始化回调用。
            if ($initial && method_exists ($obj, 'initialize')){
                $obj->initialize ();
            } else if ($initial){
                throw new MethodNotFoundException('JTK0105:ioc cfg class ' . $class . ' not support initialize method.');
            }

            # 返回创建对象。
            return $obj;
        }

        /**
         * 返回$clsName指定类的唯一实例
         *
         * 当$clsName指定的类实例第一次创建后，后面访问instance方法将返回每一次创建的对象。
         *
         * @see Jtita::create()
         *
         * @param string $className 类名；
         * @param mixed  $ctors   构造参数；
         *
         * @return mixed
         */
        public static function instance ($className, $ctors = array()) {
            # 如果指定类对象还未创建；
            if (! isset(self::$_instances[$className])){
                self::$_instances[$className] = self::create ($className, $ctors);
            }

            return self::$_instances[$className];
        }

        /**
         * 获取或设置对象默认行为包装器，如果$wrapper是非null值表示先设置。
         *
         * @param mixed $wrapper 默认包装器类名字
         *
         * @return mixed
         */
        public static function defaultObjectWrapper ($wrapper = null) {
            # 如果包含设置分发器
            if (! is_null ($wrapper)){
                self::$_defaultObjectWrapper = $wrapper;
            }

            return self::$_defaultObjectWrapper;
        }

        /**
         * 返回当前Jtita的版本号。
         *
         * @return string
         */
        public static function version () {
            return '1.0 beta';
        }

        /**
         * 根据完整的$className拆分出文件名和类名部分。
         *
         * @param string $className 完整类名声明
         *
         * @return array(fileName,className);
         *
         * @example
         *      Jtita::_split('routers.rewriters.regex_rewriter#default_rewriter');
         *      return array('routers.rewriters.regex_rewriter','regex_rewriter');
         *
         *      Jtita::_split('routers.rewriters.regex_rewriter');
         *      return array('routers.rewriters.regex_rewriter','regex_rewrite');
         */
        private static function _split ($className) {
            # 初始化文件名称
            $fileName = $className;

            # 如果：routers.rewriters.regex_rewriter#default_rewriter
            if (strpos ($className, self::CLASS_SEPARATOR)){
                $fileName = substr ($className, 0, strpos ($className, self::CLASS_SEPARATOR));
                $className = substr (strrchr ($className, self::CLASS_SEPARATOR), 1);
            } # 如果：routers.rewriters.regex_rewriter
            else if (strpos ($className, self::PACKAGE_SEPARATOR)){
                $className = substr (strrchr ($className, self::PACKAGE_SEPARATOR), 1);
            }

            return array($fileName, pascal ($className));
        }

        /**
         * 根据$className和$target创建对象包装器，封装对象行为调用方法。
         *
         * @param string $className 类名字
         * @param mixed  $target    类实例
         *
         * @return mixed  分发器
         */
        private static function _createObjectWrapper ($className, $target) {
            # 判断是否包含自定义注册行为分发器；
            $wrapper = isset(self::$_behaviors[$className]['wrapper']) ?
                             self::$_behaviors[$className]['wrapper'][0] :
                             self::$_defaultObjectWrapper;

            # 创建分发器对象。
            $objWrapper = self::ioc ($wrapper);
            $objWrapper->target = $target;
            $objWrapper->behaviors (self::$_behaviors[$className]);

            return $objWrapper;
        }

        /**
         * @var array 文件导入路径变量，记录查找文件时的全局路径；
         */
        private static $_paths = array();

        /**
         * @var array 已导入文件变量，记录已经导入的文件路径
         */
        private static $_imports = array();

        /**
         * @var array 配置变量，记录运行时配置信息。
         */
        private static $_cfgs = array();

        /**
         * @var array 行业配置信息。
         */
        private static $_behaviors = array();

        /**
         * @var array 唯一实例变量参数。
         */
        private static $_instances = array();

        /**
         * @var string 默认行为分发器
         */
        private static $_defaultObjectWrapper = 'ObjectWrapper';
    }