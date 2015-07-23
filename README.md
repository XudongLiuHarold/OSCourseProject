# OSCourseProject
For this summer's course project

###Members:
* 1352834 安哲宏 42028701 <br/>
* 1352918 刘旭东 42028701 <br/>
* 1353010 薛梦迪 42028702 <br/>

====
###项目环境
* Linux Ubuntu 14.0.4 (X86)
* Bochs (2.6.7)

###使用方法
* 确保你已经装了<a href="http://www.ubuntu.com/download/desktop">Ubuntu</a> 和 <a href="http://sourceforge.net/projects/bochs/">Bochs</a>
* 在terminal里面运行下面的命令完成配置，如果是中国IP可能需要切换Ubuntu的源（163的源速度较快）.
```bash
$ sudo  apt-get update 
$ sudo	apt-get	install	build-essential	
$ sudo	apt-get	install	xorg-dev										
$ sudo	apt-get	install	bison	
$ sudo	apt-get	install	libgtk2.0-dev
$ sudo	apt-get	install	nasm
$ sudo	apt-get	install	vgabios
```
* 将项目clone到本地,首先打开到目标文件夹下,然后运行:
```bash
$ git clone https://github.com/xdliu002/OSCourseProject.git
$ cd OSCourseProject /Tinix-master
$ sudo make image
$ bochs -f bochsrc.bxrc
$ 回车再输入 c 进入
```

=====
###项目分工

* 我主要负责了项目的整体的初始化，并将进程调度算法改为了多级队列调度，以及写了一个日历的小型应用。
* 安哲宏主要创建了开关机动画以及对内部进程的调整，并且增加了颜色，美化了整个系统的界面。
* 薛梦迪主要负责写了一个计算器、五子棋两个用户级应用，并且负责了我们小组的文档，ppt等工作。
* 大家都对书上的源码做了深入的了解，并且在源码的基础上做出了相应的改动，每个人都非常的努力。

====
###Summary:

感谢两位队友的辛苦付出，同时也受到了一些同学、学长的帮助，在此一并感谢。🙏

=======
Thx to all.<br/>
Liu © 2015.7.23

###托管地址(提交项目用)：
https://github.com/xdliu002/OSCourseProject

====

