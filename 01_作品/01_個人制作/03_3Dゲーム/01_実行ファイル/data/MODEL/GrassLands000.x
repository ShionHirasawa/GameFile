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
 28;
 -49.98980;0.35761;-132.80883;,
 -46.79073;0.35761;-102.52289;,
 -36.65288;0.35761;-74.53005;,
 -7.11150;0.35761;-74.53005;,
 -35.44619;0.35761;-18.09391;,
 -7.11150;0.35761;-29.99201;,
 1.47164;0.36151;-33.39478;,
 1.49687;0.35761;-74.53005;,
 -39.79458;0.47962;63.15815;,
 -6.40871;0.47962;22.56803;,
 1.49687;0.35761;21.43693;,
 -46.79073;0.35761;123.10502;,
 -24.27607;0.35761;123.10502;,
 1.49687;0.35761;123.10502;,
 1.45046;-23.67915;-33.39541;,
 1.47570;-23.68306;21.43631;,
 1.44074;-63.36627;21.43528;,
 1.41551;-63.36237;-33.39644;,
 1.44074;-55.17693;-74.53177;,
 1.47570;-20.59352;-74.53070;,
 3.76797;-12.38378;113.18805;,
 3.73302;-33.41562;96.81840;,
 1.44074;-36.93892;-97.13860;,
 1.47570;-13.71299;-119.35177;,
 1.49687;0.35761;-132.80883;,
 1.49687;0.35761;-132.80883;,
 -31.79902;0.00000;-145.34472;,
 -49.98980;0.35761;-132.80883;;
 
 18;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;3,5,6,7;,
 4;5,4,8,9;,
 4;5,9,10,6;,
 4;9,8,11,12;,
 4;9,12,13,10;,
 4;14,6,10,15;,
 4;14,15,16,17;,
 4;14,17,18,19;,
 4;14,19,7,6;,
 4;15,10,13,20;,
 4;15,20,21,16;,
 4;19,18,22,23;,
 4;19,23,24,7;,
 3;24,0,3;,
 3;24,3,7;,
 3;25,26,27;;
 
 MeshMaterialList {
  2;
  18;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.269600;0.800000;0.244800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.091200;0.800000;0.292000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  29;
  0.000941;0.999998;0.001792;,
  0.999543;0.009389;-0.028719;,
  0.000000;1.000000;0.000000;,
  0.999999;-0.001388;-0.000543;,
  0.999999;-0.001167;-0.000028;,
  1.000000;-0.000913;0.000014;,
  -0.000731;0.999999;-0.000979;,
  -0.000260;1.000000;-0.000083;,
  0.999884;0.004255;-0.014591;,
  0.007716;0.999970;0.000607;,
  0.007519;0.999972;-0.000309;,
  0.003548;0.999994;-0.000637;,
  -0.000114;1.000000;-0.000024;,
  0.999999;-0.001258;-0.000078;,
  0.000000;0.999955;-0.009506;,
  -0.000045;1.000000;-0.000009;,
  0.001409;0.999999;-0.000808;,
  0.003630;0.999993;-0.000196;,
  0.004329;0.999990;0.001199;,
  1.000000;-0.000913;0.000037;,
  0.999660;0.023838;-0.010539;,
  0.999999;-0.001134;0.000112;,
  0.998607;0.048559;-0.020618;,
  0.999999;-0.001323;-0.000310;,
  0.000000;0.999593;-0.028515;,
  1.000000;-0.000913;0.000060;,
  0.999036;0.043429;-0.006478;,
  0.999999;-0.001102;0.000251;,
  0.996073;0.087651;-0.012484;;
  18;
  4;14,2,2,15;,
  4;15,2,6,16;,
  4;15,16,11,12;,
  4;16,6,7,17;,
  4;16,17,10,11;,
  4;17,7,0,18;,
  4;17,18,9,10;,
  4;19,25,26,20;,
  4;19,20,8,5;,
  4;19,5,4,21;,
  4;19,21,27,25;,
  4;20,26,28,22;,
  4;20,22,1,8;,
  4;21,4,3,23;,
  4;21,23,13,27;,
  3;14,14,15;,
  3;14,15,12;,
  3;14,24,14;;
 }
 MeshTextureCoords {
  28;
  0.375750;1.000000;,
  0.000000;1.000000;,
  0.000000;0.897340;,
  0.375750;0.897340;,
  0.000000;0.695120;,
  0.375750;0.695120;,
  0.519420;0.695120;,
  0.519420;0.897340;,
  0.000000;0.461610;,
  0.375750;0.461610;,
  0.519420;0.461610;,
  0.000000;0.000000;,
  0.375750;0.000000;,
  0.519420;0.000000;,
  0.700720;0.695120;,
  0.700720;0.461610;,
  1.000000;0.461610;,
  1.000000;0.695120;,
  1.000000;0.897340;,
  0.700720;0.897340;,
  0.700720;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.700720;1.000000;,
  0.519420;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
