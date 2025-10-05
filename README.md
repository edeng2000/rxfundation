# rxfundation
底层基础库：跨平台、健壮、可靠的api集合


#### 介绍
&emsp;&emsp;本库的实现，是为了解决我们内部项目共用的跨平台需求代码，它主要包含以下功能：   
&emsp;&emsp;1、文件操作：读、写、删  
&emsp;&emsp;2、目录操作：创建、删除、遍历、获取目录下所有文件、获取目录下指定后缀的所有文件   
&emsp;&emsp;3、时间操作：    
&emsp;&emsp;&emsp1>、获取当前时间    
&emsp;&emsp;&emsp;2>、修改时间     
&emsp;&emsp;&emsp;3>、获取指定时间的时间戳，   
&emsp;&emsp;&emsp;4>、将时间戳转换为指定的日期格式   
&emsp;&emsp;4、线程操作：    
&emsp;&emsp;&emsp;1>、创建线程     
&emsp;&emsp;&emsp;2>、启动线程    
&emsp;&emsp;&emsp;3>、暂停线程   
&emsp;&emsp;&emsp;4>、终止线程，包括：强行退出、安全退出   
&emsp;&emsp;&emsp;5>、设置线程优先级   
&emsp;&emsp;&emsp;6>、线程锁管理   
&emsp;&emsp;&emsp;7>、线程信号量管理    
&emsp;&emsp;5、进程管理    
&emsp;&emsp;&emsp;1>、创建进程
&emsp;&emsp;&emsp;2>、销毁进程   
&emsp;&emsp;&emsp;3>、进程通信：共享内存   
&emsp;&emsp;6、字符串处理   
&emsp;&emsp;&emsp;1>、支持Uncode和ansi之间切换，并不用修改代码   
&emsp;&emsp;&emsp;2>、字符串分割   
&emsp;&emsp;&emsp;4>、常用字符串处理：复制、拼接、剪切，查找，截取   
&emsp;&emsp;7、日志系统   
&emsp;&emsp;&emsp;1>、支持日志分级管理   
&emsp;&emsp;&emsp;2>、支持日志文件自动分割：在日志文件过大(超过1G时候，会自动创建新的日志文件)；当日志跨天的时候，也会自动生成新的日志文件。    
&emsp;&emsp;8、常用http、https访问接口封装   
&emsp;&emsp;&emsp;1>、get    
&emsp;&emsp;&emsp;2>、put    
&emsp;&emsp;&emsp;3>、post    
&emsp;&emsp;9、http协议解析封装    
&emsp;&emsp;&emsp;1>、get     
&emsp;&emsp;&emsp;2>、post   
&emsp;&emsp;&emsp;3>、put   
&emsp;&emsp;10、加密接口封装，对openssl3.0加解密功能进行了二次封装，使之符合面向对象的调用风格    


如果您对这个库有所兴趣，并希望讨论其涉及的技术实现，可联系qq：45536611
