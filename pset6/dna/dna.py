# This program takes a sequence of DNA and a CSV file containing STR counts for a list of individuals
# and then output to whom the DNA (most likely) belongs.
from sys import argv, exit
import csv
import re

# Check for correct usage
if len(argv) != 3:
    print("This program takes exactly 2 command line arguments")
    print("Usage: python dna.py dna_database.csv dna_sequence.txt")
    exit(1)

# Load database
csvfile = open(argv[1], "r", newline='')
dict_reader = csv.DictReader(csvfile)

# Read txt file containing the DNA sample to be processed
with open(argv[2], "r") as txtfile:
    dna = txtfile.read()

max_repeat = dict.fromkeys(dict_reader.fieldnames[1:], 0)

for sequence in dict_reader.fieldnames[1:]:
    # For each sequence, also known as STR, find all occurrences on the sample
    sequence_occurrences = re.finditer(f'({sequence})+', dna)
    # Determine the largest occurrence and save it
    for occurrence in sequence_occurrences:
        if (occurrence.end() - occurrence.start()) > max_repeat[sequence]:
            max_repeat[sequence] = (occurrence.end() - occurrence.start()) / len(sequence)

person_identified = False
# Read each row looking for the person with the same number of maximum repeat of the sequences
for row in dict_reader:
    for field in dict_reader.fieldnames[1:]:
        if int(row[field]) != int(max_repeat[field]):
            break
        elif field is dict_reader.fieldnames[-1]:
            # If the loop got to the last field without breaking, then this must be the person we're looking for
            print(row["name"])
            csvfile.close()
            exit(0)

csvfile.close()
print("No match")
