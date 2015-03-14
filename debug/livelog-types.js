/**

 Livelog Types only

 */
var LiveLog_Enable = true;

var LiveLog_GlobalTypes = {
    "LOG_DEBUG":    {"id": 0},
    "LOG_ERROR":    {"id": 1},
    "LOG_INFO":     {"id": 2},
    "LOG_WARNING":  {"id": 3}
};

var LiveLog_Types = {
    "LOG_STATS_FPS": {
        'id': 0,
        'description': "Simple FPS log <b>every</b> second",
        'option_defaults': {
            'DISPLAY': true,
            'STACK': true,
            'BREAKPOINT': false
        }
    },
    "LOG_STATS_GPU_BUFFER": {
        "id": 1,
        "description": "GPU Buffer updates",
        "option_defaults": {
            'DISPLAY': true,
            'STACK': true,
            'BREAKPOINT': false
        }
    },
    "LOG_STATS_DRAW_CALLS": {
        'id': 2,
        "description": "Draw calls",
        "option_defaults": {
            'DISPLAY': true,
            'STACK': true,
            'BREAKPOINT': false
        }
    },
    "LOG_CRITICAL_RENDER_ERROR": {
        "id": 3,
        "description": "Critical render errors",
        "option_defaults": {
            "DISPLAY": true,
            "STACK": false,
            "BREAKPOINT": true
        }
    },
    "LOG_PROGRAM_MVP": {
        "id": 4,
        "description": "MVP Uniform changed / has to change",
        "option_defaults": {
            "DISPLAY": true,
            "STACK": false,
            "BREAKPOINT": false
        }
    },

    "LOG_EDITOR": {
        "id": 5,
        "description": "Editor related logs",
        "option_defaults": {
            "DISPLAY": true,
            "STACK": false,
            "BREAKPOINT": false
        }
    },
    "LOG_STD_OUTPUT": {
        "id": 6,
        "description": "Console styled outputs from standard output",
        "option_defaults": {
            "DISPLAY": true,
            "STACK": false,
            "BREAKPOINT": false
        }
    },
    "LOG_STD_ERR_OUTPUT": {
        "id": 7,
        "description": "Console styled outputs from the error output",
        "option_defaults": {
            "DISPLAY": true,
            "STACK": false,
            "BREAKPOINT": false
        }
    },
    "LOG_GL_EXTENSIONS": {
        "id": 8,
        "description": "Logs all available WebGL extensions available on current computer.",
        "option_defaults": {
            "DISPLAY": true,
            "STACK": false,
            "BREAKPOINT": false
        }
    }
};