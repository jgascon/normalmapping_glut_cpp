
//Tangente recibida desde la aplicación principal.
attribute vec3 external_tangent;

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

void main() {

    //Pasando al shader de fragmentos las coordenadas de textura que nos haran falta.
    tex0 = vec2(gl_MultiTexCoord1);
    tex1 = vec2(gl_MultiTexCoord2);

    //Se consiguen los vectores normal y tangente y el binormal se calcula a partir de ellos.
    normal   = normalize(gl_NormalMatrix * gl_Normal);
    tangent  = normalize(gl_NormalMatrix * external_tangent);
    binormal = normalize(gl_NormalMatrix * cross(tangent, normal));

    //La posicion de la luz es normalizada.
    lightDir = normalize(vec3(gl_LightSource[0].position));

    //Se cogen los colores ambiental y difuso del material y de la luz
    material_diffuse  = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
    material_ambient  = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
    material_ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;

    //El vertice es transformado a las coordenadas de pantalla.
    gl_Position = ftransform();
}


