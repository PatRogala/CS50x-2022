# Prompt user for text
text = input("Text: ")

# Calculate each of the three variables
letters = 0
words = 1
sentences = 0

for i in range(len(text)):
    if text[i].isalpha():
        letters += 1
    if text[i] == " ":
        words += 1
    if text[i] == "." or text[i] == "!" or text[i] == "?":
        sentences += 1

# Calculate the Coleman-Liau index and print grade
L = (letters / words) * 100
S = (sentences / words) * 100

index = round(0.0588 * L - 0.296 * S - 15.8)

if index < 1:
    print("Before Grade 1")
    exit(0)
if index >= 16:
    print("Grade 16+")
    exit(0)

print(f"Grade {index}")
