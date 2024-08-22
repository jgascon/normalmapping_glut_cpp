
//Variables que deben ir desde el vertex shader al fragment shader, estas 
//variables van cambiando y se van interpolando de un vertice al siguiente.

//Las variables varying tambien permiten pasar informacion de un vertice a los
//fragmentos.
varying vec3 lightDir;

//Variables que crean el espacio del vertice.
varying vec3 normal;
varying vec3 binormal;
varying vec3 tangent;

//Valores ambientales y difusos de la luz y del material.
varying vec4 material_diffuse;
varying vec4 material_ambient;

//Coordenadas de textura para pasarlas del vertice a los fragmentos.
varying vec2 tex0, tex1;

//Puntero a la textura de normales.
uniform sampler2D NormalMap;

//Puntero a la textura de color (textura difusa).
uniform sampler2D DiffuseMap;


void main()
{
    //Cogemos el pixel de color de la textura que toca.
    vec4 diffuse_tex = texture2D(DiffuseMap, tex1);

    //Cogemos el vector de la textura normal que toca, nos aseguramos
    //que pueda ser positivo o negativo.
    vec3 normal_tex = texture2D(NormalMap, tex0).rgb * 2.0 - 1.0;

    //Pasamos el vector de normal al espacio vertorial del vertice.
    normal_tex = normalize(normal_tex.x * tangent +
                           normal_tex.y * binormal +
                           normal_tex.z * normal);

    //El valor de intensidad se consigue gracias al producto escalar entre la luz
    //y el vector de normal, no puede ser superior a 1.0 ni inferior a 0.0.
    float intensity = clamp(dot(normal_tex, lightDir), 0.0, 1.0);

    //Finalmente el color es la suma de la aportacion difusa de la textura de color
    //y el color difuso del material, y este color depende de la intensidad que nos
    //diga la normal.
    vec4 color = material_diffuse.rgba * diffuse_tex * intensity;

    //Finalmente agregamos la contribucion de la luz ambiental.
    color += material_ambient.rgba * diffuse_tex;

    //Y finalmente plantamos el color definitivo.
    gl_FragColor = color;
}

