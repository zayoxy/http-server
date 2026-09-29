# HTTP server

Simple HTTP server in C

> Try to use man instead of online tutorial as much as possible.
> Sources:
>
> - <https://man7.org/linux/man-pages/man2/bind.2.html>
> - <https://thecodeforge.io/c-cpp/c-networking-sockets/>

## Client

Simulate client using netcat `nc` for now.

The server as it is, listens on localhost, port 3000.

Send data to it like this:

```bash
echo -e "GET / HTTP/1.1\nHost: localhost\n\n" | nc localhost 3000
```

It will respond back a pre-made valid http response.

> You can try it by simply accessing `localhost:3000` on any browser now.
