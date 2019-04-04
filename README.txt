----------------------------------------------------------------------------
Code Architecture:
Started by defining every kind of function I was going to need. Functions included handling
a message by the server, sending a message to the server, joining the server, and handling
errors. In the main function, I was able to utilize everything that was defined to make a more
clear and easier to read main file to better debug.
-------------------------------------------------------------------------------
Testing:
First we had to compile the code with our makefile. This will compile both the server and client.
Testing the code, we utilized a variety of test cases. We were alble to demonstrate the both the 
required test cases along with the bonus questions in the images on the report.
--------------------------------------------------------------------------------
Comments on the code:
James Maestro was responsible for the Client code. Recieving all messages from the server that is forwarded
from the other users. I implimented the various messages and types of messages by defining the 
individual message "ID's". 

Matthew Gaskamp was responsible for the Server code. Recieved all requests and messages from
the user and was responsible for forwarding messages and sending errror messages to the
appropriate users. 