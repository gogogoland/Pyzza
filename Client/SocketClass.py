import socket, select, sys

class Socket:
	#init socket
	def __init__(self, ip, port, team):
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.ip = ip
		self.port = port
		self.team = team

		#connect to the server
		self.connectServer()

	def connectServer(self):
		try:
			self.sock.connect((self.ip, self.port))
		except socket.error as err:
			print(err)
			sys.exit()


	#Close connection
	def __del__(self):
		self.sock.close()
