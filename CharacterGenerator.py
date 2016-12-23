def main():
	chars = {}
	#Frequently used
	#"Special" Without special activator (because frequently used)
	chars["TAB"] = 					"0b01000000"
	chars["SPECIAL_ACTIVATOR"] = 	"0b10000000"
	chars["SPACE"] = 				"0b00100000"
	chars["BACK_SPACE"] = 			"0b00000100"
	chars["ENTER"] = 				"0b00000001"

	#Regular letters
	chars["e"] = 					"0b00000010"; 
	chars["t"] = 					"0b00011000";
	chars["a"] =					"0b00001100";
	chars["o"] = 					"0b00000110";
	chars["i"] = 					"0b00110000";
	chars["n"] =					"0b01100000";
	chars["s"] = 					"0b00100100";
	chars["h"] = 					"0b01000010";
	chars["r"] = 					"0b01010000";
	chars["d"] = 					"0b00001010";
	chars["l"] = 					"0b00010100";
	chars["c"] = 					"0b00010010";
	chars["u"] = 					"0b00101000";
	chars["m"] = 					"0b01001000";
	chars["w"] =					"0b00100010";
	chars["f"] = 					"0b01000100";
	chars["g"] = 					"0b00011100";
	chars["y"] = 					"0b00111000";
	chars["p"] = 					"0b00110100";
	chars["b"] = 					"0b00101100";
	chars["v"] = 					"0b00110010";
	chars["k"] =					"0b01001100";
	chars["j"] =					"0b01101000";
	chars["x"] = 					"0b00010110";
	chars["q"] = 					"0b01100100";
	chars["z"] = 					"0b00100110";

	# Special Characters (must be used with left pinky down), so technically
	# for all of these, the leftmost bit would be 0
	chars["SHIFT"] = 				"0b10000001";
	chars["CTRL"] = 				"0b10000001";
	

	for k,v in chars.iteritems():
		print "const uint16_t " + k + " = " + v + ";"

if __name__ == '__main__':
	main()