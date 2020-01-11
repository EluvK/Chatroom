#### Chatroom
自学socket网络编程的练手项目
- 服务器端：腾讯云服务器 CentOS 7.5 64位
- 客户端：Windows下使用
- [socket学习笔记](https://blog.eucalyptluvk.cn/?p=200)
- [github代码仓库](https://github.com/EluvK/Chatroom)

#### v2.0（2020.01.11）
- 实现了Linux\Windows服务器作中转，多个Windows端client作为用户的聊天室
- 服务器接收每个人的聊天消息转发给所有在线用户
- 客户端固定窗口大小，输入缓冲区设置在底部，不影响上面聊天记录区更新消息
- 多线程

#### v1.0
实现Linux服务器作为server，Windows应用作为client的一对一消息发送
必须等待对方应答才可继续通信
client输入close结束
