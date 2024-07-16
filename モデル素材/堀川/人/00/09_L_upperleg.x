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
 49;
 1.88203;-3.26489;3.14681;,
 3.33156;-7.27339;0.40570;,
 3.53502;-2.13077;0.55580;,
 -1.81047;-18.76684;2.33837;,
 -0.33689;-15.03203;2.66711;,
 -2.06584;-14.83049;2.58755;,
 0.93701;-18.83164;2.35650;,
 1.84147;-7.42685;3.37377;,
 1.41876;-15.03283;2.58352;,
 -3.48782;-7.03983;0.27987;,
 -2.42743;-7.26680;3.38131;,
 -2.90573;-3.39471;3.03435;,
 1.19225;-15.16300;-2.38060;,
 2.09195;-15.15408;0.22504;,
 1.77730;-18.89343;0.18694;,
 0.75922;-19.03725;-2.20097;,
 -0.97274;-15.01707;-2.73720;,
 -1.11889;-19.04222;-2.51643;,
 0.06149;-7.20649;-3.57426;,
 -1.89797;-7.18800;-2.86747;,
 -1.99835;-3.18452;-3.24247;,
 0.36712;-2.78475;-3.84377;,
 1.88203;-3.26489;3.14681;,
 -0.10692;-3.84061;3.84377;,
 -0.29009;-7.58158;3.58346;,
 2.50832;-2.36756;-3.00267;,
 2.06099;-7.33244;-2.74317;,
 -2.96685;-14.74798;0.24195;,
 -2.91610;-18.72072;0.18165;,
 -2.91610;-18.72072;0.18165;,
 -2.96685;-14.74798;0.24195;,
 -0.97274;-15.01707;-2.73720;,
 -1.11889;-19.04222;-2.51643;,
 -3.48782;-7.03983;0.27987;,
 -2.90573;-3.39471;3.03435;,
 -1.17233;0.71229;-1.68300;,
 -0.39168;0.83021;-2.00946;,
 -1.77215;0.57336;2.46613;,
 -0.30063;0.22901;3.04283;,
 1.29801;1.16582;0.92802;,
 0.34872;0.46405;2.63512;,
 0.67212;1.04098;-1.49988;,
 0.34872;0.46405;2.63512;,
 1.88203;-3.26489;3.14681;,
 0.34872;0.46405;2.63512;,
 -0.30063;0.22901;3.04283;,
 -0.10692;-3.84061;3.84377;,
 1.88203;-3.26489;3.14681;,
 0.93750;-18.82952;2.35484;;
 
 59;
 3;0,1,2;,
 3;3,4,5;,
 3;4,3,6;,
 3;0,7,1;,
 3;6,8,4;,
 3;9,10,11;,
 3;12,13,14;,
 3;12,14,15;,
 3;16,12,15;,
 3;16,15,17;,
 3;18,19,20;,
 3;18,20,21;,
 3;7,22,23;,
 3;7,23,24;,
 3;6,14,13;,
 3;6,13,8;,
 3;25,2,1;,
 3;25,1,26;,
 3;26,18,21;,
 3;26,21,25;,
 3;5,27,28;,
 3;5,28,3;,
 3;29,30,31;,
 3;29,31,32;,
 3;18,31,19;,
 3;8,13,1;,
 3;8,1,7;,
 3;5,4,24;,
 3;5,24,10;,
 3;13,12,26;,
 3;13,26,1;,
 3;24,4,8;,
 3;24,8,7;,
 3;27,5,10;,
 3;27,10,9;,
 3;19,31,30;,
 3;19,30,33;,
 3;26,12,16;,
 3;26,16,18;,
 3;23,34,10;,
 3;23,10,24;,
 4;20,19,33,11;,
 4;35,36,21,20;,
 4;37,35,20,34;,
 4;38,37,34,23;,
 4;38,36,35,37;,
 4;39,40,0,2;,
 4;36,39,2,21;,
 3;40,39,36;,
 4;41,39,2,25;,
 4;36,41,25,21;,
 4;42,36,21,43;,
 4;39,42,43,2;,
 4;39,41,36,42;,
 4;44,45,46,47;,
 3;45,44,36;,
 3;32,15,28;,
 4;3,28,15,6;,
 3;15,14,48;;
 
 MeshMaterialList {
  9;
  59;
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
  8,
  2,
  2,
  2,
  8,
  8,
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
  0.679362;0.186876;0.709609;,
  0.767264;0.193138;-0.611558;,
  -0.755044;0.128810;0.642897;,
  -0.658268;0.142183;-0.739235;,
  0.929812;0.344682;0.129012;,
  0.019442;-0.074984;0.996995;,
  -0.031094;-0.075459;0.996664;,
  0.165500;-0.060001;-0.984383;,
  -0.995870;-0.030287;-0.085588;,
  0.987538;-0.098489;-0.122758;,
  0.464080;-0.091172;-0.881089;,
  -0.005476;-0.092163;0.995729;,
  0.021116;0.071168;0.997241;,
  0.215862;0.222483;-0.950739;,
  -0.002899;-0.100320;0.994951;,
  -0.074327;-0.099043;0.992303;,
  -0.734164;-0.056702;0.676600;,
  -0.726041;-0.047909;-0.685980;,
  -0.993468;-0.061509;-0.096111;,
  -0.999090;-0.030986;-0.029317;,
  0.990751;-0.085749;0.105161;,
  0.992078;-0.124421;-0.017367;,
  0.641459;-0.115612;-0.758396;,
  0.932964;-0.105153;-0.344268;,
  0.066613;-0.098851;0.992870;,
  0.517046;-0.047844;0.854619;,
  -0.058752;-0.092205;-0.994005;,
  -0.035569;-0.090151;-0.995293;,
  0.604717;0.355550;0.712672;,
  0.574670;0.571240;-0.586037;,
  -0.958138;0.255109;-0.129968;,
  -0.742424;0.372411;-0.556882;,
  0.803543;0.490032;0.337916;,
  0.038842;0.235146;0.971184;,
  -0.044081;0.470389;-0.881358;,
  0.037721;-0.998317;0.044045;,
  0.158047;-0.063256;-0.985403;,
  0.940844;-0.109652;0.320608;,
  0.933430;-0.129879;0.334424;,
  -0.919022;-0.061898;0.389317;,
  -0.887315;-0.086242;0.453029;,
  -0.831692;-0.001181;-0.555235;,
  -0.832276;-0.000187;-0.554362;,
  -0.109488;-0.108918;0.988003;,
  -0.950160;0.096127;-0.296573;,
  -0.279301;0.198591;0.939443;,
  -0.110601;0.988124;0.106665;,
  -0.256538;0.959802;0.113882;,
  0.053090;0.994353;0.091893;,
  0.704315;0.484618;-0.518734;,
  -0.354434;0.927508;0.118765;,
  -0.395001;0.911002;0.118531;,
  -0.932706;-0.327371;0.151289;,
  -0.372425;0.922701;0.099612;,
  -0.015528;-0.994018;0.108106;,
  -0.009067;-0.998078;0.061299;,
  -0.032474;-0.997027;0.069877;,
  -0.049363;-0.998283;0.031525;;
  59;
  3;0,20,4;,
  3;6,11,15;,
  3;11,6,5;,
  3;0,25,20;,
  3;5,24,11;,
  3;19,16,2;,
  3;23,21,9;,
  3;23,9,10;,
  3;26,36,10;,
  3;26,10,7;,
  3;27,17,3;,
  3;27,3,13;,
  3;25,0,12;,
  3;25,12,14;,
  3;37,9,21;,
  3;37,21,38;,
  3;1,4,20;,
  3;1,20,22;,
  3;22,27,13;,
  3;22,13,1;,
  3;39,18,8;,
  3;39,8,40;,
  3;8,18,41;,
  3;8,41,42;,
  3;27,26,17;,
  3;38,21,20;,
  3;38,20,25;,
  3;15,11,14;,
  3;15,14,43;,
  3;21,23,22;,
  3;21,22,20;,
  3;14,11,24;,
  3;14,24,25;,
  3;18,39,16;,
  3;18,16,19;,
  3;17,26,18;,
  3;17,18,19;,
  3;22,36,26;,
  3;22,26,27;,
  3;12,2,16;,
  3;12,16,14;,
  4;3,17,19,44;,
  4;31,34,13,3;,
  4;30,31,3,2;,
  4;33,45,2,12;,
  4;46,47,48,48;,
  4;32,28,0,4;,
  4;49,49,4,13;,
  3;50,51,47;,
  4;29,32,4,1;,
  4;34,29,1,13;,
  4;52,52,52,52;,
  4;32,28,0,4;,
  4;51,53,47,50;,
  4;28,33,12,0;,
  3;46,50,47;,
  3;54,55,56;,
  4;57,56,55,57;,
  3;55,35,35;;
 }
 MeshTextureCoords {
  49;
  0.099710;0.637760;,
  0.126380;0.677840;,
  0.124140;0.627150;,
  0.072840;0.802410;,
  0.084310;0.760420;,
  0.066350;0.760580;,
  0.103740;0.799890;,
  0.094400;0.681040;,
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
  0.087790;0.642260;,
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
