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
 30;
 -72.07102;0.00000;52.48837;,
 -43.24259;0.00000;44.55787;,
 -60.40018;0.00000;29.10830;,
 -100.66695;0.00000;29.10830;,
 -20.91641;0.00000;24.95673;,
 -20.13339;0.00000;0.98541;,
 26.35627;0.00000;29.40698;,
 20.13339;0.00000;0.98541;,
 43.24259;0.00000;52.48837;,
 60.40018;0.00000;29.10830;,
 72.07102;0.00000;52.48837;,
 100.66695;0.00000;29.10830;,
 -70.14877;0.00000;5.72821;,
 -126.61477;0.00000;-4.34653;,
 -23.38292;0.00000;-11.16609;,
 23.38292;0.00000;-11.16609;,
 70.14877;0.00000;5.72821;,
 116.91463;0.00000;-0.85809;,
 -70.14877;0.00000;-29.72401;,
 -23.38292;0.00000;-29.72401;,
 -23.38292;0.00000;-53.06588;,
 -70.14877;0.00000;-53.06588;,
 -111.93773;0.00000;-48.53779;,
 -123.25911;0.00000;-33.72379;,
 23.38292;0.00000;-29.72401;,
 23.38292;0.00000;-53.06588;,
 70.14877;0.00000;-29.72401;,
 70.14877;0.00000;-53.06588;,
 116.91463;0.00000;-32.33884;,
 108.33172;0.00000;-47.50932;;
 
 20;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;3,2,12,13;,
 4;2,5,14,12;,
 4;5,7,15,14;,
 4;7,9,16,15;,
 4;9,11,17,16;,
 4;18,12,14,19;,
 4;18,19,20,21;,
 4;18,21,22,23;,
 4;18,23,13,12;,
 4;19,14,15,24;,
 4;19,24,25,20;,
 4;24,15,16,26;,
 4;24,26,27,25;,
 4;26,16,17,28;,
 4;26,28,29,27;;
 
 MeshMaterialList {
  2;
  20;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
  1;
  0.000000;1.000000;0.000000;;
  20;
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;;
 }
 MeshTextureCoords {
  30;
  0.000000;0.000000;,
  0.200000;0.000000;,
  0.200000;0.200000;,
  0.000000;0.200000;,
  0.400000;0.000000;,
  0.400000;0.200000;,
  0.600000;0.000000;,
  0.600000;0.200000;,
  0.800000;0.000000;,
  0.800000;0.200000;,
  1.000000;0.000000;,
  1.000000;0.200000;,
  0.200000;0.400000;,
  0.000000;0.400000;,
  0.400000;0.400000;,
  0.600000;0.400000;,
  0.800000;0.400000;,
  1.000000;0.400000;,
  0.200000;0.761794;,
  0.400000;0.761794;,
  0.400000;1.000000;,
  0.200000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.761794;,
  0.600000;0.761794;,
  0.600000;1.000000;,
  0.800000;0.761794;,
  0.800000;1.000000;,
  1.000000;0.761794;,
  1.000000;1.000000;;
 }
}
