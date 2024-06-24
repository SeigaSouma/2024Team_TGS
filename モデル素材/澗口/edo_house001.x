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
 197;
 -289.38103;258.99900;325.53531;,
 -289.38103;252.49615;325.53531;,
 -289.38103;249.24473;332.53387;,
 -289.38103;262.25054;332.53387;,
 -289.38103;252.49615;325.53531;,
 -289.38103;249.24473;318.53675;,
 -289.38103;242.74185;318.53675;,
 -289.38103;249.24473;332.53387;,
 -289.38103;252.49615;311.53819;,
 -289.38103;249.24473;304.53965;,
 -289.38103;252.49615;311.53819;,
 -289.38103;258.99900;311.53819;,
 -289.38103;262.25054;304.53965;,
 -289.38103;249.24473;304.53965;,
 -289.38103;258.99900;311.53819;,
 -289.38103;262.25054;318.53675;,
 -289.38103;268.75332;318.53675;,
 -289.38103;262.25054;304.53965;,
 -289.38103;262.25054;318.53675;,
 -289.38103;268.75332;318.53675;,
 198.34926;249.24473;332.53387;,
 198.34926;262.25054;332.53387;,
 -289.38103;249.24473;332.53387;,
 -289.38103;242.74185;318.53675;,
 198.34926;242.74185;318.53675;,
 198.34926;249.24473;332.53387;,
 -289.38103;249.24473;304.53965;,
 198.34926;249.24473;304.53965;,
 198.34926;262.25054;304.53965;,
 198.34926;249.24473;304.53965;,
 198.34926;268.75332;318.53675;,
 198.34926;262.25054;304.53965;,
 -289.38103;262.25054;332.53387;,
 198.34926;262.25054;332.53387;,
 198.34926;252.49615;325.53531;,
 198.34926;258.99900;325.53531;,
 198.34926;249.24473;332.53387;,
 198.34926;242.74185;318.53675;,
 198.34926;249.24473;318.53675;,
 198.34926;252.49615;325.53531;,
 198.34926;249.24473;304.53965;,
 198.34926;252.49615;311.53819;,
 198.34926;258.99900;311.53819;,
 198.34926;252.49615;311.53819;,
 198.34926;268.75332;318.53675;,
 198.34926;262.25054;318.53675;,
 -289.38103;258.99900;325.53531;,
 -289.38103;255.74764;318.53675;,
 -289.38103;258.99900;311.53819;,
 -289.38103;255.74764;318.53675;,
 198.34926;258.99900;325.53531;,
 198.34926;255.74764;318.53675;,
 198.34926;258.99900;311.53819;,
 198.34926;262.25054;318.53675;,
 250.90467;230.55193;263.86534;,
 269.67105;174.22136;195.60309;,
 -39.93568;174.22136;195.60309;,
 -219.24402;230.55193;263.86534;,
 250.90486;230.55193;373.20814;,
 195.25297;265.36871;321.17725;,
 -276.20628;265.36871;321.17725;,
 -339.65923;230.55193;373.20814;,
 269.67105;174.22136;441.47037;,
 -395.94664;174.22136;441.47037;,
 317.91216;138.34070;326.31199;,
 317.91216;138.34070;479.52242;,
 -440.12789;138.34070;479.52242;,
 -295.80689;138.34070;322.91763;,
 362.14666;174.22136;441.47037;,
 362.14666;174.22136;324.47420;,
 250.90486;230.55193;321.17725;,
 250.90486;230.55193;373.20814;,
 317.91216;138.34070;157.55103;,
 -127.08599;138.34070;157.55103;,
 362.14666;174.22136;195.60309;,
 250.90467;230.55193;263.86534;,
 439.87203;138.34070;479.52242;,
 439.87203;138.34070;326.31199;,
 439.87203;138.34070;157.55103;,
 439.87203;138.34070;326.31199;,
 439.87203;138.34070;157.55103;,
 439.87203;138.34070;479.52242;,
 362.14666;174.22136;441.47037;,
 195.25297;265.36871;321.17725;,
 362.14666;174.22136;195.60309;,
 -141.78936;174.22136;169.73137;,
 -331.93375;230.55193;-292.52039;,
 -400.56282;174.22136;-311.18638;,
 -395.94664;174.22136;441.47037;,
 -339.65923;230.55193;373.20814;,
 -222.00315;230.55193;-292.52056;,
 -274.31372;265.36871;-237.16617;,
 -276.20628;265.36871;321.17725;,
 -219.24402;230.55193;263.86534;,
 -153.37406;174.22136;-311.18638;,
 -141.78936;174.22136;169.73137;,
 -153.37406;174.22136;-403.16778;,
 -270.99910;174.22136;-403.16778;,
 -274.31372;230.55193;-292.52056;,
 -222.00315;230.55193;-292.52056;,
 -400.56282;174.22136;-403.16778;,
 -331.93375;230.55193;-292.52039;,
 -127.08599;138.34070;157.55103;,
 -115.11749;138.34070;-480.47755;,
 -269.15135;138.34070;-480.47755;,
 -438.81944;138.34070;-480.47755;,
 -153.37406;174.22136;-403.16778;,
 -115.11749;138.34070;-480.47755;,
 -274.31372;265.36871;-237.16617;,
 -400.56282;174.22136;-403.16778;,
 -157.04331;141.01853;189.96719;,
 -157.04331;-11.24668;189.96719;,
 -161.83510;-11.24668;-435.02603;,
 -161.83510;141.01853;-435.02603;,
 -169.24124;141.01853;447.10631;,
 -157.04331;141.01853;189.96719;,
 -161.83510;141.01853;-435.02603;,
 -214.10437;141.01853;447.10631;,
 -169.24124;-11.24668;447.10631;,
 -169.24124;141.01853;447.10631;,
 -214.10437;141.01853;447.10631;,
 -214.10437;-11.24668;447.10631;,
 -415.92745;-11.24668;-435.02603;,
 -415.92745;-11.24668;447.10631;,
 -415.92745;141.01853;447.10631;,
 -415.92745;141.01853;-435.02603;,
 383.69925;-11.24668;447.10631;,
 383.69925;-11.24668;189.96719;,
 383.69925;141.01853;189.96719;,
 383.69925;141.01853;447.10631;,
 383.69925;141.01853;447.10631;,
 383.69925;-11.24668;447.10631;,
 383.69925;141.01853;189.96719;,
 383.69925;141.01853;447.10631;,
 383.69925;-11.24668;189.96719;,
 -157.04331;-11.24668;189.96719;,
 -157.04331;141.01853;189.96719;,
 383.69925;141.01853;189.96719;,
 -161.83510;141.01853;-435.02603;,
 -161.83510;-11.24668;-435.02603;,
 -415.92745;-11.24668;-435.02603;,
 -415.92745;141.01853;-435.02603;,
 -415.92745;141.01853;-435.02603;,
 -415.92745;141.01853;447.10631;,
 -415.92745;141.01853;447.10631;,
 -415.92745;-11.24668;447.10631;,
 -269.31944;258.99900;301.94244;,
 -269.31944;252.49615;301.94244;,
 -262.28326;249.24473;301.94244;,
 -262.28326;262.25054;301.94244;,
 -269.31944;252.49615;301.94244;,
 -276.35559;249.24473;301.94244;,
 -276.35559;242.74185;301.94244;,
 -262.28326;249.24473;301.94244;,
 -283.39178;252.49615;301.94244;,
 -290.42801;249.24473;301.94244;,
 -283.39178;252.49615;301.94244;,
 -283.39183;258.99900;301.94244;,
 -290.42801;262.25054;301.94244;,
 -290.42801;249.24473;301.94244;,
 -276.35559;262.25054;301.94244;,
 -276.35559;268.75332;301.94244;,
 -262.28326;262.25054;301.94244;,
 -262.28326;249.24473;301.94244;,
 -262.28326;249.24473;-269.71690;,
 -262.28326;262.25054;-269.71690;,
 -262.28326;249.24473;301.94244;,
 -276.35559;242.74185;301.94244;,
 -276.35559;242.74185;-269.71690;,
 -262.28326;249.24473;-269.71690;,
 -290.42801;249.24473;301.94244;,
 -290.42801;249.24473;-269.71690;,
 -290.42801;249.24473;301.94244;,
 -290.42801;262.25054;301.94244;,
 -290.42801;262.25054;-269.71690;,
 -290.42801;249.24473;-269.71690;,
 -276.35559;268.75332;301.94244;,
 -276.35559;268.75332;-269.71690;,
 -262.28326;262.25054;-269.71690;,
 -262.28326;249.24473;-269.71690;,
 -269.31944;252.49615;-269.71690;,
 -269.31944;258.99900;-269.71690;,
 -276.35559;242.74185;-269.71690;,
 -276.35559;249.24473;-269.71690;,
 -290.42801;249.24473;-269.71690;,
 -283.39178;252.49615;-269.71690;,
 -290.42801;262.25054;-269.71690;,
 -283.39183;258.99900;-269.71690;,
 -276.35559;268.75332;-269.71690;,
 -276.35559;262.25054;-269.71690;,
 -269.31944;258.99900;301.94244;,
 -276.35559;255.74764;301.94244;,
 -283.39183;258.99900;301.94244;,
 -276.35559;262.25054;301.94244;,
 -276.35559;255.74764;-269.71690;,
 -438.81944;138.34070;-480.47755;,
 -440.12789;138.34070;479.52242;;
 
 110;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;5,8,9,6;,
 4;10,11,12,13;,
 4;14,15,16,17;,
 4;18,0,3,19;,
 4;3,2,20,21;,
 4;22,23,24,25;,
 4;23,26,27,24;,
 4;13,12,28,29;,
 4;17,16,30,31;,
 4;16,32,33,30;,
 4;21,20,34,35;,
 4;36,37,38,39;,
 4;37,40,41,38;,
 4;29,28,42,43;,
 4;28,44,45,42;,
 4;44,21,35,45;,
 3;4,46,47;,
 3;5,4,47;,
 3;8,5,47;,
 3;48,8,47;,
 3;18,11,49;,
 3;0,18,49;,
 3;50,39,51;,
 3;39,38,51;,
 3;38,41,51;,
 3;41,52,51;,
 3;52,53,51;,
 3;53,50,51;,
 4;54,55,56,57;,
 4;58,59,60,61;,
 4;62,58,61,63;,
 4;64,65,66,67;,
 4;68,69,70,71;,
 4;59,54,57,60;,
 4;72,64,67,73;,
 4;70,69,74,75;,
 4;55,72,73,56;,
 4;65,62,63,66;,
 4;76,77,69,68;,
 4;74,69,77,78;,
 4;79,64,72,80;,
 4;81,65,64,79;,
 4;82,62,65,81;,
 3;58,62,82;,
 3;70,83,71;,
 3;75,83,70;,
 3;84,55,54;,
 4;80,72,55,84;,
 3;56,73,85;,
 3;57,56,85;,
 4;86,87,88,89;,
 4;90,91,92,93;,
 4;94,90,93,95;,
 4;96,97,98,99;,
 4;91,86,89,92;,
 4;98,97,100,101;,
 3;102,94,95;,
 4;103,104,97,96;,
 4;100,97,104,105;,
 4;104,67,66,105;,
 4;103,73,67,104;,
 4;106,94,102,107;,
 3;90,94,106;,
 3;98,108,99;,
 3;101,108,98;,
 3;109,87,86;,
 4;110,111,112,113;,
 4;114,115,116,117;,
 4;118,119,120,121;,
 4;122,123,124,125;,
 4;126,127,128,129;,
 4;130,119,118,131;,
 4;132,115,114,133;,
 4;134,135,136,137;,
 4;138,139,140,141;,
 4;117,116,142,143;,
 4;121,120,144,145;,
 4;146,147,148,149;,
 4;150,151,152,153;,
 4;151,154,155,152;,
 4;156,157,158,159;,
 4;157,160,161,158;,
 4;160,146,149,161;,
 4;162,163,164,165;,
 4;166,167,168,169;,
 4;167,170,171,168;,
 4;172,173,174,175;,
 4;173,176,177,174;,
 4;176,162,165,177;,
 4;178,179,180,181;,
 4;179,182,183,180;,
 4;182,184,185,183;,
 4;184,186,187,185;,
 4;186,188,189,187;,
 4;188,178,181,189;,
 3;150,190,191;,
 3;151,150,191;,
 3;154,151,191;,
 3;192,154,191;,
 3;193,192,191;,
 3;190,193,191;,
 3;181,180,194;,
 3;180,183,194;,
 3;183,185,194;,
 3;185,187,194;,
 3;187,189,194;,
 3;189,181,194;,
 4;109,195,196,88;;
 
 MeshMaterialList {
  6;
  110;
  1,
  5,
  5,
  1,
  0,
  1,
  1,
  5,
  5,
  1,
  0,
  0,
  1,
  5,
  5,
  1,
  1,
  1,
  5,
  5,
  5,
  5,
  1,
  1,
  5,
  5,
  5,
  5,
  5,
  5,
  0,
  0,
  0,
  0,
  1,
  0,
  0,
  1,
  0,
  0,
  1,
  1,
  0,
  0,
  0,
  0,
  1,
  1,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  0,
  1,
  0,
  1,
  0,
  0,
  0,
  0,
  1,
  1,
  0,
  0,
  1,
  2,
  5,
  3,
  4,
  4,
  3,
  5,
  3,
  3,
  5,
  3,
  0,
  5,
  5,
  0,
  0,
  0,
  1,
  5,
  5,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  5,
  5,
  5,
  5,
  5,
  5,
  0,
  0,
  0,
  0,
  0,
  0,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\map_object\\kayabuki_yane.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\map_object\\kayabuki_yane.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\map_object\\edo_nagaya001.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\map_object\\edo_nagaya000.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\map_object\\edo_nagaya000.png";
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
  56;
  -1.000000;0.000000;0.000000;,
  0.000000;-0.000000;1.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-0.906904;-0.421337;,
  0.000000;0.000000;-1.000000;,
  0.000000;1.000000;-0.000000;,
  1.000000;0.000000;0.000000;,
  0.229988;0.897436;-0.376450;,
  0.000000;-1.000000;-0.000000;,
  0.212843;0.896579;-0.388387;,
  0.272020;0.897512;-0.347100;,
  0.213147;0.892746;0.396956;,
  -0.322637;0.887821;0.328144;,
  0.229988;0.897436;0.376450;,
  0.491569;0.870839;-0.000001;,
  0.419130;0.907926;0.000000;,
  0.237994;0.901520;-0.361415;,
  0.553179;0.729032;-0.403119;,
  0.237995;0.901520;0.361415;,
  0.435517;0.900181;-0.000000;,
  0.000000;0.749837;0.661623;,
  0.379365;0.903617;0.198893;,
  0.000000;0.749837;-0.661622;,
  0.087573;0.721104;-0.687270;,
  0.015348;0.977117;0.212148;,
  -0.375334;0.847972;0.374256;,
  -0.357624;0.905424;-0.228719;,
  -0.391253;0.894749;-0.215279;,
  0.385381;0.897258;-0.215431;,
  0.385742;0.893892;-0.228386;,
  -0.000001;0.869712;-0.493560;,
  0.000000;0.907068;-0.420984;,
  -0.351436;0.905505;-0.237811;,
  0.365441;0.899961;-0.237746;,
  -0.000000;0.899263;-0.437409;,
  0.732158;0.681106;-0.006280;,
  0.191773;0.904855;-0.380079;,
  -0.652291;0.757968;-0.000758;,
  0.999971;0.000000;-0.007667;,
  0.000000;1.000000;0.000000;,
  0.000001;-1.000000;-0.000000;,
  -0.419479;-0.907765;0.000000;,
  0.000001;1.000000;0.000000;,
  0.000000;-0.906905;0.421336;,
  0.000000;0.906907;-0.421332;,
  0.000000;0.906907;0.421331;,
  0.000000;0.854653;-0.519199;,
  0.000000;0.727561;-0.686043;,
  0.116080;0.576108;-0.809089;,
  0.000000;0.727561;0.686043;,
  -0.372770;0.841810;0.390382;,
  -0.497598;0.867399;-0.003794;,
  0.827639;0.561204;-0.007915;,
  0.419481;-0.907764;-0.000000;,
  -0.419474;0.907767;0.000000;,
  0.419476;0.907766;0.000000;;
  110;
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;43,2,2,43;,
  4;2,3,3,2;,
  4;4,4,4,4;,
  4;44,5,5,44;,
  4;5,45,45,5;,
  4;6,6,6,6;,
  4;6,6,6,6;,
  4;6,6,6,6;,
  4;6,6,6,6;,
  4;6,6,6,6;,
  4;6,6,6,6;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  4;9,22,23,10;,
  4;11,21,24,12;,
  4;20,11,12,25;,
  4;8,8,8,8;,
  4;18,19,14,11;,
  4;46,9,10,46;,
  4;8,8,8,8;,
  4;14,19,16,9;,
  4;22,47,48,23;,
  4;49,20,25,50;,
  4;13,15,19,18;,
  4;16,19,15,7;,
  4;8,8,8,8;,
  4;8,8,8,8;,
  4;18,20,49,13;,
  3;11,20,18;,
  3;14,21,11;,
  3;9,21,14;,
  3;16,22,9;,
  4;7,47,22,16;,
  3;23,48,17;,
  3;10,23,17;,
  4;27,37,25,12;,
  4;28,36,24,10;,
  4;35,28,10,17;,
  4;33,34,30,28;,
  4;51,27,12,24;,
  4;30,34,32,27;,
  3;52,35,17;,
  4;29,31,34,33;,
  4;32,34,31,26;,
  4;8,8,8,8;,
  4;8,8,8,8;,
  4;33,35,52,29;,
  3;28,35,33;,
  3;30,36,28;,
  3;27,36,30;,
  3;32,37,27;,
  4;38,38,38,38;,
  4;39,39,39,39;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;6,6,6,6;,
  4;1,1,1,1;,
  4;39,39,39,39;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;39,39,39,39;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;6,6,6,6;,
  4;53,40,40,53;,
  4;40,41,41,40;,
  4;0,0,0,0;,
  4;54,42,42,54;,
  4;42,55,55,42;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  4;32,26,50,25;;
 }
 MeshTextureCoords {
  197;
  -0.089216;0.057571;,
  -0.089216;0.068680;,
  -0.101142;0.074234;,
  -0.101142;0.052017;,
  0.460940;0.088590;,
  0.421880;0.156250;,
  0.343750;0.156250;,
  0.421880;0.020930;,
  0.460940;0.223910;,
  0.421880;0.291570;,
  -0.065364;0.068680;,
  -0.065364;0.057571;,
  -0.053438;0.052017;,
  -0.053438;0.074234;,
  -0.088610;-0.026812;,
  -0.088610;-0.033426;,
  -0.088610;-0.046653;,
  -0.088610;-0.033426;,
  -0.077290;0.052017;,
  -0.077290;0.040909;,
  -0.043024;0.074234;,
  -0.043024;0.052017;,
  0.416670;0.312500;,
  0.458330;0.312500;,
  0.458330;0.688440;,
  0.416670;0.688440;,
  0.500000;0.312500;,
  0.500000;0.688440;,
  0.004680;0.052017;,
  0.004680;0.074234;,
  0.903449;-0.046653;,
  0.903449;-0.033426;,
  -0.088610;-0.033426;,
  0.903449;-0.033426;,
  -0.031098;0.068680;,
  -0.031098;0.057571;,
  0.421880;0.979070;,
  0.343750;0.843750;,
  0.421880;0.843750;,
  0.460940;0.911410;,
  0.421880;0.708430;,
  0.460940;0.776090;,
  -0.007246;0.057571;,
  -0.007246;0.068680;,
  -0.019172;0.040909;,
  -0.019172;0.052017;,
  0.539060;0.088590;,
  0.500000;0.150000;,
  0.539060;0.223910;,
  -0.077290;0.063125;,
  0.539060;0.911410;,
  0.500000;0.837500;,
  0.539060;0.776090;,
  0.578120;0.843750;,
  1.010348;0.031050;,
  1.048519;0.145628;,
  0.418770;0.145628;,
  0.054051;0.031050;,
  1.010348;0.031050;,
  0.897151;-0.039768;,
  -0.061812;-0.039768;,
  -0.190878;0.031050;,
  1.048519;0.145628;,
  -0.305368;0.145628;,
  1.146643;0.218611;,
  1.146643;0.218611;,
  -0.395234;0.218611;,
  -0.101681;0.218611;,
  -0.209141;0.202391;,
  -0.009772;0.202391;,
  -0.017409;0.106165;,
  -0.106073;0.106165;,
  1.146643;0.218611;,
  0.241503;0.218611;,
  0.209833;0.202391;,
  0.080254;0.106165;,
  -0.264722;0.263683;,
  -0.003642;0.263683;,
  0.283938;0.263683;,
  1.394714;0.218611;,
  1.394714;0.218611;,
  1.394714;0.218611;,
  1.236618;0.145628;,
  -0.024040;0.046690;,
  1.236618;0.145628;,
  0.211596;0.145628;,
  0.958920;0.106165;,
  0.982551;0.202391;,
  -0.299475;0.202391;,
  -0.176444;0.106165;,
  0.972020;0.106165;,
  0.871459;0.046690;,
  -0.080219;0.046690;,
  0.024231;0.106165;,
  1.012006;0.202391;,
  0.193871;0.202391;,
  0.188032;0.145628;,
  -0.051221;0.145628;,
  -0.057963;0.031050;,
  0.048439;0.031050;,
  -0.314757;0.145628;,
  -0.175164;0.031050;,
  0.216379;0.263683;,
  0.265847;0.218611;,
  -0.047462;0.218611;,
  -0.392573;0.218611;,
  1.168748;0.202391;,
  1.305047;0.263683;,
  -0.057963;-0.039768;,
  1.139293;0.202391;,
  -0.116258;0.019149;,
  -0.116258;0.979997;,
  1.148314;0.979997;,
  1.148314;0.019149;,
  0.557360;0.500000;,
  0.557360;0.250000;,
  0.571550;0.250000;,
  0.571550;0.500000;,
  -0.272179;0.985863;,
  -0.272179;0.158544;,
  -0.406158;0.158544;,
  -0.406158;0.985863;,
  -1.406037;0.943092;,
  2.637986;0.943092;,
  2.637986;0.167712;,
  -1.406037;0.167712;,
  2.541994;0.943092;,
  1.363173;0.943092;,
  1.363173;0.167712;,
  2.541994;0.167712;,
  1.379114;0.158544;,
  1.379114;0.985863;,
  0.375000;0.250000;,
  0.375000;0.500000;,
  1.379114;0.985863;,
  -0.235752;0.985863;,
  -0.235752;0.158544;,
  1.379114;0.158544;,
  -0.250062;0.158544;,
  -0.250062;0.985863;,
  -1.008879;0.985863;,
  -1.008879;0.158544;,
  0.625000;0.250000;,
  0.625000;0.500000;,
  -1.008879;0.158544;,
  -1.008879;0.985863;,
  -0.047804;-0.026812;,
  -0.047804;-0.013585;,
  -0.033492;-0.006972;,
  -0.033492;-0.033426;,
  0.460940;0.088590;,
  0.421880;0.156250;,
  0.343750;0.156250;,
  0.421880;0.020930;,
  0.460940;0.223910;,
  0.421880;0.291570;,
  -0.076428;-0.013585;,
  -0.076428;-0.026812;,
  -0.090740;-0.033426;,
  -0.090740;-0.006972;,
  -0.062116;-0.033426;,
  -0.062116;-0.046653;,
  -0.045783;0.052017;,
  -0.045783;0.074234;,
  0.928361;0.074234;,
  0.928361;0.052017;,
  0.416670;0.312500;,
  0.458330;0.312500;,
  0.458330;0.688440;,
  0.416670;0.688440;,
  0.500000;0.312500;,
  0.500000;0.688440;,
  -0.049137;0.074234;,
  -0.049137;0.052017;,
  0.925008;0.052017;,
  0.925008;0.074234;,
  -0.047460;0.040909;,
  0.926684;0.040909;,
  -0.033492;-0.033426;,
  -0.033492;-0.006972;,
  -0.047804;-0.013585;,
  -0.047804;-0.026812;,
  -0.062116;0.006255;,
  -0.062116;-0.006972;,
  -0.090740;-0.006972;,
  -0.076428;-0.013585;,
  -0.090740;-0.033426;,
  -0.076428;-0.026812;,
  -0.062116;-0.046653;,
  -0.062116;-0.033426;,
  0.539060;0.088590;,
  0.500000;0.150000;,
  0.539060;0.223910;,
  0.578120;0.156250;,
  -0.062116;-0.020199;,
  1.266475;0.263683;,
  -0.369583;0.263683;;
 }
}
