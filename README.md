# MGM
'Minimal Game/Graphics Math', a C and C++ Math library.

### Things to know when using this:
- You can define MGM_Z_ZERO_TO_ONE to make projection matrices use a Z from 0 to 1, instead of the default -1 to 1.
- Matrices and vectors are column major.

### Installation:
Add mgm.h to your source directory and add the following line to the file you want to use MGM in.
```sh
 #include "mgm.h"
```
Done :)

### Features:
- Column major matrices and vectors
- 2D, 3D, and 4D vectors (float2, float3, float4)
- 2x2, 3x3, and 4x4 matrices (float2x2, float3x3, float4x4)
- Minimal Quaternion support (quatf)
- Fully C compatible
- Single header file
- Compiles fast
- Compiling as C++ adds operator overloading and function overloading
- No templates and no complex macros

### TODO:
- Extend matrix functionality
- Extend quaternion funcitonality
