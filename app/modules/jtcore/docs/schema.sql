/*
 * xblives schema.sql
 */

CREATE DATABASE lives DEFAULT CHARACTER SET = 'utf8';

use lives;
  
DROP TABLE `xbl_menu` IF EXISTS;
DROP TABLE `xbl_user` IF EXISTS;

/*
 * 创建角色表
 */
CREATE TABLE xbl_role (
  `id`          INT     NOT NULL  AUTO_INCREMENT PRIMARY KEY COMMENT  '角色编号',
  `name`        VARCHAR(32) NOT NULL COMMENT '角色名字',
  `sys`         tinyint(1) NOT NULL DEFAULT 0  COMMENT '系统角色',
  `description` VARCHAR(256) DEFAULT '' COMMENT '说明'
);

/**
 * 创建管理组
 */
CREATE TABLE xbl_user_group (
	`id`			INT				NOT NULL AUTO_INCREMENT PRIMARY KEY COMMENT '编号',
	`name`			VARCHAR(32)		NOT NULL COMMENT '名字',
	`description`	VARCHAR(256)	NOT NULL DEFAULT '' COMMENT '描述'
);

/*
 * 创建管理员
 */
CREATE TABLE xbl_user (
	`id`				INT 			NOT NULL AUTO_INCREMENT PRIMARY KEY COMMENT '用户编号',
	`group_id`			INT 			NOT NULL COMMENT '用户组',
	`name`				VARCHAR(16) 	NOT NULL COMMENT '用户名',
	`email`				VARCHAR(64)		NOT NULL COMMENT '邮箱地址',
	`allow`				SMALLINT(1)     NOT NULL DEFAULT 1 COMMENT '允许访问',
	`created`			INT 			NOT NULL DEFAULT 0 COMMENT '创建时间',
	`password`			VARCHAR(128)	NOT NULL COMMENT '密码',
	`passkey`			CHAR(16)		NOT NULL COMMENT '密码锁',
	`login_count`		INT				NOT NULL DEFAULT 0 COMMENT '登录次数',
	`last_login_date` 	INT				NOT NULL DEFAULT 0 COMMENT '最后登录时间',
	`last_login_address`	VARCHAR(16)		NOT NULL DEFAULT '' COMMENT '最后登录(IP)',
	`description`		VARCHAR(256)	NOT NULL DEFAULT '' COMMENT '说明'
) DEFAULT CHARSET=utf8;

/**
 * 创建管理员角色组。
 */
CREATE TABLE xbl_user_role (
  `user_id` INT NOT NULL,
  `role_id` INT NOT NULL,
  PRIMARY KEY(`user_id`,`role_id`)
) DEFAULT CHARSET=utf8;

/**
 * 创建日志记录表
 */
CREATE TABLE xbl_log (
  `id`        INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  `subject`   VARCHAR(128) NOT NULL COMMENT '日志主题',
  `category`  VARCHAR(32)  NOT NULL COMMENT '分类',
  `message`   VARCHAR(1024) NOT NULL DEFAULT '' COMMENT '详细内容',
  `created`   INT NOT NULL COMMENT '创建时间',
  `user`      VARCHAR(32)  NOT NULL COMMENT '用户',
  `clientIP`  VARCHAR(32) NOT NULL COMMENT '客户IP地址'
) DEFAULT CHARSET=utf8;

/**
 * 创建导航
 */
CREATE TABLE xbl_nav (
  `id`       INT  NOT NULL AUTO_INCREMENT PRIMARY KEY COMMENT '主键',
  `name`     VARCHAR(32) NOT NULL
) DEFAULT CHARSET=utf8;

/**
 * 创建导航连接记录表
 */
CREATE TABLE xbl_nav_link(
  `id`            INT NOT NULL AUTO_INCREMENT PRIMARY KEY COMMENT '主键',
  `nav_id`        INT NOT NULL DEFAULT 0,
  `title`         VARCHAR(32) NOT NULL COMMENT '标题',
  `url`           VARCHAR(128) DEFAULT '' COMMENT '地址',
  `window`        char(10) DEFAULT '_self' COMMENT '打开方式',
  `sort`          INT NOT NULL COMMENT '排序',
  `deep`          INT NOT NULL DEFAULT 1 COMMENT '深度',
  `resource`      char(15) NOT NULL DEFAULT '' COMMENT '资源编号',
  `description`   VARCHAR(128) DEFAULT '' COMMENT '说明'
) DEFAULT CHARSET=utf8;

/**
 * 创建流量监控表
 */
CREATE TABLE xbl_visitor_group (
  `id`    INT NOT NULL AUTO_INCREMENT PRIMARY KEY COMMENT '主键',
  `name`  VARCHAR(16) NOT NULL COMMENT '说明'
) DEFAULT CHARSET=utf8;

/**
 * 创建流量访客表。
 */
CREATE TABLE xbl_visitor (
  `id`          INT NOT NULL AUTO_INCREMENT PRIMARY KEY COMMENT '主键',
  `group_id`    INT NOT NULL COMMENT '访问编号',
  `cat`         VARCHAR(32) NOT NULL DEFAULT '' COMMENT '分类',
  `sub_cat`     VARCHAR(32) NOT NULL DEFAULT '' COMMENT '子类',
  `visitors`    INT NOT NULL COMMENT '访问数量',
  `created`     INT NOT NULL COMMENT '时间'
);

/*
 * �˵���ݱ?�ṩxbutu lives�Ĳ˵�������¼��
 */  
CREATE TABLE xbl_menu (
	`id`		INT 			NOT NULL AUTO_INCREMENT PRIMARY KEY COMMENT '����',
	`name`  	CHAR(32) 		NOT NULL COMMENT '����',
	`title` 	VARCHAR(64) 	NOT NULL COMMENT '����',
	`icon`		VARCHAR(128) 	NOT NULL COMMENT 'ͼ��',
	`url`		VARCHAR(128) 	NOT NULL COMMENT '��ַ',
	`window` 	CHAR(10)		NOT NULL COMMENT '���ڴ򿪷�ʽ',	/* _self��_blank,  */
	`resource`	CHAR(16) 		NOT NULL DEFAULT '' COMMENT '��������',
	`parent`	INT				NOT NULL DEFAULT 0  COMMENT '�ϼ��˵�',	/* 0-��ʾ�����˵� */
	`order`		INT 			NOT NULL DEFAULT 0  COMMENT '����',	
	`descr`		VARCHAR(256)	NOT NULL DEFAULT '' COMMENT '˵��'
) SET DEFAULT CHARSET=utf8;
