#ifndef NATIVE
precision mediump float;
#endif

uniform mat4 mModifier;
uniform vec2 mEyes;

attribute vec3 vPosition;
attribute vec2 vTexCoord;
attribute vec2 vOrigin;

varying vec2 v_TexCoord;
varying vec3 v_PositionOrg;

void main() {
   // Global
   v_TexCoord = vTexCoord;
   v_PositionOrg = vPosition;

   // Parallax
   float cam_distance = mEyes.x - vOrigin.x;

   gl_Position = mModifier * vec4(vPosition.x + ((cam_distance/100.0) * (99.0 - vPosition.z)), vPosition.yz, 1.0);
}
