# Filling convex polygons

Implementation of convex polygon filling. Also, for a given point _(x, y)_ check its relationship with polygons. Every point can be either _inside_ or _outside_ of polygon.

![image](https://raw.githubusercontent.com/hermanzdosilovic/irg/master/lab3/convex.png)

CLI output for this image:

```
Drawing polygon P1.
  Success.
Drawing polygon P2.
  Success.
Point (199, 156) relation with polygons:
  Inside of polygon P0
  Outside of polygon P1
```

## Controls

* __Left mouse button click__: choose vertices of polygon
* __Right mouse button click__: draw polygon with previously choosen vertices
* __Middle mouse button click__: draw point and check its relationship with already defined polygons

## CLI Output

For every polygon outputs status message. For every choosen point output relationship with every polygon.

## Concave polygons

Example with concave polygons, with correct and expected behaviour.

![image](https://raw.githubusercontent.com/hermanzdosilovic/irg/master/lab3/concave.png)

```
Drawing polygon P1.
  Success.
Drawing polygon P2.
  Failed. Polygon has no vertices.
Drawing polygon P2.
  Success.
Point (108, 120) relation with polygons:
  Inside of polygon P0
  Outside of polygon P1
Point (297, 187) relation with polygons:
  Outside of polygon P0
  Inside of polygon P1
Point (365, 257) relation with polygons:
  Outside of polygon P0
  Outside of polygon P1
```
