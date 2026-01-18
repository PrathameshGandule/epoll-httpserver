### Currently learning about epoll and it's neat use from tutorials and online blogs
### Have some example like server.c in tempfiles which send a http1.1 response to any request made to server
### And a simple tcp echo server

Use this command to compile
```bash
gcc Networking/HTTPRequest.c Networking/Server.c Networking/test.c \
-I DataStructures \
-L DataStructures \
-lDataStructures \
-o server
```