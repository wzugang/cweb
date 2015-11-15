#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <my_global.h>
#include <mysql.h>
#include <time.h>
#include "../../lib/cgic/cgic.h"
#include "../../lib/ctemplate/ctemplate.h"
#include "../model/interface/post.h"

static void regUser(); //用户注册
static void login(); //用户登录
static void loginIndex();

int cgiMain() {
	cgiHeaderContentType("text/html;charset=utf-8");
	int action_len = 0;
	cgiFormStringSpaceNeeded("action", &action_len);
	char *action = (char *) malloc(sizeof(char) * (action_len + 1));
	cgiFormString("action", action, 200);
	if (strcmp(action, "reguser") == 0) { // 注册
		regUser();
	} else if (strcmp(action, "login") == 0) { //登录
		login();
	} else {
		loginIndex();
	}
	free(action);
	return 0;
}

//默认首页
static void loginIndex() {
	TMPL_write("../resource/template/login/index.html", 0, 0, 0, cgiOut,
			cgiOut);
}

//处理用户注册
static void regUser() {
	if (cgiFormSubmitClicked("btn") == cgiFormSuccess) {
		int pwd_len = 0;
		int username_len = 0;
		int created_time = time(NULL);
		int updated_time = time(NULL);
		char salt[20] = "user_salt";
		tableField **table_field = NULL;
		int i, field_cnt = 0;
		cgiFormStringSpaceNeeded("username", &username_len);
		cgiFormStringSpaceNeeded("pwd", &pwd_len);
		char *username = (char *) malloc(sizeof(char) * (username_len + 1));
		char *pwd = (char *) malloc(sizeof(char) * (pwd_len + 1));
		cgiFormString("username", username, username_len);
		cgiFormString("pwd", pwd, pwd_len);
		field_cnt = getModelField("user", &table_field);
		char insertSql[1024] = { 0 };
		sprintf(insertSql,
				"INSERT INTO `user` (`username`, `pwd`, `salt`, `created_time`, `updated_time`) VALUES ('%s', '%s', '%s', '%d', '%d');",
				username, pwd, salt, created_time, updated_time);
		executeQuery(insertSql);
		/*for (i = 0; i < field_cnt; i++) {
		 fprintf(cgiOut,
		 "FieldCnt= %d FieldName =%s FieldType=%d FieldLength=%d<br/>",
		 field_cnt, table_field[i]->field_name,
		 table_field[i]->field_type, table_field[i]->field_length);
		 }
		 free(table_field);*/
		TMPL_varlist *varlist;
		varlist = TMPL_add_var(0, "msg", insertSql, 0);
		TMPL_write("../resource/template/login/reguser.html", 0, 0, varlist,
				cgiOut, cgiOut);
	} else {
		TMPL_varlist *varlist;
		varlist = TMPL_add_var(0, "msg", "", 0);
		TMPL_write("../resource/template/login/reguser.html", 0, 0, varlist,
				cgiOut, cgiOut);
	}

}

//处理用户登录
static void login() {
	TMPL_varlist *varlist;
	varlist = TMPL_add_var(0, 0);
	if (cgiFormSubmitClicked("btn") == cgiFormSuccess) { //处理提交
		char username[20];
		char pwd[20];
		int content_length;
		cgiFormStringSpaceNeeded("content", &content_length);
		char *content = (char *) malloc(sizeof(char) * (content_length + 1));
		cgiFormString("username", username, 20);
		cgiFormString("pwd", pwd, 20);
		cgiFormString("content", content, content_length);
		char insertSql[1024] = { 0 };
		sprintf(insertSql,
				"INSERT INTO `post` (`keywords`, `content`) VALUES ('%s', '%s');",
				username, content);
		executeQuery(insertSql);
		varlist = TMPL_add_var(varlist, "msg", "已经提交了哦！！", 0);
		varlist = TMPL_add_var(varlist, "username", username, 0);
		varlist = TMPL_add_var(varlist, "pwd", pwd, 0);
		free(content);
	} else {
		varlist = TMPL_add_var(varlist, "msg", "初始化", 0);
	}
	TMPL_write("../resource/template/login/login.html", 0, 0, 0, cgiOut,
			cgiOut);
	TMPL_free_varlist(varlist);
}