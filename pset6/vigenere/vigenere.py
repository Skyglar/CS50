import sys
from cs50 import get_string

def main():

    if len(sys.argv) != 2:
        sys.exit("Usage: python caesar.py keyword")

    key = sys.argv[1]

    for ch in key:
        if ch.isdigit():
            sys.exit("keyword should be a word")

    plain_text = get_string("plaintext: ")
    print("ciphertext: ", end="")
    i = 0
    for ch in plain_text:
        if not ch.isalpha():
            print(ch, end="")
            continue

        ascii_offset = 65 if ch.isupper() else 97

        pi = ord(ch) - ascii_offset
        ascii_offset_key = 65 if key[i].isupper() else 97
        offset = ord(key[i]) - ascii_offset_key
        ci = (pi + offset) % 26

        print(chr(ci + ascii_offset), end="")
        if i < len(key) - 1:
            i+=1
        else:
            i = 0
    print()

if __name__ == "__main__":
    main()