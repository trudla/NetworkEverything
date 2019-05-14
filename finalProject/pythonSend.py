import socket
    
UDP_IP = "10.225.25.121"
UDP_PORT = 2390
MESSAGE = "Start"
 
while True:

  MESSAGE = input()

  if MESSAGE == "exit":
    break

  print("UDP target IP:", UDP_IP)
  print("UDP target port:", UDP_PORT)
  print("message:", MESSAGE)
     
  sock = socket.socket(socket.AF_INET, # Internet
    socket.SOCK_DGRAM) # UDP
  sock.sendto(bytes(MESSAGE, "utf-8"), (UDP_IP, UDP_PORT))
