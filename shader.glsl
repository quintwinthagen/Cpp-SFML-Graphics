#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform float size;

void main( void )
{
	if(distance(vec2(mouse.x, -mouse.y + resolution.y) , gl_FragCoord.xy) < size)
	{
		gl_FragColor = vec4(sin(time)+1, cos(time)+1, 0.0, 1.0);
	}
	else
	{
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	}

	//gl_FragColor = vec4(mouse.x/resolution.x, mouse.x/resolution.y, 0.0, 1.0);

}