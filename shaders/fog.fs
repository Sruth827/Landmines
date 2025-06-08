#version 100
precision mediump float; 

// Input from the vertex shader
varying vec2 fragTexCoord;
varying vec4 fragColor;


// Uniforms (values sent from your C++ code)
uniform sampler2D texture0;
uniform vec2 spotlightPos;
uniform float spotlightRadius;
uniform float screenHeight;

void main()
{
    // Get the original color of the game world pixel
    vec4 texColor = texture2D(texture0, fragTexCoord);

    // --- Night Vision Effect Logic ---

    // 1. Convert the original color to grayscale to get its brightness (luminance)
    float gray = dot(texColor.rgb, vec3(0.299, 0.587, 0.114));

    // 2. Create a new color by using the gray value to create a green tint
    // We multiply the gray value by a green vector. The G channel is high, R and B are low.
    vec3 nightVisionTint = vec3(gray * 0.1, gray * 0.95, gray * 0.2);

    // 3. Combine the new tinted color with the original alpha (transparency)
    vec4 tintedColor = vec4(nightVisionTint, texColor.a);


    // --- Fog of War Logic (This part stays the same) ---

    // Flip the shader's Y-coordinate to match Raylib's screen coordinates
    vec2 flippedFragCoord = vec2(gl_FragCoord.x, screenHeight - gl_FragCoord.y);

    // Calculate distance from the player
    float distance = length(flippedFragCoord - spotlightPos);
    
    // Define the fog color (black)
    vec4 fogColor = vec4(0.0, 0.0, 0.0, 1.0);

    // Calculate the visibility based on the distance from the player
    float visibility = 1.0 - smoothstep(spotlightRadius, spotlightRadius + 40.0, distance);

    // Mix the TINTED color with the fog color based on the visibility
    gl_FragColor = mix(fogColor, tintedColor * fragColor, visibility);

    
}