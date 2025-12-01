#version 330 core

out vec4 FragColor;

uniform float iTime;        // time in seconds
uniform vec3  iResolution;  // (width, height, 0.0)

// Shadertoy-style macros
#define V vec3
#define T        (iTime*4. + 5. + 5.*sin(iTime*.3))
#define P(z)     V( 12.*cos((z)*vec2(.1,.12)), z )
#define A(F,H,K) abs(dot(sin(F*p*K), H + p - p)) / K

void mainImage(out vec4 o, in vec2 u) {
    float t = 0.0;
    float s = 0.0;
    float i = 0.0;
    float d = 0.0;
    float e = 0.0;
    vec3  c = vec3(0.0);
    vec3  r = iResolution;

    // scaled coords
    u = (u - r.xy * 0.5) / r.y;

    // cinema bars
    if (abs(u.y) > 0.375) {
        o = vec4(0.0);
        return;
    }

    // setup ray origin, direction, and look-at
    vec3 p = P(T);
    vec3 Z = normalize(P(T + 4.0) - p);
    vec3 X = normalize(vec3(Z.z, 0.0, -Z.x));  // fixed: use -Z.x instead of -Z
    vec3 D = vec3(u, 1.0) * mat3(-X, cross(X, Z), Z);

    for (; i++ < 28.0 && d < 30.0;
         c += 1.0 / s + 10.0 * vec3(1.0, 2.0, 5.0) / max(e, 0.6))
    {
        // march
        p += D * s;

        // get path
        X = P(p.z);

        // store sine of iTime (not T)
        t = sin(iTime);

        // orb (sphere with xyz offset by t)
        e = length(p - vec3(
                    X.x + t,
                    X.y + t * 2.0,
                    6.0 + T + t * 2.0)) - 0.01;

        // tunnel with modulating radius
        s = cos(p.z * 0.6) * 2.0 + 4.0
          - min(length(p.xy - X.x - 6.0),
                length((p - X).xy))
          + A(4.0,  0.25, 0.1)   // noise, large scoops
          + A(T+8.0, 0.22, 2.0); // noise, detail texture

        // accumulate distance
        d += (s = min(e, 0.01 + 0.3 * abs(s)));
    }

    // adjust brightness and saturation
    o = vec4(c * c / 1e6, 1.0);
}

void main() {
    vec4 col;
    mainImage(col, gl_FragCoord.xy); // Shadertoy-style call
    FragColor = col;
}
