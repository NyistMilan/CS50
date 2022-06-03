from cs50 import get_int


def main():
    height = get_int("Height: ")

    while height > 8 or height < 1:
        height = get_int("Height: ")

    blank = height
    for row in range(height):

        blank -= 1
        for i in range(blank):
            print(" ", end='')

        block = height - blank
        for i in range(block):
            print("#", end='')

        print("  ", end='')

        for i in range(block):
            print("#", end='')

        print()


if __name__ == "__main__":
    main()