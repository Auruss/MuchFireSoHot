function hexToRgb(hex) {
    var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return result ? {
        r: parseInt(result[1], 16),
        g: parseInt(result[2], 16),
        b: parseInt(result[3], 16)
    } : null;
}
var STR_PAD_LEFT = 1;
var STR_PAD_RIGHT = 2;
var STR_PAD_BOTH = 3;

function pad(str, len, pad, dir) {

    if (typeof(len) == "undefined") { var len = 0; }
    if (typeof(pad) == "undefined") { var pad = ' '; }
    if (typeof(dir) == "undefined") { var dir = STR_PAD_RIGHT; }

    if (len + 1 >= str.length) {

        switch (dir){

            case STR_PAD_LEFT:
                str = Array(len + 1 - str.length).join(pad) + str;
                break;

            case STR_PAD_BOTH:
                var right = Math.ceil((padlen = len - str.length) / 2);
                var left = padlen - right;
                str = Array(left+1).join(pad) + str + Array(right+1).join(pad);
                break;

            default:
                str = str + Array(len + 1 - str.length).join(pad);
                break;

        } // switch

    }

    return str;

}


function EditorUI() {
    this.gui = new window.yagui.GuiMain($("#content")); // main gui
    this.layersidebar = this.gui.addRightSidebar();
    this.layersidebar.setVisibility(false);

    this.visible = false;
    this.mode = 0;
    this.data = [];
    this.common = {

    };
    this.camera = {
        x: 0,
        y: 0
    };
    this.layer = {
        x: 0,
        y: 0,
        z: 1,
        width: 0,
        height: 0,
        rotation: 0,
        texture_pos: 0
    };

    this.layerwidgets = {};
    this.camerawidgets = {};

    this.addCamera(this.layersidebar);
    this.addCommon(this.layersidebar);
    this.addLayer(this.layersidebar);
}

// --------------------------------------------------------------

/*
    Modes:
        1: Layer
 */
EditorUI.prototype.setMode = function(mode) {
    var temp = this.visible;
    this.hide();
    this.mode = mode;
    if(temp) {
        this.show();
    }
};

// --------------------------------------------------------------

EditorUI.prototype.addCommon = function(sidebar) {
    var folder = sidebar.addMenu("Common");
    folder.addButton("New Layer", function() {
        _editor_update_vals(4);
    });
};

EditorUI.prototype.addCamera = function(sidebar) {
    var cam = sidebar.addMenu("Camera");
    this.camerawidgets.x = cam.addSlider("x", this.camera.x, (function(x) {editor_ui_instance.camera.x = x; _editor_update_vals(2);}), 0, 5000, 1);
    this.camerawidgets.y = cam.addSlider("y", this.camera.y, (function(y) {editor_ui_instance.camera.y = y; _editor_update_vals(2);}), 0, 5000, 1);
};

EditorUI.prototype.addLayer = function(sidebar) {
    var layer = sidebar.addMenu("Layer");
    this.layerwidgets.x = layer.addSlider("x", this.layer.x, (function(x) {editor_ui_instance.layer.x = x; _editor_update_vals(0);}), 0, 5000, 1);
    this.layerwidgets.y = layer.addSlider("y", this.layer.y, (function(y) {editor_ui_instance.layer.y = y; _editor_update_vals(0);}), 0, 5000, 1);
    this.layerwidgets.z = layer.addSlider("z", this.layer.z, (function(z) {editor_ui_instance.layer.z = z; _editor_update_vals(0);}), 1, 100, 1);
    this.layerwidgets.width = layer.addSlider("Width", this.layer.width, (function(x) {editor_ui_instance.layer.width = x; _editor_update_vals(0);}), 0, 5000, 1);
    this.layerwidgets.height = layer.addSlider("Height", this.layer.height, (function(y) {editor_ui_instance.layer.height = y; _editor_update_vals(0);}), 0, 5000, 1);
    this.layerwidgets.rotation = layer.addSlider("Rotation", this.layer.rotation, function(rot) {editor_ui_instance.layer.rotation = rot; _editor_update_vals(0);}, 0, 360, 1);

    layer.addButton("Set Texture", function() {
        var mapper = new TextureMapper();
        mapper.openNew(function(rect) {
            editor_ui_instance.layer.texture_pos = rect;
            _editor_update_vals(5);
        });
    });

    layer.addButton("Delete Layer", function() {
        _editor_update_vals(3);
    });
};

// --------------------------------------------------------------

EditorUI.prototype.refreshLayer = function() {
    this.refreshWidgets(this.layerwidgets, this.layer);
};

EditorUI.prototype.refreshCamera = function() {
    this.refreshWidgets(this.camerawidgets, this.camera);
};

EditorUI.prototype.refreshWidgets = function(widgets, object) {
    for(var widget in widgets) {
        if(widgets.hasOwnProperty(widget)) {
            widgets[widget].setValue(object[widget]);
        }
    }
};

// --------------------------------------------------------------

EditorUI.prototype.show = function() {
    if(this.mode == 1) {
        this.layersidebar.setVisibility(true);
    }
    this.visible = true;
};

EditorUI.prototype.hide = function() {
    if(this.mode == 1) {
        this.layersidebar.setVisibility(false);
    }
    this.visible = false;
};

EditorUI.prototype.toggle = function() {
    if(this.visible) {
        this.hide();
    } else {
        this.show();
    }
};

var editor_ui_instance = new EditorUI();