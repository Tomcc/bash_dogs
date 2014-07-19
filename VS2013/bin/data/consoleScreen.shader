
vertexShader = "
	uniform mat4 WORLDVIEWPROJ;

	attribute vec4 POSITION;
	attribute vec2 TEXCOORD_0;

	varying vec2 uv;

	void main()
	{
		// Transforming The Vertex
	    gl_Position = WORLDVIEWPROJ * POSITION;

	    uv = TEXCOORD_0;
	}
"

fragmentShader = "
	#version 120

	varying vec2 uv;

	uniform float DISTORTION;
	uniform vec2 TARGET_DIMENSION;

	uniform sampler2D TEXTURE_0;	//diffuse
	uniform sampler2D TEXTURE_1;	//night

	void main()
	{
		float apixel = DISTORTION / TARGET_DIMENSION.x;
		float dd = DISTORTION / 300.f;

		float center = pow( texture2D( TEXTURE_0, uv + vec2(dd, 0) ).g, 4.f);
		vec4 rgba = vec4(
			texture2D( TEXTURE_0, uv + vec2(apixel + dd, 0) ).g - center,
			center,
			texture2D( TEXTURE_0, uv + vec2(-apixel + dd, 0) ).g - center,
			1 );

		gl_FragColor = rgba + texture2D( TEXTURE_1, uv ) * 0.5;
	}

"