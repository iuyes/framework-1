/**
 * jquery.jtcore.js 提供jtita - core 特殊js的支持。
 *
 * @version 1.0
 * @author zhenggp
 * @create 2013-02-16
 */

(function ($) {
    $.checkFields = function ($chkBoxes, fieldName, fields) {
        // 如果未设置初始化字段值；
        if (fields == undefined || fields == 'undefined') {
            fields = {};
        }

        if ($chkBoxes.size() == 0) return {};

        var i = 0;
        $chkBoxes.each(function () {
            var key = fieldName + '[' + (i++) + ']';
            fields[key] = $(this).val();
        });

        return fields;
    };
})(window.jQuery);

(function ($) {
    var JtcNotify = function (options) {
        this.$container = $(options.container)
            , this.options = options
            , this.$html = $(this.html()).hide()
            , this.$container.append(this.$html);

        if (this.options.show) this.show();
    };

    JtcNotify.prototype = {
        constructor:JtcNotify,

        show:function () {
            this.$html.show(this.options.face);

            if (this.options.autoHide) {
                window.setTimeout($.proxy(this.autoHide, this), this.options.autoHide);
            }
        },

        autoHide:function () {
            this.hide();
            this.clear();
        },

        hide:function () {
            this.$html.hide(this.options.face);
            this.$html.remove();
        },

        clear:function () {
            this.$html.empty();
        },

        html:function () {
            var html = "<div id='" + this.options.name + "' class='alert " + this.options.name + "-" + this.options.sign + "'>";
            if (this.options.title) {
                html += "<h4>" + this.options.title + "</h4>";
            }
            if (this.options.close) {
                html += "<a class='close' href='javascript:void(0);' onclick='$(this).parent().remove();'>x</a>";
            }

            html += "<p>" + this.options.message + "</p>";
            return html + "</div>";
        }
    };

    $.jtcNotify = function (options) {
        if (typeof options == 'string') options = {message:options};
        return new JtcNotify($.fn.extend({}, $.jtcNotify.defaults, typeof options == 'object' && options));
    };

    $.jtcNotify.defaults = {
        title:false,
        sign:'info',
        name:'alert',
        show:true,
        face:'fast',
        autoHide:4000,
        close:false,
        container:'#message-container'
    };

    // 清消息窗口的内容清空。
    $(document).ready(function () {
        window.setTimeout(function () {
            $('#message-container').empty();
        }, 5000);
    });
})
    (window.jQuery);

(function ($) {
    /**
     * xbl post link 提供使用post提交数据的方式。
     *
     * @param $element
     * @param options
     * @constructor
     */
    var JtcPost = function ($element, options) {
        this.$element = $element;
        this.options = options;
        this.options.target = this.options.target || this.$element.attr('href');
    };

    JtcPost.prototype = {
        constructor:JtcPost,

        submit:function () {
            var fields = this.options.fields
                , target = this.options.target
                , remote = this.options.remote;

            if (!target) return;
            if (typeof fields == 'string') fields = eval('(' + fields + ')');
            else if ($.isFunction(fields)) fields = fields();

            if ($.isEmptyObject(fields)) {
                this.options.fieldsNotify();
                return;
            }

            if (this.options.confirm && !this.options.confirmNotify(this.options.confirm)) return;

            if (remote) {
                $.post(target, fields, this.options.remoteNotify);
            } else {
                var $form = $('<form method="post" action="' + target + '"></form>');
                var inputs = '';

                $.each(fields, function (name, value) {
                    inputs += '<input name="' + name + '" value="' + value + '" type="hidden" />';
                });

                $form.hide().append(inputs).appendTo('body');
                $form.submit();
            }

            this.$element.trigger($.Event('complete'));
        }
    };

    $.fn.jtcPost = function (option) {
        return this.each(function () {
            var $this = $(this)
                , data = $this.data('post')
                , options = $.extend({}, $.fn.jtcPost.defaults, $this.data(), typeof option == 'object' && option);
            if (!data) $this.data('post', (data = new JtcPost($this, options)));
            if (typeof option == 'string') data[option]();
            else if (options.submit) data.submit();
        });
    };

    $.fn.jtcPost.defaults = {
        fields:{},
        submit:false,
        remote:false,
        confirm:'请确认是否继续执行操作',
        fieldsNotify:function () {
            $.jtcNotify({message:'提交字段为空，执行失败!', sign:'warning'});
        },
        confirmNotify:function (msg) {
            return confirm(msg);
        },
        remoteNotify:function (data) {
            if (typeof data == 'string') {
                data = eval('(' + data + ')');
            }

            $.jtcNotify(data);
        }
    };

    $.fn.jtcPost.Constructor = JtcPost;

    $(document).on('click.post.data-api', '[data-toggle="jtc-post"]', function (e) {
        var $this = $(this)
            , option = $this.data('post') ? 'submit' : $.extend({submit:true}, $this.data());

        e.preventDefault();

        $this.jtcPost(option);
    });

})(window.jQuery);

(function ($) {
    /**
     * JtcChkAll 提供js全选服务。
     *
     * @param $element
     * @param options
     * @constructor
     */
    var JtcChkAll = function ($element, options) {
        this.options = options;
        this.$element = $element;
        this.$checkBoxes = $(options.target).find(':checkbox');
        var that = this;

        if (options.doubleTouch) {
            $(options.target).find(options.doubleTouch).dblclick(function (e) {
                var $checkbox = $(this).find(':checkbox');

                if ($checkbox.size() > 0 && $checkbox.attr('checked')) {
                    $checkbox.attr('checked', false);
                } else if ($checkbox.size() > 0) {
                    $checkbox.attr('checked',true);
                }

                that.change();
            });
        }

        this.$element.click(function () {
            that[that.$element.attr('checked') ? 'check' : 'uncheck']();
        });

        this.$checkBoxes.click(function () {
            that.change();
        });

        this.change = function () {
            var chkAll = true;
            var chkOne = false;
            that.$checkBoxes.each(function () {
                if (!($(this).attr('checked'))) {
                    chkAll = false;
                } else {
                    chkOne = true;
                }
            });

            that.$element.attr('checked', chkAll);

            if (chkAll) that.$element.trigger('chkall');
            else that.$element.trigger('unchkall');

            if (chkOne) that.$element.trigger('check');
            else that.$element.trigger('uncheck');

            // 处理选择目标元素
            if (that.options.check) {
                if (chkOne) $(that.options.check).show();
                else $(that.options.check).hide();
            }
        };

        this.check = function () {
            this.$element.attr('checked', true);
            this.$checkBoxes.attr('checked', true);

            this.change();
        };

        this.uncheck = function () {
            this.$element.removeAttr('checked', true);
            this.$checkBoxes.removeAttr('checked', true);

            this.change();
        };

        /**
         * 获取选择checkbox的字段值。
         */
        this.fields = function (fieldName, fields) {
            // 初始化选项。
            if (fieldName == undefined) fieldName = 'id';
            if (fields == undefined) fields = {};

            var idx = 0;
            this.$checkBoxes.each(function () {
                if ($(this).attr('checked')) {
                    fields['fieldName[' + (idx++) + ']'] = $(this).val();
                }
            });

            return idx == 0 ? {} : fields;
        };
    };

    $.fn.jtcChkAll = function (option) {
        return this.each(function () {
            var $this = $(this)
                , data = $this.data('chkall')
                , options = $.extend({}, $.fn.jtcChkAll.defaults, $this.data(), typeof option == 'object' && option);

            if (!data) $this.data('chkall', (data = new JtcChkAll($this, options)));
            if (typeof option == 'string') data[option]();
        });
    };

    $.fn.jtcChkAll.defaults = {
        target:'table tbody',
        doubleTouch:'tr'
    };

    $(document).ready(function () {
        $("[data-toggle='jtc-chkall']").jtcChkAll({});
    });
})
    (jQuery);