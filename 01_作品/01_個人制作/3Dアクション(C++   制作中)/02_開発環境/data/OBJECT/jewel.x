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
 51;
 0.00000;40.00000;12.00000;,
 0.00000;32.86821;20.00000;,
 10.00000;32.86821;17.32051;,
 6.00000;40.00000;10.39231;,
 17.32051;32.86821;10.00000;,
 10.39231;40.00000;6.00000;,
 20.00000;32.86821;-0.00000;,
 12.00000;40.00000;-0.00000;,
 17.32051;32.86821;-10.00000;,
 10.39231;40.00000;-6.00000;,
 10.00000;32.86821;-17.32051;,
 6.00000;40.00000;-10.39231;,
 0.00000;32.86821;-20.00000;,
 0.00000;40.00000;-12.00000;,
 -10.00000;32.86821;-17.32051;,
 -6.00000;40.00000;-10.39231;,
 -17.32051;32.86821;-10.00001;,
 -10.39230;40.00000;-6.00000;,
 -20.00000;32.86821;-0.00001;,
 -12.00000;40.00000;-0.00001;,
 -17.32051;32.86821;9.99999;,
 -10.39231;40.00000;5.99999;,
 -10.00001;32.86821;17.32050;,
 -6.00001;40.00000;10.39230;,
 0.00000;32.86821;20.00000;,
 0.00000;40.00000;12.00000;,
 0.00000;8.00000;-0.00001;,
 0.00000;8.00000;-0.00001;,
 0.00000;8.00000;-0.00001;,
 0.00000;8.00000;-0.00001;,
 0.00000;8.00000;-0.00001;,
 0.00000;8.00000;-0.00001;,
 0.00000;8.00000;-0.00001;,
 0.00000;8.00000;-0.00001;,
 0.00000;8.00000;-0.00001;,
 0.00000;8.00000;-0.00001;,
 0.00000;8.00000;-0.00001;,
 0.00000;8.00000;-0.00001;,
 0.00000;40.00000;-0.00000;,
 0.00000;40.00000;12.00000;,
 6.00000;40.00000;10.39231;,
 10.39231;40.00000;6.00000;,
 12.00000;40.00000;-0.00000;,
 10.39231;40.00000;-6.00000;,
 6.00000;40.00000;-10.39231;,
 0.00000;40.00000;-12.00000;,
 -6.00000;40.00000;-10.39231;,
 -10.39230;40.00000;-6.00000;,
 -12.00000;40.00000;-0.00001;,
 -10.39231;40.00000;5.99999;,
 -6.00001;40.00000;10.39230;;
 
 36;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,16,17;,
 4;17,16,18,19;,
 4;19,18,20,21;,
 4;21,20,22,23;,
 4;23,22,24,25;,
 3;1,26,2;,
 3;2,27,4;,
 3;4,28,6;,
 3;6,29,8;,
 3;8,30,10;,
 3;10,31,12;,
 3;12,32,14;,
 3;14,33,16;,
 3;16,34,18;,
 3;18,35,20;,
 3;20,36,22;,
 3;22,37,24;,
 3;38,39,40;,
 3;38,40,41;,
 3;38,41,42;,
 3;38,42,43;,
 3;38,43,44;,
 3;38,44,45;,
 3;38,45,46;,
 3;38,46,47;,
 3;38,47,48;,
 3;38,48,49;,
 3;38,49,50;,
 3;38,50,39;;
 
 MeshMaterialList {
  1;
  36;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.379608;0.787451;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  39;
  -0.000000;0.935522;0.353267;,
  0.176634;0.935522;0.305938;,
  0.305938;0.935522;0.176634;,
  0.353267;0.935522;0.000000;,
  0.305938;0.935522;-0.176634;,
  0.176634;0.935522;-0.305938;,
  0.000000;0.935522;-0.353267;,
  -0.176633;0.935523;-0.305938;,
  -0.305938;0.935522;-0.176634;,
  -0.353267;0.935522;-0.000000;,
  -0.305939;0.935522;0.176633;,
  -0.176634;0.935522;0.305938;,
  -0.000000;0.746450;0.665441;,
  0.332721;0.746450;0.576289;,
  0.576289;0.746450;0.332721;,
  0.665441;0.746450;0.000000;,
  0.576289;0.746450;-0.332721;,
  0.332721;0.746450;-0.576289;,
  0.000000;0.746450;-0.665441;,
  -0.332720;0.746450;-0.576289;,
  -0.576289;0.746450;-0.332721;,
  -0.665441;0.746450;-0.000000;,
  -0.576289;0.746450;0.332720;,
  -0.332721;0.746450;0.576289;,
  0.669236;-0.721087;0.179321;,
  0.000000;1.000000;0.000000;,
  0.000000;-0.626708;0.779255;,
  0.389627;-0.626708;0.674854;,
  0.674854;-0.626708;0.389627;,
  0.779255;-0.626708;-0.000000;,
  0.674854;-0.626707;-0.389627;,
  0.389627;-0.626707;-0.674854;,
  -0.489915;-0.721087;-0.489915;,
  0.000000;-0.626707;-0.779255;,
  -0.389627;-0.626707;-0.674854;,
  -0.674854;-0.626707;-0.389627;,
  -0.779255;-0.626708;-0.000000;,
  -0.674854;-0.626708;0.389627;,
  -0.389628;-0.626708;0.674854;;
  36;
  4;0,12,13,1;,
  4;1,13,14,2;,
  4;2,14,15,3;,
  4;3,15,16,4;,
  4;4,16,17,5;,
  4;5,17,18,6;,
  4;6,18,19,7;,
  4;7,19,20,8;,
  4;8,20,21,9;,
  4;9,21,22,10;,
  4;10,22,23,11;,
  4;11,23,12,0;,
  3;26,24,27;,
  3;27,24,28;,
  3;28,24,29;,
  3;29,24,30;,
  3;30,24,31;,
  3;31,32,33;,
  3;33,32,34;,
  3;34,32,35;,
  3;35,32,36;,
  3;36,32,37;,
  3;37,38,38;,
  3;38,38,26;,
  3;25,0,1;,
  3;25,1,2;,
  3;25,2,3;,
  3;25,3,4;,
  3;25,4,5;,
  3;25,5,6;,
  3;25,6,7;,
  3;25,7,8;,
  3;25,8,9;,
  3;25,9,10;,
  3;25,10,11;,
  3;25,11,0;;
 }
 MeshTextureCoords {
  51;
  0.000000;1.000000;,
  0.000000;0.500000;,
  0.083333;0.500000;,
  0.083333;1.000000;,
  0.166667;0.500000;,
  0.166667;1.000000;,
  0.250000;0.500000;,
  0.250000;1.000000;,
  0.333333;0.500000;,
  0.333333;1.000000;,
  0.416667;0.500000;,
  0.416667;1.000000;,
  0.500000;0.500000;,
  0.500000;1.000000;,
  0.583333;0.500000;,
  0.583333;1.000000;,
  0.666667;0.500000;,
  0.666667;1.000000;,
  0.750000;0.500000;,
  0.750000;1.000000;,
  0.833333;0.500000;,
  0.833333;1.000000;,
  0.916667;0.500000;,
  0.916667;1.000000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.041667;0.000000;,
  0.083333;0.000000;,
  0.125000;0.000000;,
  0.166667;0.000000;,
  0.208333;0.000000;,
  0.250000;0.000000;,
  0.291667;0.000000;,
  0.333333;0.000000;,
  0.375000;0.000000;,
  0.416667;0.000000;,
  0.458333;0.000000;,
  0.500000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
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
