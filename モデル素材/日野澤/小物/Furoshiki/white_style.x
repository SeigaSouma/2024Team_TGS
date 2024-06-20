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
 176;
 0.00000;14.40202;-0.09512;,
 3.39224;13.72752;-2.26730;,
 0.00000;13.72752;-3.57903;,
 0.00000;14.40202;-0.09512;,
 4.23005;13.72752;0.68012;,
 0.00000;14.40202;-0.09512;,
 1.88255;13.72752;3.04377;,
 0.00000;14.40202;-0.09512;,
 -1.88255;13.72752;3.04377;,
 0.00000;14.40202;-0.09512;,
 -4.23005;13.72752;0.68012;,
 0.00000;14.40202;-0.09512;,
 -3.39224;13.72752;-2.26730;,
 0.00000;14.40202;-0.09512;,
 0.00000;13.72752;-3.57903;,
 6.11261;10.99979;-4.00925;,
 0.00000;10.99979;-6.37291;,
 7.62229;10.99979;1.30182;,
 3.39224;10.99979;5.56097;,
 -3.39224;10.99979;5.56097;,
 -7.62229;10.99979;1.30182;,
 -6.11260;10.99979;-4.00926;,
 0.00000;10.99979;-6.37291;,
 7.62229;8.26878;-4.97597;,
 0.00000;8.26878;-7.92339;,
 9.50484;8.26878;1.64683;,
 4.23005;8.26878;6.95791;,
 -4.23006;8.26878;6.95791;,
 -9.50485;8.26878;1.64683;,
 -7.62229;8.26878;-4.97597;,
 0.00000;8.26878;-7.92339;,
 7.62229;5.23759;-4.97597;,
 0.00000;5.23759;-7.92339;,
 9.50484;5.23759;1.64683;,
 4.23005;5.23759;6.95791;,
 -4.23006;5.23759;6.95791;,
 -9.50485;5.23759;1.64683;,
 -7.62229;5.23759;-4.97597;,
 0.00000;5.23759;-7.92339;,
 7.20620;2.73597;-4.65391;,
 0.00000;2.73597;-7.44044;,
 8.98598;2.73597;1.60736;,
 3.99914;2.73597;6.62850;,
 -3.99914;2.73597;6.62850;,
 -8.98598;2.73597;1.60735;,
 -7.20620;2.73597;-4.65391;,
 0.00000;2.73597;-7.44044;,
 3.99914;0.50793;-2.60031;,
 0.00000;0.50793;-4.14671;,
 4.98684;0.50793;0.87443;,
 2.21935;0.50793;3.66096;,
 -2.21936;0.50793;3.66096;,
 -4.98684;0.50793;0.87443;,
 -3.99914;0.50793;-2.60031;,
 0.00000;0.50793;-4.14671;,
 0.00000;-0.28725;-0.03951;,
 0.00000;-0.28725;-0.03951;,
 0.00000;-0.28725;-0.03951;,
 0.00000;-0.28725;-0.03951;,
 0.00000;-0.28725;-0.03951;,
 0.00000;-0.28725;-0.03951;,
 0.00000;-0.28725;-0.03951;,
 0.00000;15.88679;-0.44227;,
 0.51746;15.77195;-0.66908;,
 0.00000;15.77195;-0.80605;,
 0.00000;15.88679;-0.44227;,
 0.64527;15.77195;-0.36132;,
 0.00000;15.88679;-0.44227;,
 0.28717;15.77195;-0.11451;,
 0.00000;15.88679;-0.44227;,
 -0.28717;15.77195;-0.11451;,
 0.00000;15.88679;-0.44227;,
 -0.64527;15.77195;-0.36132;,
 0.00000;15.88679;-0.44227;,
 -0.51747;15.77195;-0.66908;,
 0.00000;15.88679;-0.44227;,
 0.00000;15.77195;-0.80605;,
 0.93244;15.30761;-0.85097;,
 0.00000;15.30761;-1.09778;,
 1.16273;15.30761;-0.29640;,
 0.51746;15.30761;0.14833;,
 -0.51747;15.30761;0.14833;,
 -1.16273;15.30761;-0.29640;,
 -0.93244;15.30761;-0.85097;,
 0.00000;15.30761;-1.09778;,
 1.16273;14.84269;-0.95191;,
 0.00000;14.84269;-1.25967;,
 1.44990;14.84269;-0.26038;,
 0.64527;14.84269;0.29419;,
 -0.64527;14.84269;0.29419;,
 -1.44991;14.84269;-0.26038;,
 -1.16273;14.84269;-0.95191;,
 0.00000;14.84269;-1.25967;,
 1.16273;14.32668;-0.95191;,
 0.00000;14.32668;-1.25967;,
 1.44990;14.32668;-0.26038;,
 0.64527;14.32668;0.29419;,
 -0.64527;14.32668;0.29419;,
 -1.44991;14.32668;-0.26038;,
 -1.16273;14.32668;-0.95191;,
 0.00000;14.32668;-1.25967;,
 1.09926;13.90082;-0.91828;,
 0.00000;13.90082;-1.20925;,
 1.37075;13.90082;-0.26450;,
 0.61004;13.90082;0.25980;,
 -0.61004;13.90082;0.25980;,
 -1.37076;13.90082;-0.26450;,
 -1.09926;13.90082;-0.91828;,
 0.00000;13.90082;-1.20925;,
 0.61004;13.52153;-0.70385;,
 0.00000;13.52153;-0.86532;,
 0.76071;13.52153;-0.34103;,
 0.33855;13.52153;-0.05007;,
 -0.33855;13.52153;-0.05007;,
 -0.76071;13.52153;-0.34103;,
 -0.61004;13.52153;-0.70385;,
 0.00000;13.52153;-0.86532;,
 0.00000;13.38616;-0.43646;,
 0.00000;13.38616;-0.43646;,
 0.00000;13.38616;-0.43646;,
 0.00000;13.38616;-0.43646;,
 0.00000;13.38616;-0.43646;,
 0.00000;13.38616;-0.43646;,
 0.00000;13.38616;-0.43646;,
 0.57483;14.69388;-0.99162;,
 3.50949;15.48022;-0.42057;,
 0.74102;14.07366;-0.82437;,
 3.50949;15.48022;-0.42057;,
 0.80985;13.81675;-0.42057;,
 3.50949;15.48022;-0.42057;,
 0.74102;14.07366;-0.01677;,
 3.50949;15.48022;-0.42057;,
 0.57483;14.69388;0.15049;,
 3.50949;15.48022;-0.42057;,
 0.40864;15.31411;-0.01677;,
 3.50949;15.48022;-0.42057;,
 0.33980;15.57102;-0.42057;,
 3.50949;15.48022;-0.42057;,
 0.40864;15.31411;-0.82436;,
 3.50949;15.48022;-0.42057;,
 0.57483;14.69388;-0.99162;,
 0.57483;14.69388;-0.42057;,
 0.57483;14.69388;-0.99162;,
 0.74102;14.07366;-0.82437;,
 0.80985;13.81675;-0.42057;,
 0.74102;14.07366;-0.01677;,
 0.57483;14.69388;0.15049;,
 0.40864;15.31411;-0.01677;,
 0.33980;15.57102;-0.42057;,
 0.40864;15.31411;-0.82436;,
 -0.84782;14.68358;-0.94064;,
 -3.78250;15.46992;-0.36958;,
 -0.68164;15.30381;-0.77338;,
 -3.78250;15.46992;-0.36958;,
 -0.61280;15.56071;-0.36958;,
 -3.78250;15.46992;-0.36958;,
 -0.68164;15.30381;0.03422;,
 -3.78250;15.46992;-0.36958;,
 -0.84782;14.68358;0.20148;,
 -3.78250;15.46992;-0.36958;,
 -1.01402;14.06335;0.03422;,
 -3.78250;15.46992;-0.36958;,
 -1.08285;13.80645;-0.36958;,
 -3.78250;15.46992;-0.36958;,
 -1.01402;14.06335;-0.77338;,
 -3.78250;15.46992;-0.36958;,
 -0.84782;14.68358;-0.94064;,
 -0.84782;14.68358;-0.36958;,
 -0.84782;14.68358;-0.94064;,
 -0.68164;15.30381;-0.77338;,
 -0.61280;15.56071;-0.36958;,
 -0.68164;15.30381;0.03422;,
 -0.84782;14.68358;0.20148;,
 -1.01402;14.06335;0.03422;,
 -1.08285;13.80645;-0.36958;,
 -1.01402;14.06335;-0.77338;;
 
 130;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 4;2,1,15,16;,
 4;1,4,17,15;,
 4;4,6,18,17;,
 4;6,8,19,18;,
 4;8,10,20,19;,
 4;10,12,21,20;,
 4;12,14,22,21;,
 4;16,15,23,24;,
 4;15,17,25,23;,
 4;17,18,26,25;,
 4;18,19,27,26;,
 4;19,20,28,27;,
 4;20,21,29,28;,
 4;21,22,30,29;,
 4;24,23,31,32;,
 4;23,25,33,31;,
 4;25,26,34,33;,
 4;26,27,35,34;,
 4;27,28,36,35;,
 4;28,29,37,36;,
 4;29,30,38,37;,
 4;32,31,39,40;,
 4;31,33,41,39;,
 4;33,34,42,41;,
 4;34,35,43,42;,
 4;35,36,44,43;,
 4;36,37,45,44;,
 4;37,38,46,45;,
 4;40,39,47,48;,
 4;39,41,49,47;,
 4;41,42,50,49;,
 4;42,43,51,50;,
 4;43,44,52,51;,
 4;44,45,53,52;,
 4;45,46,54,53;,
 3;48,47,55;,
 3;47,49,56;,
 3;49,50,57;,
 3;50,51,58;,
 3;51,52,59;,
 3;52,53,60;,
 3;53,54,61;,
 3;62,63,64;,
 3;65,66,63;,
 3;67,68,66;,
 3;69,70,68;,
 3;71,72,70;,
 3;73,74,72;,
 3;75,76,74;,
 4;64,63,77,78;,
 4;63,66,79,77;,
 4;66,68,80,79;,
 4;68,70,81,80;,
 4;70,72,82,81;,
 4;72,74,83,82;,
 4;74,76,84,83;,
 4;78,77,85,86;,
 4;77,79,87,85;,
 4;79,80,88,87;,
 4;80,81,89,88;,
 4;81,82,90,89;,
 4;82,83,91,90;,
 4;83,84,92,91;,
 4;86,85,93,94;,
 4;85,87,95,93;,
 4;87,88,96,95;,
 4;88,89,97,96;,
 4;89,90,98,97;,
 4;90,91,99,98;,
 4;91,92,100,99;,
 4;94,93,101,102;,
 4;93,95,103,101;,
 4;95,96,104,103;,
 4;96,97,105,104;,
 4;97,98,106,105;,
 4;98,99,107,106;,
 4;99,100,108,107;,
 4;102,101,109,110;,
 4;101,103,111,109;,
 4;103,104,112,111;,
 4;104,105,113,112;,
 4;105,106,114,113;,
 4;106,107,115,114;,
 4;107,108,116,115;,
 3;110,109,117;,
 3;109,111,118;,
 3;111,112,119;,
 3;112,113,120;,
 3;113,114,121;,
 3;114,115,122;,
 3;115,116,123;,
 3;124,125,126;,
 3;126,127,128;,
 3;128,129,130;,
 3;130,131,132;,
 3;132,133,134;,
 3;134,135,136;,
 3;136,137,138;,
 3;138,139,140;,
 3;141,142,143;,
 3;141,143,144;,
 3;141,144,145;,
 3;141,145,146;,
 3;141,146,147;,
 3;141,147,148;,
 3;141,148,149;,
 3;141,149,142;,
 3;150,151,152;,
 3;152,153,154;,
 3;154,155,156;,
 3;156,157,158;,
 3;158,159,160;,
 3;160,161,162;,
 3;162,163,164;,
 3;164,165,166;,
 3;167,168,169;,
 3;167,169,170;,
 3;167,170,171;,
 3;167,171,172;,
 3;167,172,173;,
 3;167,173,174;,
 3;167,174,175;,
 3;167,175,168;;
 
 MeshMaterialList {
  1;
  130;
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
    "data\\TEXTURE\\map_object\\white_style.png";
   }
  }
 }
 MeshNormals {
  139;
  0.000000;1.000000;0.000000;,
  0.000000;0.888166;-0.459522;,
  0.302787;0.907534;-0.291035;,
  0.380793;0.918731;0.104544;,
  0.165911;0.894005;0.416207;,
  -0.165911;0.894005;0.416207;,
  -0.380793;0.918731;0.104544;,
  -0.302787;0.907534;-0.291035;,
  0.000000;0.611666;-0.791116;,
  0.547861;0.658701;-0.515715;,
  0.701510;0.687382;0.188121;,
  0.293460;0.625489;0.722942;,
  -0.293459;0.625489;0.722942;,
  -0.701509;0.687383;0.188121;,
  -0.547860;0.658701;-0.515715;,
  0.000000;0.257375;-0.966312;,
  0.704601;0.286956;-0.648994;,
  0.920889;0.306747;0.240563;,
  0.368026;0.265747;0.891030;,
  -0.368026;0.265747;0.891030;,
  -0.920888;0.306748;0.240562;,
  -0.704600;0.286956;-0.648995;,
  0.000000;-0.095322;-0.995447;,
  0.734699;-0.103353;-0.670474;,
  0.957430;-0.096235;-0.272152;,
  0.382740;-0.077857;0.920570;,
  -0.382740;-0.077858;0.920569;,
  -0.707342;-0.078411;0.702509;,
  -0.734699;-0.103354;-0.670474;,
  0.000000;-0.558497;-0.829507;,
  0.588357;-0.594001;-0.548634;,
  0.699504;-0.714584;-0.007990;,
  0.314974;-0.552956;0.771383;,
  -0.314975;-0.552956;0.771382;,
  -0.610332;-0.705107;0.360998;,
  -0.588357;-0.594001;-0.548634;,
  0.000000;-0.924558;-0.381042;,
  0.246529;-0.938964;-0.239937;,
  0.309091;-0.947133;0.086032;,
  0.135779;-0.928946;0.344417;,
  -0.135780;-0.928946;0.344417;,
  -0.309091;-0.947133;0.086031;,
  -0.246529;-0.938964;-0.239937;,
  -0.000000;-1.000000;-0.000000;,
  0.707342;-0.078409;0.702509;,
  -0.957430;-0.096235;-0.272153;,
  0.700802;-0.156336;0.696014;,
  -0.944078;-0.191577;-0.268357;,
  0.000000;1.000000;-0.000003;,
  0.000001;0.793719;-0.608285;,
  0.312308;0.859994;-0.403577;,
  0.402680;0.903813;0.144817;,
  0.162349;0.812106;0.560470;,
  -0.162349;0.812107;0.560469;,
  -0.402680;0.903812;0.144819;,
  -0.312309;0.859995;-0.403574;,
  0.000000;0.433474;-0.901166;,
  0.529808;0.551018;-0.644735;,
  0.729085;0.640846;0.240316;,
  0.255899;0.463652;0.848259;,
  -0.255899;0.463653;0.848258;,
  -0.729083;0.640848;0.240316;,
  -0.529807;0.551019;-0.644734;,
  0.000000;0.167056;-0.985947;,
  0.634416;0.225644;-0.739325;,
  0.895315;0.245318;-0.371793;,
  0.289968;0.181167;0.939732;,
  -0.289967;0.181167;0.939733;,
  -0.556580;0.195136;0.807553;,
  -0.634412;0.225646;-0.739328;,
  0.000000;-0.058908;-0.998263;,
  0.655172;-0.078064;-0.751436;,
  0.920342;-0.083094;-0.382187;,
  0.296557;-0.050644;0.953671;,
  -0.296554;-0.050644;0.953672;,
  -0.566583;-0.056448;0.822069;,
  -0.655167;-0.078063;-0.751440;,
  0.000000;-0.416438;-0.909164;,
  0.548755;-0.508626;-0.663452;,
  0.729947;-0.683313;-0.016168;,
  0.260893;-0.428378;0.865117;,
  -0.260891;-0.428378;0.865118;,
  -0.506431;-0.451228;0.734793;,
  -0.548751;-0.508625;-0.663455;,
  0.000000;-0.843043;-0.537846;,
  0.261653;-0.899420;-0.350117;,
  0.334054;-0.934062;0.126240;,
  0.139350;-0.859356;0.492025;,
  -0.139349;-0.859356;0.492025;,
  -0.334053;-0.934062;0.126239;,
  -0.261652;-0.899420;-0.350118;,
  0.000000;-1.000000;-0.000002;,
  0.556582;0.195136;0.807551;,
  -0.895310;0.245323;-0.371801;,
  0.566587;-0.056448;0.822066;,
  -0.920337;-0.083094;-0.382198;,
  0.563876;-0.112716;0.818131;,
  -0.782558;-0.531029;-0.324979;,
  0.178427;0.047815;-0.982791;,
  0.363587;-0.487792;-0.793639;,
  0.443635;-0.719974;-0.533691;,
  0.363589;-0.487797;0.793635;,
  0.178432;0.047809;0.982790;,
  0.070981;0.604235;0.793638;,
  0.024216;0.845329;0.533697;,
  0.070979;0.604231;-0.793642;,
  0.299203;-0.237792;-0.924084;,
  -0.965926;-0.258819;-0.000000;,
  0.443635;-0.719974;0.533691;,
  0.299207;-0.237797;0.924082;,
  0.024216;0.845323;-0.533707;,
  -0.965925;-0.258821;0.000000;,
  -0.965926;-0.258818;-0.000009;,
  -0.965928;-0.258813;0.000000;,
  -0.965926;-0.258818;0.000009;,
  -0.965925;-0.258821;0.000000;,
  -0.965926;-0.258819;0.000001;,
  -0.965926;-0.258820;-0.000000;,
  -0.965926;-0.258819;-0.000001;,
  -0.178430;0.047811;-0.982790;,
  -0.070980;0.604242;-0.793633;,
  -0.024213;0.845339;-0.533682;,
  -0.070980;0.604242;0.793633;,
  -0.178430;0.047811;0.982790;,
  -0.363591;-0.487801;0.793632;,
  -0.443638;-0.719982;0.533677;,
  -0.363591;-0.487801;-0.793632;,
  -0.140222;0.355544;-0.924081;,
  0.965926;-0.258818;-0.000000;,
  -0.024213;0.845339;0.533682;,
  -0.140222;0.355544;0.924081;,
  -0.443638;-0.719982;-0.533677;,
  0.965926;-0.258819;-0.000000;,
  0.965929;-0.258808;-0.000007;,
  0.965928;-0.258813;-0.000000;,
  0.965929;-0.258808;0.000007;,
  0.965923;-0.258828;0.000009;,
  0.965925;-0.258823;-0.000000;,
  0.965923;-0.258828;-0.000009;;
  130;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,7,6;,
  3;0,1,7;,
  4;1,2,9,8;,
  4;2,3,10,9;,
  4;3,4,11,10;,
  4;4,5,12,11;,
  4;5,6,13,12;,
  4;6,7,14,13;,
  4;7,1,8,14;,
  4;8,9,16,15;,
  4;9,10,17,16;,
  4;10,11,18,17;,
  4;11,12,19,18;,
  4;12,13,20,19;,
  4;13,14,21,20;,
  4;14,8,15,21;,
  4;15,16,23,22;,
  4;16,17,24,23;,
  4;17,18,25,44;,
  4;18,19,26,25;,
  4;19,20,27,26;,
  4;20,21,28,45;,
  4;21,15,22,28;,
  4;22,23,30,29;,
  4;23,24,31,30;,
  4;44,25,32,46;,
  4;25,26,33,32;,
  4;26,27,34,33;,
  4;45,28,35,47;,
  4;28,22,29,35;,
  4;29,30,37,36;,
  4;30,31,38,37;,
  4;31,32,39,38;,
  4;32,33,40,39;,
  4;33,34,41,40;,
  4;34,35,42,41;,
  4;35,29,36,42;,
  3;36,37,43;,
  3;37,38,43;,
  3;38,39,43;,
  3;39,40,43;,
  3;40,41,43;,
  3;41,42,43;,
  3;42,36,43;,
  3;48,50,49;,
  3;48,51,50;,
  3;48,52,51;,
  3;48,53,52;,
  3;48,54,53;,
  3;48,55,54;,
  3;48,49,55;,
  4;49,50,57,56;,
  4;50,51,58,57;,
  4;51,52,59,58;,
  4;52,53,60,59;,
  4;53,54,61,60;,
  4;54,55,62,61;,
  4;55,49,56,62;,
  4;56,57,64,63;,
  4;57,58,65,64;,
  4;58,59,66,92;,
  4;59,60,67,66;,
  4;60,61,68,67;,
  4;61,62,69,93;,
  4;62,56,63,69;,
  4;63,64,71,70;,
  4;64,65,72,71;,
  4;92,66,73,94;,
  4;66,67,74,73;,
  4;67,68,75,74;,
  4;93,69,76,95;,
  4;69,63,70,76;,
  4;70,71,78,77;,
  4;71,72,79,78;,
  4;94,73,80,96;,
  4;73,74,81,80;,
  4;74,75,82,81;,
  4;95,76,83,97;,
  4;76,70,77,83;,
  4;77,78,85,84;,
  4;78,79,86,85;,
  4;79,80,87,86;,
  4;80,81,88,87;,
  4;81,82,89,88;,
  4;97,83,90,89;,
  4;83,77,84,90;,
  3;84,85,91;,
  3;85,86,91;,
  3;86,87,91;,
  3;87,88,91;,
  3;88,89,91;,
  3;89,90,91;,
  3;90,84,91;,
  3;98,106,99;,
  3;99,106,100;,
  3;108,109,101;,
  3;101,109,102;,
  3;102,109,103;,
  3;103,104,104;,
  3;110,110,105;,
  3;105,106,98;,
  3;107,111,112;,
  3;107,112,113;,
  3;107,113,114;,
  3;107,114,115;,
  3;107,115,116;,
  3;107,116,117;,
  3;107,117,118;,
  3;107,118,111;,
  3;119,127,120;,
  3;120,127,121;,
  3;129,130,122;,
  3;122,130,123;,
  3;123,130,124;,
  3;124,125,125;,
  3;131,131,126;,
  3;126,127,119;,
  3;128,132,133;,
  3;128,133,134;,
  3;128,134,135;,
  3;128,135,132;,
  3;128,132,136;,
  3;128,136,137;,
  3;128,137,138;,
  3;128,138,132;;
 }
 MeshTextureCoords {
  176;
  0.071430;0.000000;,
  0.142860;0.142860;,
  0.000000;0.142860;,
  0.214290;0.000000;,
  0.285710;0.142860;,
  0.357140;0.000000;,
  0.428570;0.142860;,
  0.500000;0.000000;,
  0.571430;0.142860;,
  0.642860;0.000000;,
  0.714290;0.142860;,
  0.785710;0.000000;,
  0.857140;0.142860;,
  0.928570;0.000000;,
  1.000000;0.142860;,
  0.142860;0.285710;,
  0.000000;0.285710;,
  0.285710;0.285710;,
  0.428570;0.285710;,
  0.571430;0.285710;,
  0.714290;0.285710;,
  0.857140;0.285710;,
  1.000000;0.285710;,
  0.142860;0.428570;,
  0.000000;0.428570;,
  0.285710;0.428570;,
  0.428570;0.428570;,
  0.571430;0.428570;,
  0.714290;0.428570;,
  0.857140;0.428570;,
  1.000000;0.428570;,
  0.142860;0.571430;,
  0.000000;0.571430;,
  0.285710;0.571430;,
  0.428570;0.571430;,
  0.571430;0.571430;,
  0.714290;0.571430;,
  0.857140;0.571430;,
  1.000000;0.571430;,
  0.142860;0.714290;,
  0.000000;0.714290;,
  0.285710;0.714290;,
  0.428570;0.714290;,
  0.571430;0.714290;,
  0.714290;0.714290;,
  0.857140;0.714290;,
  1.000000;0.714290;,
  0.142860;0.857140;,
  0.000000;0.857140;,
  0.285710;0.857140;,
  0.428570;0.857140;,
  0.571430;0.857140;,
  0.714290;0.857140;,
  0.857140;0.857140;,
  1.000000;0.857140;,
  0.071430;1.000000;,
  0.214290;1.000000;,
  0.357140;1.000000;,
  0.500000;1.000000;,
  0.642860;1.000000;,
  0.785710;1.000000;,
  0.928570;1.000000;,
  0.071430;0.000000;,
  0.142860;0.142860;,
  0.000000;0.142860;,
  0.214290;0.000000;,
  0.285710;0.142860;,
  0.357140;0.000000;,
  0.428570;0.142860;,
  0.500000;0.000000;,
  0.571430;0.142860;,
  0.642860;0.000000;,
  0.714290;0.142860;,
  0.785710;0.000000;,
  0.857140;0.142860;,
  0.928570;0.000000;,
  1.000000;0.142860;,
  0.142860;0.285710;,
  0.000000;0.285710;,
  0.285710;0.285710;,
  0.428570;0.285710;,
  0.571430;0.285710;,
  0.714290;0.285710;,
  0.857140;0.285710;,
  1.000000;0.285710;,
  0.142860;0.428570;,
  0.000000;0.428570;,
  0.285710;0.428570;,
  0.428570;0.428570;,
  0.571430;0.428570;,
  0.714290;0.428570;,
  0.857140;0.428570;,
  1.000000;0.428570;,
  0.142860;0.571430;,
  0.000000;0.571430;,
  0.285710;0.571430;,
  0.428570;0.571430;,
  0.571430;0.571430;,
  0.714290;0.571430;,
  0.857140;0.571430;,
  1.000000;0.571430;,
  0.142860;0.714290;,
  0.000000;0.714290;,
  0.285710;0.714290;,
  0.428570;0.714290;,
  0.571430;0.714290;,
  0.714290;0.714290;,
  0.857140;0.714290;,
  1.000000;0.714290;,
  0.142860;0.857140;,
  0.000000;0.857140;,
  0.285710;0.857140;,
  0.428570;0.857140;,
  0.571430;0.857140;,
  0.714290;0.857140;,
  0.857140;0.857140;,
  1.000000;0.857140;,
  0.071430;1.000000;,
  0.214290;1.000000;,
  0.357140;1.000000;,
  0.500000;1.000000;,
  0.642860;1.000000;,
  0.785710;1.000000;,
  0.928570;1.000000;,
  0.000000;1.000000;,
  0.062500;0.000000;,
  0.125000;1.000000;,
  0.125000;0.000000;,
  0.250000;1.000000;,
  0.187500;0.000000;,
  0.375000;1.000000;,
  0.250000;0.000000;,
  0.500000;1.000000;,
  0.312500;0.000000;,
  0.625000;1.000000;,
  0.375000;0.000000;,
  0.750000;1.000000;,
  0.437500;0.000000;,
  0.875000;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.062500;0.000000;,
  0.125000;1.000000;,
  0.125000;0.000000;,
  0.250000;1.000000;,
  0.187500;0.000000;,
  0.375000;1.000000;,
  0.250000;0.000000;,
  0.500000;1.000000;,
  0.312500;0.000000;,
  0.625000;1.000000;,
  0.375000;0.000000;,
  0.750000;1.000000;,
  0.437500;0.000000;,
  0.875000;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
