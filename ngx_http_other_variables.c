// #include <stdio.h>
// #include <time.h>
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
// #include <string.h>
// u_char nowtimefmtstr [128];

// u_char *ngx_get_format_time_t(char *fmt)
// {
//  time_t nowtime;
//  struct tm * timeinfo;
//  time(&nowtime);
//  timeinfo = localtime (&nowtime);
//  strftime(nowtimefmtstr,sizeof(nowtimefmtstr),fmt,timeinfo);
//  return nowtimefmtstr;
// }
static ngx_int_t ngx_add_other_variable_t(ngx_conf_t *cf);
static ngx_int_t ngx_http_other_variables_fmt(ngx_http_request_t *r, ngx_http_variable_value_t *v, u_char *t, ngx_int_t len)
{
    u_char *p;
    p = ngx_pnalloc(r->pool, len);
    if (p == NULL) {
        return NGX_ERROR;
    }
    v->len = ngx_sprintf(p, "%s", t) - p;
    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;
    v->data = p;

    return NGX_OK;
}
static ngx_http_module_t ngx_module_other_varials_ctx = {
    ngx_add_other_variable_t, /* preconfiguration 在创建和读取该模块的配置信息之前被调用。*/
    NULL, /* postconfiguration 在创建和读取该模块的配置信息之后被调用。*/
 
    NULL, /* create main configuration 调用该函数创建本模块位于http block的配置信息存储结构。该函数成功的时候，返回创建的配置对象。失败的话，返回NULL。*/
    NULL, /* init main configuration 调用该函数初始化本模块位于http block的配置信息存储结构。该函数成功的时候，返回NGX_CONF_OK。失败的话，返回NGX_CONF_ERROR或错误字符串*/
 
    NULL, /* create server configuration 调用该函数创建本模块位于http server block的配置信息存储结构，每个server block会创建一个。该函数成功的时候，返回创建的配置对象。失败的话，返回NULL。*/
    NULL, /* merge server configuration 因为有些配置指令既可以出现在http block，也可以出现在http server block中。那么遇到这种情况，每个server都会有自己存储结构来存储该server的配置，
                      但是在这种情况下http block中的配置与server block中的配置信息发生冲突的时候，就需要调用此函数进行合并，该函数并非必须提供，当预计到绝对不会发生需要合并的情况的时候，就无需提供。当然为了安全起见还是建议提供。
                    该函数执行成功的时候，返回NGX_CONF_OK。失败的话，返回NGX_CONF_ERROR或错误字符串。*/
 
    NULL,   /* create location configuration 调用该函数创建本模块位于location block的配置信息存储结构。每个在配置中指明的location创建一个。该函数执行成功，返回创建的配置对象。失败的话，返回NULL。*/
    NULL    /* merge location configuration 与merge_srv_conf类似，这个也是进行配置值合并的地方。该函数成功的时候，返回NGX_CONF_OK。失败的话，返回NGX_CONF_ERROR或错误字符串。*/
};

static ngx_int_t ngx_get_year_t(ngx_http_request_t *r,ngx_http_variable_value_t *v,uintptr_t data)
{
  u_char  tmp[4];
  ngx_cpystrn(tmp,ngx_cached_http_log_iso8601.data,5);
  return ngx_http_other_variables_fmt(r,v,tmp,sizeof(tmp));
}

static ngx_int_t ngx_get_year2_t(ngx_http_request_t *r,ngx_http_variable_value_t *v,uintptr_t data)
{
  u_char  tmp[2];
  ngx_cpystrn(tmp,ngx_cached_http_log_iso8601.data+2,3);
  return ngx_http_other_variables_fmt(r,v,tmp,sizeof(tmp));
}

static ngx_int_t ngx_get_month_t(ngx_http_request_t *r,ngx_http_variable_value_t *v,uintptr_t data)
{
  u_char  tmp[2];
  ngx_cpystrn(tmp,ngx_cached_http_log_iso8601.data+5,3);
  return ngx_http_other_variables_fmt(r,v,tmp,sizeof(tmp));  
}

static ngx_int_t ngx_get_day_t(ngx_http_request_t *r,ngx_http_variable_value_t *v,uintptr_t data)
{
  u_char  tmp[2];
  ngx_cpystrn(tmp,ngx_cached_http_log_iso8601.data+8,3);
  return ngx_http_other_variables_fmt(r,v,tmp,sizeof(tmp));
}
static ngx_int_t ngx_get_hour_t(ngx_http_request_t *r,ngx_http_variable_value_t *v,uintptr_t data)
{
  u_char  tmp[2];
  ngx_cpystrn(tmp,ngx_cached_http_log_iso8601.data+11,3);
  return ngx_http_other_variables_fmt(r,v,tmp,sizeof(tmp));
}
static ngx_int_t ngx_get_hour12_t(ngx_http_request_t *r,ngx_http_variable_value_t *v,uintptr_t data)
{
  u_char  tmp[2];
  u_char *p;
  int nt;
  ngx_cpystrn(tmp,ngx_cached_http_log_iso8601.data+11,3);
  nt=ngx_atoi(tmp,2) % 12;
  if (nt ==0){
    nt=12;
  }
  p = ngx_pnalloc(r->pool, 2);
  ngx_sprintf(p,"%02d",nt);
  return ngx_http_other_variables_fmt(r,v,p,sizeof(p));
}
static ngx_int_t ngx_get_minute_t(ngx_http_request_t *r,ngx_http_variable_value_t *v,uintptr_t data)
{
  u_char  tmp[2];
  ngx_cpystrn(tmp,ngx_cached_http_log_iso8601.data+14,3);
  return ngx_http_other_variables_fmt(r,v,tmp,sizeof(tmp));
}
static ngx_int_t ngx_get_seconds_t(ngx_http_request_t *r,ngx_http_variable_value_t *v,uintptr_t data)
{
  u_char  tmp[2];
  ngx_cpystrn(tmp,ngx_cached_http_log_iso8601.data+17,3);
  return ngx_http_other_variables_fmt(r,v,tmp,sizeof(tmp));
}
static ngx_int_t ngx_get_timestamp_t(ngx_http_request_t *r,ngx_http_variable_value_t *v,uintptr_t data)
{
  time_t  tmp;
  tmp=ngx_time();
  u_char *p;
  p = ngx_pnalloc(r->pool, 11);
  if (p == NULL) {
      return NGX_ERROR;
  }
  v->len = ngx_sprintf(p, "%l", tmp) - p;
  v->valid = 1;
  v->no_cacheable = 0;
  v->not_found = 0;
  v->data = p;
  return NGX_OK;
}


static ngx_http_variable_t ngx_http_other_commands[] = { // 定义Variables模块的变量
    { ngx_string("year"), NULL,
      ngx_get_year_t, 0,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },
    { ngx_string("year2"), NULL,
      ngx_get_year2_t, 0,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },
    { ngx_string("month"), NULL,
      ngx_get_month_t, 0,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },
    { ngx_string("day"), NULL,
      ngx_get_day_t, 0,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },
    { ngx_string("minute"), NULL,
      ngx_get_minute_t, 0,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },
    { ngx_string("seconds"), NULL,
      ngx_get_seconds_t, 0,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },
    { ngx_string("week"), NULL,
      ngx_get_year_t, 0,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },
    { ngx_string("hour"), NULL, 
      ngx_get_hour_t, 0, 
      NGX_HTTP_VAR_NOCACHEABLE, 0 },
    { ngx_string("hour12"), NULL,
      ngx_get_hour12_t,0,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },
    { ngx_string("time_http"), NULL, 
      ngx_get_year_t, 0,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },
    { ngx_string("unix_time"), NULL, 
      ngx_get_timestamp_t, 0,
      NGX_HTTP_VAR_NOCACHEABLE, 0 }
};

ngx_module_t ngx_http_other_variables = {
    NGX_MODULE_V1,
    &ngx_module_other_varials_ctx, /* module context */
    NULL,    /* module directives */
    NGX_HTTP_MODULE,                  /* module type */
    NULL,                             /* init master */
    NULL,                             /* init module */
    NULL,                             /* init process */
    NULL,                             /* init thread */
    NULL,                             /* exit thread */
    NULL,                             /* exit process */
    NULL,                             /* exit master */
    NGX_MODULE_V1_PADDING
};

static ngx_int_t ngx_add_other_variable_t(ngx_conf_t *cf)
{
  ngx_http_variable_t *var, *v;
  for (v = ngx_http_other_commands; v->name.len; v++) {
      var = ngx_http_add_variable(cf, &v->name, v->flags);
      if (var == NULL) {
          return NGX_ERROR;
      }
      var->get_handler = v->get_handler;
      var->data = v->data;
  }
  return NGX_OK;
}
