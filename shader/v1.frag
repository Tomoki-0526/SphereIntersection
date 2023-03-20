#version 330 core
uniform vec4 ColAttr;
uniform bool UseNor;
in vec3 o_nor;

void main() {
    if (UseNor){
        float coff = dot(o_nor, vec3(0.9, 0.9, 0.9));
        coff = (coff+2.0)/2.0;
        gl_FragColor = vec4(ColAttr.xyz * coff, ColAttr.w);
    }
    else{
        gl_FragColor = ColAttr;
    }
}
