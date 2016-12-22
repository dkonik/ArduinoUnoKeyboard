import itertools
def main():
	resistor_values = [1000, 3000, 5000, 7000]
	base_resistor = 1000

	for i in range(1, len(resistor_values)+1):
		for comb in itertools.combinations(resistor_values, i):
			comb = list(comb)
			comb_inverse = [1 / float(val) for val in comb]
			equiv_resistance = 1 / sum(comb_inverse)
			output = (base_resistor / (base_resistor + equiv_resistance)) * 5
			print "*********"
			print comb
			print(output, " volts")

if __name__ == '__main__':
	main()