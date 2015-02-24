from subprocess import call
from shutil import copyfile
from shutil import copytree
import os
import py_compile

#use copyfile(src, dst)

def getCPP():
    copytree("CPP/PrimMazeGenerator/Release", "ProceduralMaze/Resources/Generator")

def getCSharp():
    copytree("CSharp/ProceduralMazeEditor/ProceduralMazeEditor/bin/Release", "ProceduralMaze/Resources/Editor")

def getPython():
    #compile this one first, because python doesn't work like visual studio
    os.chdir("Python");
    py_compile("ProceduralMazeTools.py", "ProceduralMazeTools.pyc")
    os.chdir("..")
    copyfile("Python/ProceduralMazeTools.pyc", "ProceduralMaze/ProceduralMazeTools.pyc")

def getUnity():
    pass

def main():
    try:
        os.removedirs("ProceduralMaze")
    except:
        pass
    os.mkdir("ProceduralMaze/");
    getCPP()
    getCSharp()
    getUnity()
    getPython()


main()
