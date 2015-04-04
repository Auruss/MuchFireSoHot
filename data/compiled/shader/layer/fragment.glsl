#ifndef NATIVE
precision mediump float;
#endif
		
varying vec2 v_TexCoord;
varying vec3 v_PositionOrg;
        
uniform sampler2D mSampler;
uniform sampler2D mNormalSampler;

struct Light {
   vec3 position;
   vec3 color;
   float radius;
   float strength;
};

uniform int mLightCount;
uniform Light mLights[16];

void main() {
   vec4 normal = texture2D(mNormalSampler, v_TexCoord);
   vec4 diffuse = texture2D(mSampler, v_TexCoord);

   normal = normalize(normal * 2.0 - 1.0);
   vec3 distance = vec3(mLights[0].position.xy - v_PositionOrg.xy, mLights[0].position.z);
   vec3 light_dir = normalize(distance);
   float lambert = clamp(dot(normal, light_dir), 0.0, 1.0);
   float d = sqrt(dot(distance, distance));
   float att = 1.0;

   gl_FragColor = vec4((mLights[0].color * lambert) * att * diffuse.rgb, diffuse.a);
   //gl_FragColor = vec4(light_dir, 1.0);
}