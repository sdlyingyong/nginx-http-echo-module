![](https://github.com/sdlyingyong/nginx-http-echo-module/blob/readme/image/logo.jpg)


# NGINX-HTTP-ECHO-MODULE 1.0 - [Official Homepage](https://www.)
![](https://github.com/sdlyingyong/nginx-http-echo-module/blob/readme/image/clipboard.png)

=========================

Envy the power of Nginx?

Need a more robust processing module?

Want to learn more about the secrets inside Nginx?

Now you can have the highest share of high performance asynchronous servers in this world.

Write your echo basic output module

## Feature
- blog of tutorial link
- resource of Nginx module development 

## Architecture
![](https://github.com/sdlyingyong/nginx-http-echo-module/blob/readme/image/module%20load.jpg)

## Preview
![](https://github.com/sdlyingyong/nginx-http-echo-module/blob/readme/image/result.png)

![](https://github.com/sdlyingyong/nginx-http-echo-module/blob/readme/image/nginx_config.png)

## Installation


        #获取Nginx源码
        wget http://nginx.org/download/nginx-1.0.10.tar.gz
        #将扩展和config放到目录下,我放在/var
        #代码地址 https://github.com/sdlyingyong/nginx-http-echo-module
        #修改config中的 NGX_ADDON_SRCS="/var/ngx_http_echo_module.c"
        #这里把/var/替换成你ngx_http_echo_module.c所在的路径
        vim config
        #解压Nginx
        tar -zxvf nginx-1.0.0.tar.gz
        #进入目录
        cd nginx-1.0.0
        #编译安装Nginx和扩展
        #这里prefix=指定Nginx安装目录,--add-module=指定安装扩展所在的位置
        ./configure --perfix=/etc/nginx --add-module=/var
        make && make install 
        #启动Nginx
        #在你Nginx安装路径下sbin/这是二进制可执行文件目录,里面的nginx来启动
        /etc/nginx/sbin/nginx
        #访问localhost,看到Nginx欢迎提示,代表Nginx已经成功运行
        curl -i localhost 

        HTTP/1.1 200 OK
        ......
        <title>Welcome to nginx!</title>
        ......
        #修改服务器配置,在文件Nginx.conf
        vim /etc/nginx/conf/nginx.conf
        #在其中server {}新增一行,表明当收到用户请求localhost/echo时,调用我们的echo模块,
        #输出这句话:this is echo module running !
        location /echo {
            echo "this is echo module running !";
        }
        #退出并保存更改
        :wq 
        #优雅重载Nginx配置
        /etc/nginx/sbin/nginx -s reload
        
 ## Further reading
 
 bolg
 
 
 
