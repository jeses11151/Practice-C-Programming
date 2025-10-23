 import socket
 import time
 s = socket.socket()
 port = 8080
 s.connect(('192.168.225.1', port))
 try:
    while True:
        # Take user input for the message
        message = raw_input("Enter the message to send (type 'exit' to quit): ")
                
        print("Sending: {}".format(message))
        s.sendall(message.encode())
        
        if message.lower() == 'exit':
            print("Exiting and closing the connection.");
            break;
        
        # Look for the response
        data = s.recv(1024)
        print("Received: {}".format(data.decode()))
        
        # Optionally, sleep for a short time to prevent flooding the server
        time.sleep(1)  # Adjust the sleep time as needed
        
finally:
    # Close the socket
    s.close()