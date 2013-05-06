<?php
    /**
     * security.inc 提供架构安全服务。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.securities
     * @version     1.0 beta
     *
     * @create      2012-03-27
     * @modify      2013-04-28
     */

    /**
     * @const 开启安全服务标志。
     */
    define('JTITA_SECURITY', true);

    /**
     * 声明安全验证编码。
     */
    defined ('JTITA_SECURITY_SALT') or define('JTITA_SECURITY_SALT', '0famdjay1mdh1dmah2ff');

    /**
     * 安全异常，当安全检查发生错误时将引发本异常。
     *
     * @package jtita.kernels.securites
     */
    class SecurityException extends JtitaException {
    }

    /**
     * 安全验证接口，提供用户的身份验证服务。
     *
     * @package jtita.kernels.securites
     */
    interface Authentication {
        /**
         * 根据$identity验证用户身份，验证成功返回用户标识信息，失败返回false。
         *
         * @param mixed $identity 用户身份。
         * @return mixed
         */
        function authenticate ($identity);
    }

    /**
     * 安全授权接口，提供对标识用户的授权服务。
     *
     * @package jtita.kernels.securites
     *
     */
    interface Authorization {
        /**
         * 返回$sros指定的安全授权信息。
         *
         * @param string|array $sros
         * @return array
         */
        function authorize ($sros);
    }

    /**
     * jtita安全组件服务接口对象，提供安全的检查服务。
     *
     * @package jtita.kernles.securites
     */
    class Security {
        /**
         * 根据$pass和$salt返回生成的密码。
         *
         * @param string $pass 密码
         * @param string $salt 安全码
         */
        public static function encrypt ($pass, $salt = JTITA_SECURITY_SALT) {

        }

        /**
         * 根据$authenticate返回当前用户标识。
         *
         * @param string $authenticate 验证名称
         * @return mixed
         */
        public static function identity ($authenticate = 'default') {

        }

        /**
         * 根据$identity和$authenticateName 返回客户是否安全验证通过，成功返回true，失败返回false。
         *
         * @param string $identity   客户身份标识信息
         * @param string $authName   安全验证名称
         * @return boolean
         */
        public static function authenticate ($identity, $authName = 'default') {

        }

        /**
         * 根据$sros和$authorName返回安全授权的结果。
         *
         * @param string|array $sros       安全资源对象集。
         * @param string       $authorName 安全授权名称。
         */
        public static function authorizes ($sros, $authorName = 'default') {

        }
    }