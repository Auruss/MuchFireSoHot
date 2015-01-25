var LiveLogHelper_line_graph_init = function(id, fn) {
    $("#graphModal").modal({'backdrop': false});
    $("#graphModal").one('hidden.bs.modal', (function() {
        LiveLog_disableStackRuns(id, fn);
    }));
    LiveLog_enableStackRuns(id, fn);
};

var LiveLogHelper_line_graph_do = function(value, max_data, id) {

};