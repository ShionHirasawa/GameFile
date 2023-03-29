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
 33;
 0.00000;0.46299;0.00000;,
 21.27970;0.46299;-51.37375;,
 0.00000;0.46299;-55.60652;,
 0.00000;0.46299;0.00000;,
 40.00000;0.46300;-39.32000;,
 0.00000;0.46299;0.00000;,
 40.00000;0.46300;-21.28000;,
 0.00000;0.46299;0.00000;,
 40.00000;0.46300;0.00000;,
 0.00000;0.46299;0.00000;,
 40.00000;0.46300;21.28000;,
 0.00000;0.46299;0.00000;,
 40.00000;0.46300;39.32000;,
 0.00000;0.46299;0.00000;,
 21.27970;0.46299;51.37375;,
 0.00000;0.46299;0.00000;,
 0.00001;0.46299;55.60652;,
 0.00000;0.46299;0.00000;,
 -21.27969;0.46299;51.37376;,
 0.00000;0.46299;0.00000;,
 -32.20057;0.46299;46.38825;,
 0.00000;0.46299;0.00000;,
 -44.25458;0.46299;26.61405;,
 0.00000;0.46299;0.00000;,
 -33.92360;0.46299;3.15796;,
 0.00000;0.46299;0.00000;,
 -33.73043;0.46299;-21.27971;,
 0.00000;0.46299;0.00000;,
 -35.33959;0.46299;-35.73785;,
 0.00000;0.46299;0.00000;,
 -21.27968;0.46299;-51.37376;,
 0.00000;0.46299;0.00000;,
 0.00000;0.46299;-55.60652;;
 
 16;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 3;15,16,14;,
 3;17,18,16;,
 3;19,20,18;,
 3;21,22,20;,
 3;23,24,22;,
 3;25,26,24;,
 3;27,28,26;,
 3;29,30,28;,
 3;31,32,30;;
 
 MeshMaterialList {
  2;
  16;
  0,
  0,
  0,
  0,
  0,
  0,
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
  7;
  -0.000000;1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.000000;1.000000;-0.000000;,
  -0.000000;1.000000;-0.000000;,
  -0.000000;1.000000;0.000000;,
  -0.000000;1.000000;0.000000;,
  -0.000000;1.000000;0.000000;;
  16;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,4,4;,
  3;0,4,4;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,1,6;,
  3;0,1,1;,
  3;0,1,1;,
  3;0,1,1;,
  3;0,1,1;,
  3;0,1,1;,
  3;0,1,1;,
  3;0,1,1;,
  3;0,1,1;;
 }
 MeshTextureCoords {
  33;
  0.031250;0.000000;,
  0.062500;0.000000;,
  0.000000;0.000000;,
  0.093750;0.000000;,
  0.125000;0.000000;,
  0.156250;0.000000;,
  0.187500;0.000000;,
  0.218750;0.000000;,
  0.250000;0.000000;,
  0.281250;0.000000;,
  0.312500;0.000000;,
  0.343750;0.000000;,
  0.375000;0.000000;,
  0.406250;0.000000;,
  0.437500;0.000000;,
  0.468750;0.000000;,
  0.500000;0.000000;,
  0.531250;0.000000;,
  0.562500;0.000000;,
  0.593750;0.000000;,
  0.625000;0.000000;,
  0.656250;0.000000;,
  0.687500;0.000000;,
  0.718750;0.000000;,
  0.750000;0.000000;,
  0.781250;0.000000;,
  0.812500;0.000000;,
  0.843750;0.000000;,
  0.875000;0.000000;,
  0.906250;0.000000;,
  0.937500;0.000000;,
  0.968750;0.000000;,
  1.000000;0.000000;;
 }
}
