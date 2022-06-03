from cs50 import get_string
import sys


def main():
    text = get_string("Text: ")

    lettersNum = countLetters(text)
    sentencesNum = countSentences(text)
    wordsNum = countWords(text)
    l = (lettersNum / wordsNum) * 100
    s = (sentencesNum / wordsNum) * 100
    grade = colemanIndex(l, s)

    if grade > 16:
        print("Grade 16+")
    elif grade < 1:
        print("Before Grade 1")
    else:
        print("Grade " + str(grade))


def countLetters(text):
    counter = 0

    for letter in text:
        if letter.isalpha():
            counter += 1

    return counter


def countWords(text):
    counter = 1
    for letter in text:
        if(letter == " "):
            counter += 1

    return counter


def countSentences(text):
    counter = 0
    for letter in text:
        if(letter == '.' or letter == '?' or letter == '!'):
            counter += 1
    return counter


def colemanIndex(l, s):
    return round(0.0588 * l - 0.296 * s - 15.8)


if __name__ == "__main__":
    main()