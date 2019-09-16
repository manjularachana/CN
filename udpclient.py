from socket import*
serverName="127.0.0.1"
serverPort=23000
clientSocket=socket(AF_INET,SOCK_DGRAM)


sentence=input("enter the file name:")
clientSocket.sendto(bytes(sentence),(serverName,serverPort))
filecontents,serverAddress=clientSocket.recvfrom(2048)
print('from server:',filecontents)
clientSocket.close()
