#version 330 core
uniform vec4 ColAttr;
uniform bool UseNor;
in vec3 o_nor;

void main() {
    if (UseNor){
        float coff = dot(o_nor, vec3(0.9, 0.9, 0.9));
//        if (coff < 0){
//            coff = (-coff);
//        }
        coff = (coff+2.0)/2.0;
        gl_FragColor = vec4(ColAttr.xyz * coff, ColAttr.w);//vec4(o_nor, 1.0);//
    }
    else{
        gl_FragColor = ColAttr;
    }
}
