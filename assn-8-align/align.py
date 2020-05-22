#!/usr/bin/env python

import random # for seed, random
import sys    # for stdout
import copy

# Computes the score of the optimal alignment of two DNA strands.
def findOptimalAlignment(strand1, strand2, cache = {}):
	key = (strand1, strand2)
	if key in cache:
		return cache[key]

	def ret(ans):
		cache[key] = copy.deepcopy(ans)
		return ans
	
	# if one of the two strands is empty, then there is only
	# one possible alignment, and of course it's optimal
	if len(strand1) == 0: 
		return ret({
			'score': len(strand2) * -2, 
			'strand1': [' ' for l in strand2],
			'strand2': [l for l in reversed(strand2)]})
	if len(strand2) == 0: 
		return ret({
			'score': len(strand1) * -2, 
			'strand1': [l for l in reversed(strand1)],
			'strand2': [' ' for l in strand1]})

	# There's the scenario where the two leading bases of
	# each strand are forced to align, regardless of whether or not
	# they actually match.
	best = findOptimalAlignment(strand1[1:], strand2[1:])
	best['strand1'].append(strand1[0])
	best['strand2'].append(strand2[0])

	if strand1[0] == strand2[0]: 
		best['score'] += 1
		return ret(best)  # no benefit from making other recursive calls

	best['score'] -= 1
	
	# It's possible that the leading base of strand1 best
	# matches not the leading base of strand2, but the one after it.
	bestWithout = findOptimalAlignment(strand1, strand2[1:])
	bestWithout['score'] -= 2 # penalize for insertion of space
	if bestWithout['score'] > best[0]:
		bestWithout['strand1'].append(' ')
		bestWithout['strand2'].append(strand2[0])
		best = bestWithout

	# opposite scenario
	bestWithout = findOptimalAlignment(strand1[1:], strand2)
	bestWithout['score'] -= 2 # penalize for insertion of space	
	if bestWithout['score'] > best[0]:
		bestWithout['strand1'].append(strand1[0])
		bestWithout['strand2'].append(' ')
		best = bestWithout

	return ret(best)

# Utility function that generates a random DNA string of
# a random length drawn from the range [minlength, maxlength]
def generateRandomDNAStrand(minlength, maxlength):
	assert minlength > 0, \
	       "Minimum length passed to generateRandomDNAStrand" \
	       "must be a positive number" # these \'s allow mult-line statements
	assert maxlength >= minlength, \
	       "Maximum length passed to generateRandomDNAStrand must be at " \
	       "as large as the specified minimum length"
	strand = ""
	length = random.choice(xrange(minlength, maxlength + 1))
	bases = ['A', 'T', 'G', 'C']
	for i in xrange(0, length):
		strand += random.choice(bases)
	return strand

# Method that just prints out the supplied alignment score.
# This is more of a placeholder for what will ultimately
# print out not only the score but the alignment as well.

def printAlignment(alignment, out = sys.stdout):	
	[score, str1, str2] = alignment
	str1.reverse()
	str2.reverse()
	def plusValue(i):
		if str1[i] == str2[i] and str1 != ' ':
			return "1"
		return " "		
	def minusValue(i):
		if str1[i] == ' ' or str2[i] == ' ':
			return "2"
		if str1[i] != str2[i]:
			return "1"
		return " "
	plus = map(plusValue, range(len(str1)))
	minus = map(minusValue, range(len(str1)))
	out.write("\nOptimal alignment score is " + str(score) + "\n\n")
	out.write("+ " + "".join(plus) + "\n")
	out.write("  " + ''.join(str1) + "\n")
	out.write("  " + ''.join(str2) + "\n")
	out.write("- " + "".join(minus) + "\n\n")

# Unit test main in place to do little more than
# exercise the above algorithm.  As written, it
# generates two fairly short DNA strands and
# determines the optimal alignment score.
#
# As you change the implementation of findOptimalAlignment
# to use memoization, you should change the 8s to 40s and
# the 10s to 60s and still see everything execute very
# quickly.
 
def main():
	while (True):
		sys.stdout.write("Generate random DNA strands? ")
		answer = sys.stdin.readline()
		if answer == "no\n": break
		strand1 = generateRandomDNAStrand(8, 10)
		strand2 = generateRandomDNAStrand(8, 10)
		sys.stdout.write("Aligning these two strands: " + strand1 + "\n")
		sys.stdout.write("                            " + strand2 + "\n")
		alignment = findOptimalAlignment(strand1, strand2)
		printAlignment(alignment)
		
if __name__ == "__main__":
  main()
