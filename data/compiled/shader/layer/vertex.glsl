precision mediump float;

uniform mat4 mModifier;
uniform vec2 mEyes;

attribute vec3 vPosition;
attribute vec2 vTexCoord;
attribute vec2 vOrigin;

varying vec2 v_TexCoord;

void main() {
   // Global
   v_TexCoord = vTexCoord;

   // Parallax
   float cam_distance = mEyes.x - vOrigin.x;

   gl_Position = mModifier * vec4(vPosition.x + ((cam_distance/100.0) * (99.0 - vPosition.z)), vPosition.yz, 1.0);
}
