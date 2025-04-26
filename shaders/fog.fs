#version 330

uniform sampler2D texture0;
uniform vec2 spotlightPos;  //  Spotlight position
uniform float spotlightRadius;  //  Spotlight size

in vec2 fragTexCoord;
out vec4 finalColor;

void main() {
    vec2 lightDist = fragTexCoord - spotlightPos;
    float dist = length(lightDist);

    float spotlightFactor = smoothstep(spotlightRadius, 0.0, dist);  //  Smooth light fade

    vec4 texColor = texture(texture0, fragTexCoord);
    
    // Apply transparency outside the spotlight area
    texColor.a *= spotlightFactor;  // Reduce alpha for spotlight effect

    finalColor = texColor;
}