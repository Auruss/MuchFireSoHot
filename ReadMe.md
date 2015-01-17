Much Fire - So Hot
==================

AGILE/SCRUM Workflow
--------
https://waffle.io/Auruss/MuchFireSoHot is our AGILE issue board which is connected with github issues.

### Using our Agile board at waffle.io
It is self explaining however here are some rules to follow
- EVERYONE is allowed to create issues, however moving issues to ready is only allowed for the SCRUM master.
- Once you start working on an issue, please assign it to yourself, and move it to "in progress"
- Once you are done, push any changes to your branch and create an pull request with "master" (DO NOT MERGE)
    - Please put either "resolves #1" or "fixed #1" in the pull requests name, it will then automatically move the issue to "waiting for merge"
- Once merged by the SCRUM master it gets automatically moved to "Done".

Follow these simple steps/rules and you will learn to love the agile workflow.


### Current SCRUM master
Rene Eichhorn / Auruss

Building
--------
You can work with whatever IDE you want! build.sh is all you need to run in order to do a proper build.

Checklist before building:
- Emscripten SDK is installed
- Check emcc/emc++ results without any parameters given.
- Make sure cmake is installed and in PATH
