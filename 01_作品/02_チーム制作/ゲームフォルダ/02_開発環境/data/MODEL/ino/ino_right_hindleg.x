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
 17;
 1.43851;-1.22214;2.91414;,
 3.02934;5.50600;3.23533;,
 -2.47908;6.43965;2.13757;,
 -2.29243;-0.72964;3.01591;,
 2.26068;-2.98775;-2.51386;,
 3.79208;1.19316;-3.45910;,
 -2.79600;-1.67179;-2.32687;,
 -2.81726;4.14777;-4.21214;,
 -1.44721;-3.56247;4.23940;,
 2.20068;-3.56911;4.01193;,
 1.83674;-4.08749;-0.81644;,
 -2.16307;-4.08082;-0.44835;,
 -1.29183;-4.58052;4.10574;,
 2.05821;-4.57849;3.87960;,
 1.12545;-7.01953;-0.90297;,
 -1.82875;-7.00549;-0.56939;,
 3.76922;1.21369;-3.46445;;
 
 14;
 4;0,1,2,3;,
 4;4,5,1,0;,
 4;6,7,5,4;,
 4;3,2,7,6;,
 4;8,9,0,3;,
 4;9,10,4,0;,
 4;10,11,6,4;,
 4;11,8,3,6;,
 4;12,13,9,8;,
 4;13,14,10,9;,
 4;14,15,11,10;,
 4;15,12,8,11;,
 4;7,2,1,16;,
 4;13,12,15,14;;
 
 MeshMaterialList {
  5;
  14;
  0,
  0,
  0,
  0,
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
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.300000;0.300000;0.300000;;
   TextureFilename {
    "data\\TEXTURE\\ino-body.jpg";
   }
  }
  Material {
   0.502000;0.251000;0.416000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.301200;0.150600;0.249600;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   50.000000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.102000;0.059000;0.051000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.061200;0.035400;0.030600;;
  }
  Material {
   1.000000;0.949000;0.675000;1.000000;;
   30.000000;
   0.300000;0.300000;0.300000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  18;
  0.933559;-0.260730;0.245943;,
  -0.057616;-0.249529;-0.966652;,
  -0.078162;0.031644;0.996438;,
  0.985133;-0.089715;0.146506;,
  -0.108820;-0.502697;-0.857586;,
  -0.005132;0.234689;0.972057;,
  0.998705;-0.049351;-0.012340;,
  -0.138808;-0.370809;-0.918277;,
  0.068950;0.154681;0.985555;,
  0.058333;-0.576723;0.814855;,
  0.980990;-0.182720;-0.065369;,
  -0.101742;0.039921;-0.994009;,
  0.330702;0.845282;-0.419685;,
  -0.989481;-0.092959;0.110846;,
  -0.997204;0.003337;0.074658;,
  -0.973258;-0.166486;0.158276;,
  -0.974673;-0.144601;0.170598;,
  0.039061;-0.890834;0.452646;;
  14;
  4;5,2,2,5;,
  4;3,0,0,3;,
  4;4,1,1,4;,
  4;13,14,14,13;,
  4;8,8,5,5;,
  4;6,6,3,3;,
  4;7,7,4,4;,
  4;15,15,13,13;,
  4;9,9,8,8;,
  4;10,10,6,6;,
  4;11,11,7,7;,
  4;16,16,15,15;,
  4;12,12,12,12;,
  4;9,9,17,17;;
 }
 MeshTextureCoords {
  17;
  0.932870;0.641960;,
  0.950400;0.506570;,
  0.937040;0.468700;,
  0.945630;0.624940;,
  0.799100;0.703010;,
  0.757690;0.681490;,
  0.817590;0.657510;,
  0.757690;0.561660;,
  0.972940;0.722880;,
  0.957360;0.723110;,
  0.841390;0.741030;,
  0.861350;0.740800;,
  0.969270;0.758080;,
  0.954540;0.758010;,
  0.841260;0.842410;,
  0.857490;0.841920;,
  0.000000;0.000000;;
 }
}
