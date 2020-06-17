import sys
from cs50 import get_string

def main():

    if len(sys.argv) != 2:
        sys.exit("Usage: python caesar.py key")

    key = sys.argv[1]

    for ch in key:
        if not ch.isdigit():
            sys.exit("key should be a digit")

    key = int(key)

    plain_text = get_string("plaintext: ")
    print("ciphertext: ")
    for ch in plain_text:
        if not ch.isalpha():
            print(ch, end="")
            continue

        ascii_offset = 65 if ch.isupper() else 97

        pi = ord(ch) - ascii_offset
        ci = (pi + key) % 26

        print(chr(ci + ascii_offset), end="")
    print()

if __name__ == "__main__":
    main()