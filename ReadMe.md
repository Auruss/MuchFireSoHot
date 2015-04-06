Much Fire - So Hot
==================

AGILE Workflow
--------------

Once again we changed our workflow, from SCRUM to Kanban since SCRUM didn't work out for now.
You can find our kanban board on trello (ask me to get invited)

Development Environment
=======================
- Install elasticsearch and kibana, make sure you use the newest kibana4 branch and build it yourself. (important)
- Modify the port if necessary for elasticsearch in livelog.js (yes I know config would be nice)
- Check livelog-types.js to change what gets logged and what not.
- In emscripten environment you need Firebug/Chrome debugger open in order to cache breakpoints

Sample screenshot for a nice dashboard with fps graph:
![Sample kibana dashboard](https://lh4.googleusercontent.com/izMgbmPCz1bxhGsDfS4F7Xp9w5gyYHnulPkcL7fjZW4dc-k0VIBhq9Daw5HQgfH5Kj1hKtzw_yDZtYg=w1896-h835 "Sample screenshot")


Emscripten
==========

Building
--------
Our build system is currently development only (no release builds yet)
On windows simply run build.bat on any other system build.sh

Checklist before building:
- CMake 2.x
- Emscripten
    -- we made some changes to emscripten which are not live yet, so you need to use the incoming branch of emscripten or the master of my fork

Running
-------
Since modern browsers use safe checks you can't use debugger.html using file:// protocol, you are going to need at least a simple web server (http)

Native Development System
==================

Building
--------
Currently building natively is offically only supported on windows, it should work on OS X and Linux.
Run build_vs.bat to build the visual studio project files

Checklist:
- CMake 2.x
- Get Awesomium! (Free to use)
    - Don't forget to link it and change include paths
    - Copy the Awesomium bin folder to the root folder
    - Set manually the start parameters
    - Set manually the working directory to root


Running
-------

API
---
The API is currently located in an different repo called ArenaClashAPI

Website
-------
The Website is currently located in an different repo called ArenaClashWebsite

Fun facts
---------
- If you run the game longer than 2 years on the same machine without a restart the game will crash.
- We actually wanted to stay emscripten only (even in the dev environment....) (stupid right?)