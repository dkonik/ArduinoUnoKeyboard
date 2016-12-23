def main():
	chars = {}

	#Frequently used
	chars["SHIFT"] = 				"0b1000000000"	
	chars["TAB"] = 					"0b0100000000"
	chars["SPACE"] = 				"0b0010000000"
	#Open spot here
	chars["SPECIAL_ACTIVATOR"] = 	"0b0000100000"
	chars["ALT"] = 					"0b0000010000"
	# Left and right click???
	chars["BACK_SPACE"] = 			"0b0000000010"
	chars["CTRL"] = 				"0b0000000001"


	#Regular letters
	chars["e"] = 					"0b0001001000"
	chars["t"] =					"0b0000001100"
	chars["a"] = 					"0b0000000110"
	chars["o"] = 					"0b0011000000"
	chars["i"] =					"0b0110000000"
	chars["n"] = 					"0b0010000100"
	chars["s"] = 					"0b0100000010"
	chars["h"] = 					"0b0101000000"
	chars["r"] = 					"0b0000001010"
	chars["d"] = 					"0b0001000100"
	chars["l"] = 					"0b0001000010"
	chars["c"] = 					"0b0010001000"
	chars["u"] = 					"0b0100001000"
	chars["m"] =					"0b0010000010"
	chars["w"] = 					"0b0100000100"
	chars["f"] = 					"0b0001001100"
	chars["g"] = 					"0b0011001000"
	chars["y"] = 					"0b0011000100"
	chars["p"] = 					"0b0010001100"
	chars["b"] = 					"0b0011000010"
	chars["v"] =					"0b0100001100"
	chars["k"] =					"0b0110001000"
	chars["j"] = 					"0b0001000110"
	chars["x"] = 					"0b0110000100"
	chars["q"] = 					"0b0010000110"
	chars["z"] =					"0b0101001000"
	# Special case, shift+alt+ctrl == function button, maybe thing of better
	# way to do this
	chars["FUNCTION"] =				"0b1000010001"

	# Special Characters (must be used with left thumb down)
	chars["ESCAPE"] = 				"0b0011101100"
	chars["PERIOD"] = 				"0b0000101000"
	chars["COMMA"] = 				"0b0000100100"
	chars["GRTR_THAN"] = 			"0b1000101000"
	chars["LESS_THAN"] = 			"0b0000100100"
	chars["ENTER"] = 				"0b0001100000"
	chars["UP"] = 					"0b0001101000"
	chars["DOWN"] = 				"0b0010100100"
	chars["LEFT"] = 				"0b0011100000"
	chars["RIGHT"] =				"0b0000101100"
	

	for k,v in chars.iteritems():
		print "const uint16_t " + k + " = " + v + ";"

if __name__ == '__main__':
	main()