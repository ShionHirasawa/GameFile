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
 12;
 -2.66072;-2.77992;3.59355;,
 1.81161;-2.23740;3.59355;,
 3.08610;3.61256;1.75567;,
 -3.01815;1.15289;4.16281;,
 -2.66072;-2.77992;-1.56286;,
 -3.01815;2.56194;-2.13211;,
 1.81161;-2.23740;-1.56286;,
 1.93869;4.29955;-2.13211;,
 0.02195;-6.85321;3.20668;,
 -3.23716;-6.85321;2.45021;,
 -1.82020;-6.85321;-1.41510;,
 0.39974;-6.85321;-0.65857;;
 
 13;
 4;0,1,2,3;,
 4;4,0,3,5;,
 4;6,4,5,7;,
 4;1,6,7,2;,
 4;8,1,0,9;,
 4;9,0,4,10;,
 4;10,4,6,11;,
 4;11,6,1,8;,
 3;3,2,5;,
 3;2,3,5;,
 3;2,5,7;,
 3;5,2,7;,
 4;8,9,10,11;;
 
 MeshMaterialList {
  5;
  13;
  0,
  0,
  0,
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
  19;
  -0.996809;-0.079452;-0.007654;,
  0.021679;-0.096215;-0.995124;,
  0.149493;0.047133;0.987639;,
  -0.994278;-0.055876;-0.091040;,
  0.099387;-0.090932;-0.990885;,
  0.022251;-0.058600;0.998034;,
  -0.984275;-0.031881;-0.173742;,
  0.176490;-0.085096;-0.980617;,
  -0.105602;-0.162722;0.981004;,
  0.971247;-0.232469;-0.051362;,
  0.980522;-0.130204;-0.147046;,
  0.942837;-0.330155;0.045335;,
  -0.293548;0.932860;0.208811;,
  -0.306812;0.922985;0.232304;,
  0.306812;-0.922985;-0.232304;,
  0.293548;-0.932860;-0.208811;,
  0.319822;-0.912348;-0.255606;,
  -0.319822;0.912348;0.255606;,
  0.000000;-1.000000;0.000000;;
  13;
  4;5,5,2,2;,
  4;3,3,0,0;,
  4;4,4,1,1;,
  4;9,9,10,10;,
  4;8,5,5,8;,
  4;6,3,3,6;,
  4;7,4,4,7;,
  4;11,9,9,11;,
  3;12,13,13;,
  3;14,15,14;,
  3;14,14,16;,
  3;13,13,17;,
  4;18,18,18,18;;
 }
 MeshTextureCoords {
  12;
  0.485020;0.717480;,
  0.485020;0.698720;,
  0.447240;0.598550;,
  0.528820;0.716750;,
  0.359290;0.717480;,
  0.315500;0.649040;,
  0.359290;0.698720;,
  0.315500;0.565540;,
  0.475590;0.858310;,
  0.457140;0.858310;,
  0.362900;0.858310;,
  0.381340;0.858310;;
 }
}
