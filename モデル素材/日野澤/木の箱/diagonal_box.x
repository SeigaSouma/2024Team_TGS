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
 120;
 -15.08867;6.61714;-28.97679;,
 -15.08867;-0.08650;-25.10644;,
 -15.08867;1.35030;-22.61781;,
 -15.08867;8.05395;-26.48817;,
 -15.08867;-0.08650;-25.10644;,
 15.01692;-0.08650;-25.10643;,
 15.01692;1.35031;-22.61780;,
 -15.08867;1.35030;-22.61781;,
 15.01692;-0.08650;-25.10643;,
 15.01692;6.61715;-28.97679;,
 15.01692;8.05395;-26.48817;,
 15.01692;1.35031;-22.61780;,
 15.01692;6.61715;-28.97679;,
 -15.08867;6.61714;-28.97679;,
 -15.08867;8.05395;-26.48817;,
 15.01692;8.05395;-26.48817;,
 -15.08867;-0.08650;-25.10644;,
 -15.08867;6.61714;-28.97679;,
 -15.08867;8.05395;-26.48817;,
 -15.08867;1.35030;-22.61781;,
 -15.08867;38.03060;25.43291;,
 -15.08867;31.32694;29.30325;,
 -15.08867;32.76376;31.79189;,
 -15.08867;39.46743;27.92153;,
 -15.08867;31.32694;29.30325;,
 15.01692;31.32694;29.30325;,
 15.01692;32.76376;31.79189;,
 -15.08867;32.76376;31.79189;,
 15.01692;31.32694;29.30325;,
 15.01692;38.03060;25.43291;,
 15.01692;39.46743;27.92153;,
 15.01692;32.76376;31.79189;,
 15.01692;38.03060;25.43291;,
 -15.08867;38.03060;25.43291;,
 -15.08867;39.46743;27.92153;,
 15.01692;39.46743;27.92153;,
 -15.08867;31.32694;29.30325;,
 -15.08867;38.03060;25.43291;,
 -15.08867;39.46743;27.92153;,
 -15.08867;32.76376;31.79189;,
 -13.71782;6.55566;-29.08329;,
 -13.71782;-0.14799;-25.21293;,
 -15.03112;-0.14799;-25.21293;,
 -15.03112;6.55566;-29.08329;,
 -13.71782;-0.14799;-25.21293;,
 -13.71782;32.78937;31.83625;,
 -15.03112;32.78937;31.83625;,
 -15.03112;-0.14799;-25.21293;,
 -13.71782;32.78937;31.83625;,
 -13.71782;39.49304;27.96589;,
 -15.03112;39.49304;27.96589;,
 -15.03112;32.78937;31.83625;,
 -13.71782;39.49304;27.96589;,
 -13.71782;6.55566;-29.08329;,
 -15.03112;6.55566;-29.08329;,
 -15.03112;39.49304;27.96589;,
 -13.71782;-0.14799;-25.21293;,
 -13.71782;6.55566;-29.08329;,
 -15.03112;6.55566;-29.08329;,
 -15.03112;-0.14799;-25.21293;,
 15.05082;6.55566;-29.08329;,
 15.05082;-0.14799;-25.21293;,
 13.73752;-0.14799;-25.21293;,
 13.73752;6.55566;-29.08329;,
 15.05082;-0.14799;-25.21293;,
 15.05082;32.78937;31.83625;,
 13.73752;32.78937;31.83625;,
 13.73752;-0.14799;-25.21293;,
 15.05082;32.78937;31.83625;,
 15.05082;39.49304;27.96589;,
 13.73752;39.49304;27.96589;,
 13.73752;32.78937;31.83625;,
 15.05082;39.49304;27.96589;,
 15.05082;6.55566;-29.08329;,
 13.73752;6.55566;-29.08329;,
 13.73752;39.49304;27.96589;,
 15.05082;-0.14799;-25.21293;,
 15.05082;6.55566;-29.08329;,
 13.73752;6.55566;-29.08329;,
 13.73752;-0.14799;-25.21293;,
 -15.05582;1.20237;-25.99257;,
 15.04976;1.20237;-25.99257;,
 15.04976;-0.15189;-25.21068;,
 -15.05582;-0.15189;-25.21068;,
 15.04976;1.20237;-25.99257;,
 15.04976;34.13973;31.05662;,
 15.04976;32.78547;31.83850;,
 15.04976;-0.15189;-25.21068;,
 15.04976;34.13973;31.05662;,
 -15.05582;34.13973;31.05662;,
 -15.05582;32.78547;31.83850;,
 15.04976;32.78547;31.83850;,
 -15.05582;34.13973;31.05662;,
 -15.05582;1.20237;-25.99257;,
 -15.05582;-0.15189;-25.21068;,
 -15.05582;32.78547;31.83850;,
 15.04976;1.20237;-25.99257;,
 -15.05582;1.20237;-25.99257;,
 -15.05582;-0.15189;-25.21068;,
 15.04976;-0.15189;-25.21068;,
 -10.70573;21.66090;9.79202;,
 -15.08867;-1.05432;8.53527;,
 -15.08867;-1.05432;13.92238;,
 -10.70573;22.64102;12.66565;,
 -15.08867;-1.05432;8.53527;,
 15.01692;-1.05432;8.53527;,
 15.01692;-1.05432;13.92238;,
 -15.08867;-1.05432;13.92238;,
 15.01692;-1.05432;8.53527;,
 10.63398;21.66090;9.79202;,
 10.63398;22.64102;12.66565;,
 15.01692;-1.05432;13.92238;,
 10.63398;21.66090;9.79202;,
 -10.70573;21.66090;9.79202;,
 -10.70573;22.64102;12.66565;,
 10.63398;22.64102;12.66565;,
 -15.08867;-1.05432;8.53527;,
 -10.70573;21.66090;9.79202;,
 -10.70573;22.64102;12.66565;,
 -15.08867;-1.05432;13.92238;;
 
 36;
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
 4;52,53,54,55;,
 4;52,45,56,57;,
 4;58,59,46,55;,
 4;60,61,62,63;,
 4;64,65,66,67;,
 4;68,69,70,71;,
 4;72,73,74,75;,
 4;72,65,76,77;,
 4;78,79,66,75;,
 4;80,81,82,83;,
 4;84,85,86,87;,
 4;88,89,90,91;,
 4;92,93,94,95;,
 4;92,85,96,97;,
 4;98,99,86,95;,
 4;100,101,102,103;,
 4;104,105,106,107;,
 4;108,109,110,111;,
 4;112,113,114,115;,
 4;112,105,116,117;,
 4;118,119,106,115;;
 
 MeshMaterialList {
  1;
  36;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
    "C:\\Users\\student\\Desktop\\チーム制作\\2024Team_TGS\\モデル素材\\日野澤\\木の箱\\wooden.jpg";
   }
  }
 }
 MeshNormals {
  23;
  -1.000000;0.000000;0.000000;,
  -0.000000;-0.866026;0.499998;,
  1.000000;0.000000;0.000000;,
  0.000000;-0.866025;0.500000;,
  0.000000;-0.500000;-0.866025;,
  0.000000;-0.866025;0.500000;,
  0.000000;0.499999;0.866026;,
  0.000000;-0.500003;-0.866024;,
  0.000000;0.500000;0.866026;,
  -0.000000;0.866026;-0.500000;,
  0.000000;-0.500000;-0.866025;,
  -0.000000;0.500001;0.866025;,
  0.000000;0.866022;-0.500006;,
  0.000000;-0.499998;-0.866026;,
  0.000000;0.500000;0.866026;,
  0.000000;0.866025;-0.500000;,
  0.000000;0.866025;-0.500000;,
  -0.982449;0.184030;-0.030443;,
  0.000000;-1.000000;-0.000000;,
  0.982449;0.184030;-0.030443;,
  0.000000;0.946463;-0.322814;,
  0.000000;0.055242;-0.998473;,
  0.000000;0.052963;0.998596;;
  36;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;9,9,9,9;,
  4;10,10,10,10;,
  4;11,11,11,11;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;2,2,2,2;,
  4;12,12,12,12;,
  4;13,13,13,13;,
  4;14,14,14,14;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;6,6,6,6;,
  4;15,15,15,15;,
  4;2,2,2,2;,
  4;0,0,0,0;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;6,6,6,6;,
  4;15,15,15,15;,
  4;2,2,2,2;,
  4;0,0,0,0;,
  4;7,7,7,7;,
  4;2,2,2,2;,
  4;8,8,8,8;,
  4;0,0,0,0;,
  4;16,16,16,16;,
  4;5,5,5,5;,
  4;17,17,17,17;,
  4;18,18,18,18;,
  4;19,19,19,19;,
  4;20,20,20,20;,
  4;21,21,21,21;,
  4;22,22,22,22;;
 }
 MeshTextureCoords {
  120;
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
