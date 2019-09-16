from socket import*
serverName="127.0.0.1"
serverPort=23000
serverSocket=socket(AF_INET,SOCK_DGRAM)
serverSocket.bind((serverName,serverPort))
print("the server is ready to recieve")
while 1:
    sentence,clientAddress=serverSocket.recvfrom(2048)
    file=open(sentence,"r")
    l=file.read(2048)
    serverSocket.sendto(bytes(l),clientAddress)
    print("sent back to client")
file.close()
