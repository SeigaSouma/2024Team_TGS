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
 56;
 -7.09166;7.14661;1.18777;,
 -24.05290;6.71183;0.86716;,
 -10.83235;3.61396;6.72518;,
 -24.55914;3.52731;7.21027;,
 -41.51837;-1.12804;5.70005;,
 -41.52099;2.62389;5.52130;,
 -54.05004;1.59445;5.03622;,
 -54.72831;1.21441;-4.70364;,
 -54.25399;1.75620;-1.08368;,
 -41.95025;1.28436;-5.08754;,
 -41.46897;-2.48708;-5.84964;,
 -54.74484;-2.79928;-5.37783;,
 -41.92095;4.06269;0.48107;,
 -53.68422;-6.64002;0.38820;,
 -40.58162;-7.14661;0.51709;,
 -40.85353;-4.82307;5.53004;,
 -53.83647;-4.27732;4.99743;,
 -41.46897;-2.48708;-5.84964;,
 -40.58162;-7.14661;0.51709;,
 -53.68422;-6.64002;0.38820;,
 -54.74484;-2.79928;-5.37783;,
 -23.77154;-4.86251;-6.12798;,
 -23.83248;-0.27663;-7.63863;,
 -24.24785;3.99655;-5.86247;,
 -24.03135;-5.99386;7.22631;,
 -25.06961;-1.02812;7.65832;,
 -12.73113;-0.63653;8.21455;,
 -11.26043;-6.61783;6.48490;,
 -23.28265;-6.42888;0.59815;,
 -10.56713;-4.67884;-6.92945;,
 -9.24865;0.37654;-8.21455;,
 2.67414;-1.24506;-4.29437;,
 2.28530;-2.91357;-3.59672;,
 -11.26043;-6.61783;6.48490;,
 1.82704;-4.19538;5.27052;,
 0.69139;-1.05064;6.50299;,
 1.46647;0.33708;5.63167;,
 3.78107;2.36578;1.98341;,
 -7.87271;4.95240;-6.41696;,
 3.36934;1.02836;-3.20538;,
 -10.83235;3.61396;6.72518;,
 1.46647;0.33708;5.63167;,
 -10.83235;3.61396;6.72518;,
 -12.73113;-0.63653;8.21455;,
 0.69139;-1.05064;6.50299;,
 1.46647;0.33708;5.63167;,
 -10.83235;3.61396;6.72518;,
 -23.28265;-6.42888;0.59815;,
 -61.95571;-2.36356;-0.94968;,
 -61.86092;-0.61181;-0.93247;,
 -61.61130;-3.72680;0.84092;,
 -61.67576;-2.99861;2.24163;,
 -61.58754;-0.28473;2.24600;,
 -61.49629;0.50431;0.17416;,
 -61.58754;-0.28473;2.24600;,
 -54.05004;1.59445;5.03622;;
 
 53;
 3;0,1,2;,
 3;1,3,2;,
 3;4,5,6;,
 3;7,8,9;,
 3;7,9,10;,
 3;11,7,10;,
 3;5,12,6;,
 3;13,14,15;,
 3;16,13,15;,
 3;17,18,19;,
 3;20,17,19;,
 3;21,17,22;,
 3;23,9,12;,
 3;1,23,12;,
 3;24,15,14;,
 3;18,17,21;,
 3;25,24,26;,
 4;27,28,21,29;,
 4;29,30,31,32;,
 4;33,29,32,34;,
 4;26,33,34,35;,
 4;34,32,31,35;,
 4;0,2,36,37;,
 4;30,0,37,31;,
 3;31,37,36;,
 4;38,0,37,39;,
 4;30,38,39,31;,
 4;40,30,31,41;,
 4;0,40,41,37;,
 4;41,31,39,37;,
 4;42,43,44,45;,
 3;31,45,44;,
 4;9,23,22,10;,
 4;12,5,3,1;,
 4;0,38,23,1;,
 4;22,23,38,30;,
 4;21,22,30,29;,
 4;24,14,21,28;,
 4;9,8,6,12;,
 4;46,3,25,26;,
 4;47,27,26,24;,
 4;4,25,3,5;,
 4;4,15,24,25;,
 4;15,4,6,16;,
 4;48,49,7,20;,
 4;50,48,20,13;,
 3;50,49,48;,
 4;51,50,13,16;,
 4;52,51,16,6;,
 4;52,49,50,51;,
 4;53,54,55,8;,
 4;49,53,8,7;,
 3;54,53,49;;
 
 MeshMaterialList {
  8;
  53;
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
   0.125600;0.125600;0.125600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.708800;0.329600;0.263200;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.019200;0.310400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.248000;0.119200;0.065600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.700000;0.708800;0.533600;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.661600;0.044000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  65;
  0.119109;0.674584;0.728526;,
  0.183476;0.704486;-0.685592;,
  0.070066;-0.991101;0.113182;,
  0.120077;-0.761716;-0.636687;,
  0.195352;0.944771;0.263144;,
  -0.144229;0.003923;0.989537;,
  -0.174416;-0.878337;0.445088;,
  -0.281680;0.132684;-0.950290;,
  -0.202506;-0.953784;0.222009;,
  -0.176559;0.975414;-0.131885;,
  -0.180181;0.950500;-0.253149;,
  -0.079034;0.001723;0.996871;,
  0.053610;-0.129273;0.990159;,
  0.136974;0.006239;-0.990555;,
  -0.069116;0.027052;0.997242;,
  -0.083310;-0.650316;0.755082;,
  -0.022642;-0.940911;0.337896;,
  -0.033707;-0.775185;-0.630834;,
  -0.035124;-0.965412;0.258354;,
  0.023978;-0.991087;0.131043;,
  -0.068104;0.987810;0.139975;,
  -0.137123;0.987310;-0.080102;,
  -0.107169;0.745237;-0.658131;,
  -0.118935;0.865470;-0.486637;,
  -0.077005;0.062844;0.995048;,
  -0.065498;0.602777;0.795217;,
  -0.079015;-0.107739;-0.991034;,
  -0.068778;0.018054;-0.997469;,
  0.240102;0.630782;0.737879;,
  0.411442;0.641856;-0.647098;,
  0.168506;-0.976757;-0.132482;,
  0.243279;-0.769405;-0.590620;,
  0.344593;0.869530;0.353800;,
  0.155086;0.040854;0.987056;,
  0.318439;-0.041933;-0.947015;,
  -0.336683;-0.004732;0.941606;,
  -0.290349;-0.854934;0.429867;,
  -0.487453;-0.401183;-0.775527;,
  -0.359778;-0.932186;-0.039857;,
  -0.272893;0.958689;-0.080273;,
  -0.734070;0.494763;-0.465136;,
  -0.197725;0.154811;-0.967956;,
  -0.030520;0.194233;-0.980481;,
  -0.104567;0.940999;0.321849;,
  -0.144366;0.985434;0.089879;,
  -0.012804;-0.820581;-0.571387;,
  -0.071642;-0.809632;-0.582549;,
  -0.175783;-0.825893;-0.535725;,
  -0.077157;-0.091485;0.992813;,
  0.130213;-0.282632;0.950349;,
  0.986794;0.079662;0.141039;,
  0.916228;-0.364208;0.166972;,
  0.973337;-0.162683;0.161707;,
  0.338506;0.758089;-0.557418;,
  0.822267;-0.544636;0.165071;,
  0.851625;-0.496438;0.168179;,
  -0.207537;-0.965407;0.157855;,
  0.844733;-0.516139;0.141520;,
  -0.194957;-0.018132;0.980644;,
  -0.990999;0.012867;0.133248;,
  -0.972931;0.050429;0.225526;,
  -0.991782;0.118415;0.048430;,
  -0.998907;-0.024826;0.039604;,
  -0.179699;0.964160;0.195200;,
  -0.964181;0.259212;0.056256;;
  53;
  3;4,20,0;,
  3;20,25,0;,
  3;11,24,5;,
  3;10,9,23;,
  3;41,42,26;,
  3;7,41,26;,
  3;43,21,44;,
  3;8,18,15;,
  3;6,8,15;,
  3;45,46,45;,
  3;47,45,45;,
  3;17,26,27;,
  3;22,23,21;,
  3;20,22,21;,
  3;16,15,18;,
  3;46,26,17;,
  3;14,48,12;,
  4;2,19,17,3;,
  4;3,13,34,31;,
  4;2,3,31,30;,
  4;12,49,49,33;,
  4;50,50,51,52;,
  4;4,0,28,32;,
  4;53,53,53,53;,
  3;51,54,55;,
  4;1,4,32,29;,
  4;13,1,29,34;,
  4;56,56,56,56;,
  4;4,0,28,32;,
  4;55,51,57,54;,
  4;0,12,33,28;,
  3;51,55,52;,
  4;23,22,27,26;,
  4;21,43,25,20;,
  4;4,1,22,20;,
  4;27,22,1,13;,
  4;17,27,13,3;,
  4;16,18,17,19;,
  4;23,9,44,21;,
  4;0,25,14,12;,
  4;19,2,12,16;,
  4;11,14,25,24;,
  4;11,15,48,14;,
  4;15,11,5,58;,
  4;37,40,41,7;,
  4;38,37,47,8;,
  3;59,59,60;,
  4;36,38,8,6;,
  4;35,35,58,5;,
  4;61,59,59,62;,
  4;39,63,44,9;,
  4;40,39,9,10;,
  3;61,64,40;;
 }
 MeshTextureCoords {
  56;
  0.124140;0.627150;,
  0.126380;0.677840;,
  0.099710;0.637760;,
  0.094400;0.681040;,
  0.084310;0.760420;,
  0.102370;0.757840;,
  0.103740;0.799890;,
  0.156680;0.798560;,
  0.129550;0.798190;,
  0.156590;0.758660;,
  0.173080;0.759130;,
  0.171180;0.799140;,
  0.128020;0.758270;,
  0.047700;0.804280;,
  0.041040;0.765190;,
  0.066350;0.760580;,
  0.072840;0.802410;,
  0.188820;0.760740;,
  0.217850;0.763950;,
  0.210270;0.802320;,
  0.183960;0.800760;,
  0.201480;0.683590;,
  0.180930;0.680570;,
  0.159440;0.679480;,
  0.052590;0.685200;,
  0.073870;0.685260;,
  0.070830;0.648090;,
  0.033740;0.651850;,
  0.236170;0.689460;,
  0.207220;0.643850;,
  0.183730;0.636380;,
  0.183730;0.636380;,
  0.207220;0.643850;,
  0.048570;0.647600;,
  0.048570;0.647600;,
  0.070830;0.648090;,
  0.099710;0.637760;,
  0.124140;0.627150;,
  0.161300;0.629850;,
  0.161300;0.629850;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.049860;0.318880;,
  0.035420;0.324050;,
  0.035420;0.324050;,
  0.049860;0.318880;,
  0.087790;0.642260;,
  0.020410;0.690390;,
  0.183960;0.800760;,
  0.156680;0.798560;,
  0.047700;0.804280;,
  0.072840;0.802410;,
  0.103740;0.799890;,
  0.129550;0.798190;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
