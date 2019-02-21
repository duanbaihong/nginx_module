#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_stream.h>

ngx_tm_t       ngx_now_tm;
static ngx_int_t ngx_stream_add_other_variable_t(ngx_conf_t *cf);
static ngx_int_t ngx_stream_other_variables_fmt(ngx_stream_session_t *s, ngx_stream_variable_value_t *v, u_char *t, ngx_int_t len)
{
    u_char  *p;

    p = ngx_pnalloc(s->connection->pool, len);
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
static ngx_stream_module_t  ngx_stream_module_other_variables_ctx = {
    ngx_stream_add_other_variable_t,          /* preconfiguration */
    NULL,                              /* postconfiguration */

    NULL,                              /* create main configuration */
    NULL,                              /* init main configuration */

    NULL,                              /* create server configuration */
    NULL                               /* merge server configuration */
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
static ngx_int_t ngx_get_year_t(ngx_stream_session_t *s,ngx_stream_variable_value_t *v,uintptr_t data)
{
  ngx_get_format_time_t();
  u_char year[5];
  ngx_sprintf(year,"%d",ngx_now_tm.tm_year);
  // ngx_log_stderr(0, "dbh888 %s test failed",year);

  return ngx_stream_other_variables_fmt(s,v,year,sizeof(year)-1);
}

static ngx_int_t ngx_get_year2_t(ngx_stream_session_t *s,ngx_stream_variable_value_t *v,uintptr_t data)
{
  ngx_get_format_time_t();
  u_char year2[3];
  ngx_sprintf(year2,"%02d",ngx_now_tm.tm_year % 100);

  return ngx_stream_other_variables_fmt(s,v,year2,sizeof(year2)-1);
}

static ngx_int_t ngx_get_month_t(ngx_stream_session_t *s,ngx_stream_variable_value_t *v,uintptr_t data)
{
  ngx_get_format_time_t();
  u_char month[3];
  ngx_sprintf(month,"%02d",ngx_now_tm.tm_mon);

  return ngx_stream_other_variables_fmt(s,v,month,sizeof(month)-1);
}

static ngx_int_t ngx_get_day_t(ngx_stream_session_t *s,ngx_stream_variable_value_t *v,uintptr_t data)
{
  ngx_get_format_time_t();
  u_char days[3];
  ngx_sprintf(days,"%02d",ngx_now_tm.tm_mday);

  return ngx_stream_other_variables_fmt(s,v,days,sizeof(days)-1);
}
static ngx_int_t ngx_get_hour_t(ngx_stream_session_t *s,ngx_stream_variable_value_t *v,uintptr_t data)
{
  ngx_get_format_time_t();
  u_char hours[3];
  ngx_sprintf(hours,"%02d",ngx_now_tm.tm_hour);

  return ngx_stream_other_variables_fmt(s,v,hours,sizeof(hours)-1);
}
static ngx_int_t ngx_get_hour12_t(ngx_stream_session_t *s,ngx_stream_variable_value_t *v,uintptr_t data)
{
  ngx_get_format_time_t();
  u_char hour12[3];
  ngx_sprintf(hour12,"%02d",ngx_now_tm.tm_hour / 2);

  return ngx_stream_other_variables_fmt(s,v,hour12,sizeof(hour12)-1);
}
static ngx_int_t ngx_get_minute_t(ngx_stream_session_t *s,ngx_stream_variable_value_t *v,uintptr_t data)
{
  ngx_get_format_time_t();
  u_char minute[3];
  ngx_sprintf(minute,"%02d",ngx_now_tm.tm_min);

  return ngx_stream_other_variables_fmt(s,v,minute,sizeof(minute)-1);
}
static ngx_int_t ngx_get_seconds_t(ngx_stream_session_t *s,ngx_stream_variable_value_t *v,uintptr_t data)
{
  ngx_get_format_time_t();
  u_char seconds[3];
  ngx_sprintf(seconds,"%02d",ngx_now_tm.tm_sec);

  return ngx_stream_other_variables_fmt(s,v,seconds,sizeof(seconds)-1);
}
static ngx_int_t ngx_get_week_t(ngx_stream_session_t *s,ngx_stream_variable_value_t *v,uintptr_t data)
{
  ngx_get_format_time_t();
  u_char week[2];
  ngx_sprintf(week,"%d",ngx_now_tm.tm_wday);

  return ngx_stream_other_variables_fmt(s,v,week,sizeof(week)-1);
}
static ngx_int_t ngx_get_time_http_t(ngx_stream_session_t *s,ngx_stream_variable_value_t *v,uintptr_t data)
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

  return ngx_stream_other_variables_fmt(s,v,time_http,sizeof(time_http)-1);
}
static ngx_int_t ngx_get_timestamp_t(ngx_stream_session_t *s,ngx_stream_variable_value_t *v,uintptr_t data)
{
  time_t  timestamp;
  u_char timestamp_char[12];
  timestamp=ngx_time();
  ngx_sprintf(timestamp_char,"%d",timestamp);
  // ngx_log_stderr(0, "timestamp_char %s test failed",timestamp_char);
  return ngx_stream_other_variables_fmt(s,v,timestamp_char,sizeof(timestamp_char)-2);
}

static ngx_stream_variable_t ngx_stream_other_commands[] = { // 定义Variables模块的变量
    { ngx_string("year"), NULL,
      ngx_get_year_t, 0,
      NGX_STREAM_VAR_NOCACHEABLE, 0 },
    { ngx_string("year2"), NULL,
      ngx_get_year2_t, 0,
      NGX_STREAM_VAR_NOCACHEABLE, 0 },
    { ngx_string("month"), NULL,
      ngx_get_month_t, 0,
      NGX_STREAM_VAR_NOCACHEABLE, 0 },
    { ngx_string("day"), NULL,
      ngx_get_day_t, 0,
      NGX_STREAM_VAR_NOCACHEABLE, 0 },
    { ngx_string("minute"), NULL,
      ngx_get_minute_t, 0,
      NGX_STREAM_VAR_NOCACHEABLE, 0 },
    { ngx_string("seconds"), NULL,
      ngx_get_seconds_t, 0,
      NGX_STREAM_VAR_NOCACHEABLE, 0 },
    { ngx_string("week"), NULL,
      ngx_get_week_t, 0,
      NGX_STREAM_VAR_NOCACHEABLE, 0 },
    { ngx_string("hour"), NULL, 
      ngx_get_hour_t, 0, 
      NGX_STREAM_VAR_NOCACHEABLE, 0 },
    { ngx_string("hour12"), NULL,
      ngx_get_hour12_t,0,
      NGX_STREAM_VAR_NOCACHEABLE, 0 },
    { ngx_string("time_http"), NULL, 
      ngx_get_time_http_t, 0,
      NGX_STREAM_VAR_NOCACHEABLE, 0 },
    { ngx_string("unix_time"), NULL, 
      ngx_get_timestamp_t, 0,
      NGX_STREAM_VAR_NOCACHEABLE, 0 }
};

ngx_module_t ngx_stream_other_variables = {
    NGX_MODULE_V1,
    &ngx_stream_module_other_variables_ctx, /* module context */
    NULL,    /* module directives */
    NGX_STREAM_MODULE,                  /* module type */
    NULL,                             /* init master */
    NULL,                             /* init module */
    NULL,                             /* init process */
    NULL,                             /* init thread */
    NULL,                             /* exit thread */
    NULL,                             /* exit process */
    NULL,                             /* exit master */
    NGX_MODULE_V1_PADDING
};

static ngx_int_t ngx_stream_add_other_variable_t(ngx_conf_t *cf)
{
  ngx_stream_variable_t *var, *v;
  for (v = ngx_stream_other_commands; v->name.len; v++) {
      var = ngx_stream_add_variable(cf, &v->name, v->flags);
      if (var == NULL) {
          return NGX_ERROR;
      }
      var->get_handler = v->get_handler;
      var->data = v->data;
  }
  return NGX_OK;
}
