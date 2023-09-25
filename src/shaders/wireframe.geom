#version 330 core

// 0. Declares input as triangles and output as lines
// NOTE: This will basically convert triangles to lines. If we wanted to convert to points, we could use `out` as `(points, max_vertices=3)`
layout (triangles) in;
layout (line_strip, max_vertices=3) out;

in VertexShaderOutput {
    vec3 worldPosition;
    vec2 uv;
    vec3 normal;
} geometryInputs[];

in vec2 uvPass[];				// Input from Fragment Shader
in vec2 normalsPass[];			// Input from Fragment Shader

out vec3 normal;
out vec2 uv;

// 1. Passes all vertex attributes forward without changing them, the conversion to lines is a given
void main() {
	int i;
	for (i = 0; i < gl_in.length(); i++) {
		uv = geometryInputs[i].uv;
		normal = geometryInputs[i].normal;
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}
