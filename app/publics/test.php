<?php


    # 初始化错误代码。
    #ini_set ('display_errors', JTDEBUG ? 1 : 0);
    #ini_set ('error_log', JTPATH_RTIME_LOGS . '/error.log');

    #error_reporting (JTDEBUG ? E_ALL : 0);

    #if (self::$_initialized) {
    #    return Jtita::error ('Logger repeat initialize', JTERROR_NORMAL);
    #}

    #if (! mysql_connect ()) {
    #    return Jtita::error (new MysqlException('Mysql connect fail!'), JTERROR_DEADLY);
    #    call undefine field
    #}

    #if (JTDEBUG) {
    #    error_reporting (E_ALL);
    #}

    #function jt_error ($level, $message) {
    #    echo 'error:', $message;
    #}

    #function jt_exception ($ex) {
    #    print_r ($ex);
    #}

    #set_error_handler ('jt_error');
    #set_exception_handler ('jt_exception');

    # trigger_error ('repeat initialize logger', E_USER_ERROR);

    # throw new Exception('Hello World');

    # echo 'ok';

    throw new Exception('goods');

    #trigger_error ("error");

    #$arr = array();

    #$a = $arr['id'];

   # $a->hello ();

/*
    return Jtita::error ('Logger repeat initialized');

    class Jtita {

        public static function error ($message, $level = JTERROR_NORMAL) {
            # 如果消息是异常，并且是严重错误。
            if ($level == JTERROR_DEADLY && ! is_string ($message)) {
                throw $message;
            }

            trigger_error ($message, $level);
            return false;
        }
    }

    echo 'A', $a, ':';

    #throw new Exception('Hello World');

    echo 'ok';

#try {
#    $this->goods = Goods::find($this->query('id',0));

#} catch (NotFoundException $ex) {
#    $this->error(404);
#}

*/