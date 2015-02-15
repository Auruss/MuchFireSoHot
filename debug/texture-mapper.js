function TextureMapper() {
}

TextureMapper.prototype = {
    openNew: function() {
        var html = "<div id='texture-mapper-image'><img src='../data/textures/test_big.jpg' /></div>";

        html +=
            "<div id='texture-mapper-menu'>" +
                "<form class='form-horizontal'>" +
                    "<div class='form-group'>" +
                        "<div class='input-group'>" +
                            "<div class='input-group-addon'>X</div>" +
                            "<input type='text' id='texture-mapper-x' />" +
                        "</div>" +
                        "<div class='input-group'>" +
                            "<div class='input-group-addon'>Y</div>" +
                            "<input type='text' id='texture-mapper-y' />" +
                        "</div>" +
                        "<div class='input-group'>" +
                            "<div class='input-group-addon'>Width</div>" +
                            "<input type='text' id='texture-mapper-w' />" +
                        "</div>" +
                        "<div class='input-group'>" +
                            "<div class='input-group-addon'>Height</div>" +
                            "<input type='text' id='texture-mapper-h' />" +
                        "</div>" +
                    "</div>" +
                "</form>" +

                "<form class='form-horizontal'>" +
                    "<div class='input-group'>" +
                        "<div class='input-group-addon'>Texture</div>" +
                        "<input type='text' id='texture-mapper-texture' />" +
                    "</div>" +
                "</form>" +
            "</div>";

        $("#dialog").html(html);
        $("#dialog").dialog({
            title: "Texture Mapper",
            width: 1200,
            height: 800
        });
        $("#dialog img").cropper({});
    }
};