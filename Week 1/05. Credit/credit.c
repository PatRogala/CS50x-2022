// Check credit card number for validity
// Use Luhn's algorithm to determine if a credit card number is (syntactically) valid
// https://en.wikipedia.org/wiki/Luhn_algorithm
#include <cs50.h>
#include <stdio.h>

// Function prototypes
int get_checksum(long long card_number);
int get_starting_digits(long long card_number);
int get_card_length(long long card_number);
void print_card_type(int checksum, int starting_digits, int card_length);

// Main function
int main(void)
{
    long long card_number = get_long_long("Number: ");
    int checksum = get_checksum(card_number);
    int first_two_digits = get_starting_digits(card_number);
    int card_length = get_card_length(card_number);
    print_card_type(checksum, first_two_digits, card_length);
}

// Get checksum of credit card number using Luhn's algorithm
int get_checksum(long long card_number)
{
    bool is_second = false;
    int sum = 0;

    while (card_number > 0)
    {
        int digit = card_number % 10;

        if (is_second == true)
        {
            digit = digit * 2;
        }

        sum += digit / 10;
        sum += digit % 10;

        card_number = card_number / 10;
        is_second = !is_second;
    }

    return sum;
}

// Get first two digits of credit card number
int get_starting_digits(long long card_number)
{
    while (card_number >= 100)
    {
        card_number = card_number / 10;
    }

    return card_number;
}

// Get length of credit card number
int get_card_length(long long card_number)
{
    int length = 0;

    while (card_number > 0)
    {
        card_number = card_number / 10;
        length++;
    }

    return length;
}

// Print credit card type
void print_card_type(int checksum, int starting_digits, int card_length)
{
    if (checksum % 10 != 0)
    {
        printf("INVALID\n");
        return;
    }

    if ((starting_digits == 34 || starting_digits == 37) && card_length == 15)
    {
        printf("AMEX\n");
    }
    else if (starting_digits >= 51 && starting_digits <= 55 && card_length == 16)
    {
        printf("MASTERCARD\n");
    }
    else if ((starting_digits >= 40 && starting_digits <= 49) && (card_length == 13 || card_length == 16))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}
