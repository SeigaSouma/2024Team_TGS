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
 18;
 -5.25596;-3.61550;-7.17464;,
 -4.84140;-15.13235;-7.14894;,
 -7.30937;-3.93984;-0.28744;,
 -7.06792;-15.13236;0.51099;,
 1.55344;-1.26597;-7.94479;,
 2.09382;-15.13235;-7.88059;,
 9.38433;1.43599;-0.28744;,
 9.85270;-15.13235;0.86490;,
 -5.53221;-1.67712;12.17574;,
 -5.35120;-15.13235;12.78430;,
 2.24518;-0.05396;9.68981;,
 2.45417;-15.13235;10.35719;,
 -8.07848;-36.48335;-0.89354;,
 3.89501;-36.48335;-12.64675;,
 10.54243;-36.48335;0.43726;,
 -5.96646;-36.48330;17.96894;,
 1.14594;-43.42147;2.64171;,
 -0.02505;2.68438;1.20114;;
 
 32;
 3;0,1,2;,
 3;2,1,3;,
 3;4,5,0;,
 3;0,5,1;,
 3;6,7,4;,
 3;4,7,5;,
 3;8,2,9;,
 3;9,2,3;,
 3;6,10,7;,
 3;7,10,11;,
 3;10,8,11;,
 3;11,8,9;,
 3;3,1,12;,
 3;12,5,13;,
 3;13,7,14;,
 3;15,3,12;,
 3;14,11,15;,
 3;1,5,12;,
 3;5,7,13;,
 3;7,11,14;,
 3;11,9,15;,
 3;9,3,15;,
 3;12,13,16;,
 3;13,14,16;,
 3;15,12,16;,
 3;14,15,16;,
 3;8,10,17;,
 3;6,4,17;,
 3;0,2,17;,
 3;2,8,17;,
 3;4,0,17;,
 3;10,6,17;;
 
 MeshMaterialList {
  1;
  32;
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
    "data\\TEXTURE\\ninja\\Player_Tex.png";
   }
  }
 }
 MeshNormals {
  34;
  -0.912414;0.405349;0.056512;,
  0.704013;-0.029450;-0.709577;,
  -0.737525;0.589718;-0.329075;,
  -0.962120;0.257670;-0.089061;,
  0.493330;-0.004500;-0.869831;,
  0.658506;0.051579;0.750806;,
  0.777526;0.060985;-0.625886;,
  -0.999132;0.023162;-0.034632;,
  -0.959457;-0.004008;-0.281827;,
  0.093592;0.010425;-0.995556;,
  -0.990073;0.014575;0.139800;,
  0.555837;0.055831;0.829414;,
  0.803580;-0.433105;-0.408264;,
  -0.946750;0.072598;-0.313678;,
  -0.643378;-0.395559;-0.655437;,
  -0.960440;-0.252373;0.117738;,
  -0.204352;-0.945647;-0.252966;,
  -0.281852;0.956327;-0.077443;,
  -0.107137;-0.007430;-0.994217;,
  -0.103926;-0.136961;-0.985110;,
  0.807079;0.063641;0.587004;,
  0.794894;0.038320;0.605537;,
  0.295974;0.046730;0.954052;,
  0.294291;0.133036;0.946411;,
  0.820938;0.123706;-0.557456;,
  0.731577;-0.223208;0.644184;,
  0.586204;-0.159523;0.794303;,
  -0.450721;-0.724411;-0.521612;,
  0.530325;-0.803841;-0.269435;,
  0.577347;-0.609178;0.543667;,
  -0.091470;0.940310;0.327795;,
  0.038370;0.953959;0.297473;,
  0.120928;0.990449;-0.066242;,
  -0.141547;0.899658;-0.413014;;
  32;
  3;2,8,3;,
  3;3,8,7;,
  3;4,9,18;,
  3;18,9,19;,
  3;1,6,4;,
  3;4,6,9;,
  3;0,3,10;,
  3;10,3,7;,
  3;20,5,21;,
  3;21,5,11;,
  3;5,22,11;,
  3;11,22,23;,
  3;7,8,13;,
  3;13,9,14;,
  3;24,6,12;,
  3;15,7,13;,
  3;25,11,26;,
  3;19,9,27;,
  3;9,6,24;,
  3;21,11,25;,
  3;11,23,26;,
  3;10,7,15;,
  3;27,14,16;,
  3;28,12,16;,
  3;15,27,16;,
  3;25,26,29;,
  3;30,31,17;,
  3;32,33,17;,
  3;2,3,17;,
  3;3,0,17;,
  3;33,2,17;,
  3;31,32,17;;
 }
 MeshTextureCoords {
  18;
  0.074070;0.930070;,
  0.072480;0.947890;,
  0.081940;0.930570;,
  0.081010;0.947890;,
  0.047970;0.926430;,
  0.045900;0.947890;,
  0.017960;0.922250;,
  0.016170;0.947890;,
  0.075130;0.927070;,
  0.074430;0.947890;,
  0.045320;0.924560;,
  0.044520;0.947890;,
  0.084880;0.980940;,
  0.039000;0.980940;,
  0.013530;0.980940;,
  0.076790;0.980940;,
  0.049530;0.991680;,
  0.054020;0.920320;;
 }
}
