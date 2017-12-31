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
 117;
 -0.00000;0.15083;4.01431;,
 0.00000;-2.08910;3.74357;,
 1.43260;-2.08910;3.45861;,
 1.53621;0.15083;3.70874;,
 2.64710;-2.08910;2.64710;,
 2.83855;0.15083;2.83855;,
 3.45861;-2.08910;1.43260;,
 3.70874;0.15083;1.53621;,
 3.74357;-2.08910;-0.00000;,
 4.01431;0.15083;0.00000;,
 3.45861;-2.08910;-1.43260;,
 3.70874;0.15083;-1.53621;,
 2.64710;-2.08910;-2.64710;,
 2.83855;0.15083;-2.83855;,
 1.43260;-2.08910;-3.45861;,
 1.53621;0.15083;-3.70874;,
 -0.00000;-2.08910;-3.74357;,
 0.00000;0.15083;-4.01431;,
 -1.43260;-2.08910;-3.45861;,
 -1.53621;0.15083;-3.70874;,
 -2.64710;-2.08910;-2.64710;,
 -2.83854;0.15083;-2.83855;,
 -3.45861;-2.08910;-1.43260;,
 -3.70874;0.15083;-1.53621;,
 -3.74357;-2.08910;0.00000;,
 -4.01431;0.15083;-0.00000;,
 -3.45861;-2.08910;1.43260;,
 -3.70874;0.15083;1.53621;,
 -2.64710;-2.08910;2.64710;,
 -2.83854;0.15083;2.83855;,
 -1.43260;-2.08910;3.45861;,
 -1.53621;0.15083;3.70874;,
 0.00000;-2.08910;3.74357;,
 -0.00000;0.15083;4.01431;,
 0.00000;-4.32903;3.50854;,
 1.34266;-4.32903;3.24147;,
 2.48092;-4.32903;2.48092;,
 3.24147;-4.32903;1.34266;,
 3.50854;-4.32903;-0.00000;,
 3.24147;-4.32903;-1.34266;,
 2.48092;-4.32903;-2.48092;,
 1.34266;-4.32903;-3.24147;,
 -0.00000;-4.32903;-3.50854;,
 -1.34266;-4.32903;-3.24147;,
 -2.48091;-4.32903;-2.48092;,
 -3.24147;-4.32903;-1.34266;,
 -3.50854;-4.32903;0.00000;,
 -3.24147;-4.32903;1.34266;,
 -2.48091;-4.32903;2.48092;,
 -1.34266;-4.32903;3.24147;,
 0.00000;-4.32903;3.50854;,
 0.00000;-6.56896;3.28828;,
 1.25837;-6.56896;3.03797;,
 2.32516;-6.56896;2.32516;,
 3.03797;-6.56896;1.25837;,
 3.28828;-6.56896;-0.00000;,
 3.03797;-6.56896;-1.25837;,
 2.32516;-6.56896;-2.32516;,
 1.25837;-6.56896;-3.03797;,
 -0.00000;-6.56896;-3.28828;,
 -1.25837;-6.56896;-3.03797;,
 -2.32516;-6.56896;-2.32516;,
 -3.03797;-6.56896;-1.25837;,
 -3.28828;-6.56896;0.00000;,
 -3.03797;-6.56896;1.25837;,
 -2.32516;-6.56896;2.32516;,
 -1.25837;-6.56896;3.03797;,
 0.00000;-6.56896;3.28828;,
 0.00000;-15.21614;2.70702;,
 1.03593;-15.21614;2.50096;,
 1.91416;-15.21614;1.91416;,
 2.50096;-15.21614;1.03593;,
 2.70702;-15.21614;-0.00000;,
 2.50096;-15.21614;-1.03593;,
 1.91416;-15.21614;-1.91416;,
 1.03593;-15.21614;-2.50096;,
 -0.00000;-15.21614;-2.70702;,
 -1.03593;-15.21614;-2.50096;,
 -1.91415;-15.21614;-1.91416;,
 -2.50096;-15.21614;-1.03593;,
 -2.70702;-15.21614;0.00000;,
 -2.50096;-15.21614;1.03593;,
 -1.91415;-15.21614;1.91416;,
 -1.03593;-15.21614;2.50096;,
 0.00000;-15.21614;2.70702;,
 0.00000;0.15083;0.00000;,
 0.00000;0.15083;0.00000;,
 0.00000;0.15083;0.00000;,
 0.00000;0.15083;0.00000;,
 0.00000;0.15083;0.00000;,
 0.00000;0.15083;0.00000;,
 0.00000;0.15083;0.00000;,
 0.00000;0.15083;0.00000;,
 0.00000;0.15083;0.00000;,
 0.00000;0.15083;0.00000;,
 0.00000;0.15083;0.00000;,
 0.00000;0.15083;0.00000;,
 0.00000;0.15083;0.00000;,
 0.00000;0.15083;0.00000;,
 0.00000;0.15083;0.00000;,
 0.00000;0.15083;0.00000;,
 0.00000;-15.21614;-0.00000;,
 0.00000;-15.21614;-0.00000;,
 0.00000;-15.21614;-0.00000;,
 0.00000;-15.21614;-0.00000;,
 0.00000;-15.21614;-0.00000;,
 0.00000;-15.21614;-0.00000;,
 0.00000;-15.21614;-0.00000;,
 0.00000;-15.21614;-0.00000;,
 0.00000;-15.21614;-0.00000;,
 0.00000;-15.21614;-0.00000;,
 0.00000;-15.21614;-0.00000;,
 0.00000;-15.21614;-0.00000;,
 0.00000;-15.21614;-0.00000;,
 0.00000;-15.21614;-0.00000;,
 0.00000;-15.21614;-0.00000;,
 0.00000;-15.21614;-0.00000;;
 
 96;
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
 4;1,34,35,2;,
 4;2,35,36,4;,
 4;4,36,37,6;,
 4;6,37,38,8;,
 4;8,38,39,10;,
 4;10,39,40,12;,
 4;12,40,41,14;,
 4;14,41,42,16;,
 4;16,42,43,18;,
 4;18,43,44,20;,
 4;20,44,45,22;,
 4;22,45,46,24;,
 4;24,46,47,26;,
 4;26,47,48,28;,
 4;28,48,49,30;,
 4;30,49,50,32;,
 4;34,51,52,35;,
 4;35,52,53,36;,
 4;36,53,54,37;,
 4;37,54,55,38;,
 4;38,55,56,39;,
 4;39,56,57,40;,
 4;40,57,58,41;,
 4;41,58,59,42;,
 4;42,59,60,43;,
 4;43,60,61,44;,
 4;44,61,62,45;,
 4;45,62,63,46;,
 4;46,63,64,47;,
 4;47,64,65,48;,
 4;48,65,66,49;,
 4;49,66,67,50;,
 4;51,68,69,52;,
 4;52,69,70,53;,
 4;53,70,71,54;,
 4;54,71,72,55;,
 4;55,72,73,56;,
 4;56,73,74,57;,
 4;57,74,75,58;,
 4;58,75,76,59;,
 4;59,76,77,60;,
 4;60,77,78,61;,
 4;61,78,79,62;,
 4;62,79,80,63;,
 4;63,80,81,64;,
 4;64,81,82,65;,
 4;65,82,83,66;,
 4;66,83,84,67;,
 3;85,0,3;,
 3;86,3,5;,
 3;87,5,7;,
 3;88,7,9;,
 3;89,9,11;,
 3;90,11,13;,
 3;91,13,15;,
 3;92,15,17;,
 3;93,17,19;,
 3;94,19,21;,
 3;95,21,23;,
 3;96,23,25;,
 3;97,25,27;,
 3;98,27,29;,
 3;99,29,31;,
 3;100,31,33;,
 3;101,69,68;,
 3;102,70,69;,
 3;103,71,70;,
 3;104,72,71;,
 3;105,73,72;,
 3;106,74,73;,
 3;107,75,74;,
 3;108,76,75;,
 3;109,77,76;,
 3;110,78,77;,
 3;111,79,78;,
 3;112,80,79;,
 3;113,81,80;,
 3;114,82,81;,
 3;115,83,82;,
 3;116,84,83;;
 
 MeshMaterialList {
  4;
  96;
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
  82;
  0.000000;1.000000;0.000000;,
  0.000000;-0.119996;0.992774;,
  0.379918;-0.119998;0.917204;,
  0.701997;-0.119999;0.701997;,
  0.917204;-0.119998;0.379918;,
  0.992774;-0.119996;-0.000000;,
  0.917204;-0.119998;-0.379918;,
  0.701997;-0.119999;-0.701997;,
  0.379918;-0.119998;-0.917204;,
  -0.000000;-0.119996;-0.992774;,
  -0.379919;-0.119997;-0.917204;,
  -0.701997;-0.119997;-0.701998;,
  -0.917204;-0.119997;-0.379919;,
  -0.992774;-0.119996;0.000000;,
  -0.917203;-0.119997;0.379919;,
  -0.701997;-0.119997;0.701998;,
  -0.379919;-0.119997;0.917204;,
  0.000000;-0.112179;0.993688;,
  0.380267;-0.112179;0.918048;,
  0.702644;-0.112179;0.702644;,
  0.918048;-0.112179;0.380267;,
  0.993688;-0.112179;-0.000000;,
  0.918048;-0.112179;-0.380267;,
  0.702644;-0.112179;-0.702644;,
  0.380267;-0.112179;-0.918048;,
  -0.000000;-0.112179;-0.993688;,
  -0.380268;-0.112179;-0.918048;,
  -0.702643;-0.112179;-0.702644;,
  -0.918048;-0.112179;-0.380268;,
  -0.993688;-0.112179;0.000000;,
  -0.918048;-0.112179;0.380268;,
  -0.702643;-0.112179;0.702644;,
  -0.380268;-0.112179;0.918048;,
  0.000000;-0.101109;0.994875;,
  0.380721;-0.101109;0.919145;,
  0.703483;-0.101109;0.703483;,
  0.919145;-0.101109;0.380721;,
  0.994875;-0.101109;-0.000000;,
  0.919145;-0.101109;-0.380721;,
  0.703483;-0.101109;-0.703483;,
  0.380721;-0.101109;-0.919145;,
  -0.000000;-0.101109;-0.994875;,
  -0.380722;-0.101109;-0.919145;,
  -0.703483;-0.101109;-0.703483;,
  -0.919145;-0.101109;-0.380723;,
  -0.994875;-0.101109;0.000000;,
  -0.919145;-0.101109;0.380723;,
  -0.703483;-0.101109;0.703484;,
  -0.380722;-0.101109;0.919145;,
  0.000000;-0.082476;0.996593;,
  0.381379;-0.082477;0.920732;,
  0.704698;-0.082478;0.704697;,
  0.920732;-0.082477;0.381379;,
  0.996593;-0.082476;-0.000000;,
  0.920732;-0.082477;-0.381379;,
  0.704697;-0.082478;-0.704698;,
  0.381379;-0.082477;-0.920732;,
  -0.000000;-0.082476;-0.996593;,
  -0.381380;-0.082476;-0.920732;,
  -0.704697;-0.082477;-0.704698;,
  -0.920732;-0.082476;-0.381380;,
  -0.996593;-0.082476;0.000000;,
  -0.920732;-0.082476;0.381380;,
  -0.704697;-0.082477;0.704698;,
  -0.381380;-0.082476;0.920732;,
  0.000000;-0.067068;0.997748;,
  0.381821;-0.067067;0.921800;,
  0.705515;-0.067067;0.705515;,
  0.921800;-0.067068;0.381820;,
  0.997748;-0.067068;-0.000000;,
  0.921800;-0.067067;-0.381821;,
  0.705515;-0.067067;-0.705515;,
  0.381820;-0.067068;-0.921800;,
  -0.000000;-0.067068;-0.997748;,
  -0.381822;-0.067068;-0.921799;,
  -0.705514;-0.067067;-0.705515;,
  -0.921799;-0.067068;-0.381822;,
  -0.997748;-0.067068;0.000000;,
  -0.921799;-0.067068;0.381822;,
  -0.705514;-0.067068;0.705515;,
  -0.381822;-0.067068;0.921799;,
  0.000000;-1.000000;-0.000000;;
  96;
  4;1,17,18,2;,
  4;2,18,19,3;,
  4;3,19,20,4;,
  4;4,20,21,5;,
  4;5,21,22,6;,
  4;6,22,23,7;,
  4;7,23,24,8;,
  4;8,24,25,9;,
  4;9,25,26,10;,
  4;10,26,27,11;,
  4;11,27,28,12;,
  4;12,28,29,13;,
  4;13,29,30,14;,
  4;14,30,31,15;,
  4;15,31,32,16;,
  4;16,32,17,1;,
  4;17,33,34,18;,
  4;18,34,35,19;,
  4;19,35,36,20;,
  4;20,36,37,21;,
  4;21,37,38,22;,
  4;22,38,39,23;,
  4;23,39,40,24;,
  4;24,40,41,25;,
  4;25,41,42,26;,
  4;26,42,43,27;,
  4;27,43,44,28;,
  4;28,44,45,29;,
  4;29,45,46,30;,
  4;30,46,47,31;,
  4;31,47,48,32;,
  4;32,48,33,17;,
  4;33,49,50,34;,
  4;34,50,51,35;,
  4;35,51,52,36;,
  4;36,52,53,37;,
  4;37,53,54,38;,
  4;38,54,55,39;,
  4;39,55,56,40;,
  4;40,56,57,41;,
  4;41,57,58,42;,
  4;42,58,59,43;,
  4;43,59,60,44;,
  4;44,60,61,45;,
  4;45,61,62,46;,
  4;46,62,63,47;,
  4;47,63,64,48;,
  4;48,64,49,33;,
  4;49,65,66,50;,
  4;50,66,67,51;,
  4;51,67,68,52;,
  4;52,68,69,53;,
  4;53,69,70,54;,
  4;54,70,71,55;,
  4;55,71,72,56;,
  4;56,72,73,57;,
  4;57,73,74,58;,
  4;58,74,75,59;,
  4;59,75,76,60;,
  4;60,76,77,61;,
  4;61,77,78,62;,
  4;62,78,79,63;,
  4;63,79,80,64;,
  4;64,80,65,49;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;81,81,81;,
  3;81,81,81;,
  3;81,81,81;,
  3;81,81,81;,
  3;81,81,81;,
  3;81,81,81;,
  3;81,81,81;,
  3;81,81,81;,
  3;81,81,81;,
  3;81,81,81;,
  3;81,81,81;,
  3;81,81,81;,
  3;81,81,81;,
  3;81,81,81;,
  3;81,81,81;,
  3;81,81,81;;
 }
 MeshTextureCoords {
  117;
  0.000000;0.000000;,
  0.000000;0.250000;,
  0.062500;0.250000;,
  0.062500;0.000000;,
  0.125000;0.250000;,
  0.125000;0.000000;,
  0.187500;0.250000;,
  0.187500;0.000000;,
  0.250000;0.250000;,
  0.250000;0.000000;,
  0.312500;0.250000;,
  0.312500;0.000000;,
  0.375000;0.250000;,
  0.375000;0.000000;,
  0.437500;0.250000;,
  0.437500;0.000000;,
  0.500000;0.250000;,
  0.500000;0.000000;,
  0.562500;0.250000;,
  0.562500;0.000000;,
  0.625000;0.250000;,
  0.625000;0.000000;,
  0.687500;0.250000;,
  0.687500;0.000000;,
  0.750000;0.250000;,
  0.750000;0.000000;,
  0.812500;0.250000;,
  0.812500;0.000000;,
  0.875000;0.250000;,
  0.875000;0.000000;,
  0.937500;0.250000;,
  0.937500;0.000000;,
  1.000000;0.250000;,
  1.000000;0.000000;,
  0.000000;0.500000;,
  0.062500;0.500000;,
  0.125000;0.500000;,
  0.187500;0.500000;,
  0.250000;0.500000;,
  0.312500;0.500000;,
  0.375000;0.500000;,
  0.437500;0.500000;,
  0.500000;0.500000;,
  0.562500;0.500000;,
  0.625000;0.500000;,
  0.687500;0.500000;,
  0.750000;0.500000;,
  0.812500;0.500000;,
  0.875000;0.500000;,
  0.937500;0.500000;,
  1.000000;0.500000;,
  0.000000;0.750000;,
  0.062500;0.750000;,
  0.125000;0.750000;,
  0.187500;0.750000;,
  0.250000;0.750000;,
  0.312500;0.750000;,
  0.375000;0.750000;,
  0.437500;0.750000;,
  0.500000;0.750000;,
  0.562500;0.750000;,
  0.625000;0.750000;,
  0.687500;0.750000;,
  0.750000;0.750000;,
  0.812500;0.750000;,
  0.875000;0.750000;,
  0.937500;0.750000;,
  1.000000;0.750000;,
  0.000000;1.000000;,
  0.062500;1.000000;,
  0.125000;1.000000;,
  0.187500;1.000000;,
  0.250000;1.000000;,
  0.312500;1.000000;,
  0.375000;1.000000;,
  0.437500;1.000000;,
  0.500000;1.000000;,
  0.562500;1.000000;,
  0.625000;1.000000;,
  0.687500;1.000000;,
  0.750000;1.000000;,
  0.812500;1.000000;,
  0.875000;1.000000;,
  0.937500;1.000000;,
  1.000000;1.000000;,
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