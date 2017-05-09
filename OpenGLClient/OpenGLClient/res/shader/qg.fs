
varying vec2 V_Texcoord;

uniform sampler2D U_BaseTexture;
uniform sampler2D U_BlendTexture;

void main()
{
	vec4 lumCoeff=vec4(0.2125,0.7154,0.0721,1.0);
	vec4 blendColor=texture2D(U_BlendTexture,V_Texcoord);
	vec4 baseColor=texture2D(U_BaseTexture,V_Texcoord);
	float luminance=dot(blendColor.rgb,lumCoeff.rgb);
	if(luminance<0.45)
	{
		gl_FragColor=2.0*blendColor*baseColor;
	}
	else if(luminance>0.55)
	{
		gl_FragColor=vec4(1.0)-2.0*(vec4(1.0)-baseColor)*(vec4(1.0)-blendColor);
	}
	else
	{
		vec4 color1=2.0*blendColor*baseColor;
		vec4 color2=vec4(1.0)-2.0*(vec4(1.0)-baseColor)*(vec4(1.0)-blendColor);
		gl_FragColor=mix(color1,color2,(luminance-0.45)*10.0);
	}
}