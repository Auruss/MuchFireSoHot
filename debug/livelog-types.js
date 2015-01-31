/**

 Livelog Types only

 */
var LiveLog_Types = {
    "LOG_STATS_FPS": {
        'id': 0,
        'description': "Simple FPS log <b>every</b> second",
        'option_defaults': {
            'DISPLAY': true,
            'STACK': true,
            'BREAKPOINT': false
        },
        'stack_functions': {
            'time-graph': {
                'init': (function () {
                    LiveLogHelper_line_graph_init('LOG_STATS_FPS', 'time-graph');
                    graph_data = [];
                    graph_counter = 0;
                    graph_graph = new CanvasJS.Chart("live-log-graph", {
                        title: {
                            text: "Live FPS"
                        },
                        axisY: {
                            interval: 5,
                            maximum: 65,
                            minimum: 40
                        },
                        data: [{
                            type: "line",
                            dataPoints: graph_data
                        }]
                    });
                    graph_graph.render();
                }),
                'do': (function (data) {
                    graph_data.push({
                        x: graph_counter++,
                        y: data.fps.fps
                    });

                    if (graph_data.length > 30) {
                        graph_data.shift();
                    }

                    graph_graph.render();
                })
            },
            'drop-alert': {
                'init': (function () {
                    LiveLog_toggleStackRuns('LOG_STATS_FPS', 'drop-alert');
                }),
                'do': (function (data) {
                    if (data.fps.fps < 40) {
                        LiveLog_disableStackRuns('LOG_STATS_FPS', 'drop-alert');
                        alert('WARNING: FPS UNDER 40');
                    }
                })
            }
        }
    },
    "LOG_STATS_GPU_BUFFER": {
        "id": 1,
        "description": "GPU Buffer updates",
        "option_defaults": {
            'DISPLAY': true,
            'STACK': true,
            'BREAKPOINT': false
        },
        "stack_functions": {
            "time-graph": {
                "init": (function () {
                    LiveLogHelper_line_graph_init('LOG_STATS_GPU_BUFFER', 'time-graph');
                    graph_data = [[], [], []];
                    graph_counter = 0;
                    graph_graph = new CanvasJS.Chart("live-log-graph", {
                        title: {
                            text: "GPU Buffer Changes"
                        },
                        axisY: {
                            interval: 750,
                            maximum: 10000,
                            minimum: 0
                        },
                        data: [
                            {
                                type: 'line',
                                name: 'index',
                                showInLegend: true,
                                dataPoints: graph_data[0]
                            },
                            {
                                type: 'line',
                                name: 'position',
                                showInLegend: true,
                                dataPoints: graph_data[1]
                            },
                            {
                                type: 'line',
                                name: 'color',
                                showInLegend: true,
                                dataPoints: graph_data[2]
                            }
                        ]
                    });
                    graph_graph.render();
                }),
                "do": (function (data) {
                    var xval = Math.floor(graph_counter++ / 3);
                    graph_data[data.stats.buffer_id - 2].push({x: xval, y: data.stats.bytes});


                    for (var i = 0; i < 3; i++) {
                        if (graph_data[i].length > 15) {
                            graph_data[i].shift();
                        }
                    }
                    graph_graph.render();
                })
            }
        }
    },
    "LOG_STATS_DRAW_CALLS": {
        'id': 2,
        "description": "Draw calls",
        "option_defaults": {
            'DISPLAY': true,
            'STACK': true,
            'BREAKPOINT': false
        },
        "stack_functions": {
            'time-graph': {
                'init': (function () {
                    LiveLogHelper_line_graph_init('LOG_STATS_DRAW_CALLS', 'time-graph');
                    graph_data = [];
                    graph_counter = 0;
                    graph_graph = new CanvasJS.Chart("live-log-graph", {
                        title: {
                            text: "Draw Calls"
                        },
                        axisY: {
                            interval: 50,
                            maximum: 1000,
                            minimum: 0
                        },
                        data: [{
                            type: "line",
                            dataPoints: graph_data
                        }]
                    });
                    graph_graph.render();
                }),
                'do': (function (data) {
                    graph_data.push({
                        x: graph_counter++,
                        y: data.stats.draw_calls
                    });

                    if (graph_data.length > 30) {
                        graph_data.shift();
                    }

                    graph_graph.render();
                })
            }
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
    }
};