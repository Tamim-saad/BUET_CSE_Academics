#include<bits/stdc++.h>
using namespace std ; 
#ifdef __linux__
    #include <GL/glut.h>
#elif 
    #include <windows.h>
    #include <glut.h>
#endif

#define pi (2*acos(0.0)) 
#define posX 100
#define negX 101 
#define posY 102
#define negY 103
#define red 1
#define blue 2
#define green 3
#define yellow 4
#define cyan 5
#define magenta 6
#define black 7
#define white 8


class point
{
    public:
    double x ; 
    double y ; 
    double z ; 
     
    point()
    {
        x=0 ; 
        y=0 ; 
        z=0;
    }
    point(double a ,double b , double c)
    {
        x=a ; 
        y=b; 
        z=c ; 

    }
    double getx(){return x;} 
    double gety(){return y;} 
    double getz(){return z;} 

    void setx(double a){x=a;}
    void sety(double a){y=a;}
    void setz(double a){z=a;}

    point addition(point p)
    {
        return point(x + p.getx(), y + p.gety(), z + p.getz());

    }
    point subtraction(point p)
    {
        return point(x - p.getx(), y - p.gety(), z - p.getz());
    }

   point scaler_Multiplication(double n)
    {
        return point(x * n, y * n, z * n);
    }

   point cross_Multiplication(point p)
    {
        return point(
            y * p.getz() - z * p.gety(),
            z * p.getx() - x * p.getz(),
            x * p.gety() - y * p.getx()
        );
        
    }


        void show(string str="KEYBOARD")
    {
        cout<<str<<" "<<x<<","<<y<<","<<z<<"---"<<endl ; 
    }
    point normalize() {
        double length = sqrt(x * x + y * y + z * z);
        if (length == 0) return point(0, 0, 0);
        return point(x / length, y / length, z / length);
    }
    
    
    



}; 
point position_of_camera ; 
point camera_up ; 
point camera_look ; 
point camera_right ; 