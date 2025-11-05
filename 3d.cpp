#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <cmath>
using namespace std;

// ================= Base Class =================
class Shape {
public:
    int color;
    Shape(int c = WHITE) { color = c; }
    virtual void draw(int x1, int y1, int x2, int y2) = 0; // pure virtual
    void setFill() {
        setcolor(color);
        setfillstyle(SOLID_FILL, color);
    }
};

// ================= 2D Shapes =================
class LineShape : public Shape {
public:
    LineShape(int c = WHITE) : Shape(c) {}
    void draw(int x1, int y1, int x2, int y2) {
        setFill();
        line(x1, y1, x2, y2);
    }
};

class RectangleShape : public Shape {
public:
    RectangleShape(int c = WHITE) : Shape(c) {}
    void draw(int x1, int y1, int x2, int y2) {
        setFill();
        rectangle(x1, y1, x2, y2);
        floodfill((x1+x2)/2, (y1+y2)/2, color);
    }
};

class CircleShape : public Shape {
public:
    CircleShape(int c = WHITE) : Shape(c) {}
    void draw(int x1, int y1, int x2, int y2) {
        setFill();
        int r = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
        circle(x1, y1, r);
        floodfill(x1, y1, color);
    }
};

// ================= 3D Shapes =================
class CubeShape : public Shape {
public:
    CubeShape(int c = WHITE) : Shape(c) {}
    void draw(int x1, int y1, int x2, int y2) {
        setFill();
        int size = abs(x2 - x1);
        int offset = size / 2;

        // Front square
        rectangle(x1, y1, x1+size, y1+size);
        floodfill(x1+size/2, y1+size/2, color);

        // Back square
        rectangle(x1+offset, y1-offset, x1+size+offset, y1+size-offset);

        // Connect corners
        line(x1, y1, x1+offset, y1-offset);
        line(x1+size, y1, x1+size+offset, y1-offset);
        line(x1, y1+size, x1+offset, y1+size-offset);
        line(x1+size, y1+size, x1+size+offset, y1+size-offset);
    }
};

class CylinderShape : public Shape {
public:
    CylinderShape(int c = WHITE) : Shape(c) {}
    void draw(int x1, int y1, int x2, int y2) {
        setFill();
        int r = abs(x2 - x1) / 2;
        int h = abs(y2 - y1);
        int cx = (x1 + x2) / 2;

        // Top ellipse
        ellipse(cx, y1, 0, 360, r, r/2);
        floodfill(cx, y1, color);

        // Bottom ellipse
        ellipse(cx, y1+h, 0, 180, r, r/2);

        // Sides
        line(cx-r, y1, cx-r, y1+h);
        line(cx+r, y1, cx+r, y1+h);

        // Fill body
        bar(cx-r, y1, cx+r, y1+h);
    }
};

class ConeShape : public Shape {
public:
    ConeShape(int c = WHITE) : Shape(c) {}
    void draw(int x1, int y1, int x2, int y2) {
        setFill();
        int r = abs(x2 - x1) / 2;
        int h = abs(y2 - y1);
        int cx = (x1 + x2) / 2;

        // Base ellipse
        ellipse(cx, y1+h, 0, 360, r, r/2);
        floodfill(cx, y1+h, color);

        // Lines to top
        line(cx-r, y1+h, cx, y1);
        line(cx+r, y1+h, cx, y1);

        // Fill cone body
        floodfill(cx, (y1+y1+h)/2, color);
    }
};

class SphereShape : public Shape {
public:
    SphereShape(int c = WHITE) : Shape(c) {}
    void draw(int x1, int y1, int x2, int y2) {
        setFill();
        int r = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));

        circle(x1, y1, r);
        floodfill(x1, y1, color);

        // Shading effect
        setcolor(LIGHTGRAY);
        ellipse(x1, y1, 0, 360, r, r/2);
        ellipse(x1, y1, 0, 360, r/2, r);
    }
};

// ================= Main Program =================
int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);

    int choice, col = WHITE;
    Shape* shape = NULL;

    while (true) {
        cout << "\n--- Mini Paint Application (2D + 3D with Fill) ---\n";
        cout << "1. Line\n2. Rectangle\n3. Circle\n";
        cout << "4. Cube (3D)\n5. Cylinder (3D)\n6. Cone (3D)\n7. Sphere (3D)\n";
        cout << "8. Change Color\n9. Clear Screen\n10. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 10) break;
        if (choice == 9) { cleardevice(); continue; }
        if (choice == 8) {
            cout << "Select Color (1.White 2.Red 3.Green 4.Blue 5.Yellow 6.Cyan 7.Magenta): ";
            cin >> col;
            switch (col) {
                case 1: col = WHITE; break;
                case 2: col = RED; break;
                case 3: col = GREEN; break;
                case 4: col = BLUE; break;
                case 5: col = YELLOW; break;
                case 6: col = CYAN; break;
                case 7: col = MAGENTA; break;
                default: col = WHITE;
            }
            if (shape) shape->color = col;
            continue;
        }

        // Select shape
        switch (choice) {
            case 1: shape = new LineShape(col); break;
            case 2: shape = new RectangleShape(col); break;
            case 3: shape = new CircleShape(col); break;
            case 4: shape = new CubeShape(col); break;
            case 5: shape = new CylinderShape(col); break;
            case 6: shape = new ConeShape(col); break;
            case 7: shape = new SphereShape(col); break;
            default: continue;
        }

        // Mouse input
        cout << "Click first point...\n";
        while (!ismouseclick(WM_LBUTTONDOWN));
        int x1, y1, x2, y2;
        getmouseclick(WM_LBUTTONDOWN, x1, y1);

        cout << "Click second point...\n";
        while (!ismouseclick(WM_LBUTTONDOWN));
        getmouseclick(WM_LBUTTONDOWN, x2, y2);

        // Draw selected shape
        shape->draw(x1, y1, x2, y2);
        delete shape; // free memory
    }

    closegraph();
    return 0;
}