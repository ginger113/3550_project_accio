# Project-1-Accio

Chris Fuchs, 73320998

The client creates a socket, connects to the server, and then transmits the data.
The server creates a socket, binds an address to the socket, listens to the socket, accepts a connections, and the receives the data.

One of the problems I ran into was sending and receiving multiple files. I was able to solve it using loops. I also had to increase the backlog on my listen().
I had an issue receiving data for a long time. Eventually I found a typo in my code that was the cause of the issue.
I had trouble sending data to "localhost". I just went ahead and hard coded in "127.0.0.1" to solve this issue.

