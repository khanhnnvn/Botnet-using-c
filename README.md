# Botnet-using-c
Botnet using c
I will consider you comfortable with client-server programming in C, using Berkeley UNIX standards. If you are not, please refer to the series of articles on socket API.

The idea of my management system was inspired by botnets (you’re right, the very technology used by crackers to DDoS websites). A botnet is nothing but a group of infected computers controlled by the cracker using a command-and-control channel to perform various tasks, which may be to DDoS a website or to click advertisements for the cracker’s profit. For more information on botnets, please refer to the various DDoS articles published earlier.

Let us now design and develop a system that works like a botnet (a bot or client, and a server). Our server will be responsible for managing clients and issuing commands to clients or bots, whereas the bot will receive commands and execute them on each host. We need the following tools for development:

GNU C compiler
Any text editor
Any Linux distribution
For details on this, please refer to the above mentioned series of articles on socket API. We will alter the code of the chat client and insert the following instructions below the point where the message is received — code that will execute the following:

system (command_name_and _arguments);
Because our client code takes the port number and IP address of the server machine as arguments, for simplicity, let us create a shell script file named start.sh with the following code:

. /client -p port_no ip_of_server
Deploying

First, let us run the server program, which will listen on port 7400 (you can change this in the code). Now, when all the other systems boot, they will automatically run the client program with root privileges (since we have added the client initiator script start.sh into startup applications). As we are demonstrating the system, let the bot run in the foreground — but in real deployment scenarios, it will be run as a daemon process.

As the client executes a run, it will automatically connect to the server and wait for commands. When it receives commands, it will execute them. Figure 1 shows the server start up and issue commands (here, I named it iLinuxBot).

http://opensourceforu.efytimes.com/2012/05/ilinuxbot-designing-botnets-to-manage-linux-clients/
