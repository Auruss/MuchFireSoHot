Much Fire - So Hot
==================

AGILE Workflow
--------
https://waffle.io/Auruss/MuchFireSoHot is our AGILE issue board which is connected with github issues.

### Using our Agile board at waffle.io
It is self explaining, you grab any 'ready' issue you want but try to do things from the top to bottom since they are sorted.

## Agile
Our workflow is currently Kanban orientated, but we might switch to Scrum if the game reaches a further state.

## Backlog
People with access to backlog:
- Rene Eichhorn

Anybody else should not grab these issues however you are free create new tickets (this is encouraged)

Building
--------
Our build system is currently development only (no release builds yet)
On windows simply run build.bat on any other system build.sh

Checklist before building:
- Emscripten SDK is installed
- Check emcc/emc++ results without any parameters given.
- Make sure cmake, emc++, emcc is installed and in PATH

Running
-------
Since modern browsers use safe checks you can't use debugger.html using file:// protocol, you are going to need at least a simple web server (http)

WebApp
------
The web-app is currently completely on hold since the game should be focus #1 at the moment.
Once the game reaches an stage where you can actually play and enjoy it the web-app is continued.
