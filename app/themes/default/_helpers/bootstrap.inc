<?php
    /**
     * bootstrap.php bootstrap视图助手文件，提供声明bootstrap中的特殊视图HTML。
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.jtcore
     * @version     1.0 beta
     *
     * @create      2012-01-27
     * @modify      2013-04-28
     */

    /**
     * 根据$sizes显示分页数量控件。
     *
     * @param array $sizes 大小选项
     * @return string
     */
    function _paginateSize ($sizes = array()) {
        # 初始化默认值
        if (empty($sizes)) {
            $sizes = array(10, 15, 20, 30);
        }
        # 获取当前分页大小
        $request = Application::request ();
        $currSize = $request->params (Controller::PAGINATE_SIZE_PARAM, ModelFinder::DEFAULT_PAGINATE_SIZE);

        $html = '<button class="btn dropdown-toggle" data-toggle="dropdown">' . $currSize . '&nbsp;<span class="caret"></span></button>';
        $html .= '<ul class="dropdown-menu">';
        foreach ($sizes as $size) {
            $url = Router::url (array(Controller::PAGINATE_SIZE_PARAM => $size, '_fullPath' => true));
            if ($size == $currSize) {
                $html .= "<li><a href='$url'>每页&nbsp;{$size}&nbsp;条记录<i class=\"icon icon-ok pull-right\"></i></a></li>";
            }
            else {
                $html .= "<li><a href='$url'>每页&nbsp;{$size}&nbsp;条记录</a></li>";
            }
        }

        return $html . '</ul>';
    }

    /**
     * 根据$finder和$options显示分布导航；
     *
     * @param ModelFinder $finder  模型查找器
     * @param array       $options 选项
     * @return string
     */
    function _paginateNavigator ($finder, $options = array()) {
        $pageParam = Controller::PAGINATE_PAGE_PARAM;
        $pageInfo = $finder->paginate ();

        #  如果没有分页或者分页数量为1
        if ($pageInfo['pageCount'] <= 1) return '';

        $html = '';
        if ($pageInfo['page'] == 1) {
            $html .= '<a href="javascript:void(0);" class="btn"><i class="icon-arrow-left"></i></a>';
        }
        else {
            $html .= _paginateLink ($pageParam, $pageInfo['page'] - 1, '<i class="icon-arrow-left"></i>', 'btn');
        }

        $html .= '<span class="btn">' . $pageInfo['page'] . '/' . $pageInfo['pageCount'] . '</span>';

        if ($pageInfo['page'] == $pageInfo['pageCount']) {
            $html .= '<a href="javascript:void(0);" class="btn"><i class="icon icon-arrow-right"></i></a></li>';
        }
        else {
            $html .= _paginateLink ($pageParam, $pageInfo['page'] + 1, '<i class="icon icon-arrow-right"></i>', 'btn');
        }

        return $html;
    }

    /**
     * 下接过滤器，实现bootstrap的弹出菜单过滤器。
     *
     * @param array  $list         过滤列表
     * @param string $param        参数名称
     * @param string $defaultTitle 默认显示的标题
     * @param array  $addition     附加列表
     * @param array  $options      更多选项
     * @return string
     */
    function _dropFilter ($list, $param, $defaultTitle, $addition = array(), $options = array()) {
        # HTML 标记声明
        $html = '<ul class="dropdown-menu">';
        $curr = Application::request ()->params ($param);
        $className = array_kpop ($options, 'class', 'dropdown-toggle');
        $autoTitle = array_kpop ($options, 'autoTitle', false);

        # 生成选择过滤
        foreach ($list as $id => $title) {
            if ($id == $curr) {
                if ($autoTitle) $defaultTitle = $title;
                $url = Router::url (array($param => null, 'fullUrl' => true, 'page' => null));
                $html .= "<li><a href=\"$url\">$title<i class='icon icon-ok pull-right'></i></a></li>";
            }
            else {
                $url = Router::url (array($param => $id, 'fullUrl' => true, 'page' => null));
                $html .= "<li><a href=\"$url\">$title</a></li>";
            }
        }

        # 处理附加选项
        if (! empty($addition)) {
            $html .= '<li class="divider"></li>';
            foreach ($addition as $url => $title) {
                $html .= "<li><a href=\"$url\">$title</a></li>";
            }
        }

        return '<a href="javascript:void(0);" class="' . $className . '" data-toggle="dropdown">' . $defaultTitle . '&nbsp;<span class="caret"></span></a>' . $html . '</ul>';
    }

    /**
     * 根据$list、$paramName和$more生成过滤信息HTML字符串。
     *
     * @example
     *  <pre>
     *      div class="filter">
     *            <ul>
     *                <li class="curr"><a href="#">全部</a></li>
     *                <li><a href="#">会员</a></li>
     *                <li class="fr"><a href="#">更多</a></li>
     *            </ul>
     *            <span class="cls block"></span>
     *      </div>
     *  </pre>
     * @param array|ArrayAccess  $list       数据列表；
     * @param string             $paramName  参数名字；
     * @param array              $options    更多选项
     * @return string
     */
    function _filter ($list, $paramName, $options = array()) {
        # 获取查询的当前值参数；
        $curr = Application::request ()->params ($paramName);

        # 生成HTML；
        $html = '';
        $choose = false;
        foreach ($list as $id => $title) {
            $url = Router::url (array($paramName => $id, '_fullPath' => true));
            if ($id == $curr) {
                $choose = true;
                $html .= ('<li class="active"><a href="' . $url . '">' . $title . '</a></li>');
            }
            else {
                $html .= ('<li><a href="' . $url . '">' . $title . '</a></li>');
            }
        }

        # 生成全部标志；
        $url = Router::url (array($paramName => null, '_fullPath' => true));
        if ($choose) {
            $html = ("<li><a href=\"$url\">全部</a></li>" . $html);
        }
        else {
            $html = ("<li class=\"active\"><a href=\"url\">全部</a></li>" . $html);
        }

        # 如果指定有更多内容；
        if (! empty($options['more'])) {
            $html .= '<li class="fr">' . $options['more'] . '</li>';
        }

        # 返回完整生成内容；
        return '<div class="filter"><ul class="nav nav-pills">' . $html . '</ul></div>';
    }