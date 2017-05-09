
varying vec2 V_Texcoord;

uniform sampler2D U_BaseTexture;
uniform sampler2D U_BlendTexture;

void main()
{
	vec4 blendColor=texture2D(U_BlendTexture,V_Texcoord);
	vec4 baseColor=texture2D(U_BaseTexture,V_Texcoord);

	gl_FragColor=abs(vec4(blendColor.rgb-baseColor.rgb,1.0));
}