def main():
    while True:
        height = int(input("Height: "))
        if height > 0 and height < 9:
            break

    for i in range(height):
        print(" " * (height - i - 1) + "#" * (i + 1))

if __name__ == "__main__":
    main()
