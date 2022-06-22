# ChatRoom

A chat room software developed on QT framework.

基于QT开发的聊天室，使用C/S架构，包括客户端和服务器两部分。

## 服务器 Server

服务器负责维持客户端连接，以及消息转发等操作。

服务器可在多个平台运行，但需要提前配置QT环境。

执行步骤如下：

1. main.cpp中设置端口号

2. 执行qmake和make（Linux），或在QT Creator中点击执行（MAC，Windows）

3. 运行可执行文件Server（Linux）

## 客户端 Client

客户端需要先绑定名称，之后可向指定用户发送消息，同时可以接受其他用户发来的消息。

客户端可在多个平台运行，但需提前配置QT环境，并且需要连接显示器来显示窗口。

执行步骤如下：

1. main.cpp中设置服务器IP和端口号

2. 在QT Creator中点击执行（MAC，Windows，Linux）

3. 输入名称，进行绑定

4. 输入收信人和消息，进行发送

