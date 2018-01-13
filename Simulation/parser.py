import json
import sys

def is_number(s):
	try:
		complex(s)
	except ValueError:
		return False
	return True

'''
Checks if a line is a valid statement
Contains a lot of prints inorder to insure that users understand why parsing is failing
'''
def validateStatement(line, line_no):
	# blank space is valid
	if (len(line) == 0):
		return True

	fL = line[0]
	# check first if it is a flag, Error, or deleteBlock respectively
	if (fL in ['?','!', '^']):
		return True

	if (fL not in ['%', '$'] and not fL.isalpha()):
		print("Error on line number", line_no)
		print("\t" + line)
		print("\tUnknown operator", fL)
		return False
	
	if (fL not in ['$'] and '=' not in line):
		print("Error on line number", line_no)
		print("\t" + line)
		print("\tStatement does not contain an assignment")
		return False
	
	splited = line.split("=")
	
	if (len(splited) > 2 or (len(splited) is 2 and splited[1] == '')):
		print("Error on line number", line_no)
		print("\t" + line)
		print("\tStatement does not follow format <var> = <val>")
		return False
	
	if (fL in ['%', '$']):
		# check variable name is specified
		if (len(splited[0]) == 1):
			print("Error on line number", line_no)
			print("\t" + line)
			print("\tThe operator", fL, "needs to specify variable name")
			return False
		# check if variable name starts with a letter
		if (not splited[0][1].isalpha()):
			print("Error on line number", line_no)
			print("\t" + line)
			print("\tThe variable name", splited[0][1:], "must start with a letter")
			return False
	else:
		# is there a variable specified
		if (len(splited[0]) == 0):
			print("Error on line number", line_no)
			print("\t" + line)
			print("\tStatement needs to specify variable to modify")
			return False
		if (not splited[0][0].isalpha()):
			print("Error on line number", line_no)
			print("\t" + line)
			print("\tThe variable name", splited[1:], "must start with a letter")
			return False
	'''
	if (len(splited) == 2):
		# check second element
		if (not is_number(splited[1])):
			print("Error on line number", line_no)
			print("\t" + line)
			print("\tThe value", splited[1], "is not a valid number")
			return False	
	'''
	return True
'''
Method used to parse from a textFile into a usable data structure
return None with invalid input
'''
def parseFromFile(fileName):
	fo = open(fileName, 'r')
	print("Opened file", fo.name, "for instruction parsing")
	data = fo.read().splitlines()
	
	structure = []
	
	line_no = 0
	# go through each line
	for line in data:
		line_no += 1 # increment line number
		# strip away blank space from the start
		line = line.strip()
		# remove comments from line
		line = line.split("#")[0]
		
		if len(line) is 0:
			continue

		# check if start of conditional
		if (line[0] == '@'):
			
			structure.append(
				{
					"cond" : line[1:],
					"stat" : []
				}
			)
			continue
		
		# check if a condition has been used before a statement
		if (len(structure) == 0):
			print("Error on line number", line_no)
			print("\t" + line)
			print("\tMust use a condition before statement")
			return None
		
		# check if the line is a validstatement
		if (not validateStatement(line, line_no)):
			return None
		
		#assert(len(structure) > 0)
		structure[-1]["stat"].append(line)
	fo.close()
	return structure

if __name__ == "__main__":
	if (len(sys.argv) == 1):
		print("Please provide a valid file to parse")
	else:
		struct = parseFromFile(sys.argv[1])
		print(json.dumps(struct, indent=4))