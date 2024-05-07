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
 280;
 52.96000;185.55921;-8.02320;,
 79.15681;181.08080;-0.01040;,
 -9.41120;131.29281;-13.08320;,
 20.60720;-3.69520;-0.00000;,
 -4.16800;0.54000;0.00000;,
 63.74480;248.64402;0.00000;,
 -11.66000;187.36241;-16.38880;,
 89.94080;244.16560;0.00000;,
 134.09199;252.51680;-0.00640;,
 71.02960;263.43123;-64.00640;,
 68.31920;247.57681;-0.00640;,
 7.90320;273.97361;-0.00640;,
 70.93280;262.86404;63.99360;,
 105.18080;58.21120;0.00000;,
 108.27360;51.73760;-58.44400;,
 50.52800;24.14400;-32.64320;,
 46.62720;32.30720;0.00000;,
 -7.21840;-3.44880;-6.84320;,
 167.80000;94.16000;64.00000;,
 168.02401;75.13760;0.00000;,
 100.62321;67.74880;64.00000;,
 50.52800;24.14400;36.95760;,
 247.04002;92.05359;6.78800;,
 168.52481;88.47441;-79.47760;,
 -7.21840;-3.44880;9.23120;,
 41.53600;50.40800;-81.05840;,
 -2.28320;44.80240;-106.52240;,
 -5.20320;20.90800;-51.83120;,
 18.41360;27.97600;-35.93360;,
 -8.12400;-2.98720;2.86080;,
 115.58720;81.53840;-104.04400;,
 66.35360;65.06559;-129.48959;,
 89.05920;58.66721;-52.27280;,
 48.43600;20.90800;-24.34480;,
 102.78881;79.71360;-187.70401;,
 5.30080;76.61441;-161.26240;,
 4.26400;-2.98720;9.20800;,
 45.26400;49.57760;77.30320;,
 89.54880;44.06400;54.42480;,
 45.73600;20.56320;23.08800;,
 20.06560;27.51520;34.26880;,
 1.92320;-2.93760;-8.24960;,
 25.17520;80.19439;150.86801;,
 72.30880;63.99280;123.48960;,
 -3.73440;57.70080;101.49521;,
 -5.41760;20.56320;53.04000;,
 130.93681;75.35200;89.65519;,
 101.32399;78.40000;184.48480;,
 -9.89120;-2.93760;-1.33200;,
 -60.70800;73.16240;73.57521;,
 -17.59680;68.20720;113.37520;,
 -4.02640;31.92080;56.27120;,
 -26.39280;38.86240;31.79680;,
 9.54320;-4.36560;-0.83200;,
 -146.62880;116.91680;77.13200;,
 -98.40641;99.22160;119.78640;,
 -106.99520;80.83600;28.78240;,
 -58.27360;30.74560;12.68000;,
 -35.70800;113.82560;168.26719;,
 -151.44721;127.55520;174.19040;,
 -2.98480;-4.63760;-10.90000;,
 -85.95761;66.29200;-4.67120;,
 -98.87280;52.41440;45.05680;,
 -47.00720;23.66800;25.73520;,
 -37.34880;35.10960;-0.96880;,
 4.85760;-5.07840;6.41360;,
 -126.64960;116.33841;-63.35840;,
 -139.39201;90.18641;-10.48800;,
 -70.49760;83.47599;-58.54960;,
 -35.77120;33.93680;-35.30160;,
 -205.99202;117.38881;-24.49520;,
 -151.11361;90.27040;61.40080;,
 7.45280;-2.70720;-7.68240;,
 -51.20240;74.71840;-79.03760;,
 -102.68161;68.18880;-51.20960;,
 -52.45040;31.76160;-21.01840;,
 -22.18560;39.53600;-34.09680;,
 -2.21840;-4.66640;9.17200;,
 -27.56720;121.19440;-160.22240;,
 -83.20320;101.75280;-128.84560;,
 5.98480;84.09279;-108.18319;,
 6.29440;32.54000;-58.33840;,
 -118.05600;131.51281;-195.74481;,
 -149.49761;114.10000;-84.71601;,
 11.34880;-4.48640;0.55360;,
 -20.00720;128.50000;24.18960;,
 -2.95040;191.18961;-27.18880;,
 -0.97360;-0.06400;4.08800;,
 4.85440;-0.69760;-20.35360;,
 11.00560;190.56721;-51.17520;,
 4.85440;255.21600;-26.98800;,
 -25.92720;183.64401;33.34320;,
 11.01600;254.54641;-52.83120;,
 20.68080;266.87361;-95.02160;,
 -56.41440;266.81922;-47.58720;,
 5.84160;254.97039;-32.43600;,
 -8.99680;267.44881;29.51760;,
 68.09760;270.84241;-18.00400;,
 -0.23920;124.60720;44.39120;,
 -40.89600;188.44961;10.74960;,
 3.01520;-0.15200;2.92400;,
 -14.92480;-0.24880;-14.68080;,
 -54.31840;189.53282;-13.52400;,
 -42.14720;252.93201;11.67040;,
 -0.03840;178.44721;60.54560;,
 -61.11600;252.82962;-6.94320;,
 -73.61360;262.76722;-26.83680;,
 -82.47441;264.78321;34.19360;,
 -45.92880;252.85362;7.92320;,
 -21.56080;269.47039;42.73120;,
 -13.49600;273.82559;-18.28800;,
 -2.40480;0.84720;2.26560;,
 -12.89680;104.53201;-20.76080;,
 11.89040;-5.00800;-11.05520;,
 45.66800;136.80321;-70.16079;,
 25.98000;142.18560;-60.62800;,
 36.78080;192.32880;-71.07441;,
 -16.08080;149.22641;-29.32560;,
 51.89680;186.13841;-85.16000;,
 77.36800;188.40879;-111.15680;,
 4.45680;196.96641;-113.62880;,
 39.41680;191.01920;-73.49760;,
 4.55680;218.13601;-43.29200;,
 77.44880;209.42642;-40.77280;,
 1.79360;0.60560;2.86960;,
 -18.09680;105.66321;14.67280;,
 -8.76160;-3.82800;-14.20800;,
 -47.84400;147.66640;-29.38720;,
 -53.55681;143.74159;-51.00880;,
 -53.59920;199.18401;-39.20640;,
 -25.05920;150.93280;18.70800;,
 -64.75920;194.49600;-57.26320;,
 -71.26560;198.35042;-76.71041;,
 -90.85200;207.42000;-31.08640;,
 -55.81201;198.19840;-42.82560;,
 -44.86960;218.89600;-13.76320;,
 -26.36640;214.83360;-60.54560;,
 -16.76320;94.15040;56.02241;,
 -2.47200;158.26801;33.64000;,
 -0.81600;-1.22720;3.19840;,
 9.22160;164.68401;14.58640;,
 4.06720;5.30960;-16.21680;,
 4.06720;208.59121;52.22800;,
 -21.72320;134.90720;79.09920;,
 9.23040;215.50320;31.69920;,
 17.32800;237.34803;2.04960;,
 -47.26800;223.65120;39.35840;,
 4.89440;209.96641;47.87040;,
 -7.53840;201.95199;100.21120;,
 57.05760;218.30081;63.79520;,
 11.75440;-4.44160;-8.33360;,
 -2.34480;0.76880;1.75280;,
 14.23680;104.55200;-95.51440;,
 -31.73120;78.21280;-44.17280;,
 -4.25280;107.28560;-88.42241;,
 -4.80640;148.23441;-110.34800;,
 -43.30240;112.22080;-62.83840;,
 10.10160;142.72479;-121.01361;,
 25.38080;145.80800;-130.73600;,
 -17.52800;141.90241;-141.61361;,
 -1.80800;147.10001;-112.45280;,
 -28.75280;164.34398;-104.95200;,
 14.21440;172.08001;-96.66320;,
 44.19200;93.75680;8.90560;,
 72.48400;127.66321;-38.98400;,
 0.03120;0.40000;3.28080;,
 77.22800;121.15761;-59.27520;,
 -0.57120;-2.66880;-16.23760;,
 104.02001;166.75360;-46.10320;,
 64.67680;132.85200;11.41680;,
 103.38320;163.50882;-66.74080;,
 107.63120;167.26719;-101.67760;,
 65.65921;200.27680;-54.59680;,
 103.64081;166.13280;-50.40080;,
 109.61520;181.13920;-1.96880;,
 152.95199;150.33920;-49.43920;,
 53.20560;78.50240;9.04480;,
 62.69680;109.02401;59.41120;,
 0.55200;0.68640;-2.89600;,
 53.80640;106.79680;77.17680;,
 -3.07600;-4.03840;14.19760;,
 77.87360;150.40720;74.07120;,
 74.33040;112.66801;14.07680;,
 74.03760;145.41120;92.14479;,
 69.29120;148.99201;109.52720;,
 108.33440;144.49280;88.78800;,
 77.07441;149.37602;77.67680;,
 91.93920;165.93681;53.50320;,
 54.59680;174.32080;76.08560;,
 -29.25200;88.61840;66.42400;,
 -74.03280;141.80480;48.80400;,
 2.34160;-0.36560;2.76240;,
 -11.36720;1.05760;-14.08400;,
 -83.70880;146.73520;27.36000;,
 -88.48800;193.29359;64.91760;,
 -41.41280;127.76081;92.67840;,
 -102.98241;194.79921;47.10560;,
 -130.63519;207.17681;22.59280;,
 -133.83122;175.22639;94.09840;,
 -91.65680;193.39921;61.31440;,
 -60.02560;197.93520;107.56321;,
 -57.83520;232.39441;37.08800;,
 18.31600;98.94880;39.76320;,
 -17.85040;130.07520;87.86240;,
 2.38560;1.17760;-2.17760;,
 -11.83680;-6.60640;10.46560;,
 -37.60000;123.68241;97.06080;,
 -26.14560;178.09040;108.09600;,
 23.79200;141.40321;56.54880;,
 -41.18320;169.85921;121.46321;,
 -58.64160;170.94160;132.83841;,
 -9.76480;177.64481;143.49361;,
 -29.16480;176.40160;110.74160;,
 -3.37520;200.72401;99.07840;,
 -53.13760;198.55039;90.93040;,
 2.67280;1.79840;1.27680;,
 -32.03040;102.55200;-18.76400;,
 -12.95600;-9.65920;-6.06880;,
 -99.00960;108.96321;-69.81920;,
 -85.70960;121.81121;-56.33440;,
 -105.65441;164.65520;-80.72400;,
 -45.98400;145.54641;-29.00240;,
 -122.18001;152.54081;-88.49120;,
 -154.04159;144.01601;-105.36320;,
 -140.06000;179.14001;-41.02240;,
 -108.47121;162.32720;-82.00801;,
 -74.41201;202.28481;-67.92320;,
 -88.32241;167.02641;-132.20641;,
 31.68880;68.30079;35.37840;,
 11.97680;93.68960;76.37200;,
 1.87120;0.67120;-1.74160;,
 -9.42480;-3.87840;8.33680;,
 -3.41120;91.30240;84.26240;,
 14.51200;129.44800;95.35680;,
 43.48400;97.99840;50.50000;,
 2.56800;124.63681;106.01361;,
 -9.78640;127.32960;115.88640;,
 28.48720;123.91841;121.31040;,
 12.10640;128.45761;97.46479;,
 34.79760;143.51601;88.42800;,
 -3.28480;150.27201;85.25760;,
 68.11039;140.09199;17.40080;,
 89.14080;132.53361;14.56000;,
 11.03120;104.74640;20.75600;,
 14.88880;-4.48080;-6.85040;,
 -3.04560;0.74720;1.34240;,
 88.61120;185.57201;33.25680;,
 17.66240;148.75921;30.75600;,
 107.57361;180.04401;24.59440;,
 142.01041;181.14721;12.47520;,
 79.46240;209.72322;-13.36400;,
 92.45040;184.42080;31.21680;,
 49.91440;205.80081;53.77920;,
 113.42479;179.70401;80.14160;,
 101.16080;81.00880;2.47120;,
 97.62000;87.64640;19.51440;,
 70.85760;45.64480;-27.88320;,
 -0.11600;1.97600;13.91120;,
 -0.08080;-0.48480;-2.77760;,
 130.24320;112.74081;-2.14640;,
 100.26880;66.58640;-38.82160;,
 130.20560;115.34320;15.49920;,
 137.10961;125.20160;43.20240;,
 164.52641;86.14720;5.60240;,
 130.31441;113.08480;1.55680;,
 135.93920;111.52001;-41.61840;,
 110.22320;152.02080;-4.22800;,
 0.46720;-0.18720;3.10800;,
 -58.88080;72.49360;33.10160;,
 -1.90160;0.30560;-15.56400;,
 -85.26320;119.01200;-21.92880;,
 -87.66481;115.07440;-1.65360;,
 -109.70241;157.89201;2.27200;,
 -82.91841;104.64320;45.80960;,
 -112.20720;158.41280;-17.47040;,
 -122.09120;167.38480;-48.42640;,
 -154.70561;138.05120;3.27840;,
 -110.36641;157.80640;-1.82560;,
 -108.91121;163.24159;46.47520;,
 -77.70320;194.63119;-4.99680;;
 
 198;
 3;0,1,2;,
 3;1,3,2;,
 3;3,4,2;,
 4;5,0,2,6;,
 4;1,0,5,7;,
 3;8,9,10;,
 3;10,9,11;,
 3;8,10,12;,
 3;12,10,11;,
 4;13,14,15,16;,
 3;16,15,17;,
 4;18,19,13,20;,
 4;20,13,16,21;,
 3;22,23,18;,
 3;18,23,19;,
 3;19,23,13;,
 3;23,14,13;,
 4;21,16,17,24;,
 4;25,26,27,28;,
 3;28,27,29;,
 4;30,31,25,32;,
 4;32,25,28,33;,
 3;30,34,31;,
 3;34,35,31;,
 3;31,35,25;,
 3;35,26,25;,
 4;33,28,29,36;,
 4;37,38,39,40;,
 3;40,39,41;,
 4;42,43,37,44;,
 4;44,37,40,45;,
 3;46,38,47;,
 3;47,38,43;,
 3;38,37,43;,
 3;43,42,47;,
 4;45,40,41,48;,
 4;49,50,51,52;,
 3;52,51,53;,
 4;54,55,49,56;,
 4;56,49,52,57;,
 3;50,49,58;,
 3;49,55,58;,
 3;58,55,59;,
 3;55,54,59;,
 4;57,52,53,60;,
 4;61,62,63,64;,
 3;64,63,65;,
 4;66,67,61,68;,
 4;68,61,64,69;,
 3;61,67,62;,
 3;66,70,67;,
 3;62,67,71;,
 3;67,70,71;,
 4;69,64,65,72;,
 4;73,74,75,76;,
 3;76,75,77;,
 4;78,79,73,80;,
 4;80,73,76,81;,
 3;78,82,79;,
 3;73,79,74;,
 3;79,82,74;,
 3;82,83,74;,
 4;81,76,77,84;,
 3;85,86,87;,
 3;87,86,88;,
 3;86,89,88;,
 4;90,86,85,91;,
 4;89,86,90,92;,
 3;93,94,95;,
 3;95,94,96;,
 3;93,95,97;,
 3;97,95,96;,
 3;98,99,100;,
 3;100,99,101;,
 3;99,102,101;,
 4;103,99,98,104;,
 4;102,99,103,105;,
 3;106,107,108;,
 3;108,107,109;,
 3;106,108,110;,
 3;110,108,109;,
 3;111,112,113;,
 3;113,112,114;,
 3;112,115,114;,
 4;116,115,112,117;,
 4;114,115,116,118;,
 3;119,120,121;,
 3;121,120,122;,
 3;119,121,123;,
 3;123,121,122;,
 3;124,125,126;,
 3;125,127,126;,
 3;127,128,126;,
 4;129,127,125,130;,
 4;128,127,129,131;,
 3;132,133,134;,
 3;134,133,135;,
 3;132,134,136;,
 3;136,134,135;,
 3;137,138,139;,
 3;138,140,139;,
 3;140,141,139;,
 4;142,138,137,143;,
 4;140,138,142,144;,
 3;145,146,147;,
 3;147,146,148;,
 3;145,147,149;,
 3;149,147,148;,
 3;150,151,152;,
 3;151,153,152;,
 3;153,154,152;,
 4;155,154,153,156;,
 4;152,154,155,157;,
 3;158,159,160;,
 3;160,159,161;,
 3;158,160,162;,
 3;162,160,161;,
 3;163,164,165;,
 3;164,166,165;,
 3;166,167,165;,
 4;168,164,163,169;,
 4;166,164,168,170;,
 3;171,172,173;,
 3;173,172,174;,
 3;171,173,175;,
 3;175,173,174;,
 3;176,177,178;,
 3;177,179,178;,
 3;179,180,178;,
 4;181,177,176,182;,
 4;179,177,181,183;,
 3;184,185,186;,
 3;186,185,187;,
 3;184,186,188;,
 3;188,186,187;,
 3;189,190,191;,
 3;191,190,192;,
 3;190,193,192;,
 4;194,190,189,195;,
 4;193,190,194,196;,
 3;197,198,199;,
 3;199,198,200;,
 3;197,199,201;,
 3;201,199,200;,
 3;202,203,204;,
 3;204,203,205;,
 3;203,206,205;,
 4;207,203,202,208;,
 4;206,203,207,209;,
 3;210,211,212;,
 3;212,211,213;,
 3;210,212,214;,
 3;214,212,213;,
 3;215,216,217;,
 3;217,216,218;,
 3;216,219,218;,
 4;220,219,216,221;,
 4;218,219,220,222;,
 3;223,224,225;,
 3;225,224,226;,
 3;223,225,227;,
 3;227,225,226;,
 3;228,229,230;,
 3;230,229,231;,
 3;229,232,231;,
 4;233,229,228,234;,
 4;232,229,233,235;,
 3;236,237,238;,
 3;238,237,239;,
 3;236,238,240;,
 3;240,238,239;,
 3;241,242,243;,
 3;242,244,243;,
 3;244,245,243;,
 4;246,241,243,247;,
 4;242,241,246,248;,
 3;249,250,251;,
 3;251,250,252;,
 3;249,251,253;,
 3;253,251,252;,
 3;254,255,256;,
 3;255,257,256;,
 3;257,258,256;,
 4;259,254,256,260;,
 4;255,254,259,261;,
 3;262,263,264;,
 3;264,263,265;,
 3;262,264,266;,
 3;266,264,265;,
 3;267,268,269;,
 3;269,268,270;,
 3;268,271,270;,
 4;272,271,268,273;,
 4;270,271,272,274;,
 3;275,276,277;,
 3;277,276,278;,
 3;275,277,279;,
 3;279,277,278;;
 
 MeshMaterialList {
  1;
  198;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
    "data\\TEXTURE\\flower\\ranunculus1.tga";
   }
  }
 }
 MeshNormals {
  281;
  0.080092;-0.078499;-0.993692;,
  -0.017134;-0.100229;-0.994817;,
  0.136840;-0.084558;-0.986978;,
  0.180245;-0.048386;-0.982431;,
  0.195839;-0.075445;-0.977730;,
  0.141133;0.030037;-0.989535;,
  0.129032;0.024968;-0.991326;,
  0.153210;0.035101;-0.987570;,
  -0.074895;0.997183;0.004125;,
  0.162622;0.956109;0.243739;,
  0.167677;0.985833;0.004251;,
  0.400367;0.916345;0.004126;,
  0.162962;0.958111;-0.235512;,
  -0.417598;0.906114;-0.067596;,
  -0.491047;0.856632;-0.158285;,
  -0.471863;0.850950;-0.230715;,
  -0.503423;0.860656;-0.076395;,
  -0.532342;0.843235;-0.074614;,
  -0.156305;0.979219;-0.129223;,
  -0.286301;0.957711;-0.028667;,
  -0.412980;0.906427;-0.088529;,
  -0.522434;0.848948;0.079684;,
  -0.002034;0.999213;-0.039606;,
  -0.278374;0.960473;-0.000669;,
  -0.534995;0.836414;0.119131;,
  -0.250599;0.906114;0.340820;,
  -0.364797;0.856634;0.364831;,
  -0.420500;0.850951;0.314741;,
  -0.297564;0.860653;0.413197;,
  -0.309164;0.843230;0.439750;,
  -0.331615;0.937850;0.102319;,
  -0.152266;0.968345;0.197798;,
  -0.267127;0.906426;0.327163;,
  -0.167342;0.848945;0.501287;,
  -0.148108;0.977041;0.153147;,
  -0.099085;0.934793;0.341091;,
  -0.137961;0.836409;0.530460;,
  -0.085433;0.916538;-0.390717;,
  -0.128529;0.942506;-0.308484;,
  -0.039343;0.850948;-0.523775;,
  -0.188453;0.860652;-0.473036;,
  -0.204604;0.843229;-0.497093;,
  0.091988;0.937850;-0.334626;,
  0.056665;0.951053;-0.303789;,
  -0.132272;0.906427;-0.401116;,
  -0.332740;0.848945;-0.410580;,
  -0.093642;0.961911;-0.256823;,
  -0.490660;0.852426;-0.180619;,
  -0.373129;0.836410;-0.401488;,
  0.384602;0.835148;-0.393203;,
  0.496973;0.776856;-0.386669;,
  0.541040;0.775307;-0.325844;,
  0.441159;0.777614;-0.447990;,
  0.456348;0.756066;-0.469160;,
  0.419160;0.895592;-0.149064;,
  0.265236;0.921841;-0.282595;,
  0.398074;0.836496;-0.376579;,
  0.330399;0.760582;-0.558884;,
  0.253046;0.936897;-0.241230;,
  0.238917;0.871440;-0.428382;,
  0.306459;0.745658;-0.591673;,
  0.466903;0.812070;0.350061;,
  0.493814;0.820104;0.289096;,
  0.528630;0.716238;0.455580;,
  0.588226;0.745877;0.312503;,
  0.614514;0.724620;0.311927;,
  0.312699;0.834332;0.453993;,
  0.398778;0.871302;0.286023;,
  0.503723;0.802795;0.319035;,
  0.633140;0.756940;0.161786;,
  0.313552;0.909026;0.274512;,
  0.490041;0.868123;0.078888;,
  0.650826;0.749184;0.123080;,
  0.169684;0.840031;0.515321;,
  0.212094;0.874618;0.435958;,
  0.122758;0.759091;0.639306;,
  0.270494;0.765194;0.584218;,
  0.285965;0.743271;0.604791;,
  -0.007172;0.882369;0.470504;,
  0.028097;0.897346;0.440433;,
  0.215876;0.825081;0.522148;,
  0.411369;0.752267;0.514655;,
  0.177241;0.903949;0.389181;,
  0.561807;0.778040;0.281116;,
  0.450333;0.738281;0.502137;,
  -0.927721;-0.050906;-0.369787;,
  -0.947581;-0.061083;-0.313623;,
  -0.927596;-0.018371;-0.373132;,
  -0.928172;-0.017417;-0.371743;,
  -0.898928;-0.001092;-0.438096;,
  -0.931489;0.044049;-0.361093;,
  -0.935797;0.037281;-0.350563;,
  -0.927018;0.050809;-0.371559;,
  -0.071070;0.982864;0.170081;,
  0.178097;0.983310;0.037191;,
  -0.028297;0.999527;-0.012073;,
  0.015488;0.980907;-0.193860;,
  -0.233337;0.970495;-0.060773;,
  -0.783944;-0.187651;0.591793;,
  -0.745028;-0.199867;0.636386;,
  -0.789486;-0.155474;0.593751;,
  -0.788668;-0.154597;0.595066;,
  -0.832070;-0.135081;0.537971;,
  -0.786393;-0.094108;0.610516;,
  -0.778983;-0.101290;0.618810;,
  -0.793665;-0.086911;0.602115;,
  0.052241;0.970544;0.235193;,
  0.161291;0.963603;-0.213203;,
  -0.128651;0.991689;0.001055;,
  -0.298183;0.925604;-0.233120;,
  -0.401720;0.890134;0.215135;,
  -0.641828;-0.210569;-0.737372;,
  -0.711749;-0.220160;-0.667041;,
  -0.596757;-0.222675;-0.770907;,
  -0.562697;-0.191828;-0.804098;,
  -0.548305;-0.219802;-0.806876;,
  -0.595500;-0.110688;-0.795693;,
  -0.605362;-0.114418;-0.787684;,
  -0.585534;-0.106940;-0.803563;,
  -0.050349;0.991560;-0.119471;,
  0.287027;0.951700;-0.109005;,
  0.122208;0.950598;-0.285356;,
  0.287563;0.853575;-0.434416;,
  -0.049751;0.894099;-0.445098;,
  -0.882301;-0.206150;0.423140;,
  -0.804938;-0.214551;0.553211;,
  -0.866509;-0.166804;0.470466;,
  -0.909850;-0.146553;0.388195;,
  -0.924989;-0.150758;0.348809;,
  -0.902582;-0.092495;0.420465;,
  -0.896875;-0.097135;0.431486;,
  -0.908130;-0.087840;0.409370;,
  -0.092565;0.994612;0.046678;,
  0.125066;0.926620;-0.354590;,
  -0.210465;0.952023;-0.222163;,
  -0.309824;0.825708;-0.471397;,
  -0.518420;0.852968;-0.060717;,
  -0.972747;0.073936;-0.219767;,
  -0.924029;0.076058;-0.374680;,
  -0.927597;0.110938;-0.356729;,
  -0.913878;0.121314;-0.387440;,
  -0.928621;0.112899;-0.353436;,
  -0.931489;0.165420;-0.323982;,
  -0.935799;0.155447;-0.316413;,
  -0.927016;0.175365;-0.331494;,
  -0.071065;0.864611;0.497391;,
  0.178092;0.910684;0.372744;,
  -0.028292;0.942837;0.332050;,
  0.015494;0.987804;0.154933;,
  -0.233324;0.932306;0.276343;,
  -0.620692;-0.481077;-0.619117;,
  -0.533434;-0.531474;-0.658015;,
  -0.440668;-0.575863;-0.688618;,
  -0.463517;-0.540401;-0.702224;,
  -0.483477;-0.552799;-0.678722;,
  -0.514602;-0.470355;-0.716904;,
  -0.523711;-0.473855;-0.707947;,
  -0.505403;-0.466772;-0.725735;,
  -0.239681;0.876097;-0.418338;,
  0.192930;0.942747;-0.272039;,
  0.013966;0.825860;-0.563703;,
  0.266322;0.702993;-0.659449;,
  -0.166790;0.601037;-0.781624;,
  -0.853961;0.517422;-0.054996;,
  -0.872048;0.446661;-0.200065;,
  -0.853500;0.481403;-0.199475;,
  -0.849561;0.473027;-0.233435;,
  -0.852262;0.484485;-0.197292;,
  -0.821344;0.536172;-0.194717;,
  -0.826193;0.532893;-0.182841;,
  -0.816350;0.539356;-0.206560;,
  0.494097;0.867494;0.057642;,
  0.671083;0.732263;-0.115924;,
  0.499078;0.856776;-0.129829;,
  0.486442;0.815808;-0.312779;,
  0.304546;0.942469;-0.137854;,
  0.875790;-0.479742;0.053276;,
  0.812384;-0.553550;0.183343;,
  0.832575;-0.520358;0.189859;,
  0.820520;-0.526599;0.222350;,
  0.834747;-0.517429;0.188320;,
  0.863219;-0.464947;0.196666;,
  0.863922;-0.468805;0.184015;,
  0.862364;-0.461008;0.209283;,
  0.458932;0.879950;0.122758;,
  0.040625;0.951802;0.304010;,
  0.369350;0.837668;0.402359;,
  0.247303;0.721717;0.646503;,
  0.649732;0.614053;0.448092;,
  -0.763894;-0.514294;0.389829;,
  -0.727130;-0.531377;0.434650;,
  -0.777112;-0.486080;0.399779;,
  -0.776643;-0.485498;0.401395;,
  -0.818429;-0.458715;0.346055;,
  -0.790212;-0.433473;0.433204;,
  -0.782031;-0.441185;0.440208;,
  -0.798255;-0.425685;0.426124;,
  -0.226664;0.822927;0.520975;,
  -0.198850;0.942984;0.266907;,
  -0.351994;0.853417;0.384421;,
  -0.464940;0.853790;0.234251;,
  -0.489153;0.725255;0.484493;,
  0.612578;-0.368889;0.699049;,
  0.658741;-0.358603;0.661410;,
  0.608975;-0.341189;0.716058;,
  0.610121;-0.339874;0.715708;,
  0.550371;-0.347117;0.759343;,
  0.614987;-0.281234;0.736681;,
  0.624416;-0.283749;0.727730;,
  0.605449;-0.278669;0.745503;,
  0.065651;0.944791;0.321031;,
  -0.374546;0.908076;0.187386;,
  -0.169237;0.860142;0.481160;,
  -0.389186;0.699866;0.598934;,
  0.058154;0.699784;0.711984;,
  -0.526634;0.006653;0.850066;,
  -0.441071;0.026517;0.897080;,
  -0.567888;-0.022607;0.822795;,
  -0.612206;-0.007901;0.790659;,
  -0.614376;-0.039294;0.788034;,
  -0.607692;0.079886;0.790145;,
  -0.597136;0.080381;0.798102;,
  -0.618142;0.079377;0.782048;,
  -0.241743;0.934567;-0.261045;,
  -0.306317;0.774314;-0.553721;,
  -0.436470;0.825826;-0.357078;,
  -0.605465;0.668385;-0.432057;,
  -0.541204;0.829222;-0.139605;,
  0.565361;-0.551633;0.613244;,
  0.610106;-0.547354;0.572865;,
  0.568756;-0.525559;0.632696;,
  0.570134;-0.524476;0.632354;,
  0.513166;-0.523609;0.680069;,
  0.588553;-0.470360;0.657546;,
  0.596636;-0.473864;0.647671;,
  0.580367;-0.466774;0.667305;,
  0.283195;0.876095;0.390203;,
  -0.162620;0.942748;0.291172;,
  0.046606;0.825857;0.561950;,
  -0.194025;0.702987;0.684225;,
  0.249701;0.601030;0.759218;,
  -0.257549;0.228212;-0.938929;,
  -0.352370;0.217605;-0.910211;,
  -0.204060;0.214803;-0.955102;,
  -0.155106;0.243288;-0.957472;,
  -0.144618;0.214714;-0.965911;,
  -0.177789;0.323463;-0.929388;,
  -0.190594;0.320426;-0.927901;,
  -0.164953;0.326442;-0.930713;,
  0.157825;0.955163;0.250511;,
  0.390864;0.844739;0.365570;,
  0.329331;0.927049;0.179227;,
  0.489267;0.866197;0.101592;,
  0.252914;0.967370;-0.015162;,
  0.598639;-0.773411;0.208488;,
  0.570879;-0.812077;0.120947;,
  0.596870;-0.757746;0.263756;,
  0.626432;-0.721277;0.295536;,
  0.605086;-0.730342;0.316972;,
  0.684819;-0.688575;0.238512;,
  0.680670;-0.696071;0.228414;,
  0.688847;-0.680957;0.248568;,
  0.758068;0.582559;-0.293187;,
  0.622800;0.771118;-0.132277;,
  0.774229;0.623185;-0.110500;,
  0.763062;0.641823;0.076149;,
  0.890559;0.447094;-0.083728;,
  -0.787929;-0.615696;-0.009241;,
  -0.755999;-0.649818;0.078759;,
  -0.791628;-0.607429;-0.065986;,
  -0.816684;-0.568569;-0.098774;,
  -0.801831;-0.585132;-0.121195;,
  -0.855729;-0.515859;-0.040227;,
  -0.851846;-0.522947;-0.029744;,
  -0.859461;-0.508680;-0.050703;,
  -0.543329;0.784843;0.298020;,
  -0.396103;0.913362;0.094192;,
  -0.579244;0.807005;0.114979;,
  -0.594723;0.800684;-0.072182;,
  -0.736104;0.664159;0.130553;,
  -0.999968;0.006023;-0.005290;;
  198;
  3;3,4,2;,
  3;4,0,2;,
  3;0,1,2;,
  4;5,3,2,6;,
  4;4,3,5,7;,
  3;8,9,10;,
  3;10,9,11;,
  3;8,10,12;,
  3;12,10,11;,
  4;13,14,15,16;,
  3;16,15,17;,
  4;18,19,13,20;,
  4;20,13,16,21;,
  3;22,23,18;,
  3;280,280,280;,
  3;19,23,13;,
  3;23,14,13;,
  4;21,16,17,24;,
  4;25,26,27,28;,
  3;28,27,29;,
  4;30,31,25,32;,
  4;32,25,28,33;,
  3;30,34,31;,
  3;34,35,31;,
  3;31,35,25;,
  3;35,26,25;,
  4;33,28,29,36;,
  4;37,38,39,40;,
  3;40,39,41;,
  4;42,43,37,44;,
  4;44,37,40,45;,
  3;47,38,46;,
  3;46,38,43;,
  3;38,37,43;,
  3;43,42,46;,
  4;45,40,41,48;,
  4;49,50,51,52;,
  3;52,51,53;,
  4;54,55,49,56;,
  4;56,49,52,57;,
  3;50,49,59;,
  3;49,55,59;,
  3;59,55,58;,
  3;55,54,58;,
  4;57,52,53,60;,
  4;61,62,63,64;,
  3;64,63,65;,
  4;66,67,61,68;,
  4;68,61,64,69;,
  3;61,67,62;,
  3;66,70,67;,
  3;62,67,71;,
  3;67,70,71;,
  4;69,64,65,72;,
  4;73,74,75,76;,
  3;76,75,77;,
  4;78,79,73,80;,
  4;80,73,76,81;,
  3;78,82,79;,
  3;73,79,74;,
  3;79,82,74;,
  3;82,83,74;,
  4;81,76,77,84;,
  3;87,88,86;,
  3;86,88,85;,
  3;88,89,85;,
  4;90,88,87,91;,
  4;89,88,90,92;,
  3;93,94,95;,
  3;95,94,96;,
  3;93,95,97;,
  3;97,95,96;,
  3;100,101,99;,
  3;99,101,98;,
  3;101,102,98;,
  4;103,101,100,104;,
  4;102,101,103,105;,
  3;106,107,108;,
  3;108,107,109;,
  3;106,108,110;,
  3;110,108,109;,
  3;112,113,111;,
  3;111,113,115;,
  3;113,114,115;,
  4;116,114,113,117;,
  4;115,114,116,118;,
  3;119,120,121;,
  3;121,120,122;,
  3;119,121,123;,
  3;123,121,122;,
  3;125,126,124;,
  3;126,127,124;,
  3;127,128,124;,
  4;129,127,126,130;,
  4;128,127,129,131;,
  3;132,133,134;,
  3;134,133,135;,
  3;132,134,136;,
  3;136,134,135;,
  3;139,140,138;,
  3;140,141,138;,
  3;141,137,138;,
  4;142,140,139,143;,
  4;141,140,142,144;,
  3;145,146,147;,
  3;147,146,148;,
  3;145,147,149;,
  3;149,147,148;,
  3;150,151,154;,
  3;151,152,154;,
  3;152,153,154;,
  4;155,153,152,156;,
  4;154,153,155,157;,
  3;158,159,160;,
  3;160,159,161;,
  3;158,160,162;,
  3;162,160,161;,
  3;165,166,164;,
  3;166,167,164;,
  3;167,163,164;,
  4;168,166,165,169;,
  4;167,166,168,170;,
  3;171,172,173;,
  3;173,172,174;,
  3;171,173,175;,
  3;175,173,174;,
  3;178,179,177;,
  3;179,180,177;,
  3;180,176,177;,
  4;181,179,178,182;,
  4;180,179,181,183;,
  3;184,185,186;,
  3;186,185,187;,
  3;184,186,188;,
  3;188,186,187;,
  3;191,192,190;,
  3;190,192,189;,
  3;192,193,189;,
  4;194,192,191,195;,
  4;193,192,194,196;,
  3;197,198,199;,
  3;199,198,200;,
  3;197,199,201;,
  3;201,199,200;,
  3;204,205,203;,
  3;203,205,202;,
  3;205,206,202;,
  4;207,205,204,208;,
  4;206,205,207,209;,
  3;210,211,212;,
  3;212,211,213;,
  3;210,212,214;,
  3;214,212,213;,
  3;216,217,215;,
  3;215,217,219;,
  3;217,218,219;,
  4;220,218,217,221;,
  4;219,218,220,222;,
  3;223,224,225;,
  3;225,224,226;,
  3;223,225,227;,
  3;227,225,226;,
  3;230,231,229;,
  3;229,231,228;,
  3;231,232,228;,
  4;233,231,230,234;,
  4;232,231,233,235;,
  3;236,237,238;,
  3;238,237,239;,
  3;236,238,240;,
  3;240,238,239;,
  3;244,245,243;,
  3;245,241,243;,
  3;241,242,243;,
  4;246,244,243,247;,
  4;245,244,246,248;,
  3;249,250,251;,
  3;251,250,252;,
  3;249,251,253;,
  3;253,251,252;,
  3;257,258,256;,
  3;258,254,256;,
  3;254,255,256;,
  4;259,257,256,260;,
  4;258,257,259,261;,
  3;262,263,264;,
  3;264,263,265;,
  3;262,264,266;,
  3;266,264,265;,
  3;268,269,267;,
  3;267,269,271;,
  3;269,270,271;,
  4;272,270,269,273;,
  4;271,270,272,274;,
  3;275,276,277;,
  3;277,276,278;,
  3;275,277,279;,
  3;279,277,278;;
 }
 MeshTextureCoords {
  280;
  0.875000;0.625000;,
  0.823100;0.625000;,
  0.977200;0.750000;,
  0.875000;1.000000;,
  0.924100;1.000000;,
  0.875000;0.500000;,
  1.000000;0.642800;,
  0.823100;0.500000;,
  0.000000;0.375000;,
  0.125000;0.500000;,
  0.125000;0.375000;,
  0.250000;0.375000;,
  0.125000;0.250000;,
  0.750000;0.125000;,
  0.750000;0.239100;,
  0.875000;0.188800;,
  0.875000;0.125000;,
  1.000000;0.138400;,
  0.625000;0.000000;,
  0.625000;0.125000;,
  0.750000;0.000000;,
  0.875000;0.052800;,
  0.471500;0.111700;,
  0.612900;0.280200;,
  1.000000;0.107000;,
  0.750000;0.125000;,
  0.750000;0.239100;,
  0.875000;0.188800;,
  0.875000;0.125000;,
  1.000000;0.138400;,
  0.625000;0.000000;,
  0.625000;0.125000;,
  0.750000;0.000000;,
  0.875000;0.052800;,
  0.471500;0.111700;,
  0.612900;0.280200;,
  1.000000;0.107000;,
  0.750000;0.125000;,
  0.750000;0.239100;,
  0.875000;0.188800;,
  0.875000;0.125000;,
  1.000000;0.138400;,
  0.625000;0.000000;,
  0.625000;0.125000;,
  0.750000;0.000000;,
  0.875000;0.052800;,
  0.612900;0.280200;,
  0.471500;0.111700;,
  1.000000;0.107000;,
  0.750000;0.125000;,
  0.750000;0.239100;,
  0.875000;0.188800;,
  0.875000;0.125000;,
  1.000000;0.138400;,
  0.625000;0.000000;,
  0.625000;0.125000;,
  0.750000;0.000000;,
  0.875000;0.052800;,
  0.612900;0.280200;,
  0.471500;0.111700;,
  1.000000;0.107000;,
  0.750000;0.125000;,
  0.750000;0.239100;,
  0.875000;0.188800;,
  0.875000;0.125000;,
  1.000000;0.138400;,
  0.625000;0.000000;,
  0.625000;0.125000;,
  0.750000;0.000000;,
  0.875000;0.052800;,
  0.471500;0.111700;,
  0.612900;0.280200;,
  1.000000;0.107000;,
  0.750000;0.125000;,
  0.750000;0.239100;,
  0.875000;0.188800;,
  0.875000;0.125000;,
  1.000000;0.138400;,
  0.625000;0.000000;,
  0.625000;0.125000;,
  0.750000;0.000000;,
  0.875000;0.052800;,
  0.471500;0.111700;,
  0.612900;0.280200;,
  1.000000;0.107000;,
  0.977200;0.750000;,
  0.875000;0.625000;,
  0.924100;1.000000;,
  0.875000;1.000000;,
  0.823100;0.625000;,
  0.875000;0.500000;,
  1.000000;0.642800;,
  0.823100;0.500000;,
  0.000000;0.125000;,
  0.125000;0.250000;,
  0.125000;0.125000;,
  0.250000;0.125000;,
  0.125000;0.000000;,
  0.977200;0.750000;,
  0.875000;0.625000;,
  0.924100;1.000000;,
  0.875000;1.000000;,
  0.823100;0.625000;,
  0.875000;0.500000;,
  1.000000;0.642800;,
  0.823100;0.500000;,
  0.000000;0.625000;,
  0.125000;0.750000;,
  0.125000;0.625000;,
  0.250000;0.625000;,
  0.125000;0.500000;,
  0.924100;1.000000;,
  0.977200;0.750000;,
  0.875000;1.000000;,
  0.823100;0.625000;,
  0.875000;0.625000;,
  0.875000;0.500000;,
  1.000000;0.642800;,
  0.823100;0.500000;,
  0.000000;0.375000;,
  0.125000;0.500000;,
  0.125000;0.375000;,
  0.250000;0.375000;,
  0.125000;0.250000;,
  0.924100;1.000000;,
  0.977200;0.750000;,
  0.875000;1.000000;,
  0.875000;0.625000;,
  0.823100;0.625000;,
  0.875000;0.500000;,
  1.000000;0.642800;,
  0.823100;0.500000;,
  0.000000;0.625000;,
  0.125000;0.750000;,
  0.125000;0.625000;,
  0.250000;0.625000;,
  0.125000;0.500000;,
  0.977200;0.750000;,
  0.875000;0.625000;,
  0.924100;1.000000;,
  0.823100;0.625000;,
  0.875000;1.000000;,
  0.875000;0.500000;,
  1.000000;0.642800;,
  0.823100;0.500000;,
  0.000000;0.125000;,
  0.125000;0.250000;,
  0.125000;0.125000;,
  0.250000;0.125000;,
  0.125000;0.000000;,
  0.875000;1.000000;,
  0.924100;1.000000;,
  0.823100;0.625000;,
  0.977200;0.750000;,
  0.875000;0.625000;,
  0.875000;0.500000;,
  1.000000;0.642800;,
  0.823100;0.500000;,
  0.000000;0.625000;,
  0.125000;0.750000;,
  0.125000;0.625000;,
  0.250000;0.625000;,
  0.125000;0.500000;,
  0.977200;0.750000;,
  0.875000;0.625000;,
  0.924100;1.000000;,
  0.823100;0.625000;,
  0.875000;1.000000;,
  0.875000;0.500000;,
  1.000000;0.642800;,
  0.823100;0.500000;,
  0.000000;0.125000;,
  0.125000;0.250000;,
  0.125000;0.125000;,
  0.250000;0.125000;,
  0.125000;0.000000;,
  0.977200;0.750000;,
  0.875000;0.625000;,
  0.924100;1.000000;,
  0.823100;0.625000;,
  0.875000;1.000000;,
  0.875000;0.500000;,
  1.000000;0.642800;,
  0.823100;0.500000;,
  0.000000;0.625000;,
  0.125000;0.750000;,
  0.125000;0.625000;,
  0.250000;0.625000;,
  0.125000;0.500000;,
  0.977200;0.750000;,
  0.875000;0.625000;,
  0.924100;1.000000;,
  0.875000;1.000000;,
  0.823100;0.625000;,
  0.875000;0.500000;,
  1.000000;0.642800;,
  0.823100;0.500000;,
  0.000000;0.125000;,
  0.125000;0.250000;,
  0.125000;0.125000;,
  0.250000;0.125000;,
  0.125000;0.000000;,
  0.977200;0.750000;,
  0.875000;0.625000;,
  0.924100;1.000000;,
  0.875000;1.000000;,
  0.823100;0.625000;,
  0.875000;0.500000;,
  1.000000;0.642800;,
  0.823100;0.500000;,
  0.000000;0.625000;,
  0.125000;0.750000;,
  0.125000;0.625000;,
  0.250000;0.625000;,
  0.125000;0.500000;,
  0.924100;1.000000;,
  0.977200;0.750000;,
  0.875000;1.000000;,
  0.823100;0.625000;,
  0.875000;0.625000;,
  0.875000;0.500000;,
  1.000000;0.642800;,
  0.823100;0.500000;,
  0.000000;0.375000;,
  0.125000;0.500000;,
  0.125000;0.375000;,
  0.250000;0.375000;,
  0.125000;0.250000;,
  0.977200;0.750000;,
  0.875000;0.625000;,
  0.924100;1.000000;,
  0.875000;1.000000;,
  0.823100;0.625000;,
  0.875000;0.500000;,
  1.000000;0.642800;,
  0.823100;0.500000;,
  0.000000;0.625000;,
  0.125000;0.750000;,
  0.125000;0.625000;,
  0.250000;0.625000;,
  0.125000;0.500000;,
  0.875000;0.625000;,
  0.823100;0.625000;,
  0.977200;0.750000;,
  0.875000;1.000000;,
  0.924100;1.000000;,
  0.875000;0.500000;,
  1.000000;0.642800;,
  0.823100;0.500000;,
  0.000000;0.125000;,
  0.125000;0.250000;,
  0.125000;0.125000;,
  0.250000;0.125000;,
  0.125000;0.000000;,
  0.875000;0.625000;,
  0.823100;0.625000;,
  0.977200;0.750000;,
  0.875000;1.000000;,
  0.924100;1.000000;,
  0.875000;0.500000;,
  1.000000;0.642800;,
  0.823100;0.500000;,
  0.000000;0.125000;,
  0.125000;0.250000;,
  0.125000;0.125000;,
  0.250000;0.125000;,
  0.125000;0.000000;,
  0.924100;1.000000;,
  0.977200;0.750000;,
  0.875000;1.000000;,
  0.823100;0.625000;,
  0.875000;0.625000;,
  0.875000;0.500000;,
  1.000000;0.642800;,
  0.823100;0.500000;,
  0.000000;0.125000;,
  0.125000;0.250000;,
  0.125000;0.125000;,
  0.250000;0.125000;,
  0.125000;0.000000;;
 }
}
