
uniform sampler2D diffuse;
uniform int game_over;
varying vec2 texCoordVar;

void main()
{
        if(game_over == 2)
        {
            vec4 colour = texture2D(diffuse, texCoordVar);
            vec3 luminance = vec3(dot(vec3(0.2126, 0.7152, 0.0722), colour.rgb));
            gl_FragColor = vec4(luminance, colour.a);
        }
        else
        {
            gl_FragColor = texture2D(diffuse, texCoordVar);
        }

}

