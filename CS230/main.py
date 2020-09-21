from ArgumentGenerator import ArgumentGenerator

# Prints the list of options
def printList():
    print("1 Modus Ponens")
    print("2 Modus Tollens")
    print("3 Process of Elimination")
    print("4 Chain Rule")
    print("5 Affriming the Consequent")
    print("6 Denying the Antecedent")
    print("7 Begging the Question")

def inputTypeList():
    print("\n1. Automatic")
    print("2. User Input")
    inputType = input("\nSelect mode: ")

    if(inputType == "1" or inputType =="2"):
        return inputType
    else:
        print("\nPlease select a valid mode 1 for Automatic, 2 for Manual")
        inputTypeList()

validArgument = False


while(validArgument == False):
    printList()
    argument = input("\nSelect Argument: ")

    if(argument != "1" and argument != "2" and argument!= "3" and argument!="4" and argument != "5" and argument!= "6" and argument != "7"):
        print("\nPlease select a valid argument 1-7\n")
    else:
        validArgument = True
        mode = inputTypeList()
        myGenerator = ArgumentGenerator(argument, mode)
        myGenerator.generate()
      

    

        
        
    
