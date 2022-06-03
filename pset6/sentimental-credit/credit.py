from cs50 import get_string


def luhns(creditNumber):
    sum = 0
    flag = False
    for index in range(len(creditNumber) - 1, -1, -1):
        if flag:
            number = int(creditNumber[index]) * 2
            if(len(str(number)) == 1):
                sum += number
            else:
                stringedNum = str(number)
                for i in range(0, len(stringedNum)):
                    sum += int(stringedNum[i])
            flag = False
        else:
            sum += int(creditNumber[index])
            flag = True

    if(str(sum)[-1] == '0'):
        return True
    else:
        return False


def main():

    creditNumber = get_string("Number: ")

    firstDigit, secondDigit = creditNumber[0], creditNumber[1]
    length = len(creditNumber)
    if luhns(creditNumber):
        if length == 13 and firstDigit == "4":
            print("VISA\n")
        elif length == 15 and (firstDigit + secondDigit) == "34" or (firstDigit + secondDigit) == "37":
            print("AMEX\n")
        elif length == 16:
            if firstDigit == "4":
                print("VISA\n")
            else:
                for i in range(51, 56):
                    if(firstDigit + secondDigit == str(i)):
                        print("MASTERCARD\n")
                        break
                    print("INVALID\n")
    else:
        print("INVALID\n")


if __name__ == "__main__":
    main()