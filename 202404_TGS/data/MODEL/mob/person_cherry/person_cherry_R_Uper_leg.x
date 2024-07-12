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
 -1.79893;-0.63102;2.72893;,
 -3.33276;0.42135;0.32469;,
 -3.14398;-4.35059;0.18541;,
 1.62741;-15.01555;1.97877;,
 1.86442;-11.36295;2.20999;,
 0.26004;-11.54995;2.28382;,
 -0.92203;-15.07568;1.99559;,
 -1.76129;-4.49299;2.93958;,
 -1.36906;-11.55069;2.20624;,
 3.18389;-4.13386;0.06865;,
 2.69248;-0.53784;2.93021;,
 2.19989;-4.34446;2.94653;,
 -1.70175;-15.13301;-0.01758;,
 -1.99372;-11.66320;0.01777;,
 -1.15887;-11.67149;-2.40005;,
 -0.75701;-15.26645;-2.23337;,
 0.85006;-11.53607;-2.73095;,
 0.98568;-15.27109;-2.52608;,
 1.80175;-0.55645;-3.19979;,
 1.70864;-4.27134;-2.85178;,
 -0.10962;-4.28849;-3.50767;,
 -0.39322;-0.18549;-3.75775;,
 0.04665;-1.16525;3.37561;,
 -1.79893;-0.63102;2.72893;,
 0.21662;-4.63655;3.13407;,
 -2.38008;0.20162;-2.97728;,
 -1.96498;-4.40538;-2.73648;,
 2.65329;-14.97275;-0.02249;,
 2.70039;-11.28639;0.03347;,
 2.65329;-14.97275;-0.02249;,
 0.98568;-15.27109;-2.52608;,
 0.85006;-11.53607;-2.73095;,
 2.70039;-11.28639;0.03347;,
 3.18389;-4.13386;0.06865;,
 2.69248;-0.53784;2.93021;,
 -0.54266;2.65720;1.76163;,
 -1.30331;3.22698;0.30657;,
 -0.94106;3.17210;-1.62661;,
 0.21283;2.39779;2.18355;,
 -0.54266;2.65720;1.76163;,
 -1.79893;-0.63102;2.72893;,
 -0.94106;3.17210;-1.62661;,
 1.65356;2.78470;1.83133;,
 0.21283;2.39779;2.18355;,
 0.04665;-1.16525;3.37561;,
 2.69248;-0.53784;2.93021;,
 -0.07664;2.98964;-2.12461;,
 -0.07664;2.98964;-2.12461;,
 -0.39322;-0.18549;-3.75775;,
 1.36550;2.78420;-1.92895;,
 1.80175;-0.55645;-3.19979;,
 -0.07664;2.98964;-2.12461;,
 -0.39322;-0.18549;-3.75775;,
 0.98522;-15.27096;-2.52788;,
 2.65492;-14.97116;-0.02125;,
 2.65492;-14.97116;-0.02125;;
 
 59;
 3;0,1,2;,
 3;3,4,5;,
 3;5,6,3;,
 3;0,2,7;,
 3;6,5,8;,
 3;9,10,11;,
 3;12,13,14;,
 3;12,14,15;,
 3;15,14,16;,
 3;15,16,17;,
 3;18,19,20;,
 3;18,20,21;,
 3;22,23,7;,
 3;22,7,24;,
 3;6,8,13;,
 3;6,13,12;,
 3;25,26,2;,
 3;25,2,1;,
 3;21,20,26;,
 3;21,26,25;,
 3;27,28,4;,
 3;27,4,3;,
 3;29,30,31;,
 3;29,31,32;,
 3;20,19,31;,
 3;2,13,8;,
 3;2,8,7;,
 3;24,5,4;,
 3;24,4,11;,
 3;26,14,13;,
 3;26,13,2;,
 3;24,7,8;,
 3;24,8,5;,
 3;11,4,28;,
 3;11,28,9;,
 3;19,33,32;,
 3;19,32,31;,
 3;26,20,16;,
 3;26,16,14;,
 3;11,34,22;,
 3;11,22,24;,
 3;33,19,18;,
 3;18,10,33;,
 4;35,36,1,23;,
 4;36,37,25,1;,
 3;36,35,37;,
 4;38,39,40,22;,
 3;41,39,38;,
 4;42,43,44,45;,
 3;43,42,46;,
 4;37,47,48,25;,
 3;43,47,37;,
 4;49,42,45,50;,
 4;51,49,50,52;,
 3;42,49,51;,
 3;12,15,17;,
 4;6,12,30,53;,
 4;54,29,3,6;,
 4;55,54,6,17;;
 
 MeshMaterialList {
  4;
  59;
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
   0.414400;0.266400;0.081600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.021600;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  57;
  -0.776118;0.190560;-0.601106;,
  0.992355;0.121339;0.022530;,
  0.715179;0.132591;-0.686249;,
  -0.947247;0.268381;0.175202;,
  -0.019443;-0.074984;0.996995;,
  0.031093;-0.075459;0.996664;,
  -0.165504;-0.060002;-0.984382;,
  0.995870;-0.030289;-0.085595;,
  -0.987536;-0.098494;-0.122763;,
  -0.464077;-0.091177;-0.881090;,
  -0.654691;0.180066;0.734136;,
  0.005475;-0.092160;0.995729;,
  -0.056544;0.099541;0.993426;,
  -0.124935;0.149981;-0.980763;,
  0.002906;-0.100319;0.994951;,
  0.074318;-0.099040;0.992304;,
  0.940087;-0.089307;0.329030;,
  0.702172;-0.081365;-0.707344;,
  0.993468;-0.061512;-0.096118;,
  0.996120;-0.014125;-0.086866;,
  -0.990752;-0.085749;0.105160;,
  -0.992078;-0.124421;-0.017367;,
  -0.641458;-0.115612;-0.758396;,
  -0.932962;-0.105157;-0.344272;,
  -0.066600;-0.098851;0.992871;,
  -0.517030;-0.047843;0.854629;,
  0.058755;-0.092206;-0.994005;,
  0.035574;-0.090151;-0.995293;,
  -0.599744;0.566774;-0.564867;,
  0.109241;0.330694;0.937394;,
  0.967511;0.228648;-0.107896;,
  -0.808944;0.553496;0.198121;,
  -0.495452;0.421405;0.759570;,
  -0.091699;0.348537;0.932799;,
  0.063920;0.994299;0.085344;,
  0.916673;-0.085280;0.390432;,
  -0.454369;-0.342270;-0.822435;,
  0.069958;-0.994911;0.072512;,
  -0.158047;-0.063257;-0.985403;,
  -0.940844;-0.109652;0.320607;,
  -0.933431;-0.129880;0.334422;,
  0.919033;-0.061899;0.389291;,
  0.887321;-0.086253;0.453014;,
  0.994460;-0.048596;-0.093206;,
  0.831698;-0.001181;-0.555226;,
  0.127758;-0.063644;0.989761;,
  0.141940;0.159009;0.977021;,
  0.507756;0.858589;0.070775;,
  0.389784;0.916158;0.093395;,
  0.349400;0.931587;0.100325;,
  0.106034;0.986019;0.128546;,
  -0.040959;0.996737;0.069552;,
  -0.030350;0.466238;-0.884139;,
  0.234920;0.405022;-0.883612;,
  0.142495;0.989734;-0.011047;,
  0.008015;-0.997948;0.063519;,
  0.039005;-0.996919;0.068049;;
  59;
  3;10,3,20;,
  3;5,15,11;,
  3;11,4,5;,
  3;10,20,25;,
  3;4,11,24;,
  3;19,1,16;,
  3;8,21,23;,
  3;8,23,9;,
  3;9,38,26;,
  3;9,26,6;,
  3;2,17,27;,
  3;2,27,13;,
  3;12,10,25;,
  3;12,25,14;,
  3;39,40,21;,
  3;39,21,8;,
  3;0,22,20;,
  3;0,20,3;,
  3;13,27,22;,
  3;13,22,0;,
  3;7,18,41;,
  3;7,41,42;,
  3;7,43,44;,
  3;7,44,18;,
  3;27,17,26;,
  3;20,21,40;,
  3;20,40,25;,
  3;14,11,15;,
  3;14,15,45;,
  3;22,23,21;,
  3;22,21,20;,
  3;14,25,24;,
  3;14,24,11;,
  3;16,41,18;,
  3;16,18,19;,
  3;17,19,18;,
  3;17,18,26;,
  3;22,27,26;,
  3;22,26,38;,
  3;45,46,12;,
  3;45,12,14;,
  3;19,17,2;,
  3;2,1,19;,
  4;32,31,3,10;,
  4;31,28,0,3;,
  3;47,48,49;,
  4;33,32,10,12;,
  3;49,48,50;,
  4;29,33,12,46;,
  3;50,51,34;,
  4;28,52,13,0;,
  3;50,34,49;,
  4;30,30,1,2;,
  4;52,53,2,13;,
  3;51,54,34;,
  3;55,55,56;,
  4;35,35,43,35;,
  4;36,36,36,36;,
  4;37,37,37,56;;
 }
 MeshTextureCoords {
  56;
  0.401410;0.637760;,
  0.376990;0.627150;,
  0.374750;0.677840;,
  0.428300;0.802410;,
  0.434780;0.760580;,
  0.416820;0.760420;,
  0.397390;0.799890;,
  0.406730;0.681040;,
  0.398760;0.757840;,
  0.480720;0.690390;,
  0.467390;0.651840;,
  0.448540;0.685200;,
  0.371580;0.798190;,
  0.373110;0.758270;,
  0.344540;0.758670;,
  0.344450;0.798560;,
  0.328050;0.759130;,
  0.329960;0.799140;,
  0.293910;0.643860;,
  0.299650;0.683590;,
  0.320190;0.680580;,
  0.317400;0.636380;,
  0.430300;0.648090;,
  0.413340;0.642260;,
  0.427260;0.685260;,
  0.339830;0.629850;,
  0.341690;0.679480;,
  0.453440;0.804280;,
  0.460090;0.765180;,
  0.290870;0.802330;,
  0.317170;0.800760;,
  0.312310;0.760740;,
  0.283280;0.763950;,
  0.264960;0.689460;,
  0.452550;0.647600;,
  0.413340;0.642260;,
  0.376990;0.627150;,
  0.339830;0.629850;,
  0.430300;0.648090;,
  0.200710;0.318880;,
  0.200710;0.318880;,
  0.000000;0.000000;,
  0.233700;0.325920;,
  0.215150;0.324050;,
  0.215150;0.324050;,
  0.233700;0.325920;,
  0.317400;0.636380;,
  0.373850;0.642240;,
  0.373850;0.642240;,
  0.380650;0.647850;,
  0.380650;0.647850;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
