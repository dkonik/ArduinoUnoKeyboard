def main():
	chars = {}
	#Frequently used
	chars["TAB"] = 					"0b01000000"
	chars["SPECIAL_ACTIVATOR"] = 	"0b10000000"
	chars["SPACE"] = 				"0b00100000"
	chars["BACK_SPACE"] = 			"0b00000100"
	chars["ENTER"] = 				"0b00000001"

	for k,v in chars.iteritems():
		print "const uint16_t " + k + " = " + v + ";"

if __name__ == '__main__':
	main()