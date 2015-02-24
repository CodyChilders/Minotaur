using UnityEngine;
using System.Collections;
using System.IO;
using System;

public class LevelReaderAndBuilder : MonoBehaviour {

    enum MazeTypes { Node, Hallway, Empty, Start, End, Junk } ;

    public GameObject wall;
    public GameObject floor;
    public GameObject start;
    public GameObject end;

    private bool unableToRead = false;
    

	// Use this for initialization
	void Start () {
        MazeTypes[,] maze = ReadFile("UnityLevelFile.maze");
        if(!unableToRead)
            BuildLevel(maze);
	}

    void OnGUI()
    {
        if (GUI.Button(new Rect(0, 0, 100, 100), "Exit Renderer"))
        {
            Application.Quit();
        }
        if (GUI.Button(new Rect(0, 100, 100, 100), "Reset"))
        {
            Application.LoadLevel(0);
        }
        if (unableToRead)
        {
            GUIStyle gs = new GUIStyle();
            gs.fontSize = 25;
            gs.alignment = TextAnchor.MiddleCenter;
            gs.richText = true;
            GUI.Box(new Rect(Screen.width / 3, Screen.height / 3, Screen.width / 3, Screen.height / 3), "<color=white>Unable to open file</color>", gs);
        }
    }

    void BuildLevel(MazeTypes[,] maze)
    {
        for (int i = -1; i < maze.GetLength(0) + 1; i++)
        {
            for (int j = -1; j < maze.GetLength(1) + 1; j++)
            {
                try
                {
                    switch (maze[i, j])
                    {
                        case MazeTypes.Node:
                            GameObject a;
                            if (IsInCenterOfWalls(maze, i, j))
                                goto case MazeTypes.Empty;
                            else
                                a = Instantiate(floor, new Vector3(i, 0, j), Quaternion.identity) as GameObject;
                            break;
                        case MazeTypes.Hallway:
                            goto case MazeTypes.Node;
                        case MazeTypes.Empty:
                            GameObject b = Instantiate(wall, new Vector3(i, 0.5f, j), Quaternion.identity) as GameObject;
                            break;
                        case MazeTypes.Start:
                            GameObject c = Instantiate(start, new Vector3(i, 0, j), Quaternion.identity) as GameObject;
                            break;
                        case MazeTypes.End:
                            GameObject d = Instantiate(end, new Vector3(i, 0, j), Quaternion.identity) as GameObject;
                            break;
                        case MazeTypes.Junk:
                            if (IsInCenterOfRoom(maze, i, j))
                                goto case MazeTypes.Node;
                            else
                                goto case MazeTypes.Empty;
                    }
                }
                catch (IndexOutOfRangeException e)
                {
                    GameObject f = Instantiate(wall, new Vector3(i, 0.5f, j), Quaternion.identity) as GameObject;
                }
            }
        }
    }

    bool IsInCenterOfRoom(MazeTypes[,] maze, int x, int y)
    {
        int neighboringHallways = 0;
        if (maze[x - 1, y] == MazeTypes.Hallway)
        {
            neighboringHallways++;
        }
        if (maze[x + 1, y] == MazeTypes.Hallway)
        {
            neighboringHallways++;
        }
        if (maze[x, y - 1] == MazeTypes.Hallway)
        {
            neighboringHallways++;
        }
        if (maze[x, y + 1] == MazeTypes.Hallway)
        {
            neighboringHallways++;
        }
        return neighboringHallways == 4;
    }

    bool IsInCenterOfWalls(MazeTypes[,] maze, int x, int y)
    {
        int neighboringWalls = 0;
        if (maze[x - 1, y] == MazeTypes.Empty)
        {
            neighboringWalls++;
        }
        if (maze[x + 1, y] == MazeTypes.Empty)
        {
            neighboringWalls++;
        }
        if (maze[x, y - 1] == MazeTypes.Empty)
        {
            neighboringWalls++;
        }
        if (maze[x, y + 1] == MazeTypes.Empty)
        {
            neighboringWalls++;
        }
        return neighboringWalls == 4;
    }

    MazeTypes[,] ReadFile(string filename)
    {
        try
        {
            using (StreamReader rawMazeData = File.OpenText(filename))
            {
                int size = int.Parse(rawMazeData.ReadLine());
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
                            case 'E':
                                editableMaze[i, j] = MazeTypes.End;
                                break;
                            case 'S':
                                editableMaze[i, j] = MazeTypes.Start;
                                break;
                        }
                    }
                }
                rawMazeData.Close();
                return editableMaze;
            }
        }
        catch (Exception e)
        {
            unableToRead = true;
            print(e);
            return null;
        }
    }
}
