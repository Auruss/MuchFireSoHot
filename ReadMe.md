Much Fire - So Hot
==================

AGILE Workflow
--------------

## Agile
We finally migrated to a SCRUM oriented workflow using youtrack (http://youtrack.arenaclash.net:8080/) 

## SCRUM Master:
- Game: Rene Eichhorn

Anybody else should not grab these issues however you are free create new tickets (this is encouraged)


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

WebApp
------
The web-app is currently completely on hold since the game should be focus #1 at the moment.
Once the game reaches an stage where you can actually play and enjoy it the web-app is continued.
