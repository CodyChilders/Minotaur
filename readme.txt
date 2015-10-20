Procedural maze creation tools, by Cody Childers

Minotaur contains the project.  Run the .pyc file, this is the file manager
that links the whole thing together.  You shouldn't need to go into the Resources folder.

The source folder contains the source code, separated out into 4 parts:
- CSharp contains the editor, written in C#.  It takes in a filename passed in by the
Python file manager.  It contains all the tools that you can use to modify the maze.  Save
it out to a file for later editing or for rendering.
- CPP contains the generator.  It uses a graph of nodes, who contain edges to their 
up/down/left/right neighbors with a random weight.  It then calculates the minimum 
spanning tree using Prim's algorithm, and it reverse engineers the search to turn it
into a format that can be printed out to a file.  The graph and list underlying it all
were written by myself and a friend in 2013, the main file was written by me to run on
the command line also in 2013.  I made some quick (admittedly hackish) changes to redirect
that data to a file format that can be read for this project.
- Unity contains the rendering program.  It reads in a specific filename (renamed by the
Python script) and uses a near identical parser as the C# editor to put it into a readable
format.  It then uses some pre-built Unity game objects and spawns new instances of them
as it iterates over the maze.
- Python contains the file manager that ties them all together into a single coherent
program.  It handles save files, options, and passing the correct data into the correct
program and putting everything into the right directory.