
uniform vec3 U_EyePos;

varying vec3 V_Normal;
varying vec3 V_WorldPos;

void main()
{
	//L vector
	vec3 L=U_EyePos-V_WorldPos;

	L=normalize(L);
	//N vector
	vec3 n=normalize(V_Normal);
	//reflection
	float cosThta=dot(L,n);
	float alpha=0.0;
	if(cosThta>0.0)
	{
		alpha=1-cosThta;
	}
	gl_FragColor=vec4(1.0,1.0,1.0,alpha);
}