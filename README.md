# MY_TODOLIST

## 使用方法

项目的编写平台是Linux，可执行文件的运行环境也是Linux，目前只有命令行操作。

获取帮助：
```
$ ./todo -h
```
默认的用户密码：
```
User_Name: HH
Password: password
```

编译时请用：
```
$ g++ main.cpp -l sqlite3 todo.cpp todo.h passwd.cpp passwd.h md5.cpp md5.h -o todo
```

## 这一堆的由来

小学期华有两门课想上的，一门是自己选的AI001，另一面是开给信安的必修课"NIS1336 计算机编程实践"，然后很不巧这两门的上课时间撞了不能都选，最后选了AI001然后旁听NIS1336。  

这一个项目是第一节课就布置下来的，华瞅了一眼发现有亿点点的小难。然后加上之前也想过暑假写一个不大不小的项目再锻炼一下自己的coding能力，正好写一个todolist是差不多的。于是虽然没选上这课，也写了这么一个项目。  

## 作业目标

使用C++语言开发一个有用而且有趣的日程管理软件，实现如下功能：

* 账户管理：
    
        输入用户名、口令；
        口令不能明文保存，要加密保存口令或保存口令的HASH值；

* 任务录入：

        任务属性： 任务名称、启动时间、优先级（高中低）、分类（学习、娱乐、生活）、提醒时间；
        每个任务的开始时间不能相同；
        任务名称+开始时间，要具有唯一性；
        任务的属性，比如优先级，类型，具有缺省值；
        给每个任务分配唯一的id；

* 任务保存：

        输入的任务，要保存到本地文件；每一个任务输入完成后自动保存到文件；
        也可以使用数据库；

* 任务加载：

        用户登录后，从文件加载任务列表，保存到内存；

* 任务删除；

        根据任务id删除任务；

* 任务显示：

        按照月/周/天/优先级/类型 显示任务；
        显示已完成或者未完成来显示任务；

* 任务提醒：

        在约定的提醒时间屏幕打印提醒；

 * 高级进阶：

        以多线程方式运行。在后台周期性检查任务，进行提醒；
        以图形界面完成该作业；

* 命令行参数

        程序应具有完整的命令行帮助说明和使用示例。
        命令行参数应支持 run，addtask，showtask，deltask等命令。 其中run方式以shell方式循环运行，等待用户输入命令；
    比如：
```
$ myschedule  run
$ myschedule  user1 password   addtask  tasknam1….
```

* 作业提交资料

        程序源代码；
        可执行程序；
        cmake文件；
        测试程序（shell程序，调用命令行参数进行任务的增删查改等）；
        设计文档。

## 施工进度

### 要求的功能

- [x] 账户登录
- [x] 任务录入
- [x] 任务保存
- [x] 任务加载
- [x] 任务删除
- [x] 任务显示
- [ ] 任务提醒
- [ ] 多线程后台提醒
- [ ] 图形界面
- [ ] 设计文档
- [ ] shell脚本

### 还有一些自己额外想加的功能
- [x] 手写hash函数
- [x] 用上SQLite
- [ ] 用户的添加
- [ ] 密码更改
- [ ] 能和前端交互的api

设计文档不知道华日后还有没有心情水一篇blog？或者什么的。

关于ui界面华是想用python的tkiner直接搓一个或者是