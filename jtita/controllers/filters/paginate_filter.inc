<?php
    /**
     * paginate_filter.inc 提供PaginateFilter类的声明。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.controllers.filters
     * @version     1.0 beta
     *
     * @create      2013-04-08
     * @modify      2013-04-28
     */

    /**
     * 分页过滤器，提供控制器的分页支持。
     *
     * @package jtita.controllers.filters
     */
    class PaginateFilter implements Filter {
        /**
         * @var array 参数信息。
         */
        public $params = array('page' => 'page', 'size' => 'page_size', 'sort' => 'page_sort', 'dir' => 'asc');

        /**
         * @var int 分页大小。
         */
        public $size = 10;

        /**
         * @var string 排序方式。
         */
        public $dir = 'asc';

        /**
         * @var bool|string 排序方式。
         */
        public $sort = false;

        /**
         * @var ModelFinder|bool 查询对象。
         */
        public $finder = false;

        /**
         * 执行execute前过滤。
         *
         * @implement Filter::execute
         */
        public function execute ($controller, $actionMethod) {
            return true;
        }

        /**
         * 执行render前过滤。
         *
         * @implement Filter::render
         */
        public function render ($controller, &$options) {
            # 如果没有指定模型查找器。
            if ($this->finder && $controller->hasVar($this->finder)) {
                $finder = $controller->{$this->finder};
                $count = $finder->count ();
                $size = $controller->query ($this->params['size'], $this->size);
                $page = $controller->query ($this->params['page'], 1);

                # 计算分页数量
                $pageCount = ceil ($count / $size);

                # 除错处理
                if ($page > $pageCount) $page = $pageCount;
                if ($page < 1) $page = 1;

                # 获取排序方式。
                $dir = $controller->query ($this->params['dir'], $this->dir);
                # 获取排序字段。
                if ($sort = $controller->query ($this->params['sort'], $this->sort)) {
                    $finder->orderBy (array($sort => $dir));
                }

                # 指定分页大小。
                $finder->limit (array(($page - 1) * $size, $size));
                $finder->addExtra ('paginate', array('size' => $size,
                                                     'page' => $page,
                                                     'sort' => $sort,
                                                     'params' => $this->params,
                                                     'page-count' => $pageCount,
                                                     'dir' => $dir));
            }

            return true;
        }
    }