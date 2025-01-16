# C-Socket Programming Lab 8

## Manipulating Socket Characteristics

---

### Protocol Levels

Protocol levels refer to the layers of configurable options for a socket.

These protocol levels determine whether the options affect the socket itself, the TCP layer, or a lower-layer protocol like IP.

#### Examples of Protocol Levels:

`SOL_SOCKET`: Generic socket options independent of any specific protocol
`IPPROTO_TCP`: Options specific to TCP
`IPPROTO_IP` : Options specific to IP(v4).


<br>



#### Protocol Level: SOL_SOCKET

| Option Name    | Description                                         |
|----------------|-----------------------------------------------------|
| SO_BROADCAST   | Allows sending broadcast messages.                 |
| SO_DEBUG       | Enables debugging information recording.           |
| SO_DONTROUTE   | Bypasses routing tables for outgoing messages.      |
| SO_ERROR       | Retrieves pending socket errors.                   |
| SO_KEEPALIVE   | Sends periodic keepalive messages.                 |
| SO_LINGER      | Controls the socket's linger behavior on close.    |
| SO_RCVBUF      | Sets the size of the receive buffer.               |
| SO_SNDBUF      | Sets the size of the send buffer.                  |
| SO_REUSEADDR   | Allows reusing local addresses.                    |
| SO_REUSEPORT   | Allows multiple sockets to bind to the same port.  |
| SO_TYPE        | Retrieves the socket type (e.g., SOCK_STREAM).     |


<br>

#### Protocol Level: IPPROTO_TCP

| Option Name    | Description                                         |
|----------------|-----------------------------------------------------|
| TCP_NODELAY    | Disables the Nagle algorithm for low latency.       |
| TCP_MAXSEG     | Retrieves or sets the maximum segment size.         |
| TCP_KEEPALIVE  | Configures TCP keepalive behavior.                  |
| TCP_KEEPINTVL  | Sets the interval between keepalive probes.         |
| TCP_KEEPCNT    | Sets the number of keepalive probes before failure. |


<br>

#### Protocol Level: IPPROTO_IP

| Option Name          | Description                                         |
|----------------------|-----------------------------------------------------|
| IP_TTL               | Sets the Time-To-Live (TTL) for packets.            |
| IP_HDRINCL           | Includes the IPv4 header in outgoing packets.       |
| IP_OPTIONS           | Configures IP options (like routing options).       |
| IP_MULTICAST_IF      | Sets the interface for outgoing multicast packets.  |
| IP_MULTICAST_TTL     | Sets TTL for multicast packets.                     |
| IP_MULTICAST_LOOP    | Enables or disables multicast loopback.             |


---

### Getting Socket Characteristics

```c
int getsockopt(int sock, int level, int optname, void *optval, socklen_t *optlen);
```

`getsockopt()` is used for retrieving socket characteristics specified by the level and option arguments.


### Setting Socket Characteristics

```c
int setsockopt(int sock, int level, int optname, const void *optval, socklen_t optlen);
```

`setsockopt()` is used for specifying socket characteristics on the particular level, providing option and value as arguments.


<br>

### Usage Example:

```c
int state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void *) &sock_type, &optlen)

// socket option (type) is stored in sock_type (TCP = 1, UDP = 2)
```