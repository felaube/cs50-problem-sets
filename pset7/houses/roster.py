# Receives the name of a Hogwarts house as a command line argument and outputs
# information regarding the students of that house, which are saved in a SQL database
import csv
from cs50 import SQL
from sys import exit, argv


if len(argv) != 2:
    print("Usage: python import.py house")
    exit(1)

db = SQL("sqlite:///students.db")

students = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", argv[1])

for student in students:
    if student['middle'] is not None:
        print(f"{student['first']} {student['middle']} {student['last']}, born {student['birth']}")
    else:
        print(f"{student['first']} {student['last']}, born {student['birth']}")

