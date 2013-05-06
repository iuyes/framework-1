<?php
    /**
     * notify.inc 声明通知服务接口对象Notify;
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.notifies
     * @version     1.0 beta
     *
     * @create      2013-04-05
     * @modify      2013-04-28
     */

    /**
     * 定义日志服务开启标志
     */
    define('JTITA_NOTIFY', true);

    /**
     * 日志记录异常，记录日志异常信息
     *
     * @package jtita.kernels.notifies
     */
    class NotifyException extends JtitaException{
    }

    # 初始化
    if (! Notify::initialized ()){
        Notify::initialize ();
    }

    /**
     * 通知投送接口，提供将通知发送到具体位置的服务。
     *
     * @package jtita.kernels.notifies
     */
    interface NotifyDelivery{
        /**
         * 将指定的$message发送给$receiver。
         *
         * @param mixed $receiver 接收者
         * @param mixed $message  发送消息
         *
         * @return mixed
         */
        function deliver ($receiver, $message);
    }

    /**
     * 通知服务接口对象Notify，提供将消息发送给指定接收者的服务。
     *
     * @example
     *  <pre>
     *      Jtita::write('kernels/notify',array(
     *          'default' => array('class' => 'kernels.notifies.deliveries.email_notify_delivery',
     *                             'smtp'  => 'xxxx.xxx'));
     *
     *  </pre>
     *
     * @package jtita.kernels.notifies
     * @abstract
     */
    abstract class Notify{
        /**
         * @const 配置节点
         */
        const CFG_NOTIFY = 'kernels/notifies';

        /**
         * 初始化服务。
         */
        public static function initialize () {
            if (self::initialized ()){
                trigger_error ('JTK0701:Notify repeat initialize');
            }

            $config = Jtita::read (self::CFG_NOTIFY, array());
            if (empty($config)){
                $config = array('default' => array('class' => 'kernels.notifies.deliveries.email_notify_delivery'));
            }

            self::$_config = $config;
            self::$_initialized = true;

            return true;
        }

        /**
         * 使用$delivery的方式将$message发送给$receiver。
         *
         * @param mixed  $receiver 消息接收者
         * @param mixed  $message  消息
         * @param string $delivery 投送方式
         *
         * @return mixed
         */
        public static function deliver ($receiver, $message, $delivery = 'default') {
            # 如果为调试模式。
            if (defined ('JTITA_DEBUGGER')){
                Debugger::write ('notify', array('method' => 'deliver', '$receiver' => $receiver, 'message' => $message,
                                                 'delivery' => $delivery));
            }

            $notifyDelivery = self::_notifyDelivery ($delivery);
            $notifyDelivery->deliver ($receiver, $message);
        }

        /**
         * 返回一个布尔值，表示当前通知是否已完成初始化。
         */
        public static function initialized () {
            return self::$_initialized;
        }

        /**
         * 根据$delivery获取消息投送器。
         *
         * @param string $delivery 投送器名字
         *
         * @return NotifyDelivery
         */
        protected static function _notifyDelivery ($delivery) {
            # 如果还没有创建具体的投送器
            if (! isset(self::$_deliveries[$delivery])){
                self::$_deliveries[$delivery] = self::_createDelivery ($delivery);
            }

            return self::$_deliveries[$delivery];
        }

        /**
         * 根据$delivery创建消息投送器。
         *
         * @param string $delivery 投送器名称
         *
         * @return boolean|NotifyDelivery
         *
         * @throws NotifyException 如果指定的$delivery不存在。
         */
        protected static function _createDelivery ($delivery) {
            if (! isset(self::$_config[$delivery])){
                throw new NotifyException('JTK0701:Notify delivery not support:' . $delivery);
            }

            return Jtita::ioc (self::$_config[$delivery]);
        }

        /**
         * @var bool 判断是否已完成初始化
         */
        protected static $_initialized = false;

        /**
         * @var array 配置信息
         */
        protected static $_config = array();

        /**
         * @var array 投放器。
         */
        protected static $_deliveries = array();
    }




