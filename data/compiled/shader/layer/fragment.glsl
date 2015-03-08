precision mediump float;
		
varying vec2 v_TexCoord;
        
uniform sampler2D mSampler;

void main() {
   gl_FragColor = texture2D(mSampler, v_TexCoord);
}