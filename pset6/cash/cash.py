from cs50 import get_float

def main():
    while True:
        dollars = get_float("Change owed: ")
        if dollars > 0.0:
            break

    coins = round(dollars * 100)
    count = 0
    
    while coins >= 25:
        count+=1;
        coins -= 25;
    
    while coins >= 10:
        count+=1;
        coins -= 10;

    while coins >= 5:
        coins -= 5;
        count+=1;
    
    while coins >= 1:
        coins -= 1;
        count+=1;
    print(f"Count of coins = {count}");
    
if __name__ == "__main__":
    main()
    