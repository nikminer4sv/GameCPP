#ifndef POSITION2D_H
#define POSITION2D_H

class Position2D {
private:

    int x;
    int y;
    float xf;
    float yf;


public:

    Position2D();

    Position2D(int x, int y);

    Position2D(float xf, float yf);

    int GetX();

    int GetY();

    void SetX(int x);

    void SetY(int y);

    void SetXY(int x, int y);

    float GetXF();

    float GetYF();

    void SetXF(float xf);

    void SetYF(float yf);

    void SetXYF(float xf, float yf);
    
};

#endif