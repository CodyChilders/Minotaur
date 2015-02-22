from subprocess import call as invoke
import time
import os

def openRenderer():
    filename = input("Enter the filename you would like to render: ")
    try:
        os.rename(filename, "UnityLevelFile.maze")
    except:
        print("File not found\n");
        return
    invoke(["Renderer/MazeRenderer.exe"])
    time.sleep(1)
    os.rename("UnityLevelFile.maze", filename)

def listMazeFiles():
    print("Here are the known editor files: ")
    for file in os.listdir():
        if(file.endswith(".maze")):
           print(file)
    print("")

def main():
    os.chdir("Resources")
    print("Welcome to the maze generator and editor by Cody Childers.")
    while True :
        option = input("What would you like to do? (N)ew maze, (L)oad maze, (F)ile listing, (D)elete file, (R)ender, (E)xit: ")
        if option == "n" or option == "N":
            #invoke the maze generator
            invoke("Generator/PrimMazeGenerator.exe")
            #it takes a second, so hang out while it builds and saves
            print("Generating your maze...\n");
            time.sleep(1) #sleep for a second so it can finish writing
            #invoke the maze editor
            invoke(["Editor/ProceduralMazeEditor.exe", "baseMaze.maze"])
            try:
                os.remove("baseMaze.maze")
            except:
                pass
        elif option == "l" or option == "L":
            filename = input("Enter the filename you want to load: ")
            #invoke the editor
            invoke(["Editor/ProceduralMazeEditor.exe", filename])
        elif option == "d" or option == "D":
            deadFile = input("Enter the name of the file you want to delete: ")
            try:
                os.remove(deadFile)
                print("")
            except:
                print("File not found\n")
        elif option == "e" or option == "E":
            break
        elif option == "r" or option == "R":
            openRenderer()
        elif option == "f" or option == "f":
            listMazeFiles()
        else:
            print("Invalid option\n")

main()
