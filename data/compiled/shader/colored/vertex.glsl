#ifndef NATIVE
precision mediump float;
#endif

uniform mat4 mModifier;

attribute vec3 vPosition;
attribute vec4 vColor;

varying vec4 v_Color;

void main() {
   v_Color = vColor;
   gl_Position = mModifier * vec4(vPosition, 1.0);
}

