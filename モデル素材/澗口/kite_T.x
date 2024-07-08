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
 48;
 -2.16896;140.88512;0.00000;,
 -0.40704;140.88512;0.00000;,
 -0.40704;27.74800;-113.13712;,
 -2.16896;27.74800;-113.13712;,
 -0.40704;27.74800;113.13712;,
 -0.43792;-148.81441;0.00000;,
 -2.16896;27.74800;113.13712;,
 -2.19984;-148.81441;0.00000;,
 -0.18384;35.05384;-104.72664;,
 2.56264;35.05384;-104.72664;,
 2.56264;16.50232;-104.72664;,
 -0.18384;16.50232;-104.72664;,
 2.56264;35.05384;-104.72664;,
 2.56264;35.05384;104.72664;,
 2.56264;16.50232;104.72664;,
 2.56264;16.50232;-104.72664;,
 2.56264;35.05384;104.72664;,
 -0.18384;35.05384;104.72664;,
 -0.18384;16.50232;104.72664;,
 2.56264;16.50232;104.72664;,
 -0.18384;35.05384;104.72664;,
 -0.18384;35.05384;-104.72664;,
 -0.18384;16.50232;-104.72664;,
 -0.18384;16.50232;104.72664;,
 2.56264;35.05384;-104.72664;,
 -0.18384;35.05384;-104.72664;,
 -0.18384;16.50232;-104.72664;,
 2.56264;16.50232;-104.72664;,
 -0.18384;131.82672;9.27576;,
 2.56264;131.82672;9.27576;,
 2.56264;131.82672;-9.27576;,
 -0.18384;131.82672;-9.27576;,
 2.56264;131.82672;9.27576;,
 2.56264;-132.95729;9.27576;,
 2.56264;-132.95729;-9.27576;,
 2.56264;131.82672;-9.27576;,
 2.56264;-132.95729;9.27576;,
 -0.18384;-132.95729;9.27576;,
 -0.18384;-132.95729;-9.27576;,
 2.56264;-132.95729;-9.27576;,
 -0.18384;-132.95729;9.27576;,
 -0.18384;131.82672;9.27576;,
 -0.18384;131.82672;-9.27576;,
 -0.18384;-132.95729;-9.27576;,
 2.56264;131.82672;9.27576;,
 -0.18384;131.82672;9.27576;,
 -0.18384;131.82672;-9.27576;,
 2.56264;131.82672;-9.27576;;
 
 18;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,0,3,7;,
 4;6,4,1,0;,
 4;3,2,5,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;20,13,24,25;,
 4;26,27,14,23;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;36,37,38,39;,
 4;40,41,42,43;,
 4;40,33,44,45;,
 4;46,47,34,43;;
 
 MeshMaterialList {
  2;
  18;
  1,
  1,
  1,
  1,
  1,
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
  0;;
  Material {
   0.690400;0.429600;0.097600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\map_object\\kite00.jpg";
   }
  }
 }
 MeshNormals {
  12;
  0.000000;0.707107;-0.707107;,
  1.000000;-0.000087;-0.000000;,
  0.000000;-0.539518;0.841974;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;0.000000;,
  -1.000000;0.000107;0.000000;,
  0.000000;0.707107;0.707107;,
  0.000000;-0.539518;-0.841974;,
  -1.000000;0.000000;0.000000;;
  18;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;8,8,8,8;,
  4;9,9,9,9;,
  4;10,10,10,10;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;11,11,11,11;,
  4;6,6,6,6;,
  4;7,7,7,7;,
  4;6,6,6,6;,
  4;4,4,4,4;,
  4;7,7,7,7;,
  4;11,11,11,11;,
  4;5,5,5,5;,
  4;3,3,3,3;;
 }
 MeshTextureCoords {
  48;
  0.500066;0.003193;,
  0.500012;0.003193;,
  0.105663;0.389769;,
  0.105717;0.389769;,
  0.894361;0.389769;,
  0.500013;0.993062;,
  0.894415;0.389769;,
  0.500067;0.993062;,
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
