from socket import*
serverName="127.0.0.1"
serverPort=22000
clientSocket=socket(AF_INET,SOCK_STREAM)
clientSocket.connect((serverName,serverPort))
sentence=input("enter the name of the file:")
clientSocket.send(sentence.encode())
filecontents=clientSocket.recv(1024).decode()
print('From server:',filecontents)
clientSocket.close()
