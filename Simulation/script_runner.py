def RunScript(filePath):
    try:
        with open(filePath, 'r') as file:
            content = file.read()
            print(content)
    except FileNotFoundError:
        print("File not exist.")