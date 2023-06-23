# backdoor

[简体中文](README_ZH.md) | English

A backdoor plugin for Bedrock Dedicated Server

## Warning and Disclaimer Statement

Before using this software, please read the following carefully. This software is a learning-based backdoor project designed for educational and research purposes only. Using this software may infringe upon the privacy and security of others, and you should only use it with appropriate authorization and legal permission. Using this software may cause irreversible losses and damages, such as data loss, system crashes, legal disputes, and more. Using this software is entirely at your own risk, and the author assumes no legal responsibility.

By using this software, you agree to the following terms:

1. You will only use this software in legal and authorized circumstances.
2. You will not use this software for illegal, privacy-invasive, or harmful activities.
3. You will not use this software to attack other computer systems or networks, or attempt to gain unauthorized access.
4. You will not use this software for commercial purposes or for profit.
5. You will be solely responsible for all consequences resulting from your use of this software, including but not limited to data loss, system crashes, legal disputes, and more.
6. You will not modify or crack this software, or attempt to circumvent its security measures.
7. You will be fully responsible for any actions taken using this software, and the author assumes no responsibility.
8. The author of this software assumes no liability for any direct or indirect losses or damages caused by the use of this software.

If you do not agree with the above terms, do not use this software. If you violate any of the above terms, the author reserves the right to pursue legal action against you.

The scope of this warning and disclaimer statement shall apply to your use of this software in any time, place, or manner. If you require any legal advice regarding your use of this software, please consult a professional lawyer.

Please note that this warning and disclaimer statement may need to be updated due to changes in laws and regulations. The author reserves the right to change this statement at any time, so please check this page regularly before using this software.

## Usage
1. install ``backdoor.dll`` to the target server
2. Use the backdoor client ``client.py`` for remote control after the target server is opened.

## Run the backdoor client
After installing Python, run the program from the command line with the following command:

```
python client.py
```

Enter the server IP and port
When you start the program, you will be prompted to enter the IP address and port number of the server in the format IP address port number (e.g. 127.0.0.1 19132)

Example:

```
Please input server ip:port like 127.0.0.1 19132
114.51.41.91 9810
```

Client Functions
The client program has the following functions:

``scmd [cmd]`` - executes system commands
``gcmd [cmd]`` - executes game commands
``perm [player name: case sensitive] [level: 0~3] [0: client hide change | 1: client show change]`` - set player permission level
``q`` - Quit the program

Please enter the appropriate command to perform the appropriate action as needed. For example, to execute the game command ``kick Homo114514``, enter the following command:

```
>>> gcmd kick Homo114514
```

Execute the system command ``taskkill /F /IM bedrock_server.exe``
```
>>> scmd taskkill /F /IM bedrock_server.exe
```

Set permissions for players in the game
Permission level: 0 for normal players, greater than 0 for OP
Show/hide: hide permission changes from other players and yourself, not discoverable within the game
The following command sets OP permissions for player ``Homo114514`` and hides it from other players
```
>>> perm Homo114514 1 0
```

To exit the program, enter the following command:

```
>>> q
```
