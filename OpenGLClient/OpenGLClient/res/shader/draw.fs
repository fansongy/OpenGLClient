uniform vec4 U_AmbientLightColor;
uniform vec4 U_AmbientMaterial;

void main()
{
    U_AmbientLightColor = vec4(0.2,0.2,0.2,1.0);

    U_AmbientMaterial = vec4(0.2,0.2,0.2,1.0);
    
    vec4 ambientColor = U_AmbientLightColor * U_AmbientMaterial;
    gl_FragColor=vec4(ambientColor);
}