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
 215;
 11.31370;57.41611;-11.31370;,
 0.00000;57.41611;-16.00000;,
 0.00000;-0.43993;-16.00000;,
 11.31370;-0.43993;-11.31370;,
 16.00000;57.41611;0.00000;,
 11.31370;57.41611;-11.31370;,
 11.31370;-0.43993;-11.31370;,
 16.00000;-0.43993;-0.00000;,
 11.31370;57.41611;11.31370;,
 11.31370;-0.43993;11.31370;,
 0.00000;57.41611;16.00000;,
 0.00000;-0.43993;16.00000;,
 -11.31372;57.41611;11.31370;,
 -11.31372;-0.43993;11.31370;,
 -16.00000;57.41611;0.00000;,
 -16.00000;-0.43993;0.00000;,
 -11.31372;57.41611;-11.31370;,
 -11.31372;-0.43993;-11.31370;,
 -11.31372;57.41611;-11.31370;,
 -11.31372;-0.43993;-11.31370;,
 0.00000;57.41611;-20.00000;,
 0.00000;57.41611;-16.00000;,
 11.31370;57.41611;-11.31370;,
 14.14214;57.41611;-14.14214;,
 0.00000;-0.43993;-20.00000;,
 14.14214;-0.43993;-14.14214;,
 11.31370;-0.43993;-11.31370;,
 0.00000;-0.43993;-16.00000;,
 16.00000;57.41611;0.00000;,
 20.00000;57.41611;0.00000;,
 20.00000;-0.43993;-0.00000;,
 16.00000;-0.43993;-0.00000;,
 11.31370;57.41611;11.31370;,
 14.14214;57.41611;14.14214;,
 14.14214;-0.43993;14.14214;,
 11.31370;-0.43993;11.31370;,
 0.00000;57.41611;16.00000;,
 0.00000;57.41611;20.00000;,
 0.00000;-0.43993;20.00000;,
 0.00000;-0.43993;16.00000;,
 -11.31372;57.41611;11.31370;,
 -14.14214;57.41611;14.14214;,
 -14.14214;-0.43993;14.14214;,
 -11.31372;-0.43993;11.31370;,
 -16.00000;57.41611;0.00000;,
 -20.00000;57.41611;0.00000;,
 -20.00000;-0.43993;0.00000;,
 -16.00000;-0.43993;0.00000;,
 -11.31372;57.41611;-11.31370;,
 -14.14214;57.41611;-14.14214;,
 -14.14214;-0.43993;-14.14214;,
 -11.31372;-0.43993;-11.31370;,
 0.00000;-0.64009;-0.00000;,
 0.00000;-0.64009;-0.00000;,
 0.00000;-0.64009;-0.00000;,
 0.00000;-0.64009;-0.00000;,
 0.00000;28.48807;-25.15758;,
 0.00000;57.41611;-20.00000;,
 14.14214;57.41611;-14.14214;,
 17.78910;28.48807;-17.78910;,
 14.14214;-0.43993;-14.14214;,
 0.00000;-0.43993;-20.00000;,
 -14.14214;-0.43993;-14.14214;,
 -17.78910;28.48807;-17.78910;,
 -14.14214;57.41611;-14.14214;,
 20.00000;57.41611;0.00000;,
 25.15758;28.48807;0.00000;,
 20.00000;-0.43993;-0.00000;,
 -20.00000;-0.43993;0.00000;,
 -25.15758;28.48807;0.00000;,
 14.14214;57.41611;14.14214;,
 17.78910;28.48807;17.78910;,
 14.14214;-0.43993;14.14214;,
 -14.14214;-0.43993;14.14214;,
 -17.78910;28.48807;17.78910;,
 -14.14214;57.41611;14.14214;,
 0.00000;57.41611;20.00000;,
 0.00000;28.48807;25.15758;,
 0.00000;-0.43993;20.00000;,
 0.11114;51.76659;-21.76974;,
 15.50466;51.76659;-15.39352;,
 15.50466;50.32293;-15.39352;,
 0.11114;50.32293;-21.76974;,
 21.88086;51.76659;0.00000;,
 21.88086;50.32293;0.00000;,
 15.50466;51.76659;15.39352;,
 15.50466;50.32293;15.39352;,
 0.11114;51.76659;21.76974;,
 0.11114;50.32293;21.76974;,
 -15.28238;51.76659;15.39352;,
 -15.28238;50.32293;15.39352;,
 -21.65860;51.76659;0.00000;,
 -21.65860;50.32293;0.00000;,
 -15.28238;51.76659;-15.39352;,
 -15.28238;50.32293;-15.39352;,
 0.11114;51.76659;-21.76974;,
 0.11114;50.32293;-21.76974;,
 0.11114;51.76659;0.00000;,
 0.11114;51.76659;0.00000;,
 0.11114;51.76659;0.00000;,
 0.11114;51.76659;0.00000;,
 0.11114;51.76659;0.00000;,
 0.11114;51.76659;0.00000;,
 0.11114;51.76659;0.00000;,
 0.11114;51.76659;0.00000;,
 0.11114;50.32293;0.00000;,
 0.11114;50.32293;0.00000;,
 0.11114;50.32293;0.00000;,
 0.11114;50.32293;0.00000;,
 0.11114;50.32293;0.00000;,
 0.11114;50.32293;0.00000;,
 0.11114;50.32293;0.00000;,
 0.11114;50.32293;0.00000;,
 0.11114;54.71719;-21.76974;,
 15.50466;54.71719;-15.39352;,
 15.50466;53.27353;-15.39352;,
 0.11114;53.27353;-21.76974;,
 21.88086;54.71719;0.00000;,
 21.88086;53.27353;0.00000;,
 15.50466;54.71719;15.39352;,
 15.50466;53.27353;15.39352;,
 0.11114;54.71719;21.76974;,
 0.11114;53.27353;21.76974;,
 -15.28238;54.71719;15.39352;,
 -15.28238;53.27353;15.39352;,
 -21.65860;54.71719;0.00000;,
 -21.65860;53.27353;0.00000;,
 -15.28238;54.71719;-15.39352;,
 -15.28238;53.27353;-15.39352;,
 0.11114;54.71719;-21.76974;,
 0.11114;53.27353;-21.76974;,
 0.11114;54.71719;0.00000;,
 0.11114;54.71719;0.00000;,
 0.11114;54.71719;0.00000;,
 0.11114;54.71719;0.00000;,
 0.11114;54.71719;0.00000;,
 0.11114;54.71719;0.00000;,
 0.11114;54.71719;0.00000;,
 0.11114;54.71719;0.00000;,
 0.11114;53.27353;0.00000;,
 0.11114;53.27353;0.00000;,
 0.11114;53.27353;0.00000;,
 0.11114;53.27353;0.00000;,
 0.11114;53.27353;0.00000;,
 0.11114;53.27353;0.00000;,
 0.11114;53.27353;0.00000;,
 0.11114;53.27353;0.00000;,
 0.11114;2.47509;-21.76974;,
 15.50466;2.47509;-15.39352;,
 15.50466;1.03143;-15.39352;,
 0.11114;1.03143;-21.76974;,
 21.88086;2.47509;0.00000;,
 21.88086;1.03143;0.00000;,
 15.50466;2.47509;15.39352;,
 15.50466;1.03143;15.39352;,
 0.11114;2.47509;21.76974;,
 0.11114;1.03143;21.76974;,
 -15.28238;2.47509;15.39352;,
 -15.28238;1.03143;15.39352;,
 -21.65860;2.47509;0.00000;,
 -21.65860;1.03143;0.00000;,
 -15.28238;2.47509;-15.39352;,
 -15.28238;1.03143;-15.39352;,
 0.11114;2.47509;-21.76974;,
 0.11114;1.03143;-21.76974;,
 0.11114;2.47509;0.00000;,
 0.11114;2.47509;0.00000;,
 0.11114;2.47509;0.00000;,
 0.11114;2.47509;0.00000;,
 0.11114;2.47509;0.00000;,
 0.11114;2.47509;0.00000;,
 0.11114;2.47509;0.00000;,
 0.11114;2.47509;0.00000;,
 0.11114;1.03143;0.00000;,
 0.11114;1.03143;0.00000;,
 0.11114;1.03143;0.00000;,
 0.11114;1.03143;0.00000;,
 0.11114;1.03143;0.00000;,
 0.11114;1.03143;0.00000;,
 0.11114;1.03143;0.00000;,
 0.11114;1.03143;0.00000;,
 0.11114;5.09451;-21.76974;,
 15.50466;5.09451;-15.39352;,
 15.50466;3.65085;-15.39352;,
 0.11114;3.65085;-21.76974;,
 21.88086;5.09451;0.00000;,
 21.88086;3.65085;0.00000;,
 15.50466;5.09451;15.39352;,
 15.50466;3.65085;15.39352;,
 0.11114;5.09451;21.76974;,
 0.11114;3.65085;21.76974;,
 -15.28238;5.09451;15.39352;,
 -15.28238;3.65085;15.39352;,
 -21.65860;5.09451;0.00000;,
 -21.65860;3.65085;0.00000;,
 -15.28238;5.09451;-15.39352;,
 -15.28238;3.65085;-15.39352;,
 0.11114;5.09451;-21.76974;,
 0.11114;3.65085;-21.76974;,
 0.11114;5.09451;0.00000;,
 0.11114;5.09451;0.00000;,
 0.11114;5.09451;0.00000;,
 0.11114;5.09451;0.00000;,
 0.11114;5.09451;0.00000;,
 0.11114;5.09451;0.00000;,
 0.11114;5.09451;0.00000;,
 0.11114;5.09451;0.00000;,
 0.11114;3.65085;0.00000;,
 0.11114;3.65085;0.00000;,
 0.11114;3.65085;0.00000;,
 0.11114;3.65085;0.00000;,
 0.11114;3.65085;0.00000;,
 0.11114;3.65085;0.00000;,
 0.11114;3.65085;0.00000;,
 0.11114;3.65085;0.00000;;
 
 144;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,4,7,9;,
 4;10,8,9,11;,
 4;12,10,11,13;,
 4;14,12,13,15;,
 4;16,14,15,17;,
 4;1,18,19,2;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;23,22,28,29;,
 4;25,30,31,26;,
 4;29,28,32,33;,
 4;30,34,35,31;,
 4;33,32,36,37;,
 4;34,38,39,35;,
 4;37,36,40,41;,
 4;38,42,43,39;,
 4;41,40,44,45;,
 4;42,46,47,43;,
 4;45,44,48,49;,
 4;46,50,51,47;,
 4;49,48,21,20;,
 4;50,24,27,51;,
 4;52,53,54,55;,
 4;55,54,53,52;,
 4;51,27,26,31;,
 4;7,3,2,17;,
 4;35,39,43,47;,
 4;15,13,11,9;,
 4;51,31,35,47;,
 4;15,9,7,17;,
 4;56,57,58,59;,
 4;56,59,60,61;,
 4;56,61,62,63;,
 4;56,63,64,57;,
 4;59,58,65,66;,
 4;59,66,67,60;,
 4;63,62,68,69;,
 4;63,69,45,64;,
 4;66,65,70,71;,
 4;66,71,72,67;,
 4;69,68,73,74;,
 4;69,74,75,45;,
 4;71,70,76,77;,
 4;71,77,78,72;,
 4;74,73,78,77;,
 4;74,77,76,75;,
 4;79,80,81,82;,
 4;80,83,84,81;,
 4;83,85,86,84;,
 4;85,87,88,86;,
 4;87,89,90,88;,
 4;89,91,92,90;,
 4;91,93,94,92;,
 4;93,95,96,94;,
 3;97,80,79;,
 3;98,83,80;,
 3;99,85,83;,
 3;100,87,85;,
 3;101,89,87;,
 3;102,91,89;,
 3;103,93,91;,
 3;104,95,93;,
 3;105,82,81;,
 3;106,81,84;,
 3;107,84,86;,
 3;108,86,88;,
 3;109,88,90;,
 3;110,90,92;,
 3;111,92,94;,
 3;112,94,96;,
 4;113,114,115,116;,
 4;114,117,118,115;,
 4;117,119,120,118;,
 4;119,121,122,120;,
 4;121,123,124,122;,
 4;123,125,126,124;,
 4;125,127,128,126;,
 4;127,129,130,128;,
 3;131,114,113;,
 3;132,117,114;,
 3;133,119,117;,
 3;134,121,119;,
 3;135,123,121;,
 3;136,125,123;,
 3;137,127,125;,
 3;138,129,127;,
 3;139,116,115;,
 3;140,115,118;,
 3;141,118,120;,
 3;142,120,122;,
 3;143,122,124;,
 3;144,124,126;,
 3;145,126,128;,
 3;146,128,130;,
 4;147,148,149,150;,
 4;148,151,152,149;,
 4;151,153,154,152;,
 4;153,155,156,154;,
 4;155,157,158,156;,
 4;157,159,160,158;,
 4;159,161,162,160;,
 4;161,163,164,162;,
 3;165,148,147;,
 3;166,151,148;,
 3;167,153,151;,
 3;168,155,153;,
 3;169,157,155;,
 3;170,159,157;,
 3;171,161,159;,
 3;172,163,161;,
 3;173,150,149;,
 3;174,149,152;,
 3;175,152,154;,
 3;176,154,156;,
 3;177,156,158;,
 3;178,158,160;,
 3;179,160,162;,
 3;180,162,164;,
 4;181,182,183,184;,
 4;182,185,186,183;,
 4;185,187,188,186;,
 4;187,189,190,188;,
 4;189,191,192,190;,
 4;191,193,194,192;,
 4;193,195,196,194;,
 4;195,197,198,196;,
 3;199,182,181;,
 3;200,185,182;,
 3;201,187,185;,
 3;202,189,187;,
 3;203,191,189;,
 3;204,193,191;,
 3;205,195,193;,
 3;206,197,195;,
 3;207,184,183;,
 3;208,183,186;,
 3;209,186,188;,
 3;210,188,190;,
 3;211,190,192;,
 3;212,192,194;,
 3;213,194,196;,
 3;214,196,198;;
 
 MeshMaterialList {
  2;
  144;
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
  1,
  1,
  0,
  1,
  0,
  1,
  0,
  1,
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\map_object\\wood_barrel.jpg";
   }
  }
  Material {
   0.596000;0.605600;0.392000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\map_object\\Matel_barrel.jpg";
   }
  }
 }
 MeshNormals {
  47;
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.000000;0.000000;1.000000;,
  -0.707107;0.000000;0.707107;,
  -1.000000;0.000000;0.000000;,
  -0.707107;0.000000;-0.707107;,
  -0.000000;0.000000;-1.000000;,
  0.707107;0.000000;-0.707107;,
  1.000000;0.000000;0.000000;,
  0.707107;0.000000;0.707107;,
  0.000000;0.000000;0.000000;,
  0.000000;-0.000000;-1.000000;,
  0.707107;-0.000000;-0.707107;,
  -0.707107;-0.000000;-0.707107;,
  1.000000;-0.000000;-0.000000;,
  -1.000000;-0.000000;0.000000;,
  0.707107;-0.000000;0.707107;,
  -0.707107;-0.000000;0.707107;,
  0.000000;-0.000000;1.000000;,
  0.000000;0.000000;-1.000000;,
  0.707107;0.000000;-0.707106;,
  0.707107;0.000000;0.707106;,
  0.000000;0.000000;1.000000;,
  -0.707107;0.000000;0.707107;,
  -0.707107;0.000000;-0.707107;,
  0.000000;0.000000;-1.000000;,
  0.707107;0.000000;-0.707106;,
  0.707107;0.000000;0.707106;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.175522;-0.984475;,
  0.696129;0.175522;-0.696129;,
  0.696129;-0.175522;-0.696129;,
  0.000000;-0.175522;-0.984475;,
  -0.696129;-0.175522;-0.696129;,
  -0.696129;0.175522;-0.696129;,
  0.984475;0.175522;0.000000;,
  0.984475;-0.175522;-0.000000;,
  -0.984475;-0.175522;0.000000;,
  -0.984475;0.175522;0.000000;,
  0.696129;0.175522;0.696129;,
  0.696129;-0.175522;0.696129;,
  -0.696129;-0.175522;0.696129;,
  -0.696129;0.175522;0.696129;,
  0.000000;0.175522;0.984475;,
  0.000000;-0.175522;0.984475;;
  144;
  4;3,2,2,3;,
  4;4,3,3,4;,
  4;5,4,4,5;,
  4;6,5,5,6;,
  4;7,6,6,7;,
  4;8,7,7,8;,
  4;9,8,8,9;,
  4;2,9,9,2;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;10,10,10,10;,
  4;10,10,10,10;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;11,31,32,12;,
  4;11,12,33,34;,
  4;11,34,35,13;,
  4;11,13,36,31;,
  4;12,32,37,14;,
  4;12,14,38,33;,
  4;13,35,39,15;,
  4;13,15,40,36;,
  4;14,37,41,16;,
  4;14,16,42,38;,
  4;15,39,43,17;,
  4;15,17,44,40;,
  4;16,41,45,18;,
  4;16,18,46,42;,
  4;17,43,46,18;,
  4;17,18,45,44;,
  4;19,20,20,19;,
  4;20,8,8,20;,
  4;8,21,21,8;,
  4;21,22,22,21;,
  4;22,23,23,22;,
  4;23,4,4,23;,
  4;4,24,24,4;,
  4;24,19,19,24;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  4;25,26,26,25;,
  4;26,8,8,26;,
  4;8,27,27,8;,
  4;27,28,28,27;,
  4;28,3,3,28;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,25,25,5;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  4;19,20,20,19;,
  4;20,8,8,20;,
  4;8,21,21,8;,
  4;21,22,22,21;,
  4;22,23,23,22;,
  4;23,4,4,23;,
  4;4,24,24,4;,
  4;24,19,19,24;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  4;29,26,26,29;,
  4;26,8,8,26;,
  4;8,27,27,8;,
  4;27,30,30,27;,
  4;30,3,3,30;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,29,29,5;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;;
 }
 MeshTextureCoords {
  215;
  0.780040;-0.355430;,
  0.500000;-0.355430;,
  0.500000;0.497050;,
  0.780040;0.497050;,
  0.250000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
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
  0.219960;-0.355430;,
  0.219960;0.497050;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.500000;0.500000;,
  0.500000;0.000000;,
  0.853550;0.000000;,
  0.853550;0.500000;,
  0.853550;1.000000;,
  0.500000;1.000000;,
  0.146450;1.000000;,
  0.146450;0.500000;,
  0.146450;0.000000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.500000;,
  0.853550;0.000000;,
  0.853550;0.500000;,
  0.853550;1.000000;,
  0.146450;1.000000;,
  0.146450;0.500000;,
  0.146450;0.000000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.500000;1.000000;,
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
  0.937500;1.000000;;
 }
}
