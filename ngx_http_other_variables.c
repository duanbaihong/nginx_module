// #include <stdio.h>
// #include <time.h>
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
ngx_tm_t       ngx_now_tm;
static ngx_int_t ngx_add_other_variable_t(ngx_conf_t *cf);
static ngx_int_t ngx_http_other_variables_fmt(ngx_http_request_t *r, ngx_http_variable_value_t *v, u_char *t, ngx_int_t len)
{
    u_char  *p;

    p = ngx_pnalloc(r->pool, len);
    if (p == NULL) {
        return NGX_ERROR;
    }

    ngx_memcpy(p, t, len);

    v->len = len;
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
static ngx_int_t ngx_get_format_time_t()
{
  time_t           sec;
  struct timeval   tv;
  ngx_gettimeofday(&tv);
  sec=tv.tv_sec;
  ngx_localtime(sec,&ngx_now_tm);
  return NGX_OK;
}
static ngx_int_t ngx_get_year_t(ngx_http_request_t *r,ngx_http_variable_value_t *v,uintptr_t data)
{
  ngx_get_format_time_t();
  u_char year[5];
  ngx_sprintf(year,"%d",ngx_now_tm.tm_year);
  // ngx_log_stderr(0, "dbh888 %s test failed",year);

  return ngx_http_other_variables_fmt(r,v,year,sizeof(year)-1);
}

static ngx_int_t ngx_get_year2_t(ngx_http_request_t *r,ngx_http_variable_value_t *v,uintptr_t data)
{
  ngx_get_format_time_t();
  u_char year2[3];
  ngx_sprintf(year2,"%02d",ngx_now_tm.tm_year % 100);

  return ngx_http_other_variables_fmt(r,v,year2,sizeof(year2)-1);
}

static ngx_int_t ngx_get_month_t(ngx_http_request_t *r,ngx_http_variable_value_t *v,uintptr_t data)
{
  ngx_get_format_time_t();
  u_char month[3];
  ngx_sprintf(month,"%02d",ngx_now_tm.tm_mon);

  return ngx_http_other_variables_fmt(r,v,month,sizeof(month)-1);
}

static ngx_int_t ngx_get_day_t(ngx_http_request_t *r,ngx_http_variable_value_t *v,uintptr_t data)
{
  ngx_get_format_time_t();
  u_char days[3];
  ngx_sprintf(days,"%02d",ngx_now_tm.tm_mday);

  return ngx_http_other_variables_fmt(r,v,days,sizeof(days)-1);
}
static ngx_int_t ngx_get_hour_t(ngx_http_request_t *r,ngx_http_variable_value_t *v,uintptr_t data)
{
  ngx_get_format_time_t();
  u_char hours[3];
  ngx_sprintf(hours,"%02d",ngx_now_tm.tm_hour);

  return ngx_http_other_variables_fmt(r,v,hours,sizeof(hours)-1);
}
static ngx_int_t ngx_get_hour12_t(ngx_http_request_t *r,ngx_http_variable_value_t *v,uintptr_t data)
{
  ngx_get_format_time_t();
  u_char hour12[3];
  ngx_sprintf(hour12,"%02d",ngx_now_tm.tm_hour / 2);

  return ngx_http_other_variables_fmt(r,v,hour12,sizeof(hour12)-1);
}
static ngx_int_t ngx_get_minute_t(ngx_http_request_t *r,ngx_http_variable_value_t *v,uintptr_t data)
{
  ngx_get_format_time_t();
  u_char minute[3];
  ngx_sprintf(minute,"%02d",ngx_now_tm.tm_min);

  return ngx_http_other_variables_fmt(r,v,minute,sizeof(minute)-1);
}
static ngx_int_t ngx_get_seconds_t(ngx_http_request_t *r,ngx_http_variable_value_t *v,uintptr_t data)
{
  ngx_get_format_time_t();
  u_char seconds[3];
  ngx_sprintf(seconds,"%02d",ngx_now_tm.tm_sec);

  return ngx_http_other_variables_fmt(r,v,seconds,sizeof(seconds)-1);
}
static ngx_int_t ngx_get_week_t(ngx_http_request_t *r,ngx_http_variable_value_t *v,uintptr_t data)
{
  ngx_get_format_time_t();
  u_char week[2];
  ngx_sprintf(week,"%d",ngx_now_tm.tm_wday);

  return ngx_http_other_variables_fmt(r,v,week,sizeof(week)-1);
}
static ngx_int_t ngx_get_time_http_t(ngx_http_request_t *r,ngx_http_variable_value_t *v,uintptr_t data)
{
  ngx_get_format_time_t();
  u_char time_http[20];
  ngx_sprintf(time_http,"%4d/%02d/%02d %02d:%02d:%02d",
    ngx_now_tm.tm_year,
    ngx_now_tm.tm_mon,
    ngx_now_tm.tm_mday,
    ngx_now_tm.tm_hour,
    ngx_now_tm.tm_min,
    ngx_now_tm.tm_sec);

  return ngx_http_other_variables_fmt(r,v,time_http,sizeof(time_http)-1);
}
static ngx_int_t ngx_get_timestamp_t(ngx_http_request_t *r,ngx_http_variable_value_t *v,uintptr_t data)
{
  time_t  timestamp;
  u_char timestamp_char[12];
  timestamp=ngx_time();
  ngx_sprintf(timestamp_char,"%d",timestamp);
  // ngx_log_stderr(0, "timestamp_char %s test failed",timestamp_char);
  return ngx_http_other_variables_fmt(r,v,timestamp_char,sizeof(timestamp_char)-2);
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
      ngx_get_week_t, 0,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },
    { ngx_string("hour"), NULL, 
      ngx_get_hour_t, 0, 
      NGX_HTTP_VAR_NOCACHEABLE, 0 },
    { ngx_string("hour12"), NULL,
      ngx_get_hour12_t,0,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },
    { ngx_string("time_http"), NULL, 
      ngx_get_time_http_t, 0,
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
