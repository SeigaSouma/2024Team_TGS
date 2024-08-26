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
 137;
 0.34189;294.11282;-7.15932;,
 5.40432;294.11282;-5.06242;,
 5.40432;-294.21360;-5.06242;,
 0.34189;-294.21360;-7.15932;,
 7.50120;294.11282;0.00000;,
 7.50120;-294.21360;-0.00000;,
 5.40432;294.11282;5.06242;,
 5.40432;-294.21360;5.06242;,
 0.34189;294.11282;7.15932;,
 0.34189;-294.21360;7.15932;,
 -4.72054;294.11282;5.06242;,
 -4.72054;-294.21360;5.06242;,
 -6.81742;294.11282;0.00000;,
 -6.81742;-294.21360;0.00000;,
 -4.72054;294.11282;-5.06242;,
 -4.72054;-294.21360;-5.06242;,
 0.34189;294.11282;-7.15932;,
 0.34189;-294.21360;-7.15932;,
 0.34189;294.11282;0.00000;,
 0.34189;294.11282;0.00000;,
 0.34189;294.11282;0.00000;,
 0.34189;294.11282;0.00000;,
 0.34189;294.11282;0.00000;,
 0.34189;294.11282;0.00000;,
 0.34189;294.11282;0.00000;,
 0.34189;294.11282;0.00000;,
 0.34189;-294.21360;-0.00000;,
 0.34189;-294.21360;-0.00000;,
 0.34189;-294.21360;-0.00000;,
 0.34189;-294.21360;-0.00000;,
 0.34189;-294.21360;-0.00000;,
 0.34189;-294.21360;-0.00000;,
 0.34189;-294.21360;-0.00000;,
 0.34189;-294.21360;-0.00000;,
 2.70307;292.32935;-2.87400;,
 121.05762;238.75479;-2.87400;,
 121.05762;237.52493;-2.87400;,
 2.70307;183.94986;-2.87400;,
 121.05762;238.75479;2.87400;,
 121.05762;237.52493;2.87400;,
 2.70307;292.32935;2.87400;,
 2.70307;183.94986;2.87400;,
 2.70307;292.32935;2.87400;,
 2.70307;292.32935;-2.87400;,
 2.70307;183.94986;-2.87400;,
 2.70307;183.94986;2.87400;,
 -2.73413;-185.01371;3.74830;,
 -121.08427;-238.58865;4.78108;,
 -121.08427;-239.81861;4.78108;,
 -2.73413;-293.39339;3.74830;,
 -121.13428;-238.58865;-0.96663;,
 -121.13428;-239.81861;-0.96663;,
 -2.78431;-185.01371;-1.99941;,
 -2.78431;-293.39339;-1.99941;,
 -2.78431;-185.01371;-1.99941;,
 -2.73413;-185.01371;3.74830;,
 -2.73413;-293.39339;3.74830;,
 -2.78431;-293.39339;-1.99941;,
 0.34189;64.38955;0.00000;,
 20.13817;59.52233;-19.79627;,
 0.34189;59.52233;-27.99610;,
 0.34189;64.38955;0.00000;,
 28.33802;59.52233;0.00000;,
 0.34189;64.38955;0.00000;,
 20.13817;59.52233;19.79627;,
 0.34189;64.38955;0.00000;,
 0.34189;59.52233;27.99610;,
 0.34189;64.38955;0.00000;,
 -19.45439;59.52233;19.79627;,
 0.34189;64.38955;0.00000;,
 -27.65422;59.52233;0.00000;,
 0.34189;64.38955;0.00000;,
 -19.45439;59.52233;-19.79627;,
 0.34189;64.38955;0.00000;,
 0.34189;59.52233;-27.99610;,
 36.92062;45.66191;-36.57876;,
 0.34189;45.66191;-51.73012;,
 52.07194;45.66191;0.00000;,
 36.92062;45.66191;36.57876;,
 0.34189;45.66191;51.73012;,
 -36.23686;45.66191;36.57876;,
 -51.38822;45.66191;0.00000;,
 -36.23686;45.66191;-36.57876;,
 0.34189;45.66191;-51.73012;,
 48.13428;24.91816;-47.79246;,
 0.34189;24.91816;-67.58873;,
 67.93059;24.91816;0.00000;,
 48.13428;24.91816;47.79246;,
 0.34189;24.91816;67.58873;,
 -47.45052;24.91816;47.79246;,
 -67.24676;24.91816;0.00000;,
 -47.45052;24.91816;-47.79246;,
 0.34189;24.91816;-67.58873;,
 52.07194;0.44930;-51.73012;,
 0.34189;0.44930;-73.15752;,
 73.49941;0.44930;0.00000;,
 52.07194;0.44930;51.73012;,
 0.34189;0.44930;73.15752;,
 -51.38822;0.44930;51.73012;,
 -72.81557;0.44930;0.00000;,
 -51.38822;0.44930;-51.73012;,
 0.34189;0.44930;-73.15752;,
 48.13428;-24.01955;-47.79246;,
 0.34189;-24.01955;-67.58873;,
 67.93059;-24.01955;-0.00000;,
 48.13428;-24.01955;47.79246;,
 0.34189;-24.01955;67.58873;,
 -47.45052;-24.01955;47.79246;,
 -67.24676;-24.01955;0.00000;,
 -47.45052;-24.01955;-47.79246;,
 0.34189;-24.01955;-67.58873;,
 36.92062;-44.76333;-36.57876;,
 0.34189;-44.76333;-51.73012;,
 52.07194;-44.76333;-0.00000;,
 36.92062;-44.76333;36.57876;,
 0.34189;-44.76333;51.73012;,
 -36.23686;-44.76333;36.57876;,
 -51.38822;-44.76333;0.00000;,
 -36.23686;-44.76333;-36.57876;,
 0.34189;-44.76333;-51.73012;,
 20.13817;-58.62380;-19.79627;,
 0.34189;-58.62380;-27.99610;,
 28.33802;-58.62380;-0.00000;,
 20.13817;-58.62380;19.79627;,
 0.34189;-58.62380;27.99610;,
 -19.45439;-58.62380;19.79627;,
 -27.65422;-58.62380;0.00000;,
 -19.45439;-58.62380;-19.79627;,
 0.34189;-58.62380;-27.99610;,
 0.34189;-63.49094;-0.00000;,
 0.34189;-63.49094;-0.00000;,
 0.34189;-63.49094;-0.00000;,
 0.34189;-63.49094;-0.00000;,
 0.34189;-63.49094;-0.00000;,
 0.34189;-63.49094;-0.00000;,
 0.34189;-63.49094;-0.00000;,
 0.34189;-63.49094;-0.00000;;
 
 100;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 3;18,1,0;,
 3;19,4,1;,
 3;20,6,4;,
 3;21,8,6;,
 3;22,10,8;,
 3;23,12,10;,
 3;24,14,12;,
 3;25,16,14;,
 3;26,3,2;,
 3;27,2,5;,
 3;28,5,7;,
 3;29,7,9;,
 3;30,9,11;,
 3;31,11,13;,
 3;32,13,15;,
 3;33,15,17;,
 4;34,35,36,37;,
 4;35,38,39,36;,
 4;38,40,41,39;,
 4;42,43,44,45;,
 4;40,38,35,34;,
 4;37,36,39,41;,
 4;46,47,48,49;,
 4;47,50,51,48;,
 4;50,52,53,51;,
 4;54,55,56,57;,
 4;52,50,47,46;,
 4;49,48,51,53;,
 3;58,59,60;,
 3;61,62,59;,
 3;63,64,62;,
 3;65,66,64;,
 3;67,68,66;,
 3;69,70,68;,
 3;71,72,70;,
 3;73,74,72;,
 4;60,59,75,76;,
 4;59,62,77,75;,
 4;62,64,78,77;,
 4;64,66,79,78;,
 4;66,68,80,79;,
 4;68,70,81,80;,
 4;70,72,82,81;,
 4;72,74,83,82;,
 4;76,75,84,85;,
 4;75,77,86,84;,
 4;77,78,87,86;,
 4;78,79,88,87;,
 4;79,80,89,88;,
 4;80,81,90,89;,
 4;81,82,91,90;,
 4;82,83,92,91;,
 4;85,84,93,94;,
 4;84,86,95,93;,
 4;86,87,96,95;,
 4;87,88,97,96;,
 4;88,89,98,97;,
 4;89,90,99,98;,
 4;90,91,100,99;,
 4;91,92,101,100;,
 4;94,93,102,103;,
 4;93,95,104,102;,
 4;95,96,105,104;,
 4;96,97,106,105;,
 4;97,98,107,106;,
 4;98,99,108,107;,
 4;99,100,109,108;,
 4;100,101,110,109;,
 4;103,102,111,112;,
 4;102,104,113,111;,
 4;104,105,114,113;,
 4;105,106,115,114;,
 4;106,107,116,115;,
 4;107,108,117,116;,
 4;108,109,118,117;,
 4;109,110,119,118;,
 4;112,111,120,121;,
 4;111,113,122,120;,
 4;113,114,123,122;,
 4;114,115,124,123;,
 4;115,116,125,124;,
 4;116,117,126,125;,
 4;117,118,127,126;,
 4;118,119,128,127;,
 3;121,120,129;,
 3;120,122,130;,
 3;122,123,131;,
 3;123,124,132;,
 3;124,125,133;,
 3;125,126,134;,
 3;126,127,135;,
 3;127,128,136;;
 
 MeshMaterialList {
  4;
  100;
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
  3,
  3,
  3,
  0,
  3,
  3,
  1,
  1,
  1,
  0,
  1,
  1,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.775200;0.749600;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\checkpoint\\flag_red.png";
   }
  }
  Material {
   0.800000;0.480000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.410000;0.246000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\checkpoint\\flag_white.png";
   }
  }
 }
 MeshNormals {
  80;
  0.000000;1.000000;0.000000;,
  -0.000000;0.000000;-1.000000;,
  0.707107;0.000000;-0.707106;,
  1.000000;0.000000;0.000000;,
  0.707107;0.000000;0.707106;,
  -0.000000;0.000000;1.000000;,
  -0.707107;0.000000;0.707106;,
  -1.000000;0.000000;0.000000;,
  -0.707107;0.000000;-0.707106;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.412380;0.911012;0.000000;,
  0.412383;-0.911010;-0.000000;,
  0.008726;0.000000;0.999962;,
  -0.999962;0.000000;0.008701;,
  -0.008726;0.000000;-0.999962;,
  0.999962;0.000000;-0.008730;,
  -0.412367;0.911011;0.003594;,
  -0.412366;-0.911011;0.003594;,
  -0.000000;1.000000;0.000000;,
  0.000000;0.939834;-0.341631;,
  0.241569;0.939834;-0.241569;,
  0.341631;0.939834;0.000000;,
  0.241569;0.939834;0.241569;,
  0.000000;0.939834;0.341631;,
  -0.241569;0.939834;0.241569;,
  -0.341631;0.939834;0.000000;,
  -0.241569;0.939834;-0.241569;,
  0.000000;0.751569;-0.659654;,
  0.466446;0.751570;-0.466446;,
  0.659654;0.751569;0.000000;,
  0.466446;0.751570;0.466446;,
  0.000000;0.751569;0.659654;,
  -0.466446;0.751569;0.466446;,
  -0.659655;0.751569;0.000000;,
  -0.466446;0.751569;-0.466446;,
  0.000000;0.426731;-0.904379;,
  0.639492;0.426731;-0.639492;,
  0.904379;0.426731;0.000000;,
  0.639492;0.426731;0.639492;,
  0.000000;0.426731;0.904379;,
  -0.639493;0.426730;0.639492;,
  -0.904379;0.426730;0.000000;,
  -0.639493;0.426730;-0.639492;,
  0.000000;-0.000000;-1.000000;,
  0.707107;-0.000000;-0.707107;,
  1.000000;-0.000000;-0.000000;,
  0.707107;-0.000000;0.707107;,
  0.000000;-0.000000;1.000000;,
  -0.707107;-0.000000;0.707106;,
  -1.000000;-0.000000;0.000000;,
  -0.707107;-0.000000;-0.707106;,
  0.000000;-0.426730;-0.904379;,
  0.639493;-0.426730;-0.639492;,
  0.904379;-0.426731;-0.000000;,
  0.639492;-0.426730;0.639492;,
  0.000000;-0.426730;0.904379;,
  -0.639493;-0.426730;0.639492;,
  -0.904379;-0.426730;0.000000;,
  -0.639493;-0.426730;-0.639492;,
  0.000000;-0.751569;-0.659655;,
  0.466447;-0.751569;-0.466446;,
  0.659655;-0.751568;-0.000000;,
  0.466447;-0.751569;0.466446;,
  0.000000;-0.751569;0.659655;,
  -0.466447;-0.751569;0.466446;,
  -0.659656;-0.751568;0.000000;,
  -0.466447;-0.751569;-0.466446;,
  0.000000;-0.939834;-0.341630;,
  0.241569;-0.939835;-0.241569;,
  0.341631;-0.939834;-0.000000;,
  0.241569;-0.939835;0.241569;,
  0.000000;-0.939834;0.341630;,
  -0.241569;-0.939835;0.241569;,
  -0.341630;-0.939834;0.000000;,
  -0.241569;-0.939835;-0.241569;,
  -0.000000;-1.000000;0.000000;;
  100;
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,1,1,8;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  4;10,10,10,10;,
  4;11,11,11,11;,
  4;12,12,12,12;,
  4;13,13,13,13;,
  4;14,14,14,14;,
  4;15,15,15,15;,
  4;16,16,16,16;,
  4;17,17,17,17;,
  4;18,18,18,18;,
  4;19,19,19,19;,
  4;20,20,20,20;,
  4;21,21,21,21;,
  3;22,24,23;,
  3;22,25,24;,
  3;22,26,25;,
  3;22,27,26;,
  3;22,28,27;,
  3;22,29,28;,
  3;22,30,29;,
  3;22,23,30;,
  4;23,24,32,31;,
  4;24,25,33,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,23,31,38;,
  4;31,32,40,39;,
  4;32,33,41,40;,
  4;33,34,42,41;,
  4;34,35,43,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,31,39,46;,
  4;39,40,48,47;,
  4;40,41,49,48;,
  4;41,42,50,49;,
  4;42,43,51,50;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,39,47,54;,
  4;47,48,56,55;,
  4;48,49,57,56;,
  4;49,50,58,57;,
  4;50,51,59,58;,
  4;51,52,60,59;,
  4;52,53,61,60;,
  4;53,54,62,61;,
  4;54,47,55,62;,
  4;55,56,64,63;,
  4;56,57,65,64;,
  4;57,58,66,65;,
  4;58,59,67,66;,
  4;59,60,68,67;,
  4;60,61,69,68;,
  4;61,62,70,69;,
  4;62,55,63,70;,
  4;63,64,72,71;,
  4;64,65,73,72;,
  4;65,66,74,73;,
  4;66,67,75,74;,
  4;67,68,76,75;,
  4;68,69,77,76;,
  4;69,70,78,77;,
  4;70,63,71,78;,
  3;71,72,79;,
  3;72,73,79;,
  3;73,74,79;,
  3;74,75,79;,
  3;75,76,79;,
  3;76,77,79;,
  3;77,78,79;,
  3;78,71,79;;
 }
 MeshTextureCoords {
  137;
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.151550;-2.011900;,
  1.259090;-1.510560;,
  1.259090;-1.499050;,
  0.151550;-0.997700;,
  1.259090;-1.510560;,
  1.259090;-1.499050;,
  0.151550;-2.011900;,
  0.151550;-0.997700;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.215030;0.039980;,
  1.252990;-0.429890;,
  1.252990;-0.440670;,
  0.215030;-0.910540;,
  1.253430;-0.429890;,
  1.253430;-0.440670;,
  0.215470;0.039980;,
  0.215470;-0.910540;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.062500;0.000000;,
  0.125000;0.125000;,
  0.000000;0.125000;,
  0.187500;0.000000;,
  0.250000;0.125000;,
  0.312500;0.000000;,
  0.375000;0.125000;,
  0.437500;0.000000;,
  0.500000;0.125000;,
  0.562500;0.000000;,
  0.625000;0.125000;,
  0.687500;0.000000;,
  0.750000;0.125000;,
  0.812500;0.000000;,
  0.875000;0.125000;,
  0.937500;0.000000;,
  1.000000;0.125000;,
  0.125000;0.250000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.125000;0.375000;,
  0.000000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.625000;,
  0.000000;0.625000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.125000;0.875000;,
  0.000000;0.875000;,
  0.250000;0.875000;,
  0.375000;0.875000;,
  0.500000;0.875000;,
  0.625000;0.875000;,
  0.750000;0.875000;,
  0.875000;0.875000;,
  1.000000;0.875000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;;
 }
}
