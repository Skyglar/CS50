from cs50 import get_int

while True:
    n = get_int("Height from 1 to 8: ")
    
    if n > 0 and n <= 8:
        height = n
        width = height
        break

print(f"Height: {n}")
for i in range(n):
    width -= 1
    print(" " * width, end="")
    print("#" * (height - width), end="")
    print()

    