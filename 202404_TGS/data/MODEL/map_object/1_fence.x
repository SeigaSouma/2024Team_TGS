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
 210;
 -11.26758;52.71657;-3.70653;,
 -4.35542;52.71657;-3.70653;,
 -4.35542;-1.17353;-3.70653;,
 -11.26758;-1.17353;-3.70653;,
 -4.35542;52.71657;-3.70653;,
 -4.35542;52.71657;6.10239;,
 -4.35542;-1.17353;6.10239;,
 -4.35542;-1.17353;-3.70653;,
 -4.35542;52.71657;6.10239;,
 -11.26758;52.71657;6.10239;,
 -11.26758;-1.17353;6.10239;,
 -4.35542;-1.17353;6.10239;,
 -11.26758;52.71657;6.10239;,
 -11.26758;52.71657;-3.70653;,
 -11.26758;-1.17353;-3.70653;,
 -11.26758;-1.17353;6.10239;,
 -4.35542;52.71657;-3.70653;,
 -11.26758;52.71657;-3.70653;,
 -11.26758;-1.17353;-3.70653;,
 -4.35542;-1.17353;-3.70653;,
 26.80226;52.71657;-3.70653;,
 33.71442;52.71657;-3.70653;,
 33.71442;-1.17353;-3.70653;,
 26.80226;-1.17353;-3.70653;,
 33.71442;52.71657;-3.70653;,
 33.71442;52.71657;6.10239;,
 33.71442;-1.17353;6.10239;,
 33.71442;-1.17353;-3.70653;,
 33.71442;52.71657;6.10239;,
 26.80226;52.71657;6.10239;,
 26.80226;-1.17353;6.10239;,
 33.71442;-1.17353;6.10239;,
 26.80226;52.71657;6.10239;,
 26.80226;52.71657;-3.70653;,
 26.80226;-1.17353;-3.70653;,
 26.80226;-1.17353;6.10239;,
 33.71442;52.71657;-3.70653;,
 26.80226;52.71657;-3.70653;,
 26.80226;-1.17353;-3.70653;,
 33.71442;-1.17353;-3.70653;,
 -11.28559;68.73821;-3.70653;,
 -11.28559;68.73821;6.10239;,
 33.63430;68.73821;6.10239;,
 33.63430;68.73821;-3.70653;,
 -11.28559;52.94057;6.10239;,
 -11.28559;52.94057;-3.70653;,
 33.63430;52.94057;-3.70653;,
 33.63430;52.94057;6.10239;,
 -11.28559;59.37962;-3.70653;,
 33.63430;59.37962;-3.70653;,
 33.63430;52.94057;-3.70653;,
 -11.28559;52.94057;-3.70653;,
 -11.28559;59.37962;-3.70653;,
 -11.28559;52.94057;-3.70653;,
 -11.28559;59.37962;6.10239;,
 -11.28559;68.73821;-3.70653;,
 33.63430;59.37962;-3.70653;,
 33.63430;68.73821;-3.70653;,
 33.63430;59.37962;6.10239;,
 33.63430;52.94057;-3.70653;,
 -11.28559;52.94057;6.10239;,
 33.63430;52.94057;6.10239;,
 33.63430;68.73821;6.10239;,
 -11.28559;68.73821;6.10239;,
 -11.28559;62.69925;-8.89761;,
 -11.28559;59.37962;-8.89761;,
 -11.28559;68.73821;-3.70653;,
 33.63430;62.69925;-8.89761;,
 33.63430;68.73821;-3.70653;,
 33.63430;59.37962;-8.89761;,
 -9.75199;44.88912;-0.94509;,
 -9.75201;47.97161;-0.94509;,
 29.26337;47.97161;-0.94509;,
 29.26335;44.88912;-0.94509;,
 -9.75201;47.97161;-0.94509;,
 -9.75201;47.97161;3.34093;,
 29.26337;47.97161;3.34093;,
 29.26337;47.97161;-0.94509;,
 -9.75201;47.97161;3.34093;,
 -9.75199;42.34452;3.34093;,
 29.26335;42.34452;3.34093;,
 29.26337;47.97161;3.34093;,
 -9.75199;42.34452;3.34093;,
 -9.75199;44.88912;-0.94509;,
 29.26335;44.88912;-0.94509;,
 29.26335;42.34452;3.34093;,
 -9.75201;47.97161;-0.94509;,
 -9.75199;44.88912;-0.94509;,
 29.26335;44.88912;-0.94509;,
 29.26337;47.97161;-0.94509;,
 -9.75199;36.24695;-0.94509;,
 -9.75201;39.32940;-0.94509;,
 29.26337;39.32940;-0.94509;,
 29.26335;36.24695;-0.94509;,
 -9.75201;39.32940;-0.94509;,
 -9.75201;39.32940;3.34093;,
 29.26337;39.32940;3.34093;,
 29.26337;39.32940;-0.94509;,
 -9.75201;39.32940;3.34093;,
 -9.75199;33.70236;3.34093;,
 29.26335;33.70236;3.34093;,
 29.26337;39.32940;3.34093;,
 -9.75199;33.70236;3.34093;,
 -9.75199;36.24695;-0.94509;,
 29.26335;36.24695;-0.94509;,
 29.26335;33.70236;3.34093;,
 -9.75201;39.32940;-0.94509;,
 -9.75199;36.24695;-0.94509;,
 29.26335;36.24695;-0.94509;,
 29.26337;39.32940;-0.94509;,
 29.26335;1.82066;-0.94509;,
 29.26337;-1.26179;-0.94509;,
 -9.75201;-1.26179;-0.94509;,
 -9.75199;1.82066;-0.94509;,
 29.26337;-1.26179;-0.94509;,
 29.26337;-1.26179;3.34093;,
 -9.75201;-1.26179;3.34093;,
 -9.75201;-1.26179;-0.94509;,
 29.26337;-1.26179;3.34093;,
 29.26335;4.36526;3.34093;,
 -9.75199;4.36527;3.34093;,
 -9.75201;-1.26179;3.34093;,
 29.26335;4.36526;3.34093;,
 29.26335;1.82066;-0.94509;,
 -9.75199;1.82066;-0.94509;,
 -9.75199;4.36527;3.34093;,
 29.26337;-1.26179;-0.94509;,
 29.26335;1.82066;-0.94509;,
 -9.75199;1.82066;-0.94509;,
 -9.75201;-1.26179;-0.94509;,
 20.79571;35.10662;0.21366;,
 27.70786;35.10662;0.21366;,
 27.70786;-0.33872;0.21366;,
 20.79571;-0.33872;0.21366;,
 27.70786;35.10662;0.21366;,
 27.70786;35.10662;2.18220;,
 27.70786;-0.33872;2.18220;,
 27.70786;-0.33872;0.21366;,
 27.70786;35.10662;2.18220;,
 20.79571;35.10662;2.18220;,
 20.79571;-0.33872;2.18220;,
 27.70786;-0.33872;2.18220;,
 20.79571;35.10662;2.18220;,
 20.79571;35.10662;0.21366;,
 20.79571;-0.33872;0.21366;,
 20.79571;-0.33872;2.18220;,
 27.70786;35.10662;0.21366;,
 20.79571;35.10662;0.21366;,
 20.79571;-0.33872;0.21366;,
 27.70786;-0.33872;0.21366;,
 12.41584;35.10662;0.21366;,
 19.32798;35.10662;0.21366;,
 19.32798;-0.33872;0.21366;,
 12.41584;-0.33872;0.21366;,
 19.32798;35.10662;0.21366;,
 19.32798;35.10662;2.18220;,
 19.32798;-0.33872;2.18220;,
 19.32798;-0.33872;0.21366;,
 19.32798;35.10662;2.18220;,
 12.41584;35.10662;2.18220;,
 12.41584;-0.33872;2.18220;,
 19.32798;-0.33872;2.18220;,
 12.41584;35.10662;2.18220;,
 12.41584;35.10662;0.21366;,
 12.41584;-0.33872;0.21366;,
 12.41584;-0.33872;2.18220;,
 19.32798;35.10662;0.21366;,
 12.41584;35.10662;0.21366;,
 12.41584;-0.33872;0.21366;,
 19.32798;-0.33872;0.21366;,
 3.58351;35.28429;0.21366;,
 10.49567;35.28429;0.21366;,
 10.49567;-0.51642;0.21366;,
 3.58351;-0.51642;0.21366;,
 10.49567;35.28429;0.21366;,
 10.49567;35.28429;2.18220;,
 10.49567;-0.51642;2.18220;,
 10.49567;-0.51642;0.21366;,
 10.49567;35.28429;2.18220;,
 3.58351;35.28429;2.18220;,
 3.58351;-0.51642;2.18220;,
 10.49567;-0.51642;2.18220;,
 3.58351;35.28429;2.18220;,
 3.58351;35.28429;0.21366;,
 3.58351;-0.51642;0.21366;,
 3.58351;-0.51642;2.18220;,
 10.49567;35.28429;0.21366;,
 3.58351;35.28429;0.21366;,
 3.58351;-0.51642;0.21366;,
 10.49567;-0.51642;0.21366;,
 -4.74775;35.10662;0.21366;,
 2.16439;35.10662;0.21366;,
 2.16439;-0.33872;0.21366;,
 -4.74775;-0.33872;0.21366;,
 2.16439;35.10662;0.21366;,
 2.16439;35.10662;2.18220;,
 2.16439;-0.33872;2.18220;,
 2.16439;-0.33872;0.21366;,
 2.16439;35.10662;2.18220;,
 -4.74775;35.10662;2.18220;,
 -4.74775;-0.33872;2.18220;,
 2.16439;-0.33872;2.18220;,
 -4.74775;35.10662;2.18220;,
 -4.74775;35.10662;0.21366;,
 -4.74775;-0.33872;0.21366;,
 -4.74775;-0.33872;2.18220;,
 2.16439;35.10662;0.21366;,
 -4.74775;35.10662;0.21366;,
 -4.74775;-0.33872;0.21366;,
 2.16439;-0.33872;0.21366;;
 
 68;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;12,5,16,17;,
 4;18,19,6,15;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;32,25,36,37;,
 4;38,39,26,35;,
 4;40,41,42,43;,
 4;44,45,46,47;,
 4;48,49,50,51;,
 4;52,53,44,54;,
 4;52,54,41,55;,
 4;56,57,42,58;,
 4;56,58,47,59;,
 4;54,60,61,58;,
 4;54,58,62,63;,
 4;64,65,48,66;,
 4;67,64,66,68;,
 4;69,67,68,49;,
 4;65,69,49,48;,
 4;65,64,67,69;,
 4;70,71,72,73;,
 4;74,75,76,77;,
 4;78,79,80,81;,
 4;82,83,84,85;,
 4;82,75,86,87;,
 4;88,89,76,85;,
 4;90,91,92,93;,
 4;94,95,96,97;,
 4;98,99,100,101;,
 4;102,103,104,105;,
 4;102,95,106,107;,
 4;108,109,96,105;,
 4;110,111,112,113;,
 4;114,115,116,117;,
 4;118,119,120,121;,
 4;122,123,124,125;,
 4;122,115,126,127;,
 4;128,129,116,125;,
 4;130,131,132,133;,
 4;134,135,136,137;,
 4;138,139,140,141;,
 4;142,143,144,145;,
 4;142,135,146,147;,
 4;148,149,136,145;,
 4;150,151,152,153;,
 4;154,155,156,157;,
 4;158,159,160,161;,
 4;162,163,164,165;,
 4;162,155,166,167;,
 4;168,169,156,165;,
 4;170,171,172,173;,
 4;174,175,176,177;,
 4;178,179,180,181;,
 4;182,183,184,185;,
 4;182,175,186,187;,
 4;188,189,176,185;,
 4;190,191,192,193;,
 4;194,195,196,197;,
 4;198,199,200,201;,
 4;202,203,204,205;,
 4;202,195,206,207;,
 4;208,209,196,205;;
 
 MeshMaterialList {
  1;
  68;
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\map_object\\wood02.png";
   }
  }
 }
 MeshNormals {
  20;
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.908808;-0.417214;,
  0.000000;1.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.347610;-0.937639;,
  0.000000;-0.494720;-0.869053;,
  0.000000;-0.494720;-0.869052;,
  0.000000;0.494719;-0.869053;,
  0.000000;-0.859875;-0.510505;,
  -1.000000;-0.000006;-0.000001;,
  1.000000;-0.000005;-0.000002;,
  0.000000;-0.859875;-0.510504;,
  -1.000000;-0.000006;-0.000001;,
  1.000000;-0.000005;-0.000002;,
  0.000000;0.859874;-0.510506;,
  1.000000;0.000005;-0.000001;,
  -1.000000;0.000006;-0.000002;;
  68;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;6,6,6,6;,
  4;5,5,5,5;,
  4;3,3,3,3;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;6,6,6,6;,
  4;5,5,5,5;,
  4;3,3,3,3;,
  4;4,5,5,4;,
  4;3,3,3,3;,
  4;0,0,0,0;,
  4;6,6,6,6;,
  4;6,6,6,6;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;6,6,6,6;,
  4;7,7,4,4;,
  4;1,1,1,1;,
  4;3,3,3,3;,
  4;0,7,7,0;,
  4;8,0,0,8;,
  4;5,5,5,5;,
  4;2,2,2,2;,
  4;11,8,8,11;,
  4;12,12,12,12;,
  4;13,13,13,13;,
  4;9,0,0,9;,
  4;5,5,5,5;,
  4;2,2,2,2;,
  4;14,9,9,14;,
  4;15,15,15,15;,
  4;16,16,16,16;,
  4;10,0,0,10;,
  4;3,3,3,3;,
  4;2,2,2,2;,
  4;17,10,10,17;,
  4;18,18,18,18;,
  4;19,19,19,19;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;6,6,6,6;,
  4;5,5,5,5;,
  4;3,3,3,3;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;6,6,6,6;,
  4;5,5,5,5;,
  4;3,3,3,3;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;6,6,6,6;,
  4;5,5,5,5;,
  4;3,3,3,3;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;6,6,6,6;,
  4;5,5,5,5;,
  4;3,3,3,3;;
 }
 MeshTextureCoords {
  210;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  0.500000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}
