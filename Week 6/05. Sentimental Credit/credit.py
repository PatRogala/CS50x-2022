# Check credit card number for validity
# Use Luhn's algorithm to determine if a credit card number is (syntactically) valid
# https://en.wikipedia.org/wiki/Luhn_algorithm
from cs50 import get_int


# Main function
def main():
    card_number = get_int("Number: ")
    checksum = get_checksum(card_number)
    first_two_digits = get_starting_digits(card_number)
    card_length = get_card_length(card_number)
    print_card_type(checksum, first_two_digits, card_length)


# Get checksum of credit card number using Luhn's algorithm
def get_checksum(card_number):
    is_second = False
    sum = 0
    while (card_number > 0):
        digit = card_number % 10
        if (is_second == True):
            digit = digit * 2
        sum += digit // 10
        sum += digit % 10
        card_number = card_number // 10
        is_second = not is_second
    return sum


# Get first two digits of credit card number
def get_starting_digits(card_number):
    while (card_number >= 100):
        card_number = card_number // 10
    return card_number


# Get length of credit card number
def get_card_length(card_number):
    length = 0
    while (card_number > 0):
        card_number = card_number // 10
        length += 1
    return length


# Print credit card type
def print_card_type(checksum, starting_digits, card_length):
    if (checksum % 10 != 0):
        print("INVALID")
        return

    if ((starting_digits == 34 or starting_digits == 37) and card_length == 15):
        print("AMEX")
    elif (starting_digits >= 51 and starting_digits <= 55 and card_length == 16):
        print("MASTERCARD")
    elif ((starting_digits >= 40 and starting_digits <= 49) and (card_length == 13 or card_length == 16)):
        print("VISA")
    else:
        print("INVALID")


if __name__ == "__main__":
    main()
