/*头文件,模块基于http*/
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

/*定义配置*/
typedef struct
{
    ngx_str_t ed;
} ngx_http_echo_loc_conf_t;

static char *ngx_http_echo(ngx_conf_t *cf, ngx_command_t *cmd, void *conf); //echo功能模块
static ngx_int_t ngx_http_echo_init(ngx_conf_t *cf);
static void *ngx_http_echo_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_echo_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);

/*注册命令*/
// ngx_command_t 结构体
static ngx_command_t ngx_http_echo_commands[] = {
    {
        ngx_string("echo"),                     //结构体第一个参数,配置中输入echo 这里把请求echo交给命令
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,     //本模块属于http location
        ngx_http_echo,                          //托管http请求的函数
        NGX_HTTP_LOC_CONF_OFFSET,               //配置信息
        offsetof(ngx_http_echo_loc_conf_t, ed), //偏移量
        NULL,                                   //此处基本是null
    },
    ngx_null_command,
};

/*回调函数*/
static ngx_http_module_t ngx_http_echo_module_ctx = {
    NULL,
    ngx_http_echo_init, //post config
    NULL,
    NULL,
    NULL,
    NULL,
    ngx_http_echo_create_loc_conf,
    ngx_http_echo_merge_loc_conf,
};

/*模块结构体*/
ngx_module_t ngx_http_echo_module = {
    NGX_MODULE_V1,
    &ngx_http_echo_module_ctx, //回调函数数组
    ngx_http_echo_commands,    //注册命令
    NGX_HTTP_MODULE,           //模块类型
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NGX_MODULE_V1_PADDING,
};

/*处理函数*/
//真正的操作
static ngx_int_t
ngx_http_echo_handler(ngx_http_request_t *r)
{
    ngx_int_t rc;
    ngx_buf_t *b;
    ngx_chain_t out;
    ngx_http_echo_loc_conf_t *elcf;
    elcf = ngx_http_get_module_loc_conf(r, ngx_http_echo_module);
    if (!(r->method & (NGX_HTTP_HEAD | NGX_HTTP_GET | NGX_HTTP_POST)))
    {
        return NGX_HTTP_NOT_ALLOWED;
    }
    r->headers_out.content_type.len = sizeof("text/html") - 1;
    r->headers_out.content_type.data = (u_char *)"text/html";
    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = elcf->ed.len;
    if (r->method == NGX_HTTP_HEAD)
    {
        rc = ngx_http_send_header(r);
        if (rc != NGX_OK)
        {
            return rc;
        }
    }
    b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
    if (b == NULL)
    {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "Failed to allocate response buffer.");
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    out.buf = b;
    out.next = NULL;
    b->pos = elcf->ed.data;
    b->last = elcf->ed.data + (elcf->ed.len);
    b->memory = 1;
    b->last_buf = 1;
    rc = ngx_http_send_header(r);
    if (rc != NGX_OK)
    {
        return rc;
    }
    return ngx_http_output_filter(r, &out);
}

/*设置函数*/
//用来给配置信息挂上托管http请求的处理器函数
static char *
ngx_http_echo(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf;
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_echo_handler; //将新增加的处理函数挂到corecf的处理程序属性上
    ngx_conf_set_str_slot(cf, cmd, conf);
    return NGX_CONF_OK;
}

/*初始化echo模块*/
static ngx_int_t
ngx_http_echo_init(ngx_conf_t *cf)
{
    return NGX_OK;
}

/*初始化一个配置结构体*/
static void *
ngx_http_echo_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_echo_loc_conf_t *conf;
    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_echo_loc_conf_t));
    if (conf == NULL)
    {
        return NGX_CONF_ERROR;
    }
    conf->ed.len = 0;
    conf->ed.data = NULL;
    return conf;
}

/*将其父block的配置信息合并到此结构体 实现了配置的继承*/
static char *
ngx_http_echo_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_echo_loc_conf_t *prev = parent;
    ngx_http_echo_loc_conf_t *conf = child;
    ngx_conf_merge_str_value(conf->ed, prev->ed, '"');
    return NGX_CONF_OK;
}
