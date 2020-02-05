/*******************************************************
 * Homework 1: Rasterization                           *
 * CS 148 (Summer 2016), Stanford University           *
 *-----------------------------------------------------*
 * Here you will implement the circle rasterization    *
 * method you derived in the written portion of the    *
 * homework.                                           *
 * To compile this in linux:                           *
 *        g++ hw1.cpp                                  *
 * Then, run the program as follows:                   *
 *        ./a.out 200                                  *
 * to generate a 200x200 image containing a circular   *
 * arc.  Note that the coordinate system we're using   *
 * places pixel centers at integer coordinates and     *
 * has the origin at the lower left.                   *
 * Your code will generate a .ppm file containing the  *
 * final image. These images can be viewed using       *
 * "display" in Linux or Irfanview in Mac/Windows.     *
 *******************************************************/

//http://netpbm.sourceforge.net/doc/ppm.html
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cassert>
using namespace std;

// We'll store image info as globals; not great programming practice
// but ok for this short program. (Why not just pass them in as arguements instead?)
int size;
bool **image;

void renderPixel(int x, int y)
{
    assert(x >= 0 && y >= 0 && x <= size && y <= size);  //Check to make sure x and y are positive and less than radius
    image[x][y] = 1;                                     //Using 2d array, mark value at indexes to indicate that the pixel should written
    //std::cout << "X: " << x << " and Y: " << y << "\n"; //The origin of image is in the top right corner

    //light up the pixel's symmetric counterpart
    image[y][x] = 1; //Due the symmetry,flipping y and x in the matrix wil mirror the pixels in the image across diagonal.
}

void rasterizeArc(int radius)
{
    //rasterize the arc using renderPixel to light up pixels

    int x{};
    int y { radius };
    int d { 1 - radius }; //
    renderPixel(x,y); //Places first pixel at (0,radius)

    while (y > x)  //x will exceed y at 45 degrees. X and Y are eqaul at this point to Radius/(sqrt(2))
    {
        //East (If origin starts in top right, should this be west?)
        if (d < 0) { d += 2*x+ 3; } //Based on ith + 1 value and the standard form of circle x^(2) + y^(2) = R^(2). 2x+3 is ΔW
        //Southeast (NorthWest)?
        else { d += 2 * (x-y) + 5; y--; } //ΔNW

        x++;

        renderPixel(x,y);//places subsequent pixels
    }

}



// You shouldn't need to change anything below this point.
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " circleSize\n";
        return 0;
    }

//Preprocessor macro to determine if the program is being compiled on a windows OS.
//If not, then the compiler will ignore everything from ifdef to else.

#ifdef _WIN32
    sscanf_s(argv[1], "%d", &size);
#else
    sscanf(argv[1], "%d", &size);
#endif

    //exits if given negative size
    if (size <= 0)
    {
        cout << "Image must be of positive size.\n";
        return 0;
    }

    // reserve image as 2d array
    image = new bool*[size+1];
    for (int i = 0; i <= size; i++) image[i] = new bool[size+1];

    rasterizeArc(size);

    char filename[50];

#ifdef _WIN32
    sprintf_s(filename, 50, "circle%d.ppm", size);
#else
    sprintf(filename, "circle%d.ppm", size);
#endif

    ofstream outfile(filename);
    outfile << "P3\n# " << filename << "\n";
    outfile << size+1 << ' ' << size+1 << ' ' << 1 << endl;

    for (int i = 0; i <= size; i++)
    for (int j = 0; j <= size; j++)
        outfile << image[size-i][j] << " 0 0\n";

    // delete image data
    for (int i = 0; i <= size; i++) delete [] image[i];
    delete [] image;

    return 0;
}
