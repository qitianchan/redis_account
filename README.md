# redis_account
## 编译步骤：
### 1. 安装hiredis
下载[hiredis源码](https://github.com/redis/hiredis/releases/tag/v0.14.0)

解压源码
> unzip hiredis-0.14.0
 
进入源码目录
> cd hiredis-0.14.0

安装hiredis
> make install

### 2. 编译代码
命令行编译
> gcc account.c -o account -L /usr/local/lib -lhiredis

或者使用Makefile编译
> make build

### 参考：
[Redis C 语言客户端 hiredis 的使用](https://blog.csdn.net/lihao21/article/details/70339494)
[hiredis官网](https://redislabs.com/lp/hiredis/)
[Makefile由浅入深--教程、干货](https://zhuanlan.zhihu.com/p/47390641)