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
 109;
 1.84740;-3.10871;3.73619;,
 -3.25132;-3.14931;3.76646;,
 -2.13694;-8.78870;5.39534;,
 2.65479;-8.63683;5.20368;,
 0.16314;-42.80762;-14.54861;,
 2.60341;-44.45211;-15.93771;,
 -2.25295;-45.01251;-15.90494;,
 -5.29353;-8.58364;0.63623;,
 -2.13694;-8.78870;5.39534;,
 -4.63075;-2.50814;-0.27058;,
 2.73218;-37.39876;0.33306;,
 -0.50556;-35.89638;-2.79710;,
 2.06348;-24.65367;-2.72695;,
 3.62413;-24.78387;1.00143;,
 -2.88636;-37.33243;0.36960;,
 -0.56927;-38.80995;-4.01264;,
 -3.64639;-43.12215;-3.61349;,
 -3.83855;-42.23385;0.44649;,
 -3.87457;-24.76151;1.00526;,
 -2.88636;-37.33243;0.36960;,
 0.29308;-37.38558;3.23696;,
 -1.52537;-24.99103;5.02629;,
 0.57480;-8.43886;-4.28736;,
 0.61449;-2.73698;-5.39932;,
 3.58427;-2.40681;-0.28218;,
 4.23246;-8.50765;0.62655;,
 4.37841;-14.12241;1.01710;,
 2.08150;-14.01912;-3.35312;,
 0.29308;-37.38558;3.23696;,
 2.79873;-24.93372;4.65234;,
 -1.52537;-24.99103;5.02629;,
 -5.41063;-14.15054;1.01937;,
 -2.26521;-14.06536;-3.62892;,
 -1.71969;-24.67168;-2.95905;,
 2.23270;-44.77221;-3.90130;,
 2.81350;-43.72913;-9.22229;,
 3.08154;-41.29099;-4.08020;,
 -0.39089;-14.42381;6.26854;,
 -3.54089;-14.22147;5.61844;,
 4.37841;-14.12241;1.01710;,
 3.00606;-14.21696;5.67355;,
 3.62413;-24.78387;1.00143;,
 -2.91614;-2.73014;-4.78409;,
 -3.45606;-8.45997;-3.76394;,
 -0.50556;-35.89638;-2.79710;,
 -0.50556;-35.89638;-2.79710;,
 0.26214;-45.28127;3.69107;,
 0.33586;-42.09892;4.27687;,
 -3.83855;-42.23385;0.44649;,
 -2.66944;-45.54383;0.35408;,
 -2.53267;-44.65588;-8.76900;,
 2.81350;-43.72913;-9.22229;,
 2.23270;-44.77221;-3.90130;,
 -1.11296;-45.66862;-3.75186;,
 -1.11296;-45.66862;-3.75186;,
 0.95769;-45.67275;0.31805;,
 -2.66944;-45.54383;0.35408;,
 -3.64639;-43.12215;-3.61349;,
 -2.53267;-44.65588;-8.76900;,
 -0.56385;-41.16911;-9.44621;,
 -0.56927;-38.80995;-4.01264;,
 0.29308;-37.38558;3.23696;,
 0.33586;-42.09892;4.27687;,
 2.60341;-44.45211;-15.93771;,
 0.16314;-42.80762;-14.54861;,
 0.26214;-45.28127;3.69107;,
 0.95769;-45.67275;0.31805;,
 3.15193;-42.11122;0.26084;,
 0.33586;-42.09892;4.27687;,
 2.23270;-44.77221;-3.90130;,
 3.15193;-42.11122;0.26084;,
 3.15193;-42.11122;0.26084;,
 2.73218;-37.39876;0.33306;,
 0.29308;-37.38558;3.23696;,
 0.33586;-42.09892;4.27687;,
 -0.50556;-35.89638;-2.79710;,
 -0.50556;-35.89638;-2.79710;,
 -1.71969;-24.67168;-2.95905;,
 1.23941;-44.66987;-20.25122;,
 -2.25295;-45.01251;-15.90494;,
 2.60341;-44.45211;-15.93771;,
 0.26214;-45.28127;3.69107;,
 -3.64639;-43.12215;-3.61349;,
 1.23941;-44.66987;-20.25122;,
 2.60341;-44.45211;-15.93771;,
 -2.25295;-45.01251;-15.90494;,
 -0.56385;-41.16911;-9.44621;,
 -2.53267;-44.65588;-8.76900;,
 2.73218;-37.39876;0.33306;,
 0.57480;-8.43886;-4.28736;,
 0.61449;-2.73698;-5.39932;,
 -2.26521;-14.06536;-3.62892;,
 0.16314;-42.80762;-14.54861;,
 -2.25295;-45.01251;-15.90494;,
 -2.25295;-45.01251;-15.90494;,
 2.60341;-44.45211;-15.93771;,
 4.23246;-8.50765;0.62655;,
 3.58427;-2.40681;-0.28218;,
 -1.53814;1.77946;-3.39214;,
 0.49641;1.74923;-3.75527;,
 -2.71764;1.93215;-0.28718;,
 -1.89074;1.49411;2.51263;,
 1.99493;1.91931;-0.15992;,
 1.14742;1.48069;2.55943;,
 1.84740;-3.10871;3.73619;,
 0.49641;1.74923;-3.75527;,
 0.61449;-2.73698;-5.39932;,
 -1.89074;1.49411;2.51263;,
 -3.25132;-3.14931;3.76646;;
 
 95;
 3;0,1,2;,
 3;2,3,0;,
 3;4,5,6;,
 3;7,8,1;,
 3;7,1,9;,
 3;10,11,12;,
 3;10,12,13;,
 3;14,15,16;,
 3;14,16,17;,
 3;18,19,20;,
 3;18,20,21;,
 3;22,23,24;,
 3;22,24,25;,
 3;26,13,12;,
 3;26,12,27;,
 3;28,29,30;,
 3;31,32,33;,
 3;31,33,18;,
 3;34,35,36;,
 3;37,38,21;,
 3;39,40,29;,
 3;39,29,41;,
 3;9,42,43;,
 3;9,43,7;,
 3;19,18,33;,
 3;19,33,44;,
 3;14,45,15;,
 3;46,47,48;,
 3;46,48,49;,
 3;50,51,52;,
 3;50,52,53;,
 3;54,55,56;,
 3;54,57,58;,
 3;55,53,52;,
 3;59,60,36;,
 3;59,36,35;,
 3;61,14,17;,
 3;61,17,62;,
 3;63,64,59;,
 3;63,59,35;,
 3;65,66,67;,
 3;65,67,68;,
 3;67,66,69;,
 3;36,70,34;,
 3;71,72,73;,
 3;71,73,74;,
 3;72,60,75;,
 3;12,76,77;,
 3;78,79,80;,
 3;56,55,81;,
 3;49,48,82;,
 3;83,84,85;,
 3;16,15,86;,
 3;16,86,87;,
 3;72,71,36;,
 3;72,36,60;,
 3;56,57,54;,
 3;28,88,41;,
 3;28,41,29;,
 3;29,40,37;,
 3;29,37,30;,
 3;38,31,18;,
 3;38,18,21;,
 3;89,43,42;,
 3;89,42,90;,
 3;77,91,27;,
 3;77,27,12;,
 3;86,92,93;,
 3;86,93,87;,
 3;94,95,51;,
 3;94,51,50;,
 3;40,3,2;,
 3;40,2,37;,
 3;38,37,2;,
 3;39,96,3;,
 3;39,3,40;,
 3;31,38,8;,
 3;31,8,7;,
 3;91,43,89;,
 3;32,31,7;,
 3;32,7,43;,
 3;27,22,25;,
 3;27,25,26;,
 3;91,22,27;,
 3;0,3,96;,
 3;0,96,97;,
 4;98,99,23,42;,
 4;100,98,42,9;,
 4;101,100,9,1;,
 4;99,98,100,101;,
 4;102,103,104,97;,
 4;105,102,97,106;,
 3;105,103,102;,
 4;103,107,108,104;,
 3;107,103,99;;
 
 MeshMaterialList {
  9;
  95;
  0,
  0,
  4,
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
  4,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  0,
  0,
  4,
  4,
  4,
  4,
  4,
  4,
  0,
  0,
  0,
  0,
  4,
  4,
  4,
  4,
  4,
  4,
  0,
  0,
  4,
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
  4,
  4,
  4,
  4,
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
   0.000000;0.800000;0.677647;1.000000;;
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
  Material {
   0.800000;0.291765;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  90;
  0.834500;0.185421;-0.518872;,
  -0.620928;0.245601;0.744398;,
  0.009581;0.263967;0.964484;,
  0.976293;0.135953;-0.168431;,
  0.248932;0.912995;-0.323223;,
  -0.458544;0.817282;-0.348981;,
  -0.693734;-0.195452;0.693204;,
  -0.794552;0.421758;-0.436815;,
  -0.054501;0.853444;-0.518327;,
  -0.600999;-0.026241;0.798819;,
  -0.114715;0.993302;-0.013870;,
  -0.676296;0.017403;0.736425;,
  -0.866684;-0.472566;0.159810;,
  0.421260;-0.903020;0.084235;,
  0.930635;-0.171147;0.323461;,
  -0.856423;-0.044383;0.514363;,
  -0.957019;0.116633;-0.265541;,
  0.941016;-0.334856;0.048579;,
  -0.214020;-0.968746;-0.125407;,
  0.991542;-0.129787;-0.000064;,
  -0.870492;0.155134;-0.467095;,
  0.117352;-0.117484;0.986117;,
  0.659119;-0.065716;-0.749162;,
  0.992377;-0.105505;0.063688;,
  -0.834815;-0.153450;0.528712;,
  -0.839666;-0.085479;-0.536334;,
  -0.519985;-0.170847;0.836915;,
  -0.974594;0.205723;-0.088571;,
  -0.912510;0.200561;-0.356511;,
  -0.038119;-0.995410;-0.087780;,
  0.970483;-0.233548;0.060149;,
  0.546165;0.796543;-0.259275;,
  0.518721;0.034887;-0.854231;,
  0.875446;0.084460;-0.475880;,
  0.999398;-0.013196;-0.032092;,
  0.988813;0.099476;-0.111147;,
  0.958923;0.019350;0.283007;,
  0.033848;0.174065;0.984152;,
  -0.364744;0.174777;0.914557;,
  -0.694787;-0.016919;0.719016;,
  -0.999726;0.004560;0.022962;,
  -0.998229;-0.010714;-0.058514;,
  0.003942;-0.005168;0.999979;,
  0.521815;-0.035656;-0.852313;,
  0.686860;-0.037457;-0.725824;,
  -0.492870;-0.107717;-0.863410;,
  -0.925347;-0.044578;-0.376491;,
  -0.869771;0.363039;0.334218;,
  0.883764;0.279855;0.375022;,
  0.953190;0.300234;-0.035886;,
  -0.920861;0.389843;-0.006129;,
  -0.574821;0.396657;-0.715712;,
  -0.160867;0.335790;-0.928099;,
  -0.669274;0.025300;0.742585;,
  0.087138;-0.129805;0.987703;,
  -0.874589;-0.085158;-0.477328;,
  0.965445;-0.110420;0.236057;,
  -0.812489;0.071052;-0.578631;,
  0.149050;-0.988817;-0.005073;,
  0.215382;-0.958469;-0.186943;,
  -0.034828;-0.997426;0.062676;,
  -0.706190;-0.705693;0.057385;,
  0.595451;0.597383;-0.537188;,
  0.522222;0.736311;-0.430268;,
  0.596296;0.765537;-0.241630;,
  0.849682;-0.326690;0.413901;,
  0.818765;0.004598;0.574110;,
  0.765610;-0.014846;0.643134;,
  0.847611;-0.028908;0.529830;,
  0.062194;-0.042072;-0.997177;,
  -0.034237;-0.993534;0.108251;,
  0.114715;-0.993302;0.013870;,
  0.114827;-0.991668;0.058386;,
  0.135000;-0.989169;0.057611;,
  -0.783857;0.572936;-0.239403;,
  -0.870650;0.455590;-0.185490;,
  0.125134;0.063285;0.990119;,
  -0.141464;-0.133930;-0.980842;,
  -0.127484;-0.106899;-0.986063;,
  -0.156707;-0.004838;-0.987633;,
  -0.170912;0.076825;-0.982287;,
  0.942755;0.117533;0.312089;,
  0.901966;0.219498;0.371858;,
  -0.061822;0.996969;0.047238;,
  0.087700;0.995732;0.028759;,
  0.045775;0.998322;0.035462;,
  0.856685;0.268347;-0.440547;,
  -0.136808;0.989004;0.056164;,
  -0.274638;0.959062;0.069100;,
  -0.005087;0.253042;0.967442;;
  95;
  3;2,1,38;,
  3;38,37,2;,
  3;8,4,5;,
  3;40,38,1;,
  3;40,1,27;,
  3;0,32,22;,
  3;0,22,23;,
  3;20,7,16;,
  3;20,16,15;,
  3;24,53,9;,
  3;24,9,26;,
  3;44,33,3;,
  3;44,3,35;,
  3;34,23,22;,
  3;34,22,43;,
  3;9,21,54;,
  3;41,45,25;,
  3;41,25,55;,
  3;17,30,19;,
  3;42,39,26;,
  3;34,36,56;,
  3;34,56,23;,
  3;27,28,46;,
  3;27,46,40;,
  3;20,55,25;,
  3;20,25,57;,
  3;20,57,7;,
  3;6,11,15;,
  3;6,15,12;,
  3;29,58,59;,
  3;29,59,18;,
  3;18,13,60;,
  3;18,61,29;,
  3;13,18,59;,
  3;31,62,63;,
  3;31,63,64;,
  3;9,53,15;,
  3;9,15,11;,
  3;4,8,31;,
  3;4,31,64;,
  3;65,13,14;,
  3;65,14,66;,
  3;14,13,17;,
  3;19,14,17;,
  3;14,67,68;,
  3;14,68,66;,
  3;0,62,32;,
  3;22,32,69;,
  3;10,5,4;,
  3;60,13,70;,
  3;12,15,16;,
  3;71,72,73;,
  3;16,7,74;,
  3;16,74,75;,
  3;0,14,19;,
  3;0,63,62;,
  3;12,16,18;,
  3;68,67,23;,
  3;68,23,56;,
  3;21,76,42;,
  3;21,42,26;,
  3;39,41,24;,
  3;39,24,26;,
  3;77,78,79;,
  3;77,79,80;,
  3;69,45,43;,
  3;69,43,22;,
  3;74,8,5;,
  3;74,5,75;,
  3;73,72,58;,
  3;73,58,29;,
  3;76,37,38;,
  3;76,38,42;,
  3;39,42,38;,
  3;34,35,81;,
  3;34,81,36;,
  3;41,39,38;,
  3;41,38,40;,
  3;45,78,77;,
  3;45,41,40;,
  3;45,40,46;,
  3;43,44,35;,
  3;43,35,34;,
  3;45,44,43;,
  3;82,81,35;,
  3;82,35,3;,
  4;51,52,80,79;,
  4;50,51,28,27;,
  4;47,50,27,1;,
  4;83,84,84,85;,
  4;49,48,82,3;,
  4;86,49,3,33;,
  3;83,87,88;,
  4;89,89,1,2;,
  3;85,87,83;;
 }
 MeshTextureCoords {
  109;
  0.425860;0.827530;,
  0.396340;0.825220;,
  0.408740;0.857500;,
  0.425080;0.857970;,
  0.082790;0.033240;,
  0.086030;0.018870;,
  0.075860;0.020030;,
  0.372270;0.851620;,
  0.395130;0.855570;,
  0.372570;0.823960;,
  0.322440;0.991910;,
  0.338510;0.988480;,
  0.326860;0.932510;,
  0.306750;0.936910;,
  0.052120;0.140020;,
  0.071880;0.117390;,
  0.049920;0.105920;,
  0.038650;0.130290;,
  0.370730;0.928690;,
  0.371350;0.984310;,
  0.385580;0.984290;,
  0.388720;0.929280;,
  0.318880;0.849430;,
  0.318240;0.824330;,
  0.293020;0.826880;,
  0.292750;0.854210;,
  0.295380;0.883390;,
  0.320330;0.878730;,
  0.402160;0.987160;,
  0.416650;0.933530;,
  0.402330;0.931730;,
  0.371560;0.877980;,
  0.350000;0.876690;,
  0.351850;0.929220;,
  0.131370;0.095250;,
  0.111500;0.065540;,
  0.119320;0.101960;,
  0.407690;0.883240;,
  0.393560;0.880580;,
  0.447590;0.885610;,
  0.423320;0.883910;,
  0.434460;0.938410;,
  0.346070;0.822660;,
  0.348450;0.848570;,
  0.355220;0.985740;,
  0.079800;0.137500;,
  0.544510;0.613610;,
  0.548750;0.631350;,
  0.571850;0.601590;,
  0.556030;0.594040;,
  0.536360;0.514150;,
  0.516160;0.512700;,
  0.518810;0.558670;,
  0.533230;0.558970;,
  0.545590;0.559930;,
  0.532210;0.591800;,
  0.543840;0.592120;,
  0.560940;0.561800;,
  0.563570;0.516720;,
  0.095870;0.069290;,
  0.101190;0.112510;,
  0.040580;0.158290;,
  0.025710;0.152250;,
  0.103250;0.032460;,
  0.092860;0.032680;,
  0.519310;0.613450;,
  0.508710;0.592240;,
  0.490790;0.598760;,
  0.513100;0.630350;,
  0.501700;0.558950;,
  0.145920;0.117180;,
  0.136250;0.125190;,
  0.125350;0.134560;,
  0.138690;0.150920;,
  0.152160;0.143910;,
  0.098490;0.135320;,
  0.346590;0.987160;,
  0.340710;0.930650;,
  0.660050;0.030960;,
  0.651770;0.042460;,
  0.662590;0.044590;,
  0.532010;0.614890;,
  0.576350;0.563810;,
  0.663960;0.073490;,
  0.663280;0.083250;,
  0.672150;0.082650;,
  0.073170;0.070330;,
  0.057390;0.070640;,
  0.415100;0.991910;,
  0.333620;0.848350;,
  0.331280;0.822570;,
  0.335570;0.877290;,
  0.072900;0.034180;,
  0.063090;0.035790;,
  0.541070;0.460630;,
  0.511200;0.459530;,
  0.449010;0.855800;,
  0.449050;0.828450;,
  0.346070;0.822660;,
  0.318240;0.824330;,
  0.372570;0.823960;,
  0.396340;0.825220;,
  0.449050;0.828450;,
  0.212930;0.413760;,
  0.212930;0.413760;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.198170;0.412610;,
  0.198170;0.412610;;
 }
}
