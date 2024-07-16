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
 31.56225;2.61354;-3.17823;,
 27.85620;4.05060;-0.46053;,
 32.61078;4.25241;-0.60942;,
 -0.81750;-2.28333;-3.95280;,
 6.96780;0.17076;-4.50030;,
 7.38792;-2.70873;-4.36785;,
 -0.95238;2.29233;-3.98298;,
 6.96618;3.09468;-4.36110;,
 28.07211;-2.71023;-0.33588;,
 27.86223;-1.65900;-3.41070;,
 31.44216;-2.13312;-3.06675;,
 6.69477;2.71734;3.90636;,
 6.71337;4.21578;-0.43320;,
 -1.08135;3.69171;-0.36981;,
 -1.38102;1.99620;3.60711;,
 6.99903;-0.88830;4.50030;,
 -1.39137;-1.13169;4.13253;,
 27.91797;0.80871;3.48516;,
 27.93507;-1.13412;2.78445;,
 31.63656;-1.23345;3.15627;,
 32.00622;1.11159;3.75237;,
 27.71430;2.57331;-3.40317;,
 31.02996;0.64164;-3.86907;,
 27.57120;0.46005;-3.61101;,
 32.39175;3.23442;2.91855;,
 27.80166;2.79084;2.66127;,
 7.55985;-4.20921;-0.46134;,
 -0.72123;-4.12467;-0.36090;,
 -0.72123;-4.12467;-0.36090;,
 7.55985;-4.20921;-0.46134;,
 6.99903;-0.88830;4.50030;,
 -1.39137;-1.13169;4.13253;,
 28.07211;-2.71023;-0.33588;,
 31.44216;-2.13312;-3.06675;,
 35.23935;-0.41457;1.61019;,
 35.34840;0.35937;1.93380;,
 35.11095;-1.00932;-2.50338;,
 34.79256;0.44955;-3.07509;,
 35.65857;2.03454;-0.97842;,
 35.00985;1.09344;-2.67084;,
 35.54319;1.41408;1.42857;,
 35.00985;1.09344;-2.67084;,
 31.56225;2.61354;-3.17823;,
 35.00985;1.09344;-2.67084;,
 34.79256;0.44955;-3.07509;,
 31.02996;0.64164;-3.86907;,
 31.56225;2.61354;-3.17823;,
 -0.94806;2.29314;-3.98028;;
 
 52;
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
 3;17,19,20;,
 3;21,22,23;,
 3;6,13,12;,
 3;6,12,7;,
 3;24,2,1;,
 3;25,17,20;,
 3;5,26,27;,
 3;5,27,3;,
 3;28,29,30;,
 3;28,30,31;,
 3;7,12,1;,
 3;5,4,23;,
 3;23,4,7;,
 3;26,9,8;,
 3;18,29,32;,
 3;22,33,9;,
 3;22,9,23;,
 4;19,18,32,10;,
 4;34,35,20,19;,
 4;36,34,19,33;,
 4;37,36,33,22;,
 4;37,35,34,36;,
 4;38,39,0,2;,
 4;35,38,2,20;,
 3;39,38,35;,
 4;40,38,2,24;,
 4;35,40,24,20;,
 4;41,35,20,42;,
 4;38,41,42,2;,
 4;38,40,35,41;,
 4;43,44,45,46;,
 3;44,43,35;,
 3;31,14,27;,
 4;3,27,14,6;,
 3;14,13,47;,
 4;17,30,29,18;,
 4;23,9,26,5;,
 4;17,25,11,15;,
 4;1,21,23,7;,
 4;11,25,1,12;,
 4;1,25,20,24;,
 4;1,0,22,21;;
 
 MeshMaterialList {
  9;
  52;
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
   0.420000;0.420000;0.420000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.282400;0.282400;0.282400;1.000000;;
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
   0.592800;0.380000;0.122400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  58;
  0.226935;0.679624;-0.697576;,
  0.274209;0.768268;0.578424;,
  0.137635;-0.753726;-0.642615;,
  0.151224;-0.657397;0.738214;,
  0.363948;0.922389;-0.129385;,
  -0.059976;0.019453;-0.998010;,
  -0.060356;-0.031109;-0.997692;,
  -0.047980;0.165622;0.985021;,
  -0.024227;-0.996053;0.085389;,
  -0.078843;0.989258;0.123096;,
  -0.072990;0.465190;0.882196;,
  -0.017451;-0.001354;-0.999847;,
  0.075797;0.082165;-0.993732;,
  0.237302;0.283698;0.929087;,
  0.010404;-0.005025;-0.999933;,
  0.020302;-0.240513;-0.970434;,
  0.007277;-0.644051;-0.764948;,
  0.031959;-0.748241;0.662656;,
  0.040121;-0.966139;0.254882;,
  0.037845;-0.998882;0.028316;,
  -0.014580;0.983003;-0.183012;,
  -0.045864;0.997665;-0.050617;,
  -0.039706;0.616157;0.786622;,
  -0.029713;0.835459;0.548749;,
  0.014569;0.227290;-0.973718;,
  0.003504;0.456994;-0.889463;,
  -0.001486;-0.003291;0.999994;,
  -0.031210;-0.116260;0.992728;,
  0.377449;0.598503;-0.706630;,
  0.598020;0.560480;0.572917;,
  0.272217;-0.953502;0.129354;,
  0.394948;-0.736018;0.549812;,
  0.515646;0.788981;-0.334095;,
  0.251017;0.038347;-0.967223;,
  0.496069;-0.044328;0.867151;,
  -0.997371;0.047110;-0.055069;,
  -0.050664;0.158162;0.986112;,
  -0.087827;0.942911;-0.321255;,
  -0.051504;0.959443;-0.277159;,
  -0.041515;-0.912691;-0.406536;,
  -0.069010;-0.888512;-0.453634;,
  -0.000948;-0.831705;0.555217;,
  -0.000155;-0.832287;0.554346;,
  0.102655;-0.949431;0.296720;,
  0.212296;-0.278429;-0.936701;,
  0.989615;-0.103606;-0.099644;,
  0.964550;-0.241335;-0.106774;,
  0.995081;0.049638;-0.085737;,
  0.510596;0.692231;0.510008;,
  0.935952;-0.333919;-0.111764;,
  0.921144;-0.372831;-0.111760;,
  -0.348179;-0.925325;-0.150152;,
  0.931539;-0.351358;-0.093713;,
  -0.990678;-0.019373;-0.134844;,
  -0.996998;-0.011341;-0.076587;,
  -0.995359;-0.040590;-0.087249;,
  -0.997316;-0.061696;-0.039415;,
  0.068011;-0.599087;-0.797790;;
  52;
  3;0,20,4;,
  3;6,11,15;,
  3;11,6,5;,
  3;5,24,11;,
  3;19,16,2;,
  3;23,21,9;,
  3;23,9,10;,
  3;26,36,10;,
  3;26,10,7;,
  3;27,17,3;,
  3;27,3,13;,
  3;25,12,14;,
  3;37,9,21;,
  3;37,21,38;,
  3;1,4,20;,
  3;22,27,13;,
  3;39,18,8;,
  3;39,8,40;,
  3;8,18,41;,
  3;8,41,42;,
  3;38,21,20;,
  3;15,11,14;,
  3;14,11,24;,
  3;18,16,19;,
  3;17,18,19;,
  3;12,2,16;,
  3;12,16,14;,
  4;3,17,19,43;,
  4;31,34,13,3;,
  4;30,31,3,2;,
  4;33,44,2,12;,
  4;45,46,47,47;,
  4;32,28,0,4;,
  4;48,48,4,13;,
  3;49,50,46;,
  4;29,32,4,1;,
  4;34,29,1,13;,
  4;51,51,51,51;,
  4;32,28,0,4;,
  4;50,52,46,49;,
  4;28,33,12,0;,
  3;45,49,46;,
  3;53,54,55;,
  4;56,55,54,56;,
  3;54,35,35;,
  4;27,26,18,17;,
  4;14,16,57,15;,
  4;27,22,23,26;,
  4;20,25,14,24;,
  4;23,22,20,21;,
  4;20,22,13,1;,
  4;20,0,12,25;;
 }
 MeshTextureCoords {
  48;
  0.099710;0.637760;,
  0.126380;0.677840;,
  0.124140;0.627150;,
  0.072840;0.802410;,
  0.084310;0.760420;,
  0.066350;0.760580;,
  0.103740;0.799890;,
  0.102370;0.757840;,
  0.020410;0.690390;,
  0.052590;0.685200;,
  0.033740;0.651850;,
  0.156590;0.758660;,
  0.128020;0.758270;,
  0.129550;0.798190;,
  0.156680;0.798560;,
  0.173080;0.759130;,
  0.171180;0.799140;,
  0.180930;0.680570;,
  0.201480;0.683590;,
  0.207220;0.643850;,
  0.183730;0.636380;,
  0.094400;0.681040;,
  0.070830;0.648090;,
  0.073870;0.685260;,
  0.161300;0.629850;,
  0.159440;0.679480;,
  0.041040;0.765190;,
  0.047700;0.804280;,
  0.210270;0.802320;,
  0.217850;0.763950;,
  0.188820;0.760740;,
  0.183960;0.800760;,
  0.236170;0.689460;,
  0.048570;0.647600;,
  0.207220;0.643850;,
  0.183730;0.636380;,
  0.048570;0.647600;,
  0.070830;0.648090;,
  0.124140;0.627150;,
  0.099710;0.637760;,
  0.161300;0.629850;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.049860;0.318880;,
  0.035420;0.324050;,
  0.035420;0.324050;,
  0.049860;0.318880;,
  0.000000;0.000000;;
 }
}
