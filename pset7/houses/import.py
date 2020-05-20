# Reads a csv containing Harry Potter characters and save it in a SQL database
import csv
from cs50 import SQL
from sys import exit, argv


# Check if the correct number of command line arguments was given
if len(argv) != 2:
    print("Usage: python import.py characters.csv")
    exit(1)

# Instantiate database
db = SQL("sqlite:///students.db")

# Opens csv file and set up a reader
with open(argv[1], "r") as csv_file:
    reader = csv.DictReader(csv_file)
    for row in reader:
        name = row["name"].split()
        # Check if the current student has 2 or 3 names
        if len(name) == 2:
            db.execute("INSERT INTO students(first, middle, last, house, birth) VALUES (?, NULL, ?, ?, ?)",
                       name[0], name[1], row["house"], row["birth"])
        elif len(name) == 3:
            print(name[0], name[1], name[2], row["house"], row["birth"])
            db.execute("INSERT INTO students(first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",
                       name[0], name[1], name[2], row["house"], row["birth"])
