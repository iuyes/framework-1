<?php
    /**
     * paginate.inc 提供生成分页的HTML助手方法
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.views
     * @version     1.0 beta
     *
     * @create      2012-11-07
     * @modify      2013-04-28
     */

    /**
     * 根据$pageParam、$page、$title和$clsName生成分页HTML标签
     *
     * @param string $pageParam 分页参数
     * @param int    $page      分页索引
     * @param string $title     显示标题
     * @param string $clsName   HTML类名称
     *
     * @return string
     */
    function _paginateLink ($pageParam, $page, $title, $clsName) {
        $url = Router::url (array($pageParam => $page, 'fullUrl' => true));
        return '<a href="' . $url . '" class="' . $clsName . '">' . $title . '</a>';
    }


    /**
     * 根据$finder和$options生成分页数字标签
     *
     * @param ModelFinder $finder  模型查找器
     *
     * @return string
     */
    function _paginateNumbers ($finder) {
        # 获取分页信息。
        if (! ($pageInfo = $finder->extra ('paginate', false))) return '';
        $params = $pageInfo['params'];

        $html = '<ul>';
        if ($pageInfo['page'] == 1) {
            $html .= '<li><span class="first">&lt;&lt;</span></li>';
            $html .= '<li><span class="prov">&lt;</span></li>';
        }
        else {
            $html .= '<li>' . _paginateLink ($params['page'], 1, '&lt;&lt;', 'first') . '</li>';
            $html .= '<li>' . _paginateLink ($params['page'], $pageInfo['page'] - 1, '&lt;', 'prov') . '</li>';
        }

        for ($i = 1; $i <= $pageInfo['page-count']; $i ++) {
            if ($i == $pageInfo['page']) {
                $html .= '<li class="active"><span>' . $i . '</span></li>';
            }
            else {
                $html .= '<li>' . _paginateLink ($params['page'], $i, $i, 'number') . '</li>';
            }
        }

        if ($pageInfo['page'] >= $pageInfo['page-count']) {
            $html .= '<li><span class="next">&gt;</span></li>';
            $html .= '<li><span class="last">&gt;&gt;</span></li>';
        }
        else {
            $html .= '<li>' . _paginateLink ($params['page'], $pageInfo['page'] + 1, '&gt;', 'next') . '</li>';
            $html .= '<li>' . _paginateLink ($params['page'], $pageInfo['page-count'], '&gt;&gt;', 'last') . '</li>';
        }

        return $html . '</ul>';
    }

    /**
     * 使用$title、$field和$option生成排序连接
     *
     * @param ModelFinder        $finder  模型查看器
     * @param string             $title   标题
     * @param string             $field   排序字段
     * @param array|string       $options 选项
     *
     * @return string
     *
     * @example
     *      <a href="#" class="sort-asc">$title</a>
     */
    function _paginateSortLink ($finder, $title, $field, $options = array()) {
        # 如果指定选项为字符串，则设置为排序方式
        if (is_string ($options)) {
            $options = array('dir' => 'asc');
        }

        # 获取分页信息
        $info = $finder->extra ('paginate');
        $params = $info['params'];

        $cls = "page-dir";

        # 如果已经是当前字段排序
        if ($info['sort'] == $field) {
            if (array_kpop ($options, 'auto', true)) {
                $dir = ($info['dir'] == 'asc' ? 'desc' : 'asc');
            }
            else {
                $dir = array_kpop ($options, 'dir', 'asc');
            }

            $cls .= ' page-dir-' . $dir;
        }
        else {
            $dir = array_kvalue ($options, 'dir', null);
        }

        $url = Router::url (array($params['page'] => 1, $params['sort'] => $field, $params['dir'] => $dir,
            'fullUrl' => true));

        return "<a href='" . $url . "' class='$cls'>$title</a>";
    }

    /**
     * 根据$finder和$options生成分页统计信息
     *
     * @param ModelFinder          $finder  模型查找器
     * @param             string   $format  格式
     *
     * @return string
     */
    function _paginateCounter ($finder, $format = '%%/%') {

    }
