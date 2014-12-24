Much Fire - So Hot
==================

AGILE/SCRUM Workflow
--------
https://waffle.io/Auruss/MuchFireSoHot is our AGILE issue board which is connected with github issues.
(NOTE: Our forks are connected with that board as well.)

### Using our Agile board at waffle.io
1. Everybody can create an issue!
2. Once an issue gets approven. Its moved to ready 
	1. The SCRUM master is the only person that is allowed to approve
3. You can grab an issue that is approved (at ready) and move it to in progress once you start to work on that issue
	1. Please also assign the issue to yourself
4. Once you are done, pls push all changes to an branch (not master) and create an pull request to the master
5. The SCRUM master will review it / or asks somebody else to review
6. The SCRUM master will merge it to the master branch.

(NOTE: If you have an idea for making the game better please create an issue and wait for response)

### Current SCRUM master
Rene Eichhorn / Auruss

Building
--------
1. Create one root folder
2. Clone this repo + https://github.com/Auruss/SDL2_2.0.3_Fork and https://github.com/Auruss/oxygine-framework directly into the root folder
3. Open VS Project and compile
4. Check "Known building issues"

Known building issues 
---------------------
1. VS keeps ignoring "Copy Local" so SDL2.dll (+SDL2.pdb) must be updated manually in MuchFireSoHot/Debug(Release) after each patch to the SDL2 fork
