# Check if a credit card number is invalid
# If it may be valid, the program determine if it could be an AMEX, MASTERCARD or VISA card

from cs50 import get_string

dict_mapping = {0: "INVALID", 1: "MASTERCARD", 2: "AMEX", 3: "VISA"}

number = get_string("Card Number: ")

# Check if the first digits are 4, 34, 37, or 51-55, if not, declare the card as invalid
if number[0:2] in ["51", "52", "53", "54", "55"]:
    # Potential MasterCard
    result = 1
elif number[0:2] in ["34", "37"]:
    # Potential AMEX
    result = 2
elif number[0] in ["4"]:
    # Potential VISA
    result = 3
else:
    result = 0

if result != 0:
    # Sum the elements that must not be multiplied by 2
    # Get every other digit starting from the second-to-last and take the sum of them
    checksum = sum([int(i) for i in number[-1::-2]])

    # Now multiply the rest of the digits by 2 and some its digits to checksum
    for digit in number[-2::-2]:
        checksum += int((int(digit) * 2) / 10) + (int(digit) * 2) % 10

    # Check if the result of the checksum if valid
    if checksum % 10 != 0:
        result = 0

print(f"{dict_mapping[result]}")