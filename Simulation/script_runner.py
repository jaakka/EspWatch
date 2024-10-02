ioName =  ["DEBLED", "LCDLED", "HALLPWR", "GYROPWR", "HEARTRATEPWR", "BRIGHTNESS", "BATTERY", "HALL1", "HALL2"]
ioValue = [0       , 0       , 0        , 0        , 0             , 0           , 0        , 0      , 0      ]

globalVars =  []
globalValue = []
localVars =   []
localValue =  []

def RunScript(filePath:str):
    try:
        with open(filePath, 'r') as file:
            for line in file:
                RunCmd(line.strip())

    except FileNotFoundError:
        print("File does not exist.")

def CutTrashFromCommand(command:str):
    endPos = command.find(';')
    return command[:endPos]

def RunCmd(command:str):

    cmd = CutTrashFromCommand(command)
    print(cmd)
    commandWithArgs = cmd.split(' ')
    cmd = commandWithArgs[0]
    commandWithArgs.pop(0)
    args = commandWithArgs
    
    match cmd:

        case "shutdown":
            print("System shutting down.")

        case "delay":
            delay = args[0]
            print(f"wait {delay}")
            for _ in delay:
                pass

        case "run":
            script = args[0]
            print(f"Starting script {script}")
            RunScript(f"SystemScripts/{script}.jp")

        case "io":
            if(args[0] == "write"):
                try:
                    WriteIo(args[1], int(GetValue(args[2])))
                except:
                    print(f"Failed write io {args[1]}")

        case _:
            if( AreVariable(cmd) ):
                if(args[0] == "set"):
                    try:
                        newValue = int(GetValue(args[1]))
                        SetValue(cmd,newValue)
                        print(localVars)
                    except:
                        print("failed set variable.")
            else:
                print(f"command {cmd} not exists.")

def WriteIo(io:str, value:int):
    index = ioName.index(io)
    if(index != -1):
        ioValue[index] = value
        print(f"io {io} updated to {value}")
    else:
        print(f"io {io} not exists.")

def AreLocalVar(string:str):
    return (string[0] == "$" == string[-1])
        
def AreGlobalVar(string:str):
    return (string[0] == "%" == string[-1])

def AreVariable(string:str):
    return (AreLocalVar(string) or AreGlobalVar(string))

def GetValue(string:str):
    if(AreGlobalVar(string)):
        index = globalVars.index(string)
        if(index != -1):
            return globalValue[index]
        else:
            raise ValueError(f"variable {string} not exists in global variables.")
    elif(AreLocalVar(string)):
        index = localVars.index(string)
        if(index != -1):
            return localValue[index]
        else:
            raise ValueError(f"variable {string} not exists in local variables.")
    else:
        return string
    
def SetValue(string:str,value:str):
    print(f"try set '{string}' to '{value}'")
    variableExists = False
    try:
        oldvalue = GetValue(string)
        variableExists = True
    except:
        pass

    if variableExists:
        UpdateValue(string,value)
    else:
        AddValue(string,value)

def UpdateValue(string:str, value:str):
    if AreGlobalVar(string):
        index = globalVars.index(string)
        globalValue[index] = value
        print(f"updated global variable {string} to {value}")
    else:
        index = localVars.index(string)
        localValue[index] = value
        print(f"updated local variable {string} to {value}")
        print(localValue)

def AddValue(string:str, value:str):
    if AreGlobalVar(string):
        globalVars.append(string)
        globalValue.append(value)
        print(f"added new global variable {string}")
    elif AreLocalVar(string):
        localVars.append(string)
        localValue.append(value)
        print(f"added new local variable {string}")
