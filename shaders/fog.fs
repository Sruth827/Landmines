#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

// Uniforms from C++
uniform sampler2D texture0;
uniform vec2 spotlightPos;
uniform float spotlightRadius;
uniform float screenHeight; // <-- NEW: To fix the Y-axis

void main()
{
    vec4 texColor = texture(texture0, fragTexCoord);

    // Flip the shader's Y-coordinate to match Raylib's screen coordinates
    vec2 flippedFragCoord = vec2(gl_FragCoord.x, screenHeight - gl_FragCoord.y);

    // Calculate distance using the correctly flipped coordinate
    float distance = length(flippedFragCoord - spotlightPos);
    
    vec4 fogColor = vec4(0.0, 0.0, 0.0, 1.0); // Black fog

    // Smoothly blend from light to fog
    float visibility = 1.0 - smoothstep(spotlightRadius, spotlightRadius + 40.0, distance);

    finalColor = mix(fogColor, texColor * fragColor, visibility);
}