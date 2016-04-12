# Drawing 3D Objects

Given the geometric and topologic data and some arbitrary
point draw an object and answer whether the point is inside or outside the body of the object.

Objects can be found in [`objects`](https://github.com/hermanzdosilovic/irg/tree/master/lab4/objects) directory.

Program expects two arguments:

* file with geometric and topologic data
* file with point coordinates

## Controls

* __Left mouse button hold and mouse move__: move object around

* __Right mouse button hold and mouse move__: rotate object

* __Scroll wheel up/down__: zoom in or out
 
## CLI Output

For every polygon program outputs its vector normal and for every test point program outputs "inside" if point is inside of object, or "outside" if point is outside of object.

![image](https://raw.githubusercontent.com/hermanzdosilovic/irg/master/lab4/kocka.png)

```
0, -1, 0
0, -1, 0
1, 0, 0
1, 0, -0
0, 1, -0
0, 1, 0
-1, 0, 0
-1, 0, 0
0, 0, -1
0, 0, -1
0, 0, 1
0, 0, 1
Point (0.500000, 0.500000, 0.500000) is inside
Point (4.000000, 4.000000, 4.000000) is outside
Point (1.000000, 1.000000, 1.000000) is inside
```

## Examples

### Teapot

`./task objects/teapot.obj points.in`

![image](https://raw.githubusercontent.com/hermanzdosilovic/irg/master/lab4/teapot.png)

### Teddy

`./task objects/teddy.png points.in`

![image](https://raw.githubusercontent.com/hermanzdosilovic/irg/master/lab4/teddy.png)
