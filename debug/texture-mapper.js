function TextureMapper() {
}

TextureMapper.prototype = {
    refreshCropper: function() {
        var trans_data = {
            'left': parseInt($("#texture-mapper-x").val()),
            'top': parseInt($("#texture-mapper-y").val()),
            'width': parseInt($("#texture-mapper-w").val()),
            'height': parseInt($("#texture-mapper-h").val())
        };

        $("#dialog img").cropper('setCropBoxData', trans_data);
    },
    openNew: function(okCallback) {
        var html = "<div id='texture-mapper-image'><img src='../data/compiled/default-map.png' /></div>";

        html +=
            "<div id='texture-mapper-menu'>" +
                "<form class='form-horizontal'>" +
                    "<div class='form-group'>" +
                        "<div class='input-group'>" +
                            "<div class='input-group-addon'>X</div>" +
                            "<input type='text' class='form-control' id='texture-mapper-x' />" +
                            "<div class='input-group-addon'>0.0</div>" +
                        "</div>" +
                        "<div class='input-group'>" +
                            "<div class='input-group-addon'>Y</div>" +
                            "<input type='text' class='form-control' id='texture-mapper-y' />" +
                            "<div class='input-group-addon'>0.0</div>" +
                        "</div>" +
                        "<div class='input-group'>" +
                            "<div class='input-group-addon'>Width</div>" +
                          "<input type='text' class='form-control' id='texture-mapper-w' />" +
                            "<div class='input-group-addon'>0.0</div>" +
                        "</div>" +
                        "<div class='input-group'>" +
                            "<div class='input-group-addon'>Height</div>" +
                            "<input type='text' class='form-control' id='texture-mapper-h' />" +
                            "<div class='input-group-addon'>0.0</div>" +
                        "</div>" +
                    "</div>" +
                "</form>" +

                "<form class='form-horizontal'>" +
                    "<button id='ok-texture' class='btn btn-default' type='button'>OK, set!</button>" +
                "</form>" +
            "</div>";

        $("#dialog").html(html);
        $("#dialog").dialog({
            title: "Texture Mapper",
            width: 1200,
            height: 800
        });

        $("#dialog img").cropper({
            crop: $.proxy(function(data) {
                $("#texture-mapper-x").val(Math.round(data.x));
                $("#texture-mapper-y").val(Math.round(data.y));
                $("#texture-mapper-w").val(Math.round(data.width));
                $("#texture-mapper-h").val(Math.round(data.height));
                this.data_cache = data;
            }, this)
        });

        // change handlers
        $("#texture-mapper-x").change(this.refreshCropper);
        $("#texture-mapper-y").change(this.refreshCropper);
        $("#texture-mapper-w").change(this.refreshCropper);
        $("#texture-mapper-h").change(this.refreshCropper);

        $("#ok-texture").on("click", $.proxy(function() {
            okCallback(this.data_cache);
            $("#dialog").dialog("close");
        }, this));
    }
};