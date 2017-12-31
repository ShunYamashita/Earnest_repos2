xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 66;
 -21.03614;0.00000;1.59547;,
 -0.70445;0.00000;3.63320;,
 -0.70446;1.39037;3.35664;,
 -21.03613;0.61056;1.47402;,
 -0.70445;2.56906;2.56906;,
 -21.03613;1.12817;1.12817;,
 -0.70446;3.35664;1.39037;,
 -21.03614;1.47402;0.61056;,
 -0.70445;3.63320;0.00000;,
 -21.03613;1.59547;0.00000;,
 -0.70445;3.35664;-1.39037;,
 -21.03614;1.47402;-0.61056;,
 -0.70445;2.56906;-2.56906;,
 -21.03613;1.12817;-1.12817;,
 -0.70446;1.39037;-3.35664;,
 -21.03613;0.61056;-1.47402;,
 -0.70445;0.00000;-3.63320;,
 -21.03614;0.00000;-1.59547;,
 -0.70445;-1.39036;-3.35664;,
 -21.03613;-0.61056;-1.47402;,
 -0.70445;-2.56906;-2.56906;,
 -21.03613;-1.12817;-1.12817;,
 -0.70445;-3.35664;-1.39037;,
 -21.03614;-1.47402;-0.61056;,
 -0.70445;-3.63320;0.00000;,
 -21.03613;-1.59547;0.00000;,
 -0.70445;-3.35664;1.39037;,
 -21.03614;-1.47402;0.61056;,
 -0.70445;-2.56906;2.56906;,
 -21.03613;-1.12817;1.12817;,
 -0.70446;-1.39036;3.35664;,
 -21.03613;-0.61056;1.47402;,
 -0.70445;0.00000;3.63320;,
 -21.03614;0.00000;1.59547;,
 -21.03613;0.00000;0.00000;,
 -21.03613;0.00000;0.00000;,
 -21.03613;0.00000;0.00000;,
 -21.03613;0.00000;0.00000;,
 -21.03613;0.00000;0.00000;,
 -21.03613;0.00000;0.00000;,
 -21.03613;0.00000;0.00000;,
 -21.03613;0.00000;0.00000;,
 -21.03613;0.00000;0.00000;,
 -21.03613;0.00000;0.00000;,
 -21.03613;0.00000;0.00000;,
 -21.03613;0.00000;0.00000;,
 -21.03613;0.00000;0.00000;,
 -21.03613;0.00000;0.00000;,
 -21.03613;0.00000;0.00000;,
 -21.03613;0.00000;0.00000;,
 -0.70445;0.00000;0.00000;,
 -0.70445;0.00000;0.00000;,
 -0.70445;0.00000;0.00000;,
 -0.70445;0.00000;0.00000;,
 -0.70445;0.00000;0.00000;,
 -0.70445;0.00000;0.00000;,
 -0.70445;0.00000;0.00000;,
 -0.70445;0.00000;0.00000;,
 -0.70445;0.00000;0.00000;,
 -0.70445;0.00000;0.00000;,
 -0.70445;0.00000;0.00000;,
 -0.70445;0.00000;0.00000;,
 -0.70445;0.00000;0.00000;,
 -0.70445;0.00000;0.00000;,
 -0.70445;0.00000;0.00000;,
 -0.70445;0.00000;0.00000;;
 
 48;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,16,17;,
 4;17,16,18,19;,
 4;19,18,20,21;,
 4;21,20,22,23;,
 4;23,22,24,25;,
 4;25,24,26,27;,
 4;27,26,28,29;,
 4;29,28,30,31;,
 4;31,30,32,33;,
 3;34,0,3;,
 3;35,3,5;,
 3;36,5,7;,
 3;37,7,9;,
 3;38,9,11;,
 3;39,11,13;,
 3;40,13,15;,
 3;41,15,17;,
 3;42,17,19;,
 3;43,19,21;,
 3;44,21,23;,
 3;45,23,25;,
 3;46,25,27;,
 3;47,27,29;,
 3;48,29,31;,
 3;49,31,33;,
 3;50,2,1;,
 3;51,4,2;,
 3;52,6,4;,
 3;53,8,6;,
 3;54,10,8;,
 3;55,12,10;,
 3;56,14,12;,
 3;57,16,14;,
 3;58,18,16;,
 3;59,20,18;,
 3;60,22,20;,
 3;61,24,22;,
 3;62,26,24;,
 3;63,28,26;,
 3;64,30,28;,
 3;65,32,30;;
 
 MeshMaterialList {
  4;
  48;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.600000;0.600000;0.600000;1.000000;;
   5.000000;
   0.500000;0.500000;0.500000;;
   0.400000;0.400000;0.400000;;
   TextureFilename {
    "C:\\Users\\yamashita.shun\\Desktop\\DX22_lessom08\\data\\TEXTURE\\cyber2.jpg";
   }
  }
  Material {
   0.600000;0.600000;0.600000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.400000;0.400000;0.400000;;
   TextureFilename {
    "C:\\Users\\yamashita.shun\\Desktop\\md\\data\\TEXTURE\\wood.jpg";
   }
  }
  Material {
   0.600000;0.600000;0.600000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.400000;0.400000;0.400000;;
   TextureFilename {
    "C:\\Users\\yamashita.shun\\Desktop\\DX22_lessom08\\data\\TEXTURE\\cloud2.jpg";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  49;
  -1.000000;-0.000000;0.000000;,
  -0.099725;-0.000000;0.995015;,
  -0.099725;0.380775;0.919274;,
  -0.099725;0.703582;0.703582;,
  -0.099725;0.919274;0.380775;,
  -0.099725;0.995015;-0.000000;,
  -0.099725;0.919274;-0.380775;,
  -0.099725;0.703582;-0.703582;,
  -0.099725;0.380775;-0.919274;,
  -0.099725;-0.000001;-0.995015;,
  -0.099725;-0.380775;-0.919274;,
  -0.099725;-0.703581;-0.703583;,
  -0.099725;-0.919274;-0.380775;,
  -0.099725;-0.995015;0.000000;,
  -0.099725;-0.919274;0.380775;,
  -0.099725;-0.703581;0.703582;,
  -0.099725;-0.380774;0.919275;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;-0.000001;,
  -1.000000;0.000002;-0.000001;,
  -1.000000;-0.000000;0.000002;,
  -1.000000;-0.000001;0.000001;,
  -1.000000;0.000000;-0.000000;,
  -1.000000;-0.000001;0.000000;,
  -1.000000;-0.000001;-0.000001;,
  -1.000000;0.000001;0.000000;,
  -1.000000;0.000000;0.000001;,
  -1.000000;-0.000001;0.000000;,
  -1.000000;0.000001;-0.000001;,
  -1.000000;0.000001;0.000000;,
  -1.000000;0.000001;0.000001;,
  -1.000000;0.000000;0.000002;,
  -1.000000;-0.000002;-0.000001;,
  1.000000;0.000001;0.000002;,
  1.000000;-0.000000;-0.000000;,
  1.000000;0.000000;0.000000;,
  1.000000;0.000003;0.000001;,
  1.000000;0.000000;0.000004;,
  1.000000;-0.000000;-0.000000;,
  1.000000;-0.000003;-0.000003;,
  1.000000;0.000001;-0.000003;,
  1.000000;0.000004;-0.000000;,
  1.000000;0.000000;-0.000000;,
  1.000000;-0.000000;-0.000000;,
  1.000000;0.000000;-0.000000;,
  1.000000;0.000000;-0.000000;,
  1.000000;0.000000;0.000000;,
  1.000000;0.000002;0.000003;,
  1.000000;-0.000001;0.000002;;
  48;
  4;1,1,2,2;,
  4;2,2,3,3;,
  4;3,3,4,4;,
  4;4,4,5,5;,
  4;5,5,6,6;,
  4;6,6,7,7;,
  4;7,7,8,8;,
  4;8,8,9,9;,
  4;9,9,10,10;,
  4;10,10,11,11;,
  4;11,11,12,12;,
  4;12,12,13,13;,
  4;13,13,14,14;,
  4;14,14,15,15;,
  4;15,15,16,16;,
  4;16,16,1,1;,
  3;0,18,19;,
  3;0,19,20;,
  3;0,20,21;,
  3;0,21,22;,
  3;0,22,23;,
  3;0,23,24;,
  3;0,24,25;,
  3;0,25,26;,
  3;0,26,27;,
  3;0,27,28;,
  3;0,28,29;,
  3;0,29,22;,
  3;0,22,30;,
  3;0,30,31;,
  3;0,31,32;,
  3;0,32,18;,
  3;17,33,34;,
  3;17,35,33;,
  3;17,36,35;,
  3;17,37,36;,
  3;17,38,37;,
  3;17,39,38;,
  3;17,40,39;,
  3;17,41,40;,
  3;17,42,41;,
  3;17,43,42;,
  3;17,44,43;,
  3;17,45,44;,
  3;17,46,45;,
  3;17,47,46;,
  3;17,48,47;,
  3;17,34,48;;
 }
 MeshTextureCoords {
  66;
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.062500;1.000000;,
  0.062500;0.000000;,
  0.125000;1.000000;,
  0.125000;0.000000;,
  0.187500;1.000000;,
  0.187500;0.000000;,
  0.250000;1.000000;,
  0.250000;0.000000;,
  0.312500;1.000000;,
  0.312500;0.000000;,
  0.375000;1.000000;,
  0.375000;0.000000;,
  0.437500;1.000000;,
  0.437500;0.000000;,
  0.500000;1.000000;,
  0.500000;0.000000;,
  0.562500;1.000000;,
  0.562500;0.000000;,
  0.625000;1.000000;,
  0.625000;0.000000;,
  0.687500;1.000000;,
  0.687500;0.000000;,
  0.750000;1.000000;,
  0.750000;0.000000;,
  0.812500;1.000000;,
  0.812500;0.000000;,
  0.875000;1.000000;,
  0.875000;0.000000;,
  0.937500;1.000000;,
  0.937500;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.031250;0.000000;,
  0.093750;0.000000;,
  0.156250;0.000000;,
  0.218750;0.000000;,
  0.281250;0.000000;,
  0.343750;0.000000;,
  0.406250;0.000000;,
  0.468750;0.000000;,
  0.531250;0.000000;,
  0.593750;0.000000;,
  0.656250;0.000000;,
  0.718750;0.000000;,
  0.781250;0.000000;,
  0.843750;0.000000;,
  0.906250;0.000000;,
  0.968750;0.000000;,
  0.031250;1.000000;,
  0.093750;1.000000;,
  0.156250;1.000000;,
  0.218750;1.000000;,
  0.281250;1.000000;,
  0.343750;1.000000;,
  0.406250;1.000000;,
  0.468750;1.000000;,
  0.531250;1.000000;,
  0.593750;1.000000;,
  0.656250;1.000000;,
  0.718750;1.000000;,
  0.781250;1.000000;,
  0.843750;1.000000;,
  0.906250;1.000000;,
  0.968750;1.000000;;
 }
}