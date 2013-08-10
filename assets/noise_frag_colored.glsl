#version 330 core

uniform usampler2DRect tex;

out vec4 color;

void main() {

	vec4 colors[] = vec4[](
		vec4( 0.0,  0.5,  0.8,  1.0 ), 	// Deep blue  	(deep sea) 	0
		vec4( 0.0,  0.62, 1.0,  1.0 ),	// Blue       	(sea)		1
		vec4( 1.0,  1.0,  0.0,  1.0 ),  // Yellow 	  	(shore)		2
		vec4( 0.57, 0.92, 0.0,  1.0 ),	// Light green	(lawn)		3
		vec4( 0.40, 0.64, 0.0,  1.0 ),	// Dark green	(lawn)		4
		vec4( 0.59, 0.43, 0.0,  1.0 ),	// Light brown	(dirt) 		5
		vec4( 0.47, 0.34, 0.0,  1.0 ),	// Dark brown 	(hill)		6
		vec4( 0.3,  0.3,  0.3,  1.0 ), 	// Dark grey 	(stnoe)		7
		vec4( 0.5,  0.5,  0.5,  1.0 ), 	// Grey 	  	(stone)  	8
		vec4( 0.8,  0.8,  0.8,  1.0 ),	// Light grey 	(stone) 	9
		vec4( 1.0,  1.0,  1.0,  1.0 )	// White 	  	(ice cap)	10

	);


	color = colors[ texture( tex, gl_FragCoord.xy ).r ];

}
