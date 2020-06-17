from cs50 import get_string
from sys import argv
import sys

def main():

    if len(argv) != 2:
        print("Usage: python bleep.py file")
        sys.exit(1)

    banned_words = set()
    infile = argv[1]

    with open(infile, "r") as file:
        for line in file:
            banned_words.add(line.strip("\n"))

    message = get_string("What message would you like to censor?\n").split()

    for word in message:
        if word.lower() in banned_words:
            n = "*" * len(word)
            message.insert(message.index(word), n)
            message.remove(word)

    for word in message:
        print(word, end=" ")
    print()


if __name__ == "__main__":
    main()
