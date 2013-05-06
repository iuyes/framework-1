<?php
    /**
     * filter.inc 提供生成过滤HTML助手方法
     *
     * @copyright   2012-2013 Jtita soft foundation
     *              (http://www.jtita.com)
     * @author      Jtita team<jtita@outlook.com>
     * @license     http://www.apache.org/licenses/LICENSE-2.0.html
     *
     * @package     jtita.views
     * @version     1.0 beta
     *
     * @create      2012-10-31
     * @modify      2013-04-28
     */

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
            $url = Router::url (array(
                                     $paramName => $id,
                                     '_fullPath' => true
                                ));
            if ($id == $curr) {
                $choose = true;
                $html .= ('<li class="curr"><a href="' . $url . '">' . $title . '</a></li>');
            }
            else {
                $html .= ('<li><a href="' . $url . '">' . $title . '</a></li>');
            }
        }

        # 生成全部标志；
        $url = Router::url (array(
                                 $paramName => null,
                                 '_fullPath' => true
                            ));
        if ($choose) {
            $html = ("<li><a href=\"$url\">全部</a></li>" . $html);
        }
        else {
            $html = ("<li class=\"curr\"><a href=\"url\">全部</a></li>" . $html);
        }

        # 如果指定有更多内容；
        if (! empty($options['more'])) {
            $html .= '<li class="fr">' . $options['more'] . '</li>';
        }

        # 返回完整生成内容；
        return '<div class="filter"><ul>' . $html . '</ul></div>';
    }


    /**
     * _filter 方法的直接打版
     *
     * @see _filter
     */
    function __filter ($list, $paramName, $options = array()) {
        echo _filter ($list, $paramName, $options);
    }
