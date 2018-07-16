#!/usr/bin/python

import lldb
import commands
import optparse
import shlex
import tempfile
import os
import sys
import re

class Sanitizer:
	def sanitize(self,line):
		return line

class AppleScriptSanitizer(Sanitizer):
	def sanitize(self,line):
		line = line.replace('\\','\\\\')
		line = line.replace('"','\\"')
		return line

class InputReader:
	def __init__(self):
		pass
	
	def read_line(self,prompt):
		print prompt,
		data = sys.stdin.readline()
		if data == '' or data == None:
			raise KeyboardInterrupt
		if (len(data) > 1):
			data = data[0:len(data)-1]
		elif (len(data) == 1):
			data = ""
		#print '"' + data + '"'
		return data

	def read_lines(self,prompt):
		while True:
			data=self.read_line(prompt)
			if not data: break
			yield data

	def read_number(self,prompt):
		while True:
			data = self.read_line(prompt)
			try:
				data_int = int(data)
				return data_int
			except:
				continue
	
	def read_yes_no(self,prompt):
		while True:
			data = self.read_line(prompt).lower()
			if data == "yes" or data == "y" or data == "true" or data == "t" or data == "1" or data == "+":
				return True
			if data == "no" or data == "n" or data == "false" or data == "f" or data == "0" or data == "-":
				return False
			continue
	

class Shell:
	@staticmethod
	def run_command(command):
		return os.system(command)
	
	@staticmethod
	def run_command_get_output(command):
		return commands.getoutput(command)

class LLDB:
	def __init__(self,debugger):
		self.debugger = debugger
	
	def run_command(self,command):
		ci = self.debugger.GetCommandInterpreter()
		result = lldb.SBCommandReturnObject()
		ci.HandleCommand(command,result)
		return [result.GetOutput(), result.GetError()]

	# shortcut call to only get the command output
	def __call__(self,command):
		return self.run_command(command)[0]

class AccumulatedString:
	def __init__(self):
		self.data = ""
		self.separator = "\\n"
		self.sanitizer = AppleScriptSanitizer()
	
	def __rshift__(self,more_data):
		self.data = self.data + self.separator + self.sanitizer.sanitize(more_data)
		return self
	
	def __str__(self):
		return self.data

class SmartTempFile:
	def __init__(self):
		self.file = tempfile.NamedTemporaryFile(delete=False)
		self.filename = self.file.name

	def __str__(self):
		return self.filename

	def close(self):
		self.file.close()

	def write(self,data):
		self.file.write(data)

	def __enter__(self):
		return self

	def __exit__(self, type, value, traceback):
		self.close()

	def remove(self):
		os.remove(self.filename)
		self.filename = None
		self.file = None

class AppleScript:
	def __init__(self,app):
		self.app = app
		self.commands = []
	
	def add_command(self,cmd,sanitized = False):
		if sanitized:
			self.commands.append(cmd)
		else:
			self.commands.append(AppleScriptSanitizer().sanitize(cmd))
	
	@staticmethod
	def has_apple_script():
		return os.path.exists("/usr/bin/osascript")
	
	def run_script(self,temp_file):
		with temp_file:
			temp_file.write('tell Application "' + self.app + '"\n')
			for cmd in self.commands:
				temp_file.write(cmd + '\n')
			temp_file.write("end tell")
		command_line = "/usr/bin/osascript " + str(temp_file)
		return Shell.run_command_get_output(command_line)

class Radar:
	def __init__(self):
		self.description = AccumulatedString()
		self.title = ""
		self.component = "lldb"
		self.component_version = "X"
		self.category = 7
		self.categories = {1 : "Security", 2 : "Crash/Hang/Data Loss", 3 : "Power", 4 : "Performance", 5 : "UI/Usability", 7 : "Serious",  8 : "Other", 10 : "Feature", 11 : "Enhancement", 13 : "Task"}
		self.categories_inverted = dict([[v,k] for k,v in self.categories.items()])
	
	def has_radar_app(self):
		# well-known paths
		if os.path.exists("/Applications/Radar.app/Contents/MacOS/Radar"): return True
		if os.path.exists("/AppleInternal/Applications/Radar.app/Contents/MacOS/Radar"): return True
		# look for Radar using Spotlight
		find_radar = Shell.run_command_get_output('/usr/bin/mdfind -name "Radar" -count')
		try:
			find_radar = int(find_radar)
			return (find_radar > 0)
		except:
			return False
	
	def set_component(self,comp,ver):
		self.component = comp
		self.component_version = ver
	
	def set_component_to_plain(self):
		self.component = "lldb"
		self.component_version = "X"
	
	def set_component_to_ios(self):
		self.component = "Purple Dev Tools - lldb"
		self.component_version = "1.0"

	def file_radar(self,temp_file):
		applescript = AppleScript("Radar")
		applescript.add_command('NewProblem componentName "' + self.component + '" componentVersion "' + self.component_version + \
			'" description "' + str(self.description) + '"problemTitle "' + str(self.title) + '"reproducibilityCode 1 classCode ' + str(self.category) + ' without doSave\n', sanitized=True)
		radar_output = applescript.run_script(temp_file)
		try:
			radar_number = int(radar_output)
			self.number = radar_number
			temp_file.remove()
			return None
		except:
			self.number = None
			return radar_output

class DataProvider:
	def do_task(self,radar,input,debugger):
		pass

class SetTitle(DataProvider):
	def do_task(self,radar,input,debugger):
		print "First of all, give me a brief description of your issue. I will use it as a title for the radar."
		radar.title = AppleScriptSanitizer().sanitize(input.read_line('--> '))
		return True

class SetComponent(DataProvider):
	def do_task(self,radar,input,debugger):
		print "Radars that involve iOS in a significant way need to go into a special protected component"
		is_ios = input.read_yes_no("Is this an iOS radar (yes/no)?")
		if is_ios:
			radar.set_component_to_ios()
		else:
			radar.set_component_to_plain()
		return True

class GatherUserDescription(DataProvider):
	def do_task(self,radar,input,debugger):
		print "Please, provide a more detailed description of your problem. You can type multiple lines here"
		print "An empty line will terminate input."
		for line in input.read_lines('--> '):
			radar.description >> line
		return True

class SetCategory(DataProvider):
	def longest_prefix_match(self, key, list):
		matches = [X for X in list if re.match(key,X,re.I)]
		if matches:
			return max(matches, key=len)
		else:
			return None
	
	def do_task(self,radar,input,debugger):
		print "There are several different categories that Radars fit into. Please choose one to help the LLDB team correctly handle your issue."
		print "If you are not sure, category 7 (Serious bug) is a good starting point."
		for key,val in radar.categories.items():
			print str(key) + " ==> " + str(val)
		while True:
			category = (input.read_line('--> '))
			if category == None or category == '':
				category = 7 # make serious bug the default for lazy users. we can reclassify later
			try:
				category = int(category)
				if not (category in radar.categories):
					continue
				break
			except:
				category = self.longest_prefix_match(category,radar.categories.values())
				if category == None:
					continue
				category = radar.categories_inverted[category]
				break
		print "OK, cool. I am going to file a radar for " + str(radar.categories[category])
		radar.category = category
		return True

class RunLLDBCommand(DataProvider):
	def __init__(self,cmd):
		self.command = cmd
	def do_task(self,radar,input,debugger):
		radar.description >> "Input: " + str(self.command) >> "Output: " >> debugger(self.command)
		return True

class RunShellCommand(DataProvider):
	def __init__(self,cmd):
		self.command = cmd
	def do_task(self,radar,input,debugger):
		radar.description >> "Input: " + str(self.command) >> "Output: " >> Shell.run_command_get_output(self.command)
		return True

# tasks here can accumulate data without a radar# - this is the preferred place to do work
before_filing_tasks = [SetTitle(),SetComponent(),GatherUserDescription(),SetCategory(),RunLLDBCommand("version"), \
RunLLDBCommand("target list"),RunLLDBCommand("thread list"),RunLLDBCommand("bt"),RunLLDBCommand("frame variable"), \
RunShellCommand("sw_vers")]

# add here any task that requires a valid radar# to be performed
# make sure any tasks that you put here really NEED to be here
after_filing_tasks = []

def radar(debugger, command, result, internal_dict):
	print "It looks like you are having LLDB issues. I will guide you through the steps to file a radar about it."
	print "Some information about the running LLDB will be gathered. It is your duty to make sure that appropriate security and disclosure policies are followed. 'nuff said."
	print "If you are filing a radar about a crash, be aware that this command executes steps that might themselves make LLDB crash. If that happens, please file your radar manually."
	my_radar = Radar()
	my_input = InputReader()
	my_lldb = LLDB(debugger)
	
	if my_radar.has_radar_app() == False:
		print "Sorry. You need the Radar application (not the Web UI) to use this command. You should get this automatically on an AppleInternal install"
		print "Or, you can download it from IS&T. Please, obtain a copy of Radar.app before continuing."
		return "No Radar.app found"
	
	if AppleScript.has_apple_script() == False:
		print "Sorry. You need the AppleScript command line tool (/usr/bin/osascript)"
		print "This tool should be part of a standard OS X install. Get in touch with macosx-help@group.apple.com to ask for further assistance."
		return "No osascript binary found"
	
	for task in before_filing_tasks:
		is_ok = task.do_task(my_radar,my_input,my_lldb)
		if is_ok == True or is_ok == None:
			continue
		else:
			print "It looks like I cannot proceed to file your radar. I got an error: " + str(is_ok)
			return str(is_ok)
	
	my_temp_file = SmartTempFile()
	did_file = my_radar.file_radar(my_temp_file)
	if did_file == None:
		print "Hooray! I filed a draft of rdar://" + str(my_radar.number) + " for you."
		print "I might need to ask you some more questions. At the end, please remember to save the radar to make sure it gets to the LLDB team."
	else:
		print "Sorry but something went wrong with your radar. I saved the script file I was going to use as " + str(my_temp_file)
		print "You can use that file as a reference of the information I attempted to send out."
		print "It looks like I cannot proceed to file your radar. I got an error: " + str(did_file)
		return str(did_file)
	
	for task in after_filing_tasks:
		is_ok = task.do_task(my_radar,my_input,my_lldb)
		if is_ok == True or is_ok == None:
			continue
		else:
			print "Something went wrong while updating your radar. I got an error: " + str(is_ok)
			print "Since I already drafted a radar for you, you might want to save it as-is and add information manually."
			return str(is_ok)
	
	print "I am done! Please, remember to save the generated radar. This step is necessary for the LLDB folks to see it."

def __lldb_init_module(debugger, internal_dict):
	debugger.HandleCommand('command script add -f radar.radar radar')
	print 'The "radar" python command has been installed and is ready for use.'
