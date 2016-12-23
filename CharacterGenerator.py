def main():
	chars = {}

	#Frequently used
	chars["SHIFT"] = 				"0b1000000000"	
	chars["TAB"] = 					"0b0100000000"
	chars["SPACE"] = 				"0b0010000000"
	chars["SPECIAL_ACTIVATOR"] = 	"0b0000100000"
	chars["FUNCTION"] = 			"0b0000010000"
	chars["ENTER"] = 				"0b0000000100"
	chars["BACK_SPACE"] = 			"0b0000000010"
	chars["CTRL"] = 				"0b0000000001"


	#Regular letters
	chars["e"] = 					"0b0000000010" 
	chars["t"] = 					"0b0001001000"
	chars["a"] =					"0b0000001100"
	chars["o"] = 					"0b0000000110"
	chars["i"] = 					"0b0011000000"
	chars["n"] =					"0b0110000000"
	chars["s"] = 					"0b0010000100"
	chars["h"] = 					"0b0100000010"
	chars["r"] = 					"0b0101000000"
	chars["d"] = 					"0b0000001010"
	chars["l"] = 					"0b0001000100"
	chars["c"] = 					"0b0001000010"
	chars["u"] = 					"0b0010001000"
	chars["m"] = 					"0b0100001000"
	chars["w"] =					"0b0010000010"
	chars["f"] = 					"0b0100000100"
	chars["g"] = 					"0b0001001100"
	chars["y"] = 					"0b0011001000"
	chars["p"] = 					"0b0011000100"
	chars["b"] = 					"0b0010001100"
	chars["v"] = 					"0b0011000010"
	chars["k"] =					"0b0100001100"
	chars["j"] =					"0b0110001000"
	chars["x"] = 					"0b0001000110"
	chars["q"] = 					"0b0110000100"
	chars["z"] = 					"0b0010000110"

	# Special Characters (must be used with left thumb down)
	chars["ESCAPE"] = 				"0b0011101100"
	

	for k,v in chars.iteritems():
		print "const uint16_t " + k + " = " + v + ";"

if __name__ == '__main__':
	main()