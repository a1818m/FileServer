##What is this project?
* the server part of the file transfer system based on C++ can realize file transfer between remote computers.

##What is the meaning of this project?
* this project is a Demo I used to practice.
* you can use it as a file transfer tool (the current version is not user-friendly, not recommended).
* you can use it as your own exercise program to add functionality.

##pay attention to the point
* the server is divided into two parts, Linux and Windows, because of the time factor, and they are not merged.
* the Linux version is under Centos.
* Windows version in VC++.

##The client section
* ![client] link (https://github.com/a1818m/FileClient)

##File structure
* afos: tool class.
* jsoncpp:json data format structure parsing.
* SRC: source code.
<br/>* tcpserver.cpp:        receiving service.
<br/>* tcpservicetas.cpp:    service base class.
<br/>* testservicet.h:       test service.
<br/>* myftpservice.h:       specific file transfer service business.
<br/>* FlieUtils.cpp:        file directory browsing.
<br/>* TcpTaskMonitor.cpp:   thread management.
<br/>* Mysql.cpp:            database.
