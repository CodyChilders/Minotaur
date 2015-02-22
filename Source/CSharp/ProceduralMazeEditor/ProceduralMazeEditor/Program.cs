using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace ProceduralMazeEditor
{

    public enum MazeTypes { Node, Hallway, Empty, Start, End, Junk } ;

    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length != 1)
            {
                Console.WriteLine("Error: expected filename as first argument to editor program.  Exiting.");
                Thread.Sleep(5000);
                Environment.Exit(1);
            }

            if (!File.Exists(args[0]))
            {
                Console.WriteLine("Error: file {0} not found.", args[0]);
                Thread.Sleep(5000);
                Environment.Exit(2);
            }

            using (StreamReader rawMazeData = File.OpenText(args[0]))
            {
                int size = Int32.Parse(rawMazeData.ReadLine());
                MazeTypes[,] editableMaze = new MazeTypes[size * 2 - 1, size * 2 - 1];
                for (int i = 0; i < size * 2 - 1; i++)
                {
                    string line = rawMazeData.ReadLine();
                    for (int j = 0; j < size * 2 - 1; j++)
                    {
                        switch (line[j])
                        {
                            case ' ':
                                editableMaze[i, j] = MazeTypes.Empty;
                                break;
                            case '+':
                                editableMaze[i, j] = MazeTypes.Node;
                                break;
                            case '-':
                                goto case '|';
                            case '|':
                                editableMaze[i, j] = MazeTypes.Hallway;
                                break;
                            case 'j':
                                editableMaze[i, j] = MazeTypes.Junk;
                                break;
                            case 'S':
                                editableMaze[i, j] = MazeTypes.Start;
                                break;
                            case 'E':
                                editableMaze[i, j] = MazeTypes.End;
                                break;
                        }
                    }
                }
                rawMazeData.Close();
                MazeEditor.EnterEditMode(editableMaze);
            }
        }
    }
}
