###  Dos 简单介绍

**注意：本程序仅用来学习网络安全知识使用。请勿对任何企业或单位的服务器进行攻击，否则造成的后果由您承担。**

---

DoS 即拒绝服务(Denial of Service)。目的是使计算机或网络无法提供正常的服务。最常见的DoS攻击有计算机网络宽带攻击和连通性攻击。

> DoS攻击是指故意的攻击网络协议实现的缺陷或直接通过野蛮手段残忍地耗尽被攻击对象的资源，目的是让目标计算机或网络无法提供正常的服务或资源访问，使目标系统服务系统停止响应甚至崩溃，而在此攻击中并不包括侵入目标服务器或目标网络设备。这些服务资源包括网络带宽，文件系统空间容量，开放的进程或者允许的连接。这种攻击会导致资源的匮乏，无论计算机的处理速度多快、内存容量多大、网络带宽的速度多快都无法避免这种攻击带来的后果。 -- 百度百科

#### `dos` 常见的攻击手段：

- 制造大流量无用数据，造成通往被攻击主机的网络拥塞，使被攻击主机无法正常和外界通信。
- 利用被攻击主机提供服务或传输协议上处理重复连接的缺陷，反复高频的发出攻击性的重复服务请求，使被攻击主机无法及时处理其它正常的请求。
- 利用被攻击主机所提供服务程序或传输协议的本身实现缺陷，反复发送畸形的攻击数据引发系统错误的分配大量系统资源，使主机处于挂起状态甚至死机。


#### 攻击流程

要理解dos攻击，首先要理解TCP连接的三次握手过程(Three-wayhandshake)。在TCP/IP协议中，TCP协议提供可靠的连接服务，采用三次握手建立一个连接。

- 第一次握手:建立连接时，`client`发送`SYN`包到服务器，并进入`SYN SEND`状态，等待服务器确认;
- 第二次握手:服务器收到`SYN`包，必须确认客户的`SYN`同时自己也发送一个`SYN+ACK`包，此时服务器进入`SYN_RECV`状态;
- 客户端收到服务器的`SYN+ACK`包，向服务器发送确认包`ACK`，此包发送完毕，客户端和服务器进入`ESTABLISHED`状态，完成三次握手，客户端与服务器开始传送数据。

---

在三次握手过程中，还有一些重要的概念：

- `half-connection`: 收到`SYN`包而还未收到`ACK`包时的连接状态称为半连接，即尚未完全完成三次握手的`TCP`连接。

- `halft-connection-queue`: 在三次握手协议中，服务器维护一个半连接队列，该队列为每个客户端的`SYN`包开设一个条目，该条目表明服务器已收到`SYN`包，并向客户发出确认，正在等待客户的确认包。这些条目所标识的连接在服务器处于`SYN_ RECV`状态，当服务器收到客户的确认包时，删除该条目，服务器进入`ESTABLISHED`状态。

- `Backlog`参数:表示半连接队列的最大容纳数目。

- `SYN-ACK`重传次数: 服务器发送完`SYN-ACK`包，如果未收到客户确认包，服务器进行首次重传，等待一段时间仍未收到客户确认包，进行第二次重传，如果重传次数超过系统规定的最大重传次数，系统将该连接信息、从半连接队列中删除。注意，每次`重传等待的时间`不一定相同。

- 半连接存活时间: 是指半连接队列的条目存活的最长时间，也即服务从收到SYN包到确认这个报文无效的最长时间，该时间值是所有重传请求包的最长等待时间总和。有时也称半连接存活时间为Timeout时间、SYN_RECV存活时间。


#### 介绍下典型的`SYN`泛洪攻击

`SYN`洪水攻击属于`DoS`攻击的一种，它利用`TCP`协议缺陷，通过发送大量的半连接请求，耗费`CPU`和内存资源。`SYN`攻击除了能影响主机外，还可以危害路由器、防火墙等网络系统，事实上`SYN`攻击并不管目标是什么系统，只要这些系统打开`TCP`服务就可以实施。配合`IP`欺骗，`SYN`攻击能达到很好的效果，通常，客户端在短时间内伪造大量不存在的`IP`地址，向服务器不断地发送`SYN`包，服务器回复确认包，并等待客户的确认，由于源地址是不存在的，服务器需要不断的重发直至超时，这些伪造的`SYN`包将长时间占用未连接队列，正常的`SYN`请求被丢弃。


#### 其他典型的`DOS`攻击

- `Ping of Death`

发送畸形的 `Ping` 包。使得未采取保护措施的网络系统出现内存分配错误。导致 `TCP / IP` 协议栈崩溃，最终接收方宕机。这种攻击通过发送大于`65536`字节的`ICMP`包使操作系统崩溃；通常不可能发送大于`65536`个字节的`ICMP`包，但可以把报文分割成片段，然后在目标主机上重组；最终会导致被攻击目标缓冲区溢 出，引起拒绝服务攻击。有些时候导致`telnet`和`http`服务停止，有些时候路由器重启。

- 泪滴
 
 发送含有重叠偏移的伪造分段将服务器搞崩溃。


- `UPD Flood`

大概就是利用 `echo` 服务与 `chargen` 服务的缺陷，来回传送毫无用处且占满带宽的垃圾数据，在两台主机之间生成足够多的无用数据流，可导致网络可用带宽耗尽。

- `SYN Flood`

- `IP` 复位欺骗

- `Land LandAttack`攻击


#### 常见攻击与防范

- `SYN Flood` 防范：
    
    1. 第一种方法就是缩短`SYN Timeout`时间。
    2. 第二种方法就是设置`SYN Cookie`。给每一个请求连接的IP地址分配一个Cookie，如果短时间内连续接收到某个IP的重复SYN报文，立定一个一个评判标准。如果满足标准，就认为受到了攻击，一段时间内此IP地址来的包会被一概丢弃。

- `Smurf`攻击的防范：
    （伪造某服务器的包发送一个网络的广播地址，致使许多主机将对真正的服务器短时间内发送大量回复报文。）
    
    1. 在路由器配置上可以防止包传递到广播地址上。

 - `Ping of Death`的防范：

配置路由器、防火墙和入侵检测系统来抵御常见DDoS攻击