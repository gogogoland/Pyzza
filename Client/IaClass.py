from SocketClass import Socket
from threading import Thread
import sys, select, socket, random, signal, os

class IA(Thread):

	def __init__(self, ip, port, team):
		
		Thread.__init__(self)

		#Init of IA Class
		self.data = {
			"Name" : "",
			"Action" : [0, 0, 0],
			"X" : 0,
			"Y" : 0,
			"Cleft" : 0,
			"Eggs" : 0,
			"Team" : None,
			"Mate" : [],
			"Lead" : True,
			"Invoc" : False,
			'IsHungry' : True,
			"Hung" : 1,
			"Inv" : [0, 0, 0, 0, 0, 0, 0],
			"Rsc" : {},
			"Friend" : [],
			"Move" : [],
			"Level" : 0,
			"InvocName" : None,
			"Levelneed" : [ [1, 0, 0, 0, 0, 0, 0],
							[1, 1, 1, 0, 0, 0, 1],
							[2, 0, 1, 0, 2, 0, 1],
							[1, 1, 2, 0, 1, 0, 3],
							[1, 2, 1, 3, 0, 0, 3],
							[1, 2, 3, 0, 1, 0, 5],
							[2, 2, 2, 2, 2, 1, 5],
							[0, 0, 0, 0, 0, 0, 0]],
		}

		#All ressources for Zappy...
		self.ressources = [
			"linemate",
			"deraumere",
			"sibur",
			"mendiane",
			"phiras",
			"thystame",
			"nourriture"
		]

		#Broadcast msg.
		self.Bmsg = [
			"I'm your new lord.",
			"need to eat.",
			"Invocation.",
			"Help ! ~",
			"list of player ",
			"change from ",
			"here for invocation : ",
			"Stop ritual",
			"My master need me ?"
		]

		self.Mcmd = [
			"message",
			"elevation en cour"
		]

		self.Sate = 756*9/9#1008
		self.Eat = 126
		self.data['Team'] = team
		self.data['Name'] = str(random.randint(10000000,99999999))

		#All Commands
		self.cmd = [
			"avance\n",
			"droite\n",
			"gauche\n",
			"voir\n",
			"inventaire\n",
			"prend ",
			"pose ",
			"expulse\n",
			"broadcast " + self.data['Team'] + " ",
			"incantation\n",
			"fork\n",
			"connect_nbr\n",
			"mange\n"
		]

		#Need it for br list of player after someone take lead
		self.act = False

		self.cmd_time = [
				7,
				7,
				7,
				7,
				1,
				7,
				7,
				7,
				7,
				300,
				42,
				0,
				7
		]

		#Add IA thought time
		self.cmd_time_late = 15#14.5

		#Time action if fail
		self.time_acti = 0
		self.time_left = 0
		self.time_name = None

		#Last result of view command
		self.view_data = ""

		#Limit number of necessary player
		self.limitThread = 8

		#Limit of level
		self.limitLevel = 8

		#Bienvenue message and other things
		self.welcome = 0

		#connect to the server...
		self.client = Socket(ip, port, team)

		#Send Team
		self.client.sock.send((self.data['Team'] + '\n').encode())
		

	#First connection of the client and creation of new client connection
	def connect_fct(self, msg):
		if self.data['Action'][2] <= 0:
			self.data['Action'][1] += self.data['Action'][2]

			#Present Himself
			if self.data['Action'][1] == 0:
				self.client.sock.send((self.cmd[8] + self.data['Name'] + " " + self.Bmsg[0] + '\n').encode())
				print(str(self.data['Name']) + ", send (" + str(self.data['Hung']) + ") : " + self.cmd[8] + str(self.data['Name']) + " " + self.Bmsg[0])
				self.data['Hung'] -= self.cmd_time[8] + self.cmd_time_late

			#Get avaible connection
			elif self.data['Action'][1] == 3:
				self.client.sock.send(self.cmd[11].encode())
				print(str(self.data['Name']) + ", send (" + str(self.data['Hung']) + ") : " + self.cmd[11], end='')
				self.data['Hung'] -= self.cmd_time[11] + self.cmd_time_late
				self.data['Action'][2] = 1

			#Connect other client
			elif self.data['Action'][1] == 4:
				new_thread = IA(self.client.ip, self.client.port, self.client.team)
				new_thread.start()
				self.data['Action'][1] = 1 + (self.data['Eggs'] > 0)
				self.data['Eggs'] -= 1

			#Fork
			elif self.data['Action'][1] > 0:
				self.client.sock.send(self.cmd[10].encode())
				print(str(self.data['Name']) + ", send (" + str(self.data['Hung']) + ") : " + self.cmd[10], end='')
				self.data['Hung'] -= self.cmd_time[10] + self.cmd_time_late
				self.data['Eggs'] += 1
			self.data['Action'][2] = 1
			self.data['Action'][1] += 1
		elif msg == None:
			return
		elif "ok" in msg and self.Mcmd[0] not in msg:
			self.data['Action'][2] = 0
		else:
			message = msg.replace("\n", "")
			if message.isdigit():
				self.data['Action'][2] = -1
				if int(message) > 0:
					self.data['Action'][2] = 0

	#Check if player is still alive
	def amIAlive(self, msg):
		return 0 if msg == "mort" else 1

	#Get player's name of same team
	def getMate(self, msg):
		if msg != None and self.Bmsg[4] in msg and self.data['Team'] in msg:
			allnames = msg.split()
			#if self.data['Name'] in allnames[3]:
			#	Old_name = self.data['Name']
			#	while self.data['Name'] in allnames[3] or self.data['Name'] in self.data['Mate']:
			#		self.data['Name'] = str(random.randint(10000000,99999999))
			#	if Old_name in self.data['Mate']:
			#		self.data['Mate'].remove(Old_name)
			#	self.client.sock.send((self.cmd[8] + self.data['Name'] + " " + self.Bmsg[5] + Old_name + ' \n').encode())
			#	self.data['Hung'] -= self.cmd_time[8] + self.cmd_time_late
			#for var in msg.split():
			#	if not var in self.data['Mate'] and var.isdigit():
			#		self.data['Mate'].append(var)

	#Server message for command send (for multithread / other client connection)
	def	handleData(self, msg):
		if self.data['Action'][0] == 0:
			self.connect_fct(msg)
		if len(self.data['Mate']) + 1 >= self.limitThread and self.data['Lead'] == True and (self.data['Action'][0] != 0 or self.data['Action'][1] < 1 or self.data['Action'][2] == 0):
			self.data['Lead'] = False
			self.data['Action'][0] = -1
			self.data['Action'][1] = 0 if self.data['Action'][2] != 0 else -1

		if (msg == None):
			return

		if self.Bmsg[0] in msg and self.data['Team'] in msg:
			self.act = True
			if self.data['Lead'] == True:
				self.data['Lead'] = False
				self.data['Action'][0] = -1
				self.data['Action'][1] = 0 if self.data['Action'][2] != 0 else -1

		if self.Bmsg[4] in msg and self.data['Team'] in msg:
			#TODO Add new name delete old name
			for var in msg.split():
				if var.isdigit() and len(str(var)) == 8 and var not in self.data['Mate']:
					self.data['Mate'].append(var)
			#print("MATE = " + str(self.data['Mate']))
			#print("MSG = " + str(msg))

		if self.data['Action'][0] < 0 and self.act == True:
			ami = ''
			for var in self.data['Mate']:
				ami += ' ' + var
			#print('[AMI] :' + ami)
			self.client.sock.send((self.cmd[8] + self.data['Name'] + " " + self.Bmsg[4] + " " + ami + '\n').encode())
			print(str(self.data['Name']) + ", send (" + str(self.data['Hung']) + ") : " + self.cmd[8] + str(self.data['Name']) + " " + self.Bmsg[4] + " " + ami)
			self.data['Hung'] -= self.cmd_time[8] + self.cmd_time_late
			self.data['Action'][0] = 1
			self.act = False

	#Set path to follow from X, Y coord
	def getPath(self):
		#print("X = " + str(self.data['X']) + ", Y = " + str(self.data['Y']))
		turn = 0
		while (self.data['Y'] > 0):
			self.data['Move'].append(0)
			self.data['Y'] -= 1
			#1st right
		if (self.data['X'] > 0):
			turn += 1
			self.data['Move'].append(1)
			while (self.data['X'] > 0):
				self.data['Move'].append(0)
				self.data['X'] -= 1
		elif (self.data['X'] < 0):
			#1st left
			turn -= 1
			self.data['Move'].append(2)
			while (self.data['X'] < 0):
				self.data['Move'].append(0)
				self.data['X'] += 1
		if (self.data['Y'] < 0):
			#2nd right or left.
			if turn < 0:
				self.data['Move'].append(2)
			while turn >= 0 and turn < 2:
				self.data['Move'].append(1)
				turn += 1
			while (self.data['Y'] < 0):
				self.data['Move'].append(0)
				self.data['Y'] += 1

	#Move player from path saved
	def move(self):
		if self.data['Action'][0] < 0 and len(self.data['Move']) > 0:
			move_cmd = self.data['Move'].pop(0)
			self.client.sock.send(self.cmd[move_cmd].encode())
			print(str(self.data['Name']) + ", send (" + str(self.data['Hung']) + ") : " + self.cmd[move_cmd], end='')
			self.data['Hung'] -= self.cmd_time[move_cmd] + self.cmd_time_late
			self.data['Action'][0] = 1

	def getViewData(self, msg):
		#if none current action then  send command voir
		if self.data['Action'][0] < 0:
			if self.data['InvocName'] is not None:
				return
			self.view_data = ""
			#~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~#
			self.client.sock.send((self.cmd[3] + self.cmd[4]).encode())
			print(str(self.data['Name']) + ", send (" + str(self.data['Hung']) + ") : " + self.cmd[3] + self.cmd[4], end='')
			#self.client.sock.send((self.cmd[3] + self.cmd[8] + self.data['Name'] + ' ' + self.Bmsg[9] + '\n').encode())
			#self.client.sock.send((self.cmd[3]).encode())
			#self.client.sock.send((self.cmd[8] + self.data['Name'] + ' ' + self.Bmsg[9] + '\n').encode())
			#print(str(self.data['Name']) + ", send (" + str(self.data['Hung']) + ") : " + self.cmd[3] + self.cmd[8] + str(self.data['Name']) + ' ' + self.Bmsg[9])
			#print(str(self.data['Name']) + ", send (" + str(self.data['Hung']) + ") : " + self.cmd[3], end='')
			#print(str(self.data['Name']) + ", send (" + str(self.data['Hung']) + ") : " + self.cmd[8] + str(self.data['Name']) + ' ' + self.Bmsg[9])
			#~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~#
			self.data['Hung'] -= self.cmd_time[3] + self.cmd_time_late
			#self.data['Hung'] -= self.cmd_time[8] + self.cmd_time_late
			self.data['Hung'] -= self.cmd_time[4] + self.cmd_time_late
			self.data['Action'][0] = 7

		if self.data['Action'][0] == 7 and msg != None and "message" not in msg and "|" in msg:
			self.view_data += msg

	def IsInventory(self, msg):
		isit = True
		i = 0
		while i < 7 and isit is True:
			#isit = True if self.ressources[i] in msg else False
			if self.ressources[i] not in msg:
				isit = False
			i += 1
		#isit = isit if '|' not in msg and self.Mcmd[0] not in msg else False
		if ('|' in msg or self.Mcmd[0] in msg) and (',' and self.Mcmd[0]) not in msg:
			isit = False
		if isit is True:
			i = 0
			var = msg.split(',')
			while i < 7 and i < len(var):
				cur = var[i].split(' ')
				if i == 0:
					if cur[1].isdigit():
						self.data['Inv'][6] = int(cur[1])
					elif cur[2].isdigit():
						self.data['Inv'][6] = int(cur[2])
				elif cur[1].isdigit():
					self.data['Inv'][i - 1] = int(cur[1])
				elif cur[2].isdigit():
					self.data['Inv'][i - 1] = int(cur[2])
				i += 1
		return isit

	def UseViewData(self):
		var = self.view_data
		self.view_data = ""

		#print(str(self.data['Name']) + ", view result : " + var)

		self.data['Action'][0] = -1
		if self.data['InvocName'] is not None:
			print(str(self.data['Name']) + ", DON'T USE VIEWDATA, GOT NAME : " + str(self.data['InvocName']))
			return
		tmp_ressource = 0
		nbr_ressource = [0, 0]
		box = []
		liste = {
			self.ressources[0] : 0,
			self.ressources[1] : 0,
			self.ressources[2] : 0,
			self.ressources[3] : 0,
			self.ressources[4] : 0,
			self.ressources[5] : 0,
			self.ressources[6] : 0,
			"joueur" : 0,
			}

		index = 0
		sp = var.split('|')
		while (index < len(sp)):
			box.append(liste.copy())
			for var in sp[index].split():
				if var in self.ressources:
					box[index][var] += 1
			index += 1

		nbr_ressource[0] = tmp_ressource
		nbr_ressource[1] = index
		#Check best box of ressources
		for index, var in enumerate(box):
			tmp_ressource, irsc = 0, 0
			#Check current box by ressources
			while irsc < 8:
				#If ressources for incantation, check if it's for current level + there not enough ressources in inventory
				if irsc < 6 and self.data["Levelneed"][self.data["Level"]][irsc] > self.data["Inv"][irsc] and box[index][self.ressources[irsc]] > 0:
					if box[index][self.ressources[irsc]] > self.data["Levelneed"][self.data["Level"]][irsc] - self.data["Inv"][irsc]:
						box[index][self.ressources[irsc]] = self.data["Levelneed"][self.data["Level"]][irsc] - self.data["Inv"][irsc]
					tmp_ressource += box[index][self.ressources[irsc]]
				#If food, check if there is not enough food in inventory
				elif irsc == 6:
					if self.data['Hung'] + ((self.data['Inv'][0] - 10) * self.Eat) + (self.Eat * box[index][self.ressources[irsc]]) > self.Sate:
						box[index][self.ressources[irsc]] -= (self.data['Hung'] + ((self.data['Inv'][0] - 10) * self.Eat) + (box[index][self.ressources[irsc]] * self.Eat) - self.Sate) / self.Eat
					if tmp_ressource > 0 and box[index][self.ressources[irsc]] > 0:
						tmp_ressource += box[index][self.ressources[irsc]]
					elif box[index][self.ressources[irsc]] > 0:
						tmp_ressource = 1
				#Else, no need for another ressources
				elif irsc < 6:
					box[index][self.ressources[irsc]] = 0
				irsc += 1
			#Compare and get current box from old one saved
			if tmp_ressource > nbr_ressource[0]:
				nbr_ressource[0] = tmp_ressource
				nbr_ressource[1] = index

		#Get random coord if there are none good box
		if nbr_ressource[0] == 0:
			self.data['X'] = random.randint(-5,5)
			self.data['Y'] = random.randint(-5,5)
			#print(str(self.data['Name']) + " NOT FOUND: X=" + str(self.data['X']) + " Y=" + str(self.data['Y']))
		#Get list of ressource and coord of box
		else:
			#print("Nbr_rsc : " + str(nbr_ressource[1]))
			self.data['Rsc'] = box[nbr_ressource[1]]
			y = 0
			x = 0
			while y <= self.data['Level'] + 1 and nbr_ressource[1] > 0:
				if nbr_ressource[1] > 0:
					nbr_ressource[1] -= 1
					y += 1
				x = -1 * y
				while x < y and nbr_ressource[1] > 0:
					nbr_ressource[1] -= 1
					x += 1
			self.data['X'] = x
			self.data['Y'] = y
			#print(str(self.data['Name']) + " HAS FOUND: X=" + str(self.data['X']) + " Y=" + str(self.data['Y']) + " RSC:", end='')
			i = 0
			while i < 7:
				print(" " + self.ressources[i] + ":" + str(self.data['Rsc'][self.ressources[i]]), end='')
				i += 1
			print('\n')

		self.getPath()

	#Function for main invocator
	def canIncant(self, msg):
		#Stop at max level
		if self.data['Level'] == self.limitLevel:
			return

		#check current messsage for incantator
		if msg != None:
			for var in msg.splitlines():
				if self.data['InvocName'] == self.data['Name'] and var != None and self.data['Team'] in var and self.Bmsg[6] in var and var.split()[3] not in self.data['Friend'] and self.data['Name'] in var and len(self.data['Friend']) < self.data['Levelneed'][self.data['Level']][6]:
					#add new acolyte
					self.data['Friend'].append(var.split()[3])

		#Check if there are no other incantator
		if self.data['IsHungry'] == False and self.data['Action'][0] < 0:
			#Check ressources for ritual
			incantation = False
			if self.data['InvocName'] == None:
				incantation = True
				irsc = 0
				while irsc < 6 and incantation:
					if self.data["Levelneed"][self.data["Level"]][irsc] > self.data["Inv"][irsc]:
						incantation = False
					irsc += 1
			if incantation == True or self.data['InvocName'] == self.data['Name']:
				#If there are enough acolyte, ...
				if len(self.data['Friend']) >= self.data['Levelneed'][self.data['Level']][6]:
					#print("MY LEVEL IS: ", self.data['Level'])
					if self.data['Levelneed'][self.data['Level']][6] <= 0:
						self.data['InvocName'] = self.data['Name']
						#print("0: " + self.data['InvocName'] + " is his own master.")
					#Prevent them
					if self.data['Action'][2] < 5 and self.data['Levelneed'][self.data['Level']][6] > 0:
						self.data['Action'][2] += 1
						self.ComeAtMe()
					#Cast spell
					else:
						self.client.sock.send((self.cmd[9]).encode())
						print(str(self.data['Name']) + ", send (" + str(self.data['Hung']) + ") : " + self.cmd[9], end='')
						self.data['Action'][0] = 6
				#Or look for acolyte
				else:
					self.data['Action'][2] = 0
					self.ComeAtMe()

	def Casting(self, msg):
		if msg is not None:
			if self.Mcmd[1] in msg and self.Mcmd[0] not in msg:
				self.data['Hung'] -= self.cmd_time[9] + self.cmd_time_late
				self.data['Action'][0] = 6
				self.time_left = 0
				#print("\n\n\n\n\n\n\nCASTINGPOPOIPPO\n\n\n\n\n\n\n\n")
			#if self.time_left > 0 and self.data['Action'][0] <= 0:
			#	self.time_left = 0
			#	self.time_name = None
			#elif self.time_name is None and self.data['Team'] in msg:
			#	self.time_name = msg.split()[3]
			#elif self.time_name == msg and self.data['Team'] in msg:
			#	self.time_left += self.cmd_time[9]
			#if self.time_acti >= 0 and self.time_acti <= 12 and self.time_left > (self.cmd_time[self.time_acti] / (self.cmd_time[9] + 1)):
			#	self.data['Action'][0] *= -1

	#Come At me friends
	def ComeAtMe(self):
		friendly = ' '
		for var in self.data['Friend']:
			friendly += var + ' '
		self.client.sock.send((self.cmd[8] + self.data['Name'] + ' ' + str(self.data['Level']) + ' ' + self.Bmsg[3] + friendly + '\n').encode())
		print(str(self.data['Name']) + ", send (" + str(self.data['Hung']) + ") : " + self.cmd[8] + str(self.data['Name']) + ' ' + str(self.data['Level']) + ' ' + self.Bmsg[3] + friendly + '')
		self.data['Hung'] -= self.cmd_time[8] + self.cmd_time_late
		self.data['Action'][0] = 5

	def ThereAreIncant(self, msg):
		if self.data['Hung'] > 0 and msg != None and self.data["Team"] in msg and self.Bmsg[3] in msg and str(self.data['Level']) == msg.split(' ')[4] and (self.data['InvocName'] == msg.split(' ')[3] or self.data['InvocName'] is None):
			Lv = self.data['Levelneed'][self.data['Level']][6] - len(msg.split('~')[1].split())

			#First listen of caster
			if self.data['InvocName'] is None:
				self.data['InvocName'] = msg.split(' ')[3]
				#print("1: " + self.data['InvocName'] + " is the master of " + self.data['Name'] + ".")
				self.data['X'] = 0
				self.data['Y'] = 0
				self.data["Rsc"] = {}
				self.data['Move'] = []
				#print("My new master is " + self.data['InvocName'])
				if self.data['Action'][0] == 5:
					self.data['Action'][2] = -1

#			#If secound call of caster, follow him
#			elif self.data['Action'][2] == 1:
#				self.data['Action'][2] = 2

			#If can move, go after it
			if self.data['Action'][0] < 0 and self.data['Move'] == []:
				IncantMsgData = msg.split(',')[0].split()[1]
				#print("I can serve my master")

				#If the casting is full
				if self.data['Name'] not in msg and Lv < 1:
					self.data['InvocName'] = None
					#print("2: " + self.data['Name'] + " has no master.")
					#print("My master doesn't need me for his spell")

				#If not close of the caster, move to him
				elif IncantMsgData != "0":
					self.data['X'] = 0
					self.data['Y'] = 0
					self.data['Move'] = []
					self.data['Action'][2] = 0
					if IncantMsgData in ["1", "2", "8"]:
						self.data['Y'] = 1
					elif IncantMsgData in ["3", "4"]:
						self.data['X'] = -1
					elif IncantMsgData in ["6", "7"]:
						self.data['X'] = 1
					elif IncantMsgData in ["5"]:
						self.data['Y'] = -1
					self.getPath()
					#print("Travel to Master")

				#Else if cast spell, lvl up
				elif self.data['Name'] in msg and Lv < 1:
					#countdown of incantator
					self.data['Move'] = []
					self.time_left += 1
					#print("I'm at the will of my master")

				#Wait for other acolytes
				elif self.data['Name'] in msg and Lv > 0:
					self.data['Move'] = []
					self.data['Hung'] -= self.cmd_time[8] + self.cmd_time_late
					#print("My master need more help")

				#Send present to caster
				elif self.data['Name'] not in msg and Lv > 0:
					self.data['Move'] = []
					self.client.sock.send((self.cmd[8] + self.data['Name'] + ' ' + self.Bmsg[6] + self.data['InvocName'] + '\n').encode())
					print(str(self.data['Name']) + ", send (" + str(self.data['Hung']) + ") : " + self.cmd[8] + str(self.data['Name']) + ' ' + self.Bmsg[6] + self.data['InvocName'])
					self.data['Hung'] -= self.cmd_time[8] + self.cmd_time_late
					self.data['Action'][0] = 1
					#print("Master, I'm here !")

		#If caster is going to eat
		elif msg != None and self.data["Team"] in msg and self.Bmsg[7] in msg and self.data['InvocName'] is not None and self.data['InvocName'] in msg:
			self.data['InvocName'] = None
			#print("3: " + self.data['Name'] + " has no master.")
			self.data['Move'] = []
			self.data['X'] = 0
			self.data['Y'] = 0
			if self.time_left >= 5:
				self.data['Hung'] -= self.cmd_time[9] + self.cmd_time_late
			self.data['Hung'] -= (self.time_left * (self.cmd_time[8] + self.cmd_time_late))
			if self.data['Action'][0] == 6:
				self.data['Action'][0] = -1
			self.time_left = 0

#		#Check if the current caster was the first
#		elif self.data['InvocName'] is not None and self.data['Action'][0] < 0 and self.data['Action'][2] == 0:
#			self.client.sock.send((self.cmd[8] + self.data['Name'] + self.Bmsg[8] + self.data['InvocName'] +  '\n').encode())
#			self.data['Hung'] -= self.cmd_time[8] + self.cmd_time_late
#			self.data['Action'][2] = 1

#		#If the current caster was not the first one
#		elif self.data['InvocName'] is not None and self.data['Action'][0] < 0 and self.data['Action'][2] == 1:
#			self.data['InvocName'] = None
#			#print("3.5: " + self.data['Name'] + " has no master.")
#			self.data['Move'] = []
#			self.data['X'] = 0
#			self.data['Y'] = 0
#			if self.data['Action'][0] == 6:
#				self.data['Action'][0] = -1
#			self.data['Action'][2] = 0

	#Change current action to look for food
	def Hungry(self):
		#set current limit of sate stade
		lim_food = 0
		if self.data['IsHungry'] == True:
			lim_food = self.Sate
		#if is hungry (< 0) eat to be sate
		if self.data['Hung'] >= lim_food:
			self.data['IsHungry'] = False
			self.Sate = 756

		#prevent acolyte if master is going to eat
		if self.data['InvocName'] == self.data['Name'] and self.data['Action'][0] < 0 and self.data['Hung'] < 0:
			if len(self.data['Friend']) < self.data['Levelneed'][self.data['Level']][6]:
				self.client.sock.send((self.cmd[8] + self.data['Name'] + " " + self.Bmsg[7] + '\n').encode())
				print(str(self.data['Name']) + ", send (" + str(self.data['Hung']) + ") : " + self.cmd[8] + str(self.data['Name']) + " " + self.Bmsg[7])
				self.data['Hung'] -= self.cmd_time[8] + self.cmd_time_late
				self.data['Action'][0] = 1
				self.data['InvocName'] = None
				#print("4: " + self.data['Name'] + " has no master.")
				self.data['Friend'] = []
			else:
				return

		#eat
		if self.data['Action'][0] < 0 and self.data['Hung'] < lim_food:
			self.data['IsHungry'] = True
			if self.data['Inv'][6] > 0:
				#print("HUNGRY="+str(self.data['Hung']))
				self.data['Action'][0] = 2
				self.client.sock.send((self.cmd[12]).encode())
				print(str(self.data['Name']) + ", send (" + str(self.data['Hung']) + ") : " + self.cmd[12], end='')
				self.data['Hung'] -= self.cmd_time[12] + self.cmd_time_late
			#if self.data['Action'][0] > 0:
				#print("ACTION1: " + str(self.data['Action'][0]))

	#Check pos and remain ressource on box, take it if so
	def TakeRessource(self):
		if self.data['Action'][0] < 0 and self.data['X'] == 0 and self.data['Y'] == 0 and self.data["Rsc"] != {}:
			if ("joueur" in self.data["Rsc"]):
				del self.data["Rsc"]["joueur"]
			index = len(self.data["Rsc"]) - 1
			while index >= 0 and self.data["Rsc"][self.ressources[index]] <= 0:
				index -= 1
			if index >= 0:
				self.client.sock.send((self.cmd[5] + self.ressources[index] + '\n').encode())
				print(str(self.data['Name']) + ", send (" + str(self.data['Hung']) + ") : " + self.cmd[5] + self.ressources[index])
				self.data["Rsc"][self.ressources[index]] -= 1
				self.data['Hung'] -= self.cmd_time[5] + self.cmd_time_late
				self.data['Action'][0] = 4
				self.data['Action'][1] = 0
				self.data['Action'][2] = index
			else:
				self.data["Rsc"] = {}
			#print("Ressources: " + str(self.data['Rsc']))

	def	handleMsgAction(self, msg):
		if msg == None:
			return
		#Command answer
		elif self.data['Action'][0] > 0 and self.data['Action'][0] < 8 and "ok" in msg or "ko" in msg and not "message" in msg:
			self.data['Action'][0] *= -1
			#print(str(self.data['Name']) + " HUNGRY: " + str(self.data['Hung']))
			if "ko" in msg:
				self.data['Action'][1] = -1
			else:
				self.data['Action'][1] = 1
			#print(str(self.data['Name']) + " ACTION: " + str(self.data['Action'][0]))
		elif self.data['Action'][0] == 4 and self.data['Team'] in msg and self.time_name is None and len(msg.split()) > 4:
			self.time_name = msg.split(' ')[3]
		elif self.data['Action'][0] == 4 and self.data['Team'] in msg and len(msg.split(' ')) > 3 and self.time_name == msg.split(' ')[3]:
			self.time_left += 1
		
		#Eat command
		if self.data['Action'][0] == -2:
			if self.data['Action'][1] == 1:
				self.data["Hung"] += self.Eat
				self.data['Inv'][6] -= 1
			else:
				self.data["Rsc"] = []
				self.data["Move"] = []
				self.data['Inv'][6] = 0
			self.data['Action'][0] = -1
		
		#Incant command
		elif self.data['Action'][0] == -6:
			if self.data['Action'][1] == 1:
				irsc = 0
				if self.data['InvocName'] == self.data['Name']:
					while irsc < 6:
						self.data["Inv"][irsc] -= self.data["Levelneed"][self.data["Level"]][irsc]
						irsc += 1
				self.data['Level'] += 1
			if self.data['InvocName'] == self.data['Name']:
				self.client.sock.send((self.cmd[8] + self.data['Name'] + " " + self.Bmsg[7] + '\n').encode())
				print(str(self.data['Name']) + ", send (" + str(self.data['Hung']) + ") : " + self.cmd[8] + str(self.data['Name']) + " " + self.Bmsg[7])
				self.data['Hung'] -= self.cmd_time[8] + self.cmd_time_late
				self.data['Action'][0] = 1
			self.data['InvocName'] = None
			#print("5: " + self.data['Name'] + " has no master.")
			self.data['Friend'] = []

		#Take command
		elif self.data['Action'][0] == -4 or (self.data['Action'][0] == 4 and self.time_left > 2):
				if self.data['Action'][0] == 4:
					self.data['Action'][1] = -1
					self.data['Action'][0] *= -1
				if self.data['Action'][1] == 1:
					self.data['Inv'][self.data['Action'][2]] += 1
				else:
					self.data['Action'][2] = 0
					self.data["Rsc"] = []
					self.data["Move"] = []

		#Return of acolyte calling
		#First to call help invoc
		elif self.data['Action'][0] == -5:
			if self.data['InvocName'] == None:
				self.data['InvocName'] = self.data['Name']
				#print("6: " + self.data['InvocName'] + " is his own master.")
			elif self.data['Action'][2] == -1:
				self.client.sock.send((self.cmd[8] + self.data['Name'] + " " + self.Bmsg[7] + '\n').encode())
				print(str(self.data['Name']) + ", send (" + str(self.data['Hung']) + ") : " + self.cmd[8] + str( self.data['Name']) + " " + self.Bmsg[7])
				self.data['Hung'] -= self.cmd_time[8] + self.cmd_time_late
				self.data['Action'][0] = 1
				self.data['Action'][2] = 0

		#View + inventory
		elif self.data['Action'][0] == 7 and self.IsInventory(msg):
			self.data['Action'][0] *= -1
			self.UseViewData()

	def FolloWelcome(self, msg):
		if msg is None:
			return
		if self.welcome == 0 and msg == "Bienvenue":
			self.welcome += 1
		if self.welcome == 1 and msg.isdigit():
			self.welcome += 1
		if self.welcome == 2:
			nbr_digit = 0
			table = msg.split(' ')
			for var in table:
				if var.isdigit():
					nbr_digit += 1
			if nbr_digit == 2:
				self.welcome += 1

	def	AllAction(self, msg):
		if self.welcome < 3:
			self.FolloWelcome(msg)
		else:
			self.Casting(msg)
			self.handleData(msg)
			self.handleMsgAction(msg)
			self.move()
			self.ThereAreIncant(msg)
			self.Hungry()
			self.TakeRessource()
			self.canIncant(msg)
			self.getViewData(msg)

	#Main Function
	def	run(self):

		read = []
		current_action = self.data['Action'][0]

		while True:

			#Reset Msg received from server.
			new_msg = None

			try:
				read, _, _= select.select([self.client.sock], [], [], 0)
			except (select.error, socket.timeout) as err:
				self.client.sock.close()
				break

			for cl in read:
				new_msg = cl.recv(1023).decode('ascii')

			#Check if he is dead or if he is leader.
			if new_msg != None:
				if new_msg == '':
					print("[SERVER DISCONNECTED]")
					return
				table = new_msg.split('\n')
				for msg in table:
					if msg == "":
						continue
					print(str(self.data['Name']) + ", recv (" + str(self.data['Hung']) + ") : " + msg)
					if self.amIAlive(msg) == 0:
						return
					self.AllAction(msg)
			else:
				self.AllAction(None)
			if current_action != self.data['Action'][0]:
				#print(str(self.data['Name']) + " CHANGE ACTION FROM " + str(current_action) + " TO " + str(self.data['Action'][0]))
				current_action = self.data['Action'][0]
