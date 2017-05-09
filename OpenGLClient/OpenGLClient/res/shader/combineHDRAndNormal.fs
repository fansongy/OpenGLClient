
varying vec2 V_Texcoord;

uniform sampler2D U_MainTexture;
uniform sampler2D U_HDRTexture;

void main()
{
	gl_FragColor=texture2D(U_MainTexture,V_Texcoord)+texture(U_HDRTexture,V_Texcoord);
}