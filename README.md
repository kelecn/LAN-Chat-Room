# 基于QT开发的局域网聊天室

![](https://w.wallhaven.cc/full/8x/wallhaven-8xjljk.png)
<!--more-->

**注意事项：**

```c++
开发工具：QT
开发语言：C++
测试软件：chatPrivateTest文件夹
源码：chatPrivate文件夹
注意：Windows环境下请在全英文路径下打开工程，否则会因为编码出错。
```

### 一、功能简介

<hr>


本次设计是一个简易的局域网聊天室，功能设计主要分为群聊和私聊两部分，每部分都支持基础聊天以及文件传输功能。参考了[《Qt及Qt Quick开发实战精解》](https://raw.githubusercontent.com/kelecn/images/master/Qt%E5%8F%8AQt%20Quick%E5%BC%80%E5%8F%91%E5%AE%9E%E6%88%98%E7%B2%BE%E8%A7%A3.pdf)中的群聊实例，并在群聊的基础设计了私聊这部分内容以及其他一些功能，其中消息传递使用UDP来实现，而文件传输使用TCP来实现。


![](https://cdn.jsdelivr.net/gh/kelecn/images@master/功能简介1.png)

### 二、UDP群聊部分

<hr>


本程序实现的功能是：局域网内，每个用户登录到聊天软件，则软件界面的右端可以显示在线用户列表，分别显示的是用户名，主机名，ip地址。软件左边那大块是聊天内容显示界面，这里局域网相当于qq中的qq群，即群聊。每个人可以在聊天输入界面中输入文字（还可修改文字格式&颜色）并发送。其聊天界面如下：

![](https://cdn.jsdelivr.net/gh/kelecn/images@master/%E4%B8%BB%E7%95%8C%E9%9D%A2.png)

**注：此处的客户端，同时也是服务器。**

下面分服务器端和客户端两部分来介绍：

**服务器：** 建立一个UDP Socket并绑定在固定端口后，用信号与槽的方式进行监听是否有数据来临。如果用，接收其数据并分析数据的消息类型，如果消息是新用户登录则更新用户列表并在聊天显示窗口中添加新用户上线通知；同理，如果是用户下线，则在用户列表中删除该用户且在聊天显示窗口中显示下线通知；如果是聊天消息，则接收该消息并且在窗口中显示。其流程图如下：

![](https://cdn.jsdelivr.net/gh/kelecn/images@master/%E6%9C%8D%E5%8A%A1%E5%99%A8.png)

**客户端：** 首先当客户端登录时，获取本机的用户名，计算机名和ip地址，并广播给局域网的服务器更新用户列表。然后当客户端需要发送信息时，则在聊天输入栏中输入信息并按发送键发送聊天内容，当然于此同时也广播本地系统的各种信息。其流程图如下：

![](https://cdn.jsdelivr.net/gh/kelecn/images@master/%E5%AE%A2%E6%88%B7%E7%AB%AF.png)


### 三、TCP文件传输部分

<hr>


**发送端界面：**

![](https://cdn.jsdelivr.net/gh/kelecn/images@master/%E4%BC%A0%E6%96%87%E4%BB%B61.png)

**接收端界面：**

![](https://cdn.jsdelivr.net/gh/kelecn/images@master/%E6%8E%A5%E6%94%B6%E6%96%87%E4%BB%B61.png)

 **发送端，也即承担服务器角色的操作：**

在主界面程序右侧选择一个需要发送文件的用户，弹出发送端界面后，点击打开按钮，在本地计算机中选择需要发送的文件，点击发送按钮，则进度条上会显示当前文件传送的信息，有已传送文件大小信息，传送速度等信息。如果想关闭发送过程，则单击关闭按钮。其流程图如下：

![](https://cdn.jsdelivr.net/gh/kelecn/images@master/%E5%8F%91%E9%80%81%E7%AB%AF%E6%B5%81%E7%A8%8B%E5%9B%BE.png)

 **接收端，也即承担客户端角色的操作：**

当在主界面中突然弹出一个对话框，问是否接自某个用户名和IP地址的文件传送信息，如果接受则单击yes按钮，否则就单击no按钮。当接收文件时，选择好接收文件所存目录和文件名后就开始接收文件了，其过程也会显示已接收文件的大小，接收速度和剩余时间的大小等信息。其流程图如下：

![](https://cdn.jsdelivr.net/gh/kelecn/images@master/%E6%8E%A5%E6%94%B6%E7%AB%AF%E6%B5%81%E7%A8%8B%E5%9B%BE.png)

### 四、私聊部分

<hr>


**私聊界面：**

![](https://cdn.jsdelivr.net/gh/kelecn/images@master/%E7%A7%81%E8%81%8A.png)

**私聊发送端流程图：**

![](https://cdn.jsdelivr.net/gh/kelecn/images@master/%E7%A7%81%E8%81%8A%E5%8F%91%E9%80%81%E7%AB%AF%E6%B5%81%E7%A8%8B%E5%9B%BE.png)

**私聊接收端流程图：**

![](https://cdn.jsdelivr.net/gh/kelecn/images@master/%E7%A7%81%E8%81%8A%E6%8E%A5%E6%94%B6%E7%AB%AF%E6%B5%81%E7%A8%8B%E5%9B%BE.png)

**下面来介绍下2者实现的具体过程：**

**A方(主动开始首次发送的一方)：**

1. 在主窗口右侧双击自己想与之聊天的B方，此时A方实际上完成的工作有：用B方的主机名和ip地址新建了私聊的类privatechat，在新建该类的过程中，已经设置了显示顶端为：xxx与聊天中，对方IP：xxx，且绑定了本地ip和私聊的专用端口，同时设置了信号与槽的联系，即该端口如果有数据输入，则触发槽函数processPendingDatagrams().该函数是char.cpp中的。
2. 当上面的新建私聊类完成后，用通讯对方ip地址和其群聊专用的端口（但用的是主udp群聊的socket进行的）将以下内容分别发送出去：消息类型(Xchat)，用户名，主机名，本地ip地址。完成后，在屏幕中显示私聊窗口。
3. 在私聊窗口中输入需要聊天的内容，单击发送键。该过程玩成的内容有：分别将消息类型(Message)+用户名+本地名+本地IP+消息内容本身通过私聊专用端口发送出去。在私聊窗口中显示主机名+聊天时间，换行后显示消息内容本身。

**B方(第一次信息是他人发送过来的)：**

1. 当A在2步骤中用群聊的方法发送其消息类型(Xchat)，其用户名，其主机名，其ip地址后，由于程序运行时已经初始化了widget.cpp中的构造函数，所以每个程序都绑定了本地地址+群聊专用的端口，一旦有数据传入，就触发widget.cpp中的槽函数processPendingDatagrams().
2. 在processPendingDatagrams()函数中，判断消息类型为Xchat后，接收缓存区内接收对方用户名，对方主机名和对方ip地址。并用接收到的主机名和ip地址新建一个私聊类。新建该私聊的过程与A中的步骤1一样。完后在程序中显示私聊窗口。
3. 当对方A按完发送按钮后，通过私聊专用端口绑定槽函数来触发chart.cpp中的processPendingDatagrams()函数，该函数中先读取消息类型(Message)，然后依次读取用户名，主机名，ip地址，消息内容本身，并将对方信息和消息内容显示在聊天窗口中。

### 五、参考资料及资源下载

<hr>


[《Qt及Qt Quick开发实战精解》](https://raw.githubusercontent.com/kelecn/images/master/Qt%E5%8F%8AQt%20Quick%E5%BC%80%E5%8F%91%E5%AE%9E%E6%88%98%E7%B2%BE%E8%A7%A3.pdf)

[局域网聊天室测试软件.zip](https://cdn.jsdelivr.net/gh/kelecn/images@master/%E5%B1%80%E5%9F%9F%E7%BD%91%E8%81%8A%E5%A4%A9%E5%AE%A4.zip)

[局域网聊天室源码.zip](https://cdn.jsdelivr.net/gh/kelecn/images@master/%E5%B1%80%E5%9F%9F%E7%BD%91%E8%81%8A%E5%A4%A9%E5%AE%A4%E6%BA%90%E7%A0%81.zip)
