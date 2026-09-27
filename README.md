# HTTP server

Simple HTTP server in C

> Try to use man instead of online tutorial as much as possible.
> Sources:
> - <https://man7.org/linux/man-pages/man2/bind.2.html>
> - <https://thecodeforge.io/c-cpp/c-networking-sockets/>

## Client

Simulate client using netcat `nc` for now.

The server as it is, listens on localhost, port 3000.

Send data to it like this:

```bash
echo -en "ping" | nc localhost 3000
```

It will respond back (max 9 chars otherwise, it crashes for now)