uniform vec4 U_AmbientLightColor;
uniform vec4 U_AmbientMaterial;

uniform sampler2D U_MainTexture;

varying vec3 V_Normal;
varying vec4 V_WorldPos;
varying vec2 V_Texcoord;

void main()
{
    vec3 lightPos = vec3(10.0,10.0,0.0);
    vec3 L = lightPos;

    L = normalize(L);
    vec3 n = normalize(V_Normal);

    //ambient
    U_AmbientLightColor = vec4(0.2,0.2,0.2,1.0);
    U_AmbientMaterial = vec4(0.2,0.2,0.2,1.0);
    
    vec4 ambientColor = U_AmbientLightColor * U_AmbientMaterial;
    
    //diffuse
    vec4 DiffuseLightColor = vec4(1.0,1.0,1.0,1.0);
    vec4 DiffuseMaterial = vec4(0.8,0.8,0.8,1.0);
    vec4 diffuseColor = DiffuseLightColor * DiffuseMaterial * max(0.0,dot(L,n));
  
    gl_FragColor= ambientColor+texture2D(U_MainTexture,V_Texcoord)*diffuseColor;
}