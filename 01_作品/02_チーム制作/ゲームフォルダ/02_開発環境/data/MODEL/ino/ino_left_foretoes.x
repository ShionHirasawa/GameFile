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
 10;
 1.61991;-3.92985;-0.48591;,
 -1.20998;-3.92985;-0.41834;,
 -0.73983;-3.92985;-4.71225;,
 0.93694;-3.92985;-4.47572;,
 1.59771;2.32206;1.39108;,
 -0.98138;2.32206;1.50065;,
 -1.77416;-1.72085;-3.14185;,
 -0.72487;2.32206;-1.66533;,
 2.06108;-1.72477;-2.94965;,
 1.08163;2.32206;-1.56059;;
 
 9;
 4;0,1,2,3;,
 4;1,0,4,5;,
 4;6,1,5,7;,
 4;8,3,2,6;,
 4;8,6,7,9;,
 4;0,8,9,4;,
 3;1,6,2;,
 3;0,3,8;,
 4;5,4,9,7;;
 
 MeshMaterialList {
  5;
  9;
  3,
  0,
  0,
  3,
  0,
  0,
  3,
  3,
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
  12;
  0.000000;-1.000000;0.000000;,
  -0.988505;0.099421;0.113902;,
  0.051037;0.333852;-0.941243;,
  0.031738;-0.290661;0.956300;,
  -0.990726;-0.135720;0.006432;,
  0.064450;0.462862;-0.884084;,
  0.076568;0.582572;-0.809164;,
  0.988803;-0.120855;-0.087535;,
  0.991930;0.126237;-0.011802;,
  -0.926722;-0.361788;-0.101469;,
  0.919624;-0.359875;-0.157420;,
  0.000000;1.000000;0.000000;;
  9;
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;4,4,1,1;,
  4;5,6,6,5;,
  4;5,5,2,2;,
  4;7,7,8,8;,
  3;4,4,9;,
  3;7,10,7;,
  4;11,11,11,11;;
 }
 MeshTextureCoords {
  10;
  0.418760;1.000000;,
  0.401910;1.000000;,
  0.304080;1.000000;,
  0.320560;1.000000;,
  0.475590;0.858310;,
  0.457140;0.858310;,
  0.334240;0.923620;,
  0.362900;0.858310;,
  0.363750;0.923760;,
  0.381340;0.858310;;
 }
}
