// LiveLog ----------------------------
var LiveLog_active_stack = [];
var LiveLog_object_cache = [];

var LiveLog_init = function(table) {
    var html = "<table id='livelog-table' class='table table-hover'>";
    html += "<thead>";
    html += "<th width='85px'>ID</th>";
    html += "<th>Message</th>";
    html += "<th width='35px'>Count</th>";
    html += "<th width='10%'>Functions</th>";
    html += "</thead>";

    for(var key in LiveLog_Types) {
        if(LiveLog_Types.hasOwnProperty(key)) {
            if(LiveLog_Types[key].option_defaults.STACK != true) continue;
            LiveLog_active_stack[key] = [];

            html += "<tr class='info'>";
            html += "<td><a href='#'>" + key + "</a></td>";
            html += "<td>" + LiveLog_Types[key].description + "</td>";
            html += "<td id='livelog-stack-" + LiveLog_Types[key].id + "' align='right'>" + 0 + "</td>";
            html += "<td>";
            for(var fn in LiveLog_Types[key].stack_functions) {
                if(LiveLog_Types[key].stack_functions.hasOwnProperty(fn)) {
                    html +=
                        "<button id='"+fn+"-"+key+"' class='btn btn-success btn-sm' onclick='" +
                            "LiveLog_Types[\""+key+"\"].stack_functions[\""+fn+"\"].init();'>" + fn + "</button>";
                }
            }
            html += "</td>";
            html += "</tr>";
        }
    }

    html += "</table>";

    $(table).html(html);
};

var LiveLog_toggleStackRuns = function(id, fn) {
    if(LiveLog_active_stack[id][fn] == true) {
        LiveLog_disableStackRuns(id, fn);
    } else {
        LiveLog_enableStackRuns(id, fn);
    }
};

var LiveLog_enableStackRuns = function(id, fn) {
    $("#"+fn+"-"+id).toggleClass('btn-success');
    $("#"+fn+"-"+id).toggleClass('btn-danger');

    LiveLog_active_stack[id][fn] = true;
};

var LiveLog_disableStackRuns = function(id, fn) {
    $("#"+fn+"-"+id).toggleClass('btn-success');
    $("#"+fn+"-"+id).toggleClass('btn-danger');

    LiveLog_active_stack[id][fn] = false;
};

var LiveLog_push = function(builder) {
    var log_type = LiveLog_getLogType(builder.log_id);
    var global_type = LiveLog_getGlobalType(builder.global_type);

    if(log_type.option_defaults.STACK == true) {
        var d = $("#livelog-stack-" + log_type.id);
        d.html(parseInt(d.html()) + 1);

        for(var fn in LiveLog_active_stack[log_type.key]) {
            if(LiveLog_active_stack[log_type.key].hasOwnProperty(fn)) {
                if(LiveLog_active_stack[log_type.key][fn] == true)
                    log_type.stack_functions[fn].do(builder.parsed_refs);
            }
        }
        return;
    }
    else if (log_type.option_defaults.BREAKPOINT == true) {
        console.log(Module.stackTrace());
        debugger;
    }

    LiveLog_object_cache[LiveLog_object_cache.length] = builder.parsed_refs;
    var cache_id = LiveLog_object_cache.length - 1;

    var html = "<tr style='background-color: "+global_type.bg+"; color: "+global_type.fg+";'>";
    html += "<td><a href='#'>" + log_type.key + "</a></td>";
    html += "<td>" + builder.msg + "</td>";
    html += "<td align='right'>" + builder.refs.length + "</td>";
    if(builder.refs.length > 0) {
        html += "<td>" + "<button class='btn btn-success btn-sm' onclick='LiveLog_showObjects(" + cache_id + ");'>Show objects</button>" + "</td>";
    } else { html += "<td></td>"}
    html += "</tr>";
    $("#livelog-table").append(html);
};

var LiveLog_json = {
    replacer: function(match, pIndent, pKey, pVal, pEnd) {
        var key = '<span class=json-key>';
        var val = '<span class=json-value>';
        var str = '<span class=json-string>';
        var r = pIndent || '';
        if (pKey)
            r = r + key + pKey.replace(/[": ]/g, '') + '</span>: ';
        if (pVal)
            r = r + (pVal[0] == '"' ? str : val) + pVal + '</span>';
        return r + (pEnd || '');
    },
    prettyPrint: function(obj) {
        var jsonLine = /^( *)("[\w]+": )?("[^"]*"|[\w.+-]*)?([,[{])?$/mg;
        return JSON.stringify(obj, null, 3)
            .replace(/&/g, '&amp;').replace(/\\"/g, '&quot;')
            .replace(/</g, '&lt;').replace(/>/g, '&gt;')
            .replace(jsonLine, LiveLog_json.replacer);
    }
};

var LiveLog_showObjects = function(id) {
    $("#live-log-graph").html("<pre>"+LiveLog_json.prettyPrint(LiveLog_object_cache[id])+"</pre>");
    $("#graphModal").modal({'backdrop': false});
};

var LiveLog_getLogType = function(id) {
    for(var key in LiveLog_Types) {
        if(LiveLog_Types.hasOwnProperty(key)) {
            if (LiveLog_Types[key].id == id) {
                LiveLog_Types[key].key = key;
                return LiveLog_Types[key];
            }
        }
    }
};

var LiveLog_getGlobalType = function(id) {
    for(var key in LiveLog_GlobalTypes) {
        if(LiveLog_GlobalTypes.hasOwnProperty(key)) {
            if (LiveLog_GlobalTypes[key].id == id) {
                LiveLog_GlobalTypes[key].key = key;
                return LiveLog_GlobalTypes[key];
            }
        }
    }
};

// Builder ------------------
var live_log_instance;

function LiveLogBuilder() {
}

LiveLogBuilder.prototype.init = function(id, global_type) {
    this.log_id = id;
    this.global_type = global_type;
    this.refs = [];
    this.parsed_refs = [];
    this.options = [];
    this.msg = LiveLog_getLogType(id).description;
};

LiveLogBuilder.prototype.setMessage = function(msg) {
    this.msg = Pointer_stringify(msg);
};

LiveLogBuilder.prototype.addReferenceObject = function(name, obj_json) {
    this.refs[this.refs.length] = { 'name': Pointer_stringify(name), 'value': Pointer_stringify(obj_json) };
    this.parsed_refs = $.extend({}, this.parsed_refs, JSON.parse(this.refs[this.refs.length-1].value));
};


LiveLogBuilder.prototype.push = function() {
    LiveLog_push(this);
};
