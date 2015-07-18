# Old-Presto-Engine
The earliest version of the engine, originally made for MinGW

This is the earliest version of my Presto game engine, done using MinGW's GCC compiler and C++.
Originally, the game engine was meant to implement several bitmaps to be used as layers. You know,
pretty much like Photoshop layers. The renderer draws in each layer, then mix them using the bit blitting
techniques provided by the Allegro library, with the back buffer, and then the Allegro renderer would do
the swapping.

It was a good experiment but, the process was extremely low and results in an extreme frame loss. So
obviously, I stopped developing the project at this point, and decided to rewrite it from scratch,
using Visual Studio and Allegro, and re-dessigning the whole rendering process to incorporate
"virtual" layers to define the order in which the sprites are drawn, and draw everything directly
into the back buffer instead, which is way faster.

This project compiles with MinGW and the latest version of the Allegro library for
windows/MinGW, but I used the Code::Blocks IDE to organize everything. The resources
were used for testing purposes only, and were not meant to become a full game even if
that's what their names suggest. Another reason to redo the project from scratch.

If you want to take a look into the last compilation without compiling the whole source
code (for example, you don't want to install and configure C::B or MinGW, and you just
want to study the source), please download it here:

https://www.dropbox.com/s/98vr6nplijx6p7a/OldPrestoEngine.7z?dl=0
