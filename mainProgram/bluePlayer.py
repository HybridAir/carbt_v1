#!/usr/bin/env python

# Dependencies:
# sudo apt-get install -y python-gobject

import time
import signal
import dbus
import dbus.service
import dbus.mainloop.glib
import gobject
import threading

import musicPage

SERVICE_NAME = "org.bluez"
AGENT_IFACE = SERVICE_NAME + '.Agent1'
ADAPTER_IFACE = SERVICE_NAME + ".Adapter1"
DEVICE_IFACE = SERVICE_NAME + ".Device1"
PLAYER_IFACE = SERVICE_NAME + '.MediaPlayer1'
TRANSPORT_IFACE = SERVICE_NAME + '.MediaTransport1'

class BluePlayer():
	bus = None
	mainloop = None
	device = None
	deviceAlias = None
	player = None
	connected = None
	state = None
	status = None
	track = []
	conStatus = False

	def __init__(self):
		"""Specify a signal handler, and find any connected media players"""
		gobject.threads_init()
		dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
		
		self.position = None

		#get the system dbus into a var
		self.bus = dbus.SystemBus()

		#this is to notify playerHandler of any relevant activity in the org.bluez dbus
		self.bus.add_signal_receiver(self.playerHandler,
				bus_name="org.bluez",
				dbus_interface="org.freedesktop.DBus.Properties",
				signal_name="PropertiesChanged",
				path_keyword="path")

		self.findPlayer()
		self.updateDisplay()

	#starts the thread
	def start(self):
		"""Start the BluePlayer by running the gobject Mainloop()"""
		self.mainloop = gobject.MainLoop()
		try:
			self.mainloop.run()
		except Exception as ex:
			logging.error("friuck {}".format(ex))

	#cleanly ends the thread
	def end(self):
		"""Stop the gobject Mainloop()"""
		if (self.mainloop):
			self.mainloop.quit();

	#finds the player and "connects" to it
	def findPlayer(self):
		"""Find any current media players and associated device"""
		
		#look for any players in org.bluez on the dbus
		manager = dbus.Interface(self.bus.get_object("org.bluez", "/"), "org.freedesktop.DBus.ObjectManager")
		
		#get the response out?
		objects = manager.GetManagedObjects()

		player_path = None
		
		#for each path and interface in the response we got
		for path, interfaces in objects.iteritems():
			if PLAYER_IFACE in interfaces:		#if that interface is a player
				player_path = path				#get its path out, we need it to communicate with the player
				break

		if player_path:			#if we found a valid player from before
			self.connected = True
			self.getPlayer(player_path)		#"connect" to that player or something
			
			#get all the properties that the player is willing to expose to us into a dictionary?
			player_properties = self.player.GetAll(PLAYER_IFACE, dbus_interface="org.freedesktop.DBus.Properties")
			
			#then get these two properties out if they exist
			if "Status" in player_properties:
				self.status = player_properties["Status"]
			if "Track" in player_properties:
				self.track = player_properties["Track"]
			#if "Position" in player_properties:
			#	print(player_properties["Position"])

	#I guess save the player info to some variables
	def getPlayer(self, path):
		"""Get a media player from a dbus path, and the associated device"""
		self.player = self.bus.get_object("org.bluez", path)
		device_path = self.player.Get("org.bluez.MediaPlayer1", "Device", dbus_interface="org.freedesktop.DBus.Properties")
		self.getDevice(device_path)

	def getDevice(self, path):
		"""Get a device from a dbus path"""
		self.device = self.bus.get_object("org.bluez", path)
		self.deviceAlias = self.device.Get(DEVICE_IFACE, "Alias", dbus_interface="org.freedesktop.DBus.Properties")

	#this is automagically called in the event the player sends out an update over the dbus
	def playerHandler(self, interface, changed, invalidated, path):
		"""Handle relevant property change signals"""
		iface = interface[interface.rfind(".") + 1:]
#		print("Interface: {}; changed: {}".format(iface, changed))

		if iface == "Device1":
			if "Connected" in changed:
				self.connected = changed["Connected"]
		elif iface == "MediaControl1":
			if "Connected" in changed:
				self.connected = changed["Connected"]
				if self.connected:
					self.conStatus = True
				else:
					self.conStatus = False
				
				if changed["Connected"]:
					self.findPlayer()
		elif iface == "MediaPlayer1":
			if "Track" in changed:
				self.track = changed["Track"]
				self.updateDisplay()			#if the track was just updated, update the "display"
			if "Status" in changed:
				self.status = changed["Status"]
			if "Position" in changed:
				self.position = changed["Position"]
				musicPage.setNewPosition()
	def updateDisplay(self):
		if self.player:							#if there is an active connected player
			self.conStatus = True
			#if "Artist" in self.track:
				#print(self.track["Artist"])
			#if "Title" in self.track:
				#print(self.track["Title"])
				
			if "Artist" in self.track and "Title" in self.track:
				player_properties = self.player.GetAll(PLAYER_IFACE, dbus_interface="org.freedesktop.DBus.Properties")
				if "Position" in player_properties:
					self.position = player_properties["Position"]
			
				musicPage.setNewPosition()
				musicPage.setNewTrack()
		else:
			self.conStatus = False
			print("Waiting for media player")
			
	def getConnected(self):
		return self.conStatus
		#return self.connected
			
	def getStatus(self):
		# player_properties = self.player.GetAll(PLAYER_IFACE, dbus_interface="org.freedesktop.DBus.Properties")
		# if "Status" in player_properties:
			# print(player_properties["Status"])
		return self.status
		
	#returns the current track's duration in seconds
	def getDuration(self):
		seconds = self.track["Duration"] / 1000
		return int(round(seconds))
		
	#returns the current track's position in sections from the beginning
	def getPosition(self):
		if self.position == None:
			player_properties = self.player.GetAll(PLAYER_IFACE, dbus_interface="org.freedesktop.DBus.Properties")
			if "Position" in player_properties:
				self.position = player_properties["Position"]
		
		return self.position / 1000
		# player_properties = self.player.GetAll(PLAYER_IFACE, dbus_interface="org.freedesktop.DBus.Properties")
		# if "Position" in player_properties:
			#print("pos: " + str(player_properties["Position"]) + " " + str(round(player_properties["Position"] / 1000)))
			# seconds = player_properties["Position"] / 1000
			# return int(round(seconds))
		
	
	#returns the current track's position in seconds
	#def getPosition(self):
			
	def getTitle(self):
		return self.track["Title"]
	
	def getArtist(self):
		return self.track["Artist"]

	def next(self):
		self.player.Next(dbus_interface=PLAYER_IFACE)
		# player_properties = self.player.GetAll(PLAYER_IFACE, dbus_interface="org.freedesktop.DBus.Properties")
		# if "Position" in player_properties:
			# self.position = player_properties["Position"]
			# musicPage.setNewPosition()

	def previous(self):
		# player_properties = self.player.GetAll(PLAYER_IFACE, dbus_interface="org.freedesktop.DBus.Properties")
		# if "Position" in player_properties:
			# self.position = player_properties["Position"]
			# musicPage.setNewPosition()
			
		# if self.position > 500:
			# self.player.Previous(dbus_interface=PLAYER_IFACE)
			
		self.player.Previous(dbus_interface=PLAYER_IFACE)

	def play(self):
		self.player.Play(dbus_interface=PLAYER_IFACE)

	def pause(self):
		self.player.Pause(dbus_interface=PLAYER_IFACE)