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
 1.21797;-6.22921;-0.00442;,
 -1.77811;-6.22921;0.27711;,
 -1.98079;-6.22921;-5.05409;,
 0.29847;-6.22921;-5.02920;,
 1.33343;-3.77388;-3.28177;,
 -2.93162;-3.76835;-3.11558;,
 1.02778;2.14578;1.05020;,
 -0.88307;2.14781;1.18801;,
 -1.12612;2.39192;-0.99689;,
 1.22522;2.40596;-1.25013;;
 
 9;
 4;0,1,2,3;,
 4;4,3,2,5;,
 3;1,5,2;,
 3;0,3,4;,
 4;6,7,1,0;,
 4;7,8,5,1;,
 4;8,9,4,5;,
 4;9,6,0,4;,
 4;7,6,9,8;;
 
 MeshMaterialList {
  5;
  9;
  3,
  3,
  3,
  3,
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
  12;
  0.000000;-1.000000;0.000000;,
  -0.041092;0.469555;-0.881946;,
  0.082087;-0.120241;0.989345;,
  -0.956726;0.149813;0.249462;,
  -0.070006;0.321955;-0.944163;,
  -0.011088;0.604716;-0.796364;,
  -0.981305;-0.122341;0.148572;,
  -0.922619;-0.384114;0.035076;,
  0.988861;-0.138524;-0.054446;,
  0.945616;-0.275440;-0.173040;,
  0.997812;0.003196;0.066035;,
  0.007610;0.993738;0.111479;;
  9;
  4;0,0,0,0;,
  4;1,5,5,1;,
  3;6,6,7;,
  3;8,9,8;,
  4;2,2,2,2;,
  4;3,3,6,6;,
  4;4,4,1,1;,
  4;10,10,8,8;,
  4;11,11,11,11;;
 }
 MeshTextureCoords {
  10;
  0.922560;1.000000;,
  0.900940;1.000000;,
  0.778960;1.000000;,
  0.800780;1.000000;,
  0.849810;0.915110;,
  0.813750;0.914920;,
  0.969270;0.758080;,
  0.954540;0.758010;,
  0.841260;0.842410;,
  0.857490;0.841920;;
 }
}
