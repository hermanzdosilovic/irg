#include "bresenham.h"

void swap(int *a, int *b) {
  int x = *a;
  *a = *b;
  *b = x;
}

void drawLine(int x1, int y1, int x2, int y2) {
  if (x1 > x2) {
    swap(&x1, &x2);
    swap(&y1, &y2);
  }
  
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_POINTS);

  double dx = (x2 - x1);
  double dy = (y2 - y1);
  
  if (dx >= dy && dy > 0) {
    printf("[0, 45]\n");
    double k = dy/dx;
    int yc = y1;
    double yf = -0.5;
    for (int xi = x1; xi <= x2; xi++) {
      glVertex2i(xi, yc);
      yf += k;
      if (yf >= 0) {
        yc++;
        yf -= 1;
      }
    }
  } else if (dx < dy && dy > 0) {
    printf("(45, 90]\n");
    swap(&x1, &y1);
    swap(&x2, &y2);
    double k = dx/dy;
    int yc = y1;
    double yf = -0.5;
    for (int xi = x1; xi <= x2; xi++) {
      glVertex2i(yc, xi);
      yf += k;
      if (yf >= 0) {
        yc++;
        yf -= 1;
      }
    }
  } else if (dx >= -dy) {
    printf("[-45, 0)\n");
    double k = dy/dx;
    int yc = y1;
    double yf = 0.5;
    for (int xi = x1; xi <= x2; xi++) {
      glVertex2i(xi, yc);
      yf += k;
      if (yf <= 0) {
        yc--;
        yf += 1;
      }
    }
  } else {
    printf("[-90, -45)\n");
    swap(&x1, &y1);
    swap(&x2, &y2);
    if (x1 > x2) {
      swap(&x1, &x2);
      swap(&y1, &y2);
    }
    double k = dx/dy;
    int yc = y1;
    double yf = 0.5;
    for (int xi = x1; xi <= x2; xi++) {
      glVertex2i(yc, xi);
      yf += k;
      if (yf <= 0) {
        yc--;
        yf += 1;
      }
    }
  }
  glEnd();
  glFlush();
}
