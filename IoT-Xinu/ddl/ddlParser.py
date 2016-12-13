import json
import os 
dir_path = os.path.dirname(os.path.realpath(__file__))
devicePath = os.path.abspath(os.path.join(os.path.dirname( __file__ ), '..', 'include'))

# Concatenate arguments and create method call statement
def writeMethodCall(name, args):
	argList = ', '.join([str(x) for x in args])
	return name + "(" + argList + ");" + "\n"

# Generate entire method definition
# argNamesAndTypes is of the form [ [ArgType1, Arg1], [ArgType2, Arg2]... ]
def writeMethod(type, name, argNamesAndTypes, body):
	argList = ', '.join([str(a) + " " + str(b) for a,b in argNamesAndTypes])
	return type + " " + name + "(" + argList + ") {\n" + body + "}"

def handleActuator(device):
	deviceName = device["name"]
	ledInitOut = open(devicePath + "/ledInit-ddl-out", "w+")
	ledWriteOut = open(devicePath + "/ledWrite-ddl-out", "w+")

	#Generate code to set direction for all three led gpio pins
	pins = device["interface"]["connection"]["pin"]
	for pin in pins:
		if pin["port"]:
			s = writeMethodCall("pin_setdir", [pin["port"], pin["pin"], pin["direction"]])
			ledInitOut.write(s)

	#Generate switch case statement to decide which led to turn on
	signal = device["interface"]["signal"][0]
	method = signal["method"]
	cases = "";
	for config in method["configuration"]:
		caseLabel = str(config["arg"])
		cases += "\t\tcase " + caseLabel + ":\n"
		for p in config["pin"]:
			methodName = "pin_" + p["level"]
			cases += "\t\t\t" + writeMethodCall(methodName, [pins[p["index"]]["port"], pins[p["index"]]["pin"]]);
		cases += "\t\t\tbreak;\n";
	switchStatement = "\tswitch(arg) {\n" + cases + "\t}\n"
	ledWriteOut.write(writeMethod("void", method["name"], [["uint32", "arg"]], switchStatement))

	print deviceName + " of type " + device["type"] + " being handled!"


def handleSensor(device):
	tempReadOut = open(devicePath + "/tempRead-ddl-out", "w+")
	signal = device["interface"]["signal"]
	

	method = device["interface"]["reading"]["method"]
	methodBody = "\treturn " + method["computation"]["expression"] + ";\n"
	tempReadOut.write(writeMethod("uint32", method["name"], [["float", signal[0]["id"]]], methodBody))


	tempModeOut = open(devicePath + "/tempMode-ddl-out", "w+")
	cases = "";
	for mode in signal[0]["sampleMode"]:
		caseLabel = str(mode["id"])
		cases += "\t\tcase " + caseLabel + ":\n"
		cases += "\t\t\tadc_csrptr->step[0].step_config |= " + mode["name"] + ";\n\t\t\tbreak;\n"

	switchStatement = "\tswitch(tempMode) {\n" + cases + "\t}\n"
	tempModeOut.write(switchStatement)


	print device["name"] + " of type " + device["type"] + " not handled yet!"

def handleCloud(device):
	cloudConf = open(dir_path + "/cloudconf.js", "w+")
	cloudConf.write("var ip = '" + device["interface"]["ip"] + "';\n")
	cloudConf.write("var port = " + str(device["interface"]["port"]) + ";\n")

def handleEdge(device):
	edgeConf = open(dir_path + "/edgeconf" + device["id"] + ".php", "w+")
	edgeConf.write("$ip = '" + device["interface"]["ip"] + "';\n")
	edgeConf.write("$port = " + str(device["interface"]["port"]) + ";\n")

def handleCloudInteraction(interaction):
	cloudConf = open(dir_path + "/" + interaction["sourceId"] + "-" + interaction["destinationId"] + ".json", "w+")
	json.dump(interaction, cloudConf)

def handleEdgeInteraction(interaction):
	edgeConf = open(dir_path + "/" + interaction["sourceId"] + "-" + interaction["destinationId"] + ".json", "w+")
	json.dump(interaction, edgeConf)


def handleBeaglebone(device):
	print "beaglebone  not yet implemented!"

with open(dir_path + "/ddl.json") as data_file:    
	data = json.load(data_file)
	for device in data["device"]:
		devType = device["type"]
		if devType == "actuator":
			handleActuator(device)
		elif devType == "sensor":
			handleSensor(device)
		elif devType == "cloud":
			handleCloud(device)
		elif devType == "edge":
			handleEdge(device)
		elif devType == "beaglebone":
			handleBeaglebone(device)

	for interaction in data["interaction"]:
		mode = interaction["mode"]
		if mode == "TCP":
			handleCloudInteraction(interaction)
		elif mode == "UDP":
			handleEdgeInteraction(interaction)
