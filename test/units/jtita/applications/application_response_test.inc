<?php
    /**
     * application_response_test.inc 实现对应用响应的单元测试服务。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.jtita.com/license.txt
     *
     * @package     jtita.applications
     * @version     1.0 beta
     *
     * @create      2012-09-03
     * @modify      2014-04-07
     */

    # 引入根节点
    defined ('JTPATH_ROOT') or define('JTPATH_ROOT', dirname (dirname (dirname (dirname (dirname (__FILE__))))));

    # 引用待测试类；
    require_once (JTPATH_ROOT . '/jtita/applications/application_response.inc');

    /**
     * 应用响应测试，提供对应用响应的单元测试服务。
     */
    class ApplicationResponseTest extends PHPUnit_Framework_TestCase {
        /**
         * 测试缓存状态；
         */
        public function testCache () {
            $response = new ApplicationResponse();

            $this->assertEquals (false, $response->cache ());
            $this->assertEquals ('max-age=60', $response->cache ('max-age=60'));
            $this->assertEquals ('max-age=60', $response->cache ());
        }

        /**
         * 测试指定缓存数组。
         */
        public function testCacheForArray () {
            $response = new ApplicationResponse();

            $caches = $response->cache (array('If-Modified-Since' => 'Mon, 19 Nov 2012 08:38:01 GMT',
                'If-None-Match' => '"0693f67a67cc1:0"', 'Cache-Control' => 'max-age=0'));

            $this->assertEquals ('Mon, 19 Nov 2012 08:38:01 GMT', $caches['If-Modified-Since']);
            $this->assertEquals ('"0693f67a67cc1:0"', $caches['If-None-Match']);
            $this->assertEquals ('max-age=0', $caches['Cache-Control']);
        }

        /**
         * 测试启动缓存
         */
        public function testEnableCache () {
            $response = new ApplicationResponse();

            $response->enableCache ();
            $caches = $response->cache ();

            $this->assertEquals ('max-age=20', $caches['cache-control']);
            # $this->assertEquals('max-age=20', $caches['date']);
        }

        /**
         * 测试禁止缓存
         *
         * @depends testCache
         */
        public function testDisableCache () {
            $response = new ApplicationResponse();

            $response->disableCache ();
            $caches = $response->cache ();

            $this->assertEquals ('no-cache', $caches['cache-control']);
            $this->assertEquals ('no-cache', $caches['Pragma']);
        }

        /**
         * 测试自定义状态码。
         */
        public function testStatus () {
            $response = new ApplicationResponse();

            $this->assertEquals (false, $response->status ());
            $this->assertEquals (400, $response->status (400));
        }

        /**
         * 测试内容类型
         */
        public function testContentType () {
            $response = new ApplicationResponse();

            $this->assertEquals (false, $response->contentType ());
            $this->assertEquals ('text/javascript', $response->contentType ('text/javascript'));
        }

        /**
         * 测试header。
         */
        public function testHeader () {
            $response = new ApplicationResponse();

            $this->assertTrue (empty($headers));

            $response->header ('custom', '12');
            $response->header ('custom1', 'hello world');

            $this->assertEquals ('12', $response->header ('custom'));
            $this->assertEquals ('hello world', $response->header ('custom1'));
        }

        /**
         * 测试body。
         */
        public function testBody () {
            $response = new ApplicationResponse();

            $this->assertEquals ('', $response->body ());
            $this->assertEquals ('Hello World', $response->body ('Hello World'));
        }


    }
