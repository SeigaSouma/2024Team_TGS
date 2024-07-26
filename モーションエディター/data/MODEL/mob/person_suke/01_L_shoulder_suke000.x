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
 41;
 1.90202;0.84751;2.96841;,
 4.62227;1.48492;1.76307;,
 1.13234;1.57438;1.82904;,
 10.75049;-0.77622;2.61291;,
 8.21594;-0.12822;2.75748;,
 8.07914;-0.88851;2.72250;,
 10.79444;0.43194;2.62089;,
 8.21648;0.64377;2.72070;,
 4.46378;-1.51379;1.70772;,
 4.61783;-1.04751;3.07152;,
 1.99007;-1.25780;2.91897;,
 8.30480;0.54417;0.53784;,
 8.29877;0.93981;1.68363;,
 10.83641;0.80146;1.66686;,
 10.93400;0.35376;0.61683;,
 8.20577;-0.40785;0.38103;,
 10.93739;-0.47211;0.47811;,
 4.72643;0.82965;3.06822;,
 2.29271;-0.02709;3.27486;,
 4.83146;-0.10767;3.16041;,
 4.66235;0.92620;0.37839;,
 4.57691;0.04696;0.01293;,
 1.57616;0.18136;-0.10557;,
 1.29305;1.12291;0.26430;,
 8.02316;-1.28468;1.69104;,
 10.71917;-1.26237;1.66452;,
 4.56434;-0.81471;0.32376;,
 1.84745;-0.85884;0.15885;,
 -0.79705;-0.49559;0.84459;,
 -0.87706;-0.15230;0.70104;,
 -0.70276;-0.75935;2.66910;,
 -0.46909;-0.11229;2.92269;,
 -1.10482;0.59067;1.99275;,
 -0.62860;0.17325;2.74341;,
 -1.02013;0.31545;0.92511;,
 11.94836;0.05227;1.25217;,
 11.94473;-0.36479;1.24032;,
 11.84069;-0.77663;1.78131;,
 11.86016;-0.55667;2.20446;,
 11.88191;0.09514;2.21571;,
 11.90270;0.33180;1.75080;;
 
 58;
 3;0,1,2;,
 3;3,4,5;,
 3;4,3,6;,
 3;6,7,4;,
 3;8,9,10;,
 3;11,12,13;,
 3;11,13,14;,
 3;15,11,14;,
 3;15,14,16;,
 3;17,18,19;,
 3;6,13,12;,
 3;6,12,7;,
 3;20,21,22;,
 3;20,22,23;,
 3;5,24,25;,
 3;5,25,3;,
 3;25,24,15;,
 3;25,15,16;,
 3;7,12,1;,
 3;7,1,17;,
 3;12,20,1;,
 3;19,4,7;,
 3;19,7,17;,
 3;24,5,9;,
 3;24,9,8;,
 3;26,24,8;,
 3;20,15,21;,
 4;27,26,8,10;,
 4;28,29,22,27;,
 4;30,28,27,10;,
 4;31,30,10,18;,
 4;31,29,28,30;,
 4;32,33,0,2;,
 4;29,32,2,22;,
 3;33,32,29;,
 4;34,32,2,23;,
 4;29,34,23,22;,
 4;33,29,22,0;,
 4;32,33,0,2;,
 4;32,34,29,33;,
 4;33,31,18,0;,
 3;31,33,29;,
 4;2,1,20,23;,
 4;26,27,22,21;,
 4;12,11,15,20;,
 4;21,15,24,26;,
 4;10,9,19,18;,
 4;9,5,4,19;,
 4;1,0,18,17;,
 4;35,36,16,14;,
 4;36,37,25,16;,
 3;36,35,37;,
 4;37,38,3,25;,
 4;38,39,6,3;,
 4;38,37,35,39;,
 4;40,35,14,13;,
 4;39,40,13,6;,
 3;35,40,39;;
 
 MeshMaterialList {
  8;
  58;
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
   0.800000;0.674400;0.574400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.810000;0.810000;0.810000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\mob\\sukekaku001.png";
   }
  }
  Material {
   0.274480;0.409530;0.546770;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.103200;0.103200;0.103200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.571200;0.417600;0.125600;1.000000;;
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
    "data\\TEXTURE\\mob\\sukekaku000.png";
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
  65;
  -0.142745;0.691265;0.708362;,
  -0.183480;0.704493;-0.685584;,
  -0.060527;-0.997421;-0.038582;,
  -0.127223;-0.743541;-0.656476;,
  -0.195353;0.944773;0.263139;,
  0.149746;0.005274;0.988711;,
  0.149913;-0.028815;0.988279;,
  0.323058;0.127676;-0.937727;,
  0.153591;-0.985775;-0.068244;,
  0.210559;0.975029;-0.070588;,
  0.269519;0.500385;-0.822785;,
  0.074185;-0.011945;0.997173;,
  -0.056844;0.139260;0.988623;,
  -0.147763;0.214900;-0.965393;,
  0.077731;-0.004090;0.996966;,
  0.077300;-0.072906;0.994339;,
  0.066093;-0.941934;0.329230;,
  0.031947;-0.742665;-0.668901;,
  0.055638;-0.987869;-0.144981;,
  0.029859;-0.999128;-0.029177;,
  0.077705;0.966974;0.242741;,
  0.105197;0.989551;-0.098602;,
  0.092686;0.649214;-0.754937;,
  0.078222;0.859014;-0.505940;,
  0.088468;0.067932;0.993760;,
  0.060775;0.483499;0.873233;,
  0.088939;0.157950;-0.983434;,
  0.088746;-0.044720;-0.995050;,
  -0.240103;0.630781;0.737879;,
  -0.411445;0.641849;-0.647103;,
  -0.168514;-0.976755;-0.132488;,
  -0.243281;-0.769406;-0.590617;,
  -0.344596;0.869530;0.353794;,
  -0.155085;0.040850;0.987056;,
  -0.318438;-0.041943;-0.947015;,
  0.347068;-0.023280;0.937551;,
  0.383186;-0.497088;0.778506;,
  0.556988;-0.396042;-0.730011;,
  0.408468;-0.912243;-0.031087;,
  0.424355;0.905399;0.013269;,
  0.907896;0.265240;-0.324611;,
  0.041098;0.158222;-0.986548;,
  0.170716;0.921171;0.349715;,
  0.109562;0.935612;0.335598;,
  0.048660;-0.919857;0.389224;,
  0.191327;-0.875814;0.443107;,
  0.000766;-0.831699;-0.555227;,
  0.206802;-0.823462;-0.528339;,
  -0.130214;-0.282626;0.950351;,
  -0.063510;-0.235862;0.969709;,
  -0.973329;-0.162728;0.161708;,
  -0.916221;-0.364224;0.166972;,
  -0.986794;0.079655;0.141041;,
  -0.338507;0.758084;-0.557424;,
  -0.851615;-0.496455;0.168179;,
  -0.822274;-0.544625;0.165072;,
  0.207539;-0.965405;0.157864;,
  -0.844731;-0.516138;0.141529;,
  0.057618;-0.140101;0.988459;,
  0.983789;-0.013643;0.178811;,
  0.992751;-0.045299;0.111327;,
  0.996117;-0.076699;0.043215;,
  0.998343;-0.016549;0.055112;,
  0.363514;0.844194;0.393946;,
  0.996810;0.043664;0.066802;;
  58;
  3;0,20,4;,
  3;6,11,15;,
  3;11,6,5;,
  3;5,24,11;,
  3;19,16,2;,
  3;23,21,9;,
  3;23,9,10;,
  3;26,41,10;,
  3;26,10,7;,
  3;25,12,14;,
  3;42,9,21;,
  3;42,21,43;,
  3;22,27,13;,
  3;22,13,1;,
  3;44,18,8;,
  3;44,8,45;,
  3;8,18,46;,
  3;8,46,47;,
  3;43,21,20;,
  3;43,20,25;,
  3;21,22,20;,
  3;14,11,24;,
  3;14,24,25;,
  3;18,44,16;,
  3;18,16,19;,
  3;17,18,19;,
  3;22,26,27;,
  4;3,17,19,2;,
  4;31,34,13,3;,
  4;30,31,3,2;,
  4;33,48,49,12;,
  4;50,51,52,52;,
  4;32,28,0,4;,
  4;53,53,53,13;,
  3;54,55,51;,
  4;29,32,4,1;,
  4;34,29,1,13;,
  4;56,56,56,56;,
  4;32,28,0,4;,
  4;55,57,51,54;,
  4;28,33,12,0;,
  3;50,54,51;,
  4;4,20,22,1;,
  4;17,3,13,27;,
  4;21,23,26,22;,
  4;27,26,18,17;,
  4;49,58,14,12;,
  4;58,15,11,14;,
  4;20,0,12,25;,
  4;40,37,7,10;,
  4;37,38,8,47;,
  3;59,59,60;,
  4;38,36,45,8;,
  4;36,35,5,6;,
  4;61,60,40,62;,
  4;39,40,10,9;,
  4;63,39,9,42;,
  3;40,64,62;;
 }
 MeshTextureCoords {
  41;
  10.183665;12.026917;,
  11.051866;11.183051;,
  9.706045;11.956016;,
  13.953071;10.543931;,
  12.826598;10.906285;,
  12.969139;11.197296;,
  13.660816;10.121423;,
  12.629413;10.642653;,
  11.758525;12.243109;,
  11.697697;12.048481;,
  10.755348;12.725263;,
  12.688359;10.656316;,
  12.584912;10.522657;,
  13.582246;9.985633;,
  13.733710;10.115978;,
  12.894241;11.004074;,
  13.946161;10.397097;,
  11.258894;11.382734;,
  10.555390;12.235497;,
  11.538371;11.678494;,
  11.209917;11.364534;,
  11.402342;11.684322;,
  10.230467;12.329322;,
  9.882402;12.073118;,
  13.049216;11.345414;,
  14.065501;10.717083;,
  11.617895;11.981333;,
  10.599273;12.621917;,
  9.503931;13.106784;,
  9.385825;13.008027;,
  9.607115;13.175105;,
  9.530245;12.900439;,
  9.109515;12.806860;,
  9.396769;12.839699;,
  9.211990;12.881305;,
  14.195319;9.985349;,
  14.300580;10.128542;,
  14.366445;10.293073;,
  14.317584;10.213510;,
  14.159168;9.986015;,
  14.106535;9.900445;;
 }
}
