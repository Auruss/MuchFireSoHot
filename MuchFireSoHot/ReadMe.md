Much Fire - So Hot
==================

Workflow
--------
https://waffle.io/Auruss/MuchFireSoHot is our AGILE issue board which is connected with github issues.
(NOTE: Our forks are not connected to any board.)

_Rules_: Do not touch backlog issues if you are not authorized to do so.

Building
--------
1. Create one root folder
2. Clone this repo + https://github.com/Auruss/SDL2_2.0.3_Fork and https://github.com/Auruss/oxygine-framework directly into the root folder
3. Open VS Project and compile
4. Check "Known building issues"

Known building issues 
---------------------
1. VS keeps ignoring "Copy Local" so SDL2.dll (+SDL2.pdb) must be updated manually in MuchFireSoHot/Debug(Release) after each patch to the SDL2 fork
