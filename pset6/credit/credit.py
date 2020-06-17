from cs50 import get_int

def main():
    while True:
        credit_number = get_int("Your credit number: ")
        if credit_number > 0:
            break

    if credit_number > 5100000000000000 and credit_number < 5600000000000000:

        if func(credit_number, 8) == 0:
            print("MASTERCARD")
        else:
            print("INVALID")
    elif credit_number > 340000000000000 and credit_number < 380000000000000:

        if credit_number > 340000000000000 and credit_number < 350000000000000:

            if func(credit_number, 8) == 0:

                print("AMEX")
            else:
                print("INVALID")

        elif credit_number > 370000000000000 and credit_number < 380000000000000:

            if func(credit_number, 8) == 0:
                print("AMEX")
            else:
                print("INVALID")
        else:
            print("INVALID")
    elif credit_number > 4000000000000 and credit_number < 5000000000000000:

        if credit_number > 4000000000000 and credit_number < 5000000000000:

            if func(credit_number, 7) == 0:
                print("VISA")
            else:
                print("INVALID")

        elif credit_number > 4000000000000000 and credit_number < 5000000000000000:

            if func(credit_number, 8) == 0:

                print("VISA")
            else:
                print("INVALID")
    else:
        print("INVALID")



def func(credit_number, count):

    temp = 0
    sum = 0
    sumAnother = 0
    i = 10

    for j in range(0, count):
        temp = ((credit_number // i) % 10) * 2
        if temp >= 10:
            sum += (temp // 1) % 10
            sum += (temp // 10) % 10
        else:
            sum += temp
        i *= 100

    h = 1
    for k in range(0, count):
        sumAnother += ((credit_number // h) % 10)
        h *= 100

    return (sum + sumAnother) % 10



if __name__ == "__main__":
    main()