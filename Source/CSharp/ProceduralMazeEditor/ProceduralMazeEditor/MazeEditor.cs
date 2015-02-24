using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProceduralMazeEditor
{
    class MazeEditor
    {

        static bool savedSinceLastEdit = true;
        static Random rng = new Random();
        static Stack<MazeTypes[,]> undoStates = new Stack<MazeTypes[,]>();

        public static void EnterEditMode(MazeTypes[,] maze)
        {
            Console.WriteLine("Welcome to the procedural maze editor!\n");
            Console.WriteLine("Here is the maze you are working with:\n" + PrintMaze(maze) + "\n");
            undoStates.Push(CopyMaze(maze));
            while (true)
            {
                Console.Write("What would you like to do? \n(R)emove dead ends\n(A)dd hallways\n(O)ne edge\n(I)dentify start and end point\n(U)ndo\n(S)ave\n(E)xit\n(H)elp\n>> ");
                string command = Console.ReadLine();
                command = command.ToLower();
                switch (command)
                {
                    case "r":
                        undoStates.Push(CopyMaze(maze));
                        Autosave(CopyMaze(maze));
                        RemoveDeadEnds(maze);
                        goto case "p";
                    case "a":
                        undoStates.Push(CopyMaze(maze));
                        Autosave(CopyMaze(maze));
                        AddHallways(maze);
                        goto case "p";
                    case "o":
                        undoStates.Push(CopyMaze(maze));
                        Autosave(CopyMaze(maze));
                        OneEdge(maze);
                        goto case "p";
                    case "i":
                        undoStates.Push(CopyMaze(maze));
                        Autosave(CopyMaze(maze));
                        SetStartAndEndPoint(maze);
                        goto case "p";
                    case "u":
                        if (undoStates.Count() > 1)
                        {
                            maze = undoStates.Pop();
                            goto case "p";
                        }
                        else
                        {
                            Console.WriteLine("Unable to undo.  There are no previous changes\n");
                        }
                        break;
                    case "s":
                        Save(maze);
                        break;
                    case "e":
                        Exit();
                        break;
                    case "h":
                        PrintHelpInfo();
                        break;
                    case "p":
                        Console.WriteLine(PrintMaze(maze));
                        break;
                    default:
                        Console.WriteLine("Unrecognized command: {0}\n", command);
                        break;
                }
            }
        }

        private static void PrintHelpInfo()
        {
            Console.WriteLine("- The plusses and dashes are the maze, not the blank areas between them.");
            Console.WriteLine("- If you connect 4 nodes in a 2x2 square with hallways, they will be built and rendered as a single room.  An \"XXX\" indicates that it is a room.");
            Console.WriteLine("- Adding hallways and removing dead ends work on a percentage.  Enter the percent you want removed, and that many will be modified.");
            Console.WriteLine("- Entering a percentage can be done as either a floating point number from 0 to 1, or an integer number from 1 to 100.");
            Console.WriteLine("- The undo function can undo steps all the way to when you opened the file.");
            Console.WriteLine("- If you do not identify at least one start point in the maze, the renderer will be unable to automatically move you into the maze.  If you enter more than one, it will choose randomly.");
            Console.WriteLine("- The one-edge functionality allows you finer control over the maze.  You can only do it one hallway at a time.");
            Console.WriteLine("- There is a secret command \"p\" on the editor main menu that prints the maze again.  It is not listed because every operation that changes the maze also prints the new maze out.");
            Console.WriteLine("");
        }

        private static void SetStartAndEndPoint(MazeTypes[,] maze)
        {
            savedSinceLastEdit = false;
            Console.Write("Do you want to add a (s)tart or (e)nd node? ");
            bool startPoint = true;
SetStartAndEndPointCharInput:
            string option = Console.ReadLine().ToLower();
            switch (option)
            {
                case "s":
                    startPoint = true;
                    break;
                case "e":
                    startPoint = false;
                    break;
                default:
                    Console.WriteLine("Invalid Option");
                    goto SetStartAndEndPointCharInput;
            }
            Console.Write("Enter the coordinate of the node you want to make a " + (startPoint ? "starting" : "ending" ) + " point (expects 2 integers): ");
            int node_x, node_y;
SetStartAndEndPointTwoIntInput:
            try
            {
                string[] inputs = Console.ReadLine().Split(null);
                node_x = int.Parse(inputs[0]);
                node_y = int.Parse(inputs[1]);
            }
            catch (FormatException e)
            {
                Console.WriteLine("Unable to parse the inputs as integers");
                goto SetStartAndEndPointTwoIntInput;
            }
            catch (IndexOutOfRangeException e)
            {
                Console.WriteLine("Not enough inputs");
                goto SetStartAndEndPointTwoIntInput;
            }
            if (node_x < 0 || node_x > maze.GetLength(0) / 2 || node_y < 0 || node_y > maze.GetLength(0) / 2)
            {
                Console.WriteLine("Inputs are out of range");
                goto SetStartAndEndPointTwoIntInput;
            }
            //the *2 is because only the even number slots have nodes
            //figure out if it is a normal or special node, switch them
            maze[node_x * 2, node_y * 2] = (maze[node_x * 2, node_y * 2] == MazeTypes.Node ? (startPoint ? MazeTypes.Start : MazeTypes.End ) : MazeTypes.Node); 
        }

        private static void OneEdge(MazeTypes[,] maze)
        {
            savedSinceLastEdit = false;
            Console.WriteLine("Remember that this counts by the + symbols, starting at 0 and counting up.");
            Console.Write("Which node do you want to start with?  Enter row number first, followed by the column: ");
            int node_x;
            int node_y;
OneEdgeTwoIntInput:
            try
            {
                string[] node = Console.ReadLine().Split(null);
                node_x = int.Parse(node[0]);
                node_y = int.Parse(node[1]);
            }
            catch (FormatException e)
            {
                Console.WriteLine("Unable to parse two inputs as integers");
                goto OneEdgeTwoIntInput;
            }
            catch (IndexOutOfRangeException e)
            {
                Console.WriteLine("Not enough inputs");
                goto OneEdgeTwoIntInput;
            }
            if (node_x >= maze.GetLength(0) / 2 + 1 || node_x < 0 || node_y >= maze.GetLength(1) / 2 + 1 || node_y < 0)
            {
                Console.WriteLine("Numbers entered are out of bounds");
                goto OneEdgeTwoIntInput;
            }
            Console.Write("What direction do you want to go? (u)p, (d)own, (l)eft, (r)ight: ");
            char direction = '\0';
OneEdgeDirectionInput:
            string input = Console.ReadLine().ToLower();
            switch (input)
            {
                case "u":
                    direction = 'u';
                    break;
                case "d":
                    direction = 'd';
                    break;
                case "l":
                    direction = 'l';
                    break;
                case "r":
                    direction = 'r';
                    break;
                default:
                    Console.WriteLine("Invalid option: {0}\n", input);
                    goto OneEdgeDirectionInput;
            }
            Console.Write("What operation would you like to perform? (a)dd hallway, (r)emove hallway: ");
            bool addingHallway = true;
OneEdgeAddRemovePrompt:
            input = Console.ReadLine().ToLower();
            switch (input)
            {
                case "a":
                    addingHallway = true;
                    break;
                case "r":
                    addingHallway = false;
                    break;
                default:
                    Console.WriteLine("Invalid option: {0}", input);
                    goto OneEdgeAddRemovePrompt;
            }
            //finally we can do the operation
            try
            {
                MazeTypes edge = (addingHallway ? MazeTypes.Hallway : MazeTypes.Empty);
                switch (direction)
                {
                    case 'r':
                        maze[node_x * 2, node_y * 2 + 1] = edge;
                        break;
                    case 'l':
                        maze[node_x * 2, node_y * 2 - 1] = edge;
                        break;
                    case 'u':
                        maze[node_x * 2 - 1, node_y * 2] = edge;
                        break;
                    case 'd':
                        maze[node_x * 2 + 1, node_y * 2] = edge;
                        break;
                }
            }
            catch (IndexOutOfRangeException e)
            {
                Console.WriteLine("\n\nThe hallway you have attempted to change has gone off the edge of the map.  Unable to perform operation.\n\n");
            }
        }

        private static MazeTypes[,] CopyMaze(MazeTypes[,] maze)
        {
            MazeTypes[,] newMaze = new MazeTypes[maze.GetLength(0), maze.GetLength(1)];
            for (int i = 0; i < maze.GetLength(0); i++)
            {
                for (int j = 0; j < maze.GetLength(1); j++)
                {
                    newMaze[i, j] = maze[i, j];
                }
            }
            return newMaze;
        }

        private static void Exit()
        {
            if (savedSinceLastEdit)
            {
                Environment.Exit(0);
            }
            else
            {
                Console.Write("You have not saved since your last edit.  Would you like to go back and save? (y/n): ");
                while (true)
                {
                    string line = Console.ReadLine();
                    line = line.ToLower();
                    switch (line)
                    {
                        case "y":
                            return;
                        case "n":
                            Environment.Exit(0);
                            break;
                        default:
                            Console.WriteLine("Invalid command");
                            break;
                    }
                }
            }
        }

        private static void Save(MazeTypes[,] maze)
        {
            savedSinceLastEdit = true;
            Console.Write("Enter filename to save to: ");
SaveReadFilenamePrompt:
            string filename = Console.ReadLine();
            if (!filename.EndsWith(".maze"))
            {
                filename += ".maze";
            }
            if (filename.ToLower() == "basemaze.maze" || filename.ToLower() == "unitymazefile.maze" || filename.ToLower() == "autosave.maze")
            {
                Console.Write("{0} is a filename used by the editor, you can't save a file to that name.  Choose a new filename: ", filename);
                goto SaveReadFilenamePrompt;
            }
            System.IO.File.WriteAllText(@filename, ((maze.GetLength(0) + 1) / 2).ToString() + "\n" + PrintMaze(maze, true));
        }

        private static void Autosave(MazeTypes[,] maze)
        {
            System.IO.File.WriteAllText(@"autosave.maze", ((maze.GetLength(0) + 1) / 2).ToString() + "\n" + PrintMaze(maze, true));
        }

        private static void AddHallways(MazeTypes[,] maze)
        {
            savedSinceLastEdit = false;
            float percentage = GetFloat();
            //starting at 0 and counting by 2's should only hit the nodes in the maze
            for (int i = 0; i < maze.GetLength(0); i += 2)
            {
                for (int j = 0; j < maze.GetLength(1); j += 2)
                {
                    //check to the left
                    try
                    {
                        if (maze[i - 1, j] == MazeTypes.Empty)
                        {
                            float dieRoll = (float)rng.NextDouble();
                            if (dieRoll < percentage)
                            {
                                maze[i - 1, j] = MazeTypes.Hallway;
                            }
                        }
                    }
                    catch (IndexOutOfRangeException e)
                    {
                        //it went off the edge of the map, no need to freak out.  Just keep going
                    }
                    //check to the right
                    try
                    {
                        if (maze[i + 1, j] == MazeTypes.Empty)
                        {
                            float dieRoll = (float)rng.NextDouble();
                            if (dieRoll < percentage)
                            {
                                maze[i + 1, j] = MazeTypes.Hallway;
                            }
                        }
                    }
                    catch (IndexOutOfRangeException e)
                    {
                        //it went off the edge of the map, no need to freak out.  Just keep going
                    }
                    //check up
                    try
                    {
                        if (maze[i, j - 1] == MazeTypes.Empty)
                        {
                            float dieRoll = (float)rng.NextDouble();
                            if (dieRoll < percentage)
                            {
                                maze[i, j - 1] = MazeTypes.Hallway;
                            }
                        }
                    }
                    catch (IndexOutOfRangeException e)
                    {
                        //it went off the edge of the map, no need to freak out.  Just keep going
                    }
                    //check down
                    try
                    {
                        if (maze[i, j + 1] == MazeTypes.Empty)
                        {
                            float dieRoll = (float)rng.NextDouble();
                            if (dieRoll < percentage)
                            {
                                maze[i, j + 1] = MazeTypes.Hallway;
                            }
                        }
                    }
                    catch (IndexOutOfRangeException e)
                    {
                        //it went off the edge of the map, no need to freak out.  Just keep going
                    }
                }
            }
        }

        //note: this will return false if it is a node with nothing connected to it.
        private static bool IsDeadEnd(int x, int y, MazeTypes[,] maze, out int numberOfConnections)
        {
            int neighboringHallways = 0;
            try
            {
                if (maze[x - 1, y] == MazeTypes.Hallway)
                    neighboringHallways++;
            }
            catch (IndexOutOfRangeException e)
            {
                //off the edge of the map.  carry on...
            }
            try
            {
                if (maze[x + 1, y] == MazeTypes.Hallway)
                    neighboringHallways++;
            }
            catch (IndexOutOfRangeException e)
            {
                //off the edge again.  we're still good
            }
            try
            {
                if (maze[x, y - 1] == MazeTypes.Hallway)
                    neighboringHallways++;
            }
            catch (IndexOutOfRangeException e)
            {
                //still doing alright...
            }
            try
            {
                if (maze[x, y + 1] == MazeTypes.Hallway)
                    neighboringHallways++;
            }
            catch (IndexOutOfRangeException e)
            {
                //no problem here, officer.
            }
            //now figure out if it is a dead end
            numberOfConnections = neighboringHallways;
            if (neighboringHallways == 1)
                return true;
            else
                return false;
        }

        //This will check all neighbors, one at a time, and return after the first hallway it finds has been changed into an empty
        //Assumes that that node has already been verified as a dead end using IsDeadEnd.
        private static void RemoveDeadEnd(int x, int y, MazeTypes[,] maze)
        {
            try
            {
                if (maze[x - 1, y] == MazeTypes.Hallway)
                {
                    maze[x - 1, y] = MazeTypes.Empty;
                    return;
                }
            }
            catch (IndexOutOfRangeException e)
            {
                //off the edge of the map.  carry on...
            }
            try
            {
                if (maze[x + 1, y] == MazeTypes.Hallway)
                {
                    maze[x + 1, y] = MazeTypes.Empty;
                    return;
                }
            }
            catch (IndexOutOfRangeException e)
            {
                //off the edge again.  we're still good
            }
            try
            {
                if (maze[x, y - 1] == MazeTypes.Hallway)
                {
                    maze[x, y - 1] = MazeTypes.Empty;
                    return;
                }
            }
            catch (IndexOutOfRangeException e)
            {
                //still doing alright...
            }
            try
            {
                if (maze[x, y + 1] == MazeTypes.Hallway)
                {
                    maze[x, y + 1] = MazeTypes.Empty;
                    return;
                }
            }
            catch (IndexOutOfRangeException e)
            {
                //no problem here, officer.
            }
        }

        private static void RemoveDeadEnds(MazeTypes[,] maze)
        {
            savedSinceLastEdit = false;
            float percentage = GetFloat();
            MazeTypes[,] mazeCopy = CopyMaze(maze);//This one exists so that a branch that was previously not an end node and became one through editing doesn't get treated as one
            for (int i = 0; i < maze.GetLength(0); i += 2)
            {
                for (int j = 0; j < maze.GetLength(1); j += 2)
                {
                    int junk; //Don't need this, just to satisfy IsDeadEnd.
                    if (IsDeadEnd(i, j, mazeCopy, out junk) && mazeCopy[i, j] == MazeTypes.Node)//It has to be a node here so it doesn't pull hallways away from start/end points
                    {
                        float dieRoll = (float)rng.NextDouble();
                        if (dieRoll < percentage)
                        {
                            RemoveDeadEnd(i, j, maze);
                        }
                    }
                }
            }
        }

        private static bool IsSurroundedByHallways(MazeTypes[,] maze, int x, int y)
        {
            //check up, down, left, and right to see if a hallway exists at that space.
            //don't need to try/catch it all because junk can't be on an edge.
            int neighboringHallways = 0;
            if (maze[x, y + 1] == MazeTypes.Hallway)
                neighboringHallways++;
            if (maze[x, y - 1] == MazeTypes.Hallway)
                neighboringHallways++;
            if (maze[x + 1, y] == MazeTypes.Hallway)
                neighboringHallways++;
            if (maze[x - 1, y] == MazeTypes.Hallway)
                neighboringHallways++;
            return neighboringHallways == 4;
        }

        private static float GetFloat()
        {
            Console.Write("Enter the percentage of this operation happening at each point, or leave blank if you want it random: ");
            while (true)
            {
                string input = Console.ReadLine();
                //they entered nothing, they want a random value
                if (input == "")
                {
                    return (float)rng.NextDouble();
                }
                //attempt to parse it as an int
                try
                {
                    int val = int.Parse(input);
                    if (val < 0 || val > 100)
                    {
                        Console.WriteLine("Range must be from 0 to 100 for inputting an integer");
                        continue;
                    }
                    return val / 100.0f;
                }
                catch (FormatException e)
                {
                    // no good as an int, try as a float
                }
                //see if they were kind enough to enter a valid float for us
                try
                {
                    float val = float.Parse(input);
                    if (val < 0 || val > 1)
                    {
                        Console.WriteLine("Range must be from 0 to 1 for inputting a floating point value");
                        continue;
                    }
                    return val;
                }
                catch (FormatException e)
                {
                    //it wasn't a valid float either, ERROR
                }
                Console.WriteLine("Unable to parse {0} as a percentage.  Enter either a floating point number n such that 0 <= n <= 1, or an integer n such that 0 <= n <= 100");
            }
        }

        private static string PrintMaze(MazeTypes[,] maze, bool printingToFile = false)
        {
            string mazeString = "";
            if (!printingToFile)
            {
                //show the column numbers on the top
                mazeString += "  ";
                for (int i = 0; i < maze.GetLength(0) / 2 + 1; i++)
                {
                    string number = i.ToString();
                    number = string.Format("{0,4}", number);
                    mazeString += number;
                }
                mazeString += "\n";
            }
            for (int i = 0; i < maze.GetLength(0); i++)
            {
                if (!printingToFile)
                {
                    if (i % 2 == 0)
                    {
                        //print the row numbers
                        string number = (i / 2).ToString();
                        number = string.Format("{0,4}", number) + " ";
                        mazeString += number;
                    }
                    else
                    {
                        mazeString += "     ";
                    }
                }
                for (int j = 0; j < maze.GetLength(1); j++)
                {
                    switch (maze[i, j])
                    {
                        case MazeTypes.Empty:
                            if (i % 2 == 0)
                                mazeString += (printingToFile ? " " : "   ");
                            else
                                mazeString += " ";
                            break;
                        case MazeTypes.Hallway:
                            if (i % 2 == 0)
                                mazeString += (printingToFile ? "-" : "---");
                            else
                                mazeString += "|";
                            break;
                        case MazeTypes.Node:
                            int neighbors;
                            IsDeadEnd(i, j, maze, out neighbors); //Throwing away the bool here, but we want to see if it is all alone
                            if (!printingToFile)
                                mazeString += (neighbors != 0 ? "+" : " ");
                            else
                                mazeString += "+";
                            break;
                        case MazeTypes.Start:
                            mazeString += "S";
                            break;
                        case MazeTypes.End:
                            mazeString += "E";
                            break;
                        case MazeTypes.Junk:
                            if (printingToFile)
                            {
                                mazeString += "j";
                            }
                            else
                            {
                                if (IsSurroundedByHallways(maze, i, j))
                                {
                                    mazeString += "XXX";
                                }
                                else
                                {
                                    mazeString += "   ";
                                }
                            }
                            //mazeString += (printingToFile ? "j" : "   ");
                            break;
                        default:
                            mazeString += "\u262D"; //This is an error indicator.  Hopefully the commies don't get into our maze...
                            break;
                    }
                }
                mazeString += "\n";
            }
            return mazeString;
        }
    }
}
