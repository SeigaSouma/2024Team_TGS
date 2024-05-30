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
 86;
 -5.23795;115.58965;1.08561;,
 2.98376;111.10549;1.08561;,
 2.98376;-0.62836;1.08561;,
 -5.23795;-0.62836;1.08561;,
 2.98376;111.10549;1.08561;,
 2.98376;111.10549;6.31066;,
 2.98376;-0.62836;6.31066;,
 2.98376;-0.62836;1.08561;,
 2.98376;111.10549;6.31066;,
 -5.23795;115.58965;6.31066;,
 -5.23795;-0.62836;6.31066;,
 2.98376;-0.62836;6.31066;,
 -5.23795;115.58965;6.31066;,
 -5.23795;115.58965;1.08561;,
 -5.23795;-0.62836;1.08561;,
 -5.23795;-0.62836;6.31066;,
 2.98376;111.10549;1.08561;,
 -5.23795;115.58965;1.08561;,
 -5.23795;-0.62836;1.08561;,
 2.98376;-0.62836;1.08561;,
 23.75467;100.39403;-5.07268;,
 23.75467;100.39403;2.52268;,
 23.75467;69.53793;2.52268;,
 23.75467;69.53793;-5.07268;,
 -26.00886;100.39403;2.52268;,
 -26.00886;100.39403;-5.07268;,
 -26.00886;69.53793;-5.07268;,
 -26.00886;69.53793;2.52268;,
 -1.46446;116.63548;2.52268;,
 -26.00886;100.39403;2.52268;,
 -26.00886;69.53793;2.52268;,
 -1.12709;69.53793;2.52268;,
 23.75467;69.53793;2.52268;,
 23.75467;100.39403;2.52268;,
 23.75467;100.39403;-5.07268;,
 -1.46446;116.63548;-5.07268;,
 -26.00886;100.39403;-5.07268;,
 -26.00886;69.53793;-5.07268;,
 -1.12709;69.53793;-5.07268;,
 23.75467;69.53793;-5.07268;,
 -1.46446;116.63548;-5.07268;,
 23.75467;100.39403;-5.07268;,
 23.75467;69.53793;-5.07268;,
 -1.12709;69.53793;-5.07268;,
 -26.00886;69.53793;-5.07268;,
 -26.00886;100.39403;-5.07268;,
 -0.55118;119.37168;7.08662;,
 -0.55122;119.37168;-10.47726;,
 -42.09459;88.39607;-10.47726;,
 -42.09459;88.39607;7.08662;,
 -0.55122;119.37168;-10.47726;,
 1.61433;117.23733;-10.47726;,
 -39.92904;86.26172;-10.47726;,
 -42.09459;88.39607;-10.47726;,
 1.61433;117.23733;-10.47726;,
 1.61433;117.23733;7.08662;,
 -39.92904;86.26172;7.08662;,
 -39.92904;86.26172;-10.47726;,
 1.61433;117.23733;7.08662;,
 -0.55118;119.37168;7.08662;,
 -42.09459;88.39607;7.08662;,
 -39.92904;86.26172;7.08662;,
 -0.55122;119.37168;-10.47726;,
 -0.55118;119.37168;7.08662;,
 -42.09459;88.39607;7.08662;,
 -42.09459;88.39607;-10.47726;,
 42.90541;91.31714;7.89120;,
 42.90541;91.31714;-10.50880;,
 -0.47680;119.30235;-10.50880;,
 -0.47680;119.30235;7.89120;,
 42.90541;91.31714;-10.50880;,
 41.37412;88.70314;-10.50880;,
 -2.00810;116.68833;-10.50880;,
 -0.47680;119.30235;-10.50880;,
 41.37412;88.70314;-10.50880;,
 41.37412;88.70314;7.89120;,
 -2.00807;116.68833;7.89120;,
 -2.00810;116.68833;-10.50880;,
 41.37412;88.70314;7.89120;,
 42.90541;91.31714;7.89120;,
 -0.47680;119.30235;7.89120;,
 -2.00807;116.68833;7.89120;,
 42.90541;91.31714;-10.50880;,
 42.90541;91.31714;7.89120;,
 -0.47680;119.30235;7.89120;,
 -0.47680;119.30235;-10.50880;;
 
 28;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;12,5,16,17;,
 4;18,19,6,15;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;28,29,30,31;,
 4;28,31,32,33;,
 4;28,21,34,35;,
 4;28,35,36,24;,
 4;31,27,37,38;,
 4;31,38,39,22;,
 4;40,41,42,43;,
 4;40,43,44,45;,
 4;46,47,48,49;,
 4;50,51,52,53;,
 4;54,55,56,57;,
 4;58,59,60,61;,
 4;58,51,62,63;,
 4;64,65,52,61;,
 4;66,67,68,69;,
 4;70,71,72,73;,
 4;74,75,76,77;,
 4;78,79,80,81;,
 4;78,71,82,83;,
 4;84,85,72,81;;
 
 MeshMaterialList {
  2;
  28;
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
   0.621176;0.621176;0.621176;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "whitewood.jpeg";
   }
  }
  Material {
   0.288627;0.288627;0.288627;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "whitewood.jpeg";
   }
  }
 }
 MeshNormals {
  18;
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -0.880863;0.473371;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.877908;0.478829;0.000000;,
  0.541445;0.840736;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.597751;0.801682;0.000001;,
  0.597751;-0.801682;-0.000000;,
  0.542081;0.840326;0.000000;,
  -0.542081;-0.840326;0.000001;,
  0.478818;0.877914;0.000000;,
  -0.551839;0.833950;0.000000;,
  0.701961;0.712215;-0.000001;,
  -0.701958;-0.712218;0.000000;,
  0.862849;-0.505462;-0.000000;,
  -0.862852;0.505456;0.000001;;
  28;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;4,4,4,4;,
  4;12,12,12,12;,
  4;7,7,7,7;,
  4;5,5,1,1;,
  4;3,3,4,4;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;6,5,5,6;,
  4;13,13,3,3;,
  4;7,7,7,7;,
  4;7,7,7,7;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;8,8,8,8;,
  4;0,0,0,0;,
  4;9,9,9,9;,
  4;2,2,2,2;,
  4;14,14,14,14;,
  4;15,15,15,15;,
  4;10,10,10,10;,
  4;0,0,0,0;,
  4;11,11,11,11;,
  4;2,2,2,2;,
  4;16,16,16,16;,
  4;17,17,17,17;;
 }
 MeshTextureCoords {
  86;
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
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
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
