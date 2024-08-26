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
 207;
 23.21368;-41.70803;104.21691;,
 -29.20456;-13.44980;83.16063;,
 -21.35042;-52.34334;96.42612;,
 -14.36204;-66.22539;-25.57275;,
 -39.37983;-65.38259;14.15442;,
 -44.11764;-20.32359;-11.17802;,
 -8.21121;-27.87491;-60.52599;,
 -44.11764;-20.32359;-11.17802;,
 -39.37983;-65.38259;14.15442;,
 -44.75086;-29.08290;44.90878;,
 -14.36204;-66.22539;-25.57275;,
 -8.21121;-27.87491;-60.52599;,
 23.80994;-46.65696;-36.16200;,
 49.35273;-21.75124;-14.20524;,
 43.43052;-67.37668;18.21078;,
 23.80994;-46.65696;-36.16200;,
 69.40283;-11.08919;31.92504;,
 50.13015;-44.19814;53.43142;,
 43.43052;-67.37668;18.21078;,
 49.35273;-21.75124;-14.20524;,
 15.67132;-111.22580;123.25831;,
 -36.12147;-85.45096;71.87719;,
 -30.45945;-134.37081;95.08394;,
 15.67132;-111.22580;123.25831;,
 -17.45793;-158.09444;144.19307;,
 19.63171;-165.29674;163.16027;,
 -36.12147;-85.45096;71.87719;,
 -16.55535;-122.85035;34.16112;,
 -30.45945;-134.37081;95.08394;,
 -11.85800;-159.13695;67.11376;,
 -30.45945;-134.37081;95.08394;,
 -9.28095;-199.74269;106.26168;,
 -30.45945;-134.37081;95.08394;,
 -11.85800;-159.13695;67.11376;,
 9.10182;-91.23583;-1.13204;,
 34.27669;-128.80910;58.83339;,
 -16.55535;-122.85035;34.16112;,
 -11.85800;-159.13695;67.11376;,
 -16.55535;-122.85035;34.16112;,
 34.27669;-128.80910;58.83339;,
 -9.28095;-199.74269;106.26168;,
 -11.85800;-159.13695;67.11376;,
 35.93051;-171.52456;116.86871;,
 -11.85800;-159.13695;67.11376;,
 34.27669;-128.80910;58.83339;,
 35.93051;-171.52456;116.86871;,
 15.67132;-111.22580;123.25831;,
 35.93051;-171.52456;116.86871;,
 34.27669;-128.80910;58.83339;,
 42.96978;-88.64660;82.48611;,
 15.67132;-111.22580;123.25831;,
 34.27669;-128.80910;58.83339;,
 35.93051;-171.52456;116.86871;,
 15.67132;-111.22580;123.25831;,
 19.63171;-165.29674;163.16027;,
 -9.28095;-199.74269;106.26168;,
 -17.45793;-158.09444;144.19307;,
 -30.45945;-134.37081;95.08394;,
 15.67132;-111.22580;123.25831;,
 -30.45945;-134.37081;95.08394;,
 -17.45793;-158.09444;144.19307;,
 -44.75086;-29.08290;44.90878;,
 -21.35042;-52.34334;96.42612;,
 -29.20456;-13.44980;83.16063;,
 -39.37983;-65.38259;14.15442;,
 -14.36204;-66.22539;-25.57275;,
 9.10182;-91.23583;-1.13204;,
 -16.55535;-122.85035;34.16112;,
 -39.37983;-65.38259;14.15442;,
 9.10182;-91.23583;-1.13204;,
 -16.55535;-122.85035;34.16112;,
 -36.12147;-85.45096;71.87719;,
 -39.37983;-65.38259;14.15442;,
 -21.35042;-52.34334;96.42612;,
 -44.75086;-29.08290;44.90878;,
 -36.12147;-85.45096;71.87719;,
 -39.37983;-65.38259;14.15442;,
 -36.12147;-85.45096;71.87719;,
 -44.75086;-29.08290;44.90878;,
 -36.12147;-85.45096;71.87719;,
 15.67132;-111.22580;123.25831;,
 -21.35042;-52.34334;96.42612;,
 15.67132;-111.22580;123.25831;,
 23.21368;-41.70803;104.21691;,
 -21.35042;-52.34334;96.42612;,
 23.21368;-41.70803;104.21691;,
 42.96978;-88.64660;82.48611;,
 50.13015;-44.19814;53.43142;,
 42.96978;-88.64660;82.48611;,
 23.21368;-41.70803;104.21691;,
 15.67132;-111.22580;123.25831;,
 50.13015;-44.19814;53.43142;,
 42.96978;-88.64660;82.48611;,
 43.43052;-67.37668;18.21078;,
 42.96978;-88.64660;82.48611;,
 34.27669;-128.80910;58.83339;,
 43.43052;-67.37668;18.21078;,
 43.43052;-67.37668;18.21078;,
 9.10182;-91.23583;-1.13204;,
 23.80994;-46.65696;-36.16200;,
 9.10182;-91.23583;-1.13204;,
 43.43052;-67.37668;18.21078;,
 34.27669;-128.80910;58.83339;,
 23.80994;-46.65696;-36.16200;,
 9.10182;-91.23583;-1.13204;,
 -14.36204;-66.22539;-25.57275;,
 -9.28095;-199.74269;106.26168;,
 -10.87457;-210.77959;165.50184;,
 -17.45793;-158.09444;144.19307;,
 -9.28095;-199.74269;106.26168;,
 -14.04424;-245.64072;138.50312;,
 -10.87457;-210.77959;165.50184;,
 19.63171;-165.29674;163.16027;,
 -17.45793;-158.09444;144.19307;,
 -10.87457;-210.77959;165.50184;,
 19.63171;-165.29674;163.16027;,
 -10.87457;-210.77959;165.50184;,
 27.63866;-222.74223;139.37966;,
 19.63171;-165.29674;163.16027;,
 27.63866;-222.74223;139.37966;,
 35.93051;-171.52456;116.86871;,
 35.93051;-171.52456;116.86871;,
 27.63866;-222.74223;139.37966;,
 -9.28095;-199.74269;106.26168;,
 27.63866;-222.74223;139.37966;,
 -14.04424;-245.64072;138.50312;,
 -9.28095;-199.74269;106.26168;,
 -21.38976;-410.70167;178.25290;,
 10.74255;-445.16633;180.99108;,
 22.32734;-398.87344;176.45397;,
 4.20889;-360.77454;184.88077;,
 -21.38976;-410.70167;178.25290;,
 22.32734;-398.87344;176.45397;,
 4.20889;-360.77454;184.88077;,
 -7.97034;-309.24649;156.36565;,
 -9.32344;-385.22191;141.11461;,
 -7.97034;-309.24649;156.36565;,
 4.20889;-360.77454;184.88077;,
 -9.32344;-385.22191;141.11461;,
 -21.38976;-410.70167;178.25290;,
 -9.32344;-385.22191;141.11461;,
 -18.90175;-454.79222;115.06586;,
 20.51280;-433.52139;139.40710;,
 -9.32344;-385.22191;141.11461;,
 22.32734;-398.87344;176.45397;,
 -9.32344;-385.22191;141.11461;,
 4.20889;-360.77454;184.88077;,
 22.32734;-398.87344;176.45397;,
 -14.04424;-245.64072;138.50312;,
 10.16155;-274.92604;165.69448;,
 -10.87457;-210.77959;165.50184;,
 -7.97034;-309.24649;156.36565;,
 -14.04424;-245.64072;138.50312;,
 10.16155;-274.92604;165.69448;,
 10.16155;-274.92604;165.69448;,
 -14.04424;-245.64072;138.50312;,
 27.63866;-222.74223;139.37966;,
 27.63866;-222.74223;139.37966;,
 -10.87457;-210.77959;165.50184;,
 10.16155;-274.92604;165.69448;,
 -21.38976;-410.70167;178.25290;,
 4.20889;-360.77454;184.88077;,
 -9.32344;-385.22191;141.11461;,
 10.16155;-274.92604;165.69448;,
 -14.04424;-245.64072;138.50312;,
 -7.97034;-309.24649;156.36565;,
 -18.90175;-454.79222;115.06586;,
 -29.94530;-460.47677;170.01656;,
 -21.38976;-410.70167;178.25290;,
 -29.94530;-460.47677;170.01656;,
 -18.90175;-454.79222;115.06586;,
 8.73761;-473.42190;151.56253;,
 -21.38976;-410.70167;178.25290;,
 -29.94530;-460.47677;170.01656;,
 10.74255;-445.16633;180.99108;,
 10.74255;-445.16633;180.99108;,
 -29.94530;-460.47677;170.01656;,
 8.73761;-473.42190;151.56253;,
 10.74255;-445.16633;180.99108;,
 20.51280;-433.52139;139.40710;,
 22.32734;-398.87344;176.45397;,
 8.73761;-473.42190;151.56253;,
 20.51280;-433.52139;139.40710;,
 10.74255;-445.16633;180.99108;,
 -18.90175;-454.79222;115.06586;,
 20.51280;-433.52139;139.40710;,
 8.73761;-473.42190;151.56253;,
 -9.32344;-385.22191;141.11461;,
 20.51280;-433.52139;139.40710;,
 -18.90175;-454.79222;115.06586;,
 23.21368;-41.70803;104.21691;,
 50.13015;-44.19814;53.43142;,
 69.40283;-11.08919;31.92504;,
 -29.20456;-13.44980;83.16063;,
 23.21368;-41.70803;104.21691;,
 -8.21121;-27.87491;-60.52599;,
 49.35273;-21.75124;-14.20524;,
 -21.93163;35.89257;27.24855;,
 -27.05129;29.10530;15.90393;,
 -44.75086;-29.08290;44.90878;,
 29.09760;37.91529;4.03172;,
 -21.61110;34.57125;-8.66460;,
 24.02645;33.63360;-12.33435;,
 24.02645;33.63360;-12.33435;,
 -27.05129;29.10530;15.90393;,
 -3.30995;30.71705;-40.12967;,
 -21.61110;34.57125;-8.66460;;
 
 78;
 3;0,1,2;,
 3;3,4,5;,
 3;3,5,6;,
 3;7,8,9;,
 3;10,11,12;,
 3;13,14,15;,
 3;16,17,18;,
 3;16,18,19;,
 3;20,21,22;,
 3;23,24,25;,
 3;26,27,28;,
 3;29,30,27;,
 3;31,32,33;,
 3;34,35,36;,
 3;37,38,39;,
 3;40,41,42;,
 3;43,44,45;,
 3;46,47,48;,
 3;49,50,51;,
 3;52,53,54;,
 3;55,56,57;,
 3;58,59,60;,
 3;61,62,63;,
 3;64,65,66;,
 3;67,68,69;,
 3;70,71,72;,
 3;73,74,75;,
 3;76,77,78;,
 3;79,80,81;,
 3;82,83,84;,
 3;85,86,87;,
 3;88,89,90;,
 3;91,92,93;,
 3;94,95,96;,
 3;97,98,99;,
 3;100,101,102;,
 3;103,104,105;,
 3;106,107,108;,
 3;109,110,111;,
 3;112,113,114;,
 3;115,116,117;,
 3;118,119,120;,
 3;121,122,123;,
 3;124,125,126;,
 3;127,128,129;,
 3;130,131,132;,
 3;133,134,135;,
 3;136,137,138;,
 3;139,140,141;,
 3;142,143,144;,
 3;145,146,147;,
 3;148,149,150;,
 3;151,152,153;,
 3;154,155,156;,
 3;157,158,159;,
 3;160,161,162;,
 3;163,164,165;,
 3;166,167,168;,
 3;169,170,171;,
 3;172,173,174;,
 3;175,176,177;,
 3;178,179,180;,
 3;181,182,183;,
 3;184,185,186;,
 3;187,188,189;,
 3;190,191,16;,
 3;192,193,194;,
 3;195,196,15;,
 4;197,198,199,1;,
 4;200,197,1,192;,
 3;200,198,197;,
 4;198,201,7,199;,
 3;198,202,201;,
 4;203,200,192,196;,
 3;200,203,204;,
 4;205,203,196,195;,
 4;206,205,195,5;,
 3;203,205,206;;
 
 MeshMaterialList {
  1;
  78;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
    "data\\TEXTURE\\mob\\Horse_v01.jpg";
   }
  }
 }
 MeshNormals {
  84;
  -0.781000;0.226229;0.582116;,
  -0.663958;0.091441;0.742158;,
  -0.934037;-0.037397;-0.355214;,
  -0.982269;0.081933;0.168627;,
  0.515381;-0.498850;-0.696801;,
  -0.553486;-0.596378;-0.581367;,
  -0.825413;-0.019185;-0.564204;,
  0.815288;-0.006678;-0.579017;,
  -0.443730;0.517005;0.731990;,
  -0.866815;0.012872;0.498463;,
  -0.851525;-0.380004;-0.361250;,
  -0.892627;-0.351077;-0.282777;,
  0.454499;-0.587631;-0.669418;,
  0.387987;-0.677076;-0.625327;,
  0.986020;-0.162711;-0.035910;,
  -0.078180;0.501412;0.861670;,
  0.949209;-0.178099;0.259390;,
  -0.531204;0.344653;0.773975;,
  -0.342189;0.433400;0.833709;,
  -0.825794;0.243378;0.508755;,
  -0.857252;-0.414965;-0.304833;,
  0.955451;-0.114615;0.271988;,
  0.878653;-0.395246;-0.267860;,
  0.122517;-0.730275;-0.672077;,
  -0.984151;-0.112718;0.136902;,
  0.718277;-0.031451;0.695046;,
  -0.982811;-0.173696;-0.062550;,
  -0.964646;0.196793;-0.175301;,
  0.052194;-0.041157;0.997788;,
  -0.037750;-0.069356;0.996877;,
  -0.975825;0.210934;0.057207;,
  -0.780421;-0.067947;0.621552;,
  -0.903005;-0.012146;0.429458;,
  -0.385149;-0.906833;-0.171214;,
  -0.833082;0.051014;0.550791;,
  0.087746;-0.175167;0.980621;,
  -0.965332;0.194694;-0.173864;,
  -0.040982;-0.272411;0.961308;,
  0.906769;-0.024998;-0.420887;,
  -0.905869;0.372790;0.201070;,
  -0.889272;0.339972;-0.305964;,
  -0.770897;0.636845;-0.012085;,
  0.629278;0.271868;-0.728078;,
  -0.048550;0.998425;-0.028132;,
  0.308466;0.667162;0.678043;,
  0.148681;-0.679046;-0.718881;,
  0.527503;-0.134733;-0.838802;,
  -0.933501;-0.314281;-0.172634;,
  -0.909558;-0.311602;-0.274969;,
  0.235264;-0.715718;-0.657570;,
  0.395851;-0.653599;-0.645066;,
  0.431582;-0.525763;-0.733014;,
  0.926020;0.066528;0.371567;,
  0.943002;0.190866;0.272611;,
  0.979940;-0.098513;0.173240;,
  0.849689;-0.002378;0.527279;,
  -0.975426;-0.218346;0.029466;,
  0.860957;0.010187;0.508576;,
  0.117945;0.287646;0.950447;,
  0.478532;-0.278083;0.832872;,
  0.358023;-0.499606;-0.788805;,
  0.440021;-0.419311;-0.794078;,
  -0.960732;0.212627;0.178283;,
  0.833082;-0.051014;-0.550791;,
  0.871013;0.261915;-0.415617;,
  0.748439;0.333044;-0.573516;,
  -0.951787;0.286002;-0.110926;,
  0.915714;0.266108;-0.301089;,
  0.492513;-0.337683;-0.802123;,
  0.591439;0.196305;0.782090;,
  -0.069043;-0.453462;0.888597;,
  0.086238;-0.721578;0.686942;,
  0.959874;-0.229415;0.161281;,
  0.890729;-0.350887;-0.288930;,
  0.548587;-0.062410;-0.833761;,
  0.277820;0.753071;0.596406;,
  -0.210977;0.927084;-0.309844;,
  -0.100964;0.989276;-0.105540;,
  -0.227016;0.877463;-0.422520;,
  0.024990;0.998132;0.055748;,
  0.740396;0.344014;-0.577467;,
  0.821724;0.402318;-0.403621;,
  -0.776426;0.309310;-0.549080;,
  0.011145;0.993281;-0.115186;;
  78;
  3;15,0,1;,
  3;5,20,2;,
  3;5,2,6;,
  3;2,20,3;,
  3;45,46,4;,
  3;7,22,4;,
  3;14,16,22;,
  3;14,22,7;,
  3;17,9,19;,
  3;17,8,18;,
  3;47,10,48;,
  3;11,48,10;,
  3;26,48,11;,
  3;23,13,49;,
  3;50,49,13;,
  3;51,50,12;,
  3;50,13,12;,
  3;52,53,54;,
  3;21,52,54;,
  3;53,52,55;,
  3;26,56,19;,
  3;17,19,8;,
  3;3,1,0;,
  3;20,5,23;,
  3;10,20,23;,
  3;10,47,20;,
  3;1,3,9;,
  3;20,9,3;,
  3;9,17,1;,
  3;17,15,1;,
  3;57,21,16;,
  3;21,57,52;,
  3;16,21,22;,
  3;21,54,22;,
  3;22,23,4;,
  3;23,22,13;,
  3;4,23,5;,
  3;26,24,56;,
  3;26,32,24;,
  3;18,8,58;,
  3;55,59,25;,
  3;55,25,53;,
  3;12,60,51;,
  3;60,61,51;,
  3;29,37,28;,
  3;35,29,28;,
  3;62,34,30;,
  3;63,64,65;,
  3;66,30,27;,
  3;38,65,67;,
  3;65,64,67;,
  3;32,31,24;,
  3;63,61,68;,
  3;68,61,60;,
  3;25,58,69;,
  3;66,62,30;,
  3;31,32,34;,
  3;27,36,66;,
  3;33,33,33;,
  3;29,70,37;,
  3;37,70,71;,
  3;72,38,67;,
  3;73,38,72;,
  3;74,38,73;,
  3;65,38,74;,
  3;57,16,14;,
  3;75,75,15;,
  3;46,7,4;,
  4;39,41,3,0;,
  4;44,44,75,75;,
  3;76,77,78;,
  4;41,40,2,3;,
  3;77,43,79;,
  4;80,81,14,7;,
  3;76,43,41;,
  4;42,80,7,46;,
  4;40,82,6,2;,
  3;43,83,79;;
 }
 MeshTextureCoords {
  207;
  0.157200;0.259800;,
  0.151100;0.263400;,
  0.154600;0.268200;,
  0.093400;0.322000;,
  0.097200;0.315500;,
  0.090000;0.312600;,
  0.085800;0.319400;,
  0.108300;0.292200;,
  0.107700;0.302300;,
  0.114000;0.296100;,
  0.088200;0.340200;,
  0.080400;0.337800;,
  0.080500;0.344000;,
  0.079200;0.377200;,
  0.084300;0.378500;,
  0.081400;0.370500;,
  0.082000;0.394400;,
  0.086100;0.394800;,
  0.082600;0.385900;,
  0.078100;0.385000;,
  0.169700;0.293900;,
  0.164400;0.297700;,
  0.171300;0.303200;,
  0.211800;0.308800;,
  0.213100;0.315600;,
  0.216200;0.312700;,
  0.151700;0.319700;,
  0.156300;0.328500;,
  0.160200;0.324200;,
  0.161400;0.336900;,
  0.164800;0.332500;,
  0.187500;0.353400;,
  0.183600;0.346500;,
  0.181000;0.350600;,
  0.123600;0.362600;,
  0.132600;0.365400;,
  0.132900;0.360000;,
  0.150200;0.366000;,
  0.141800;0.363000;,
  0.141400;0.368200;,
  0.176600;0.382000;,
  0.170500;0.379300;,
  0.169700;0.384100;,
  0.164200;0.376600;,
  0.156800;0.378900;,
  0.163500;0.381500;,
  0.151500;0.406100;,
  0.157200;0.402900;,
  0.150700;0.401200;,
  0.219000;0.260500;,
  0.221900;0.266900;,
  0.225300;0.262700;,
  0.237500;0.283900;,
  0.231300;0.283800;,
  0.234900;0.288200;,
  0.195500;0.342000;,
  0.198600;0.338300;,
  0.192700;0.335200;,
  0.185300;0.313300;,
  0.187800;0.321200;,
  0.191800;0.317700;,
  0.134900;0.280500;,
  0.141800;0.276100;,
  0.138000;0.271300;,
  0.105100;0.318800;,
  0.101600;0.325000;,
  0.110300;0.328300;,
  0.128500;0.335400;,
  0.122300;0.326200;,
  0.119300;0.331800;,
  0.138800;0.319800;,
  0.143300;0.314900;,
  0.135200;0.310100;,
  0.146400;0.281300;,
  0.139900;0.285600;,
  0.145600;0.290900;,
  0.133300;0.300100;,
  0.139400;0.295300;,
  0.133200;0.290200;,
  0.157800;0.292200;,
  0.163500;0.288200;,
  0.157800;0.282600;,
  0.174500;0.280700;,
  0.174800;0.271300;,
  0.169300;0.275000;,
  0.195500;0.240200;,
  0.203600;0.240300;,
  0.201400;0.235100;,
  0.204100;0.256700;,
  0.195700;0.255300;,
  0.199300;0.261100;,
  0.101700;0.404500;,
  0.108800;0.403900;,
  0.106900;0.396500;,
  0.115900;0.403300;,
  0.121300;0.396700;,
  0.114000;0.396600;,
  0.093700;0.373500;,
  0.099200;0.368100;,
  0.091600;0.366400;,
  0.115800;0.371900;,
  0.108700;0.376500;,
  0.116900;0.378100;,
  0.088200;0.346300;,
  0.096500;0.348800;,
  0.096600;0.342800;,
  0.207600;0.349000;,
  0.210200;0.345500;,
  0.204300;0.341700;,
  0.213900;0.353200;,
  0.220500;0.357800;,
  0.222700;0.354500;,
  0.228900;0.324200;,
  0.226300;0.326900;,
  0.231100;0.331300;,
  0.238400;0.313100;,
  0.240400;0.320800;,
  0.242700;0.317900;,
  0.246200;0.302400;,
  0.253100;0.304800;,
  0.248900;0.298400;,
  0.181500;0.389400;,
  0.189100;0.387400;,
  0.182700;0.384600;,
  0.204600;0.388800;,
  0.206400;0.384300;,
  0.199200;0.380900;,
  0.336900;0.397100;,
  0.339700;0.394100;,
  0.336800;0.390900;,
  0.313700;0.387000;,
  0.317000;0.389500;,
  0.319300;0.386000;,
  0.288800;0.413200;,
  0.287200;0.415900;,
  0.290600;0.418500;,
  0.274300;0.432200;,
  0.272200;0.434300;,
  0.275500;0.436900;,
  0.306200;0.439600;,
  0.301400;0.439400;,
  0.304400;0.442300;,
  0.286100;0.460900;,
  0.285800;0.455500;,
  0.283200;0.457900;,
  0.277000;0.446600;,
  0.271800;0.445900;,
  0.274600;0.448800;,
  0.242600;0.356100;,
  0.251600;0.358900;,
  0.244500;0.353000;,
  0.241500;0.394800;,
  0.232100;0.393200;,
  0.239800;0.397900;,
  0.220300;0.396300;,
  0.214100;0.388100;,
  0.212200;0.392400;,
  0.263800;0.328900;,
  0.261400;0.331500;,
  0.267900;0.337200;,
  0.300600;0.417600;,
  0.299200;0.412200;,
  0.297400;0.415300;,
  0.255000;0.370800;,
  0.246100;0.368200;,
  0.253200;0.373900;,
  0.324100;0.440600;,
  0.326000;0.437000;,
  0.322700;0.434500;,
  0.335000;0.444400;,
  0.330100;0.445700;,
  0.333000;0.448200;,
  0.332700;0.408100;,
  0.334000;0.413300;,
  0.335700;0.410800;,
  0.347700;0.409600;,
  0.348400;0.415000;,
  0.350500;0.412800;,
  0.349500;0.384200;,
  0.356200;0.384400;,
  0.353400;0.380500;,
  0.361700;0.392000;,
  0.359000;0.388200;,
  0.355200;0.391400;,
  0.305000;0.471900;,
  0.302000;0.474700;,
  0.305000;0.477300;,
  0.294500;0.458900;,
  0.295200;0.464100;,
  0.297700;0.461700;,
  0.097750;0.120100;,
  0.198450;0.237650;,
  0.041000;0.197200;,
  0.075550;0.131700;,
  0.000000;0.000000;,
  0.040200;0.168900;,
  0.039600;0.188600;,
  0.151100;0.263400;,
  0.069950;0.142800;,
  0.069950;0.142800;,
  0.041000;0.197200;,
  0.108300;0.292200;,
  0.109550;0.331400;,
  0.039600;0.188600;,
  0.139900;0.285600;,
  0.040200;0.168900;,
  0.090000;0.312600;;
 }
}
