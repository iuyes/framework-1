<?php
    /**
     * file_cache_engine.inc 文件缓存引擎声明文件。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.kernels.caches
     * @version     1.0 beta
     *
     * @create      2012-07-16
     * @modify      2013-04-28
     */

    /**
     * 文件缓存引擎，通过文件系统缓存信息。
     *
     * @package jtita.kernels.caches.engines
     */
    class FileCacheEngine implements CacheEngine{
        /**
         * @const 文件扩展名
         */
        const FILE_EXT = '.jmp';

        /**
         * @var string 缓存文件前缀
         */
        public $prefix = 'cache-file-';

        /**
         * @var string 缓存目录
         */
        public $dir = JTITA_CACHE_DIR;

        /**
         * @see CacheEngine::write
         */
        public function write ($key, $value, $expire) {
            file_put_contents ($this->_cacheFile ($key), $this->_serialize ($value, time () + $expire));
        }

        /**
         * @see CacheEngine::read
         */
        public function read ($key) {
            $cacheFile = $this->_cacheFile ($key);

            # 如果缓存文件不存在，直接返回false。
            if (! file_exists ($cacheFile)) return false;
            if ($content = file_get_contents ($cacheFile)){
                list($expire, $value) = $this->_unserialize ($content);

                if (time () <= $expire) return $value;
                else unlink ($cacheFile);
            }

            return false;
        }


        /**
         * @see clear::delete
         */
        public function clear ($key) {
            $cacheFile = $this->_cacheFile ($key);

            # 如果缓存文件存在，则删除文件。
            if (file_exists ($cacheFile)){
                unlink ($cacheFile);
            }
        }

        /**
         * @see flush::clear
         */
        public function flush () {
            if ($handle = opendir ($this->dir)){
                $cacheFile = '/^' . $this->prefix . '\w+' . self::FILE_EXT . '$/';

                while (false !== ($file = readdir ($handle))) {
                    if ($file != '.' && $file != '..'){
                        if (preg_match ($cacheFile, $file)){
                            unlink ($file);
                        }
                    }
                }

                closedir ($handle);
            }
        }

        /**
         * 根据$key生成缓存文件名称
         *
         * @param string $key 关键字
         *
         * @return string
         */
        protected function _cacheFile ($key) {
            return $this->dir . DIRECTORY_SEPARATOR . $this->prefix . $key . self::FILE_EXT;
        }

        /**
         * 根据$value和$expire生成缓存内容。
         *
         * @param mixed $value  缓存值
         * @param int   $expire 过期时间（秒）
         *
         * @return string
         */
        protected function _serialize ($value, $expire) {
            return serialize (array($expire, $value));
        }

        /**
         * 根据$content 反向生成内容。
         *
         * @param string $content
         *
         * @return array
         */
        protected function _unserialize ($content) {
            return unserialize ($content);
        }
    }