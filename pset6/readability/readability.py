# Analyze a text and computes the approximate grade level needed to crompehend it
# The grade level is compute using the Coleman-Liau index
from cs50 import get_string

text = get_string("Text: ")

n_letters = n_words = n_sentences = 0
word_started = False

# Count the number of letters, words ans sentences in the text
for char in text:
    if char.isalpha():
        n_letters += 1
        if word_started is False:
            word_started = True
            n_words += 1
    elif char in [".", "!", "?"]:
        n_sentences += 1
        word_started = False
    elif char == " ":
        word_started = False

# Calculate the Coleman-Liau index
index = 0.0588 * n_letters / n_words * 100 - 0.296 * n_sentences / n_words * 100 - 15.8

if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print(f"Grade {round(index)}")
