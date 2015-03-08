precision mediump float;

uniform mat4 mModifier;

attribute vec3 vPosition;
attribute vec2 vTexCoord;

varying vec2 v_TexCoord;

void main() {
   v_TexCoord = vTexCoord;
   gl_Position = mModifier * vec4(vPosition, 1.0);
}
