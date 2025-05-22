#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0,255,0,255);
const int width = 200;
const int height = 200;

void line(Vec2i p0,Vec2i p1, TGAImage& image, TGAColor color) {
    bool steep = false;
    if (std::abs(p0.x - p1.x) < std::abs(p0.y - p1.y)) {
        std::swap(p0.x, p0.y);
        std::swap(p1.x, p1.y);
        steep = true;
    }
    if (p0.x> p1.x) {
        std::swap(p0, p1);
       
    }

    for (int x = p0.x; x <= p1.x; x++) {
        float t = (x - p0.x) / (float)(p1.x - p0.x);
        int y = p0.y * (1. - t) + p1.y * t;
        if (steep) {
            image.set(y, x, color);
        }
        else {
            image.set(x, y, color);
        }
    }
}
/*
void triangle(Vec2i t0,Vec2i t1,Vec2i t2,TGAImage &image,TGAColor color)
{

    if (t0.y > t1.y) { std::swap(t0,t1); }
    if (t0.y > t2.y) { std::swap(t0, t2); }
    if (t1.y > t2.y) { std::swap(t1, t2); }
    int total_height = t2.y - t0.y;

    for (int y = t0.y;y < t1.y;y++) 
    {
        int segment_height = t1.y - t0.y + 1;
        float alpha = (float)(y - t0.y) / total_height;
        float beta = (float)(y - t0.y) / segment_height;
        Vec2i A = t0 + (t2 - t0) * alpha;
        Vec2i B = t0 + (t1 - t0) * beta;
        if (A.x > B.x) { std::swap(A,B); }
        for (int j = A.x;j <= B.x;j++)
        {
            image.set(j,y,color);
        }
    }
    for (int y = t1.y;y <= t2.y;y++)
    {
        int segment_height = t2.y - t1.y + 1;
        float alpha = (float)(y - t0.y) / total_height;
        float beta = (float)(y - t1.y) / segment_height;
        Vec2i A = t0 + (t2 - t0) * alpha;
        Vec2i B = t1 + (t2 - t1) * beta;
        if (A.x > B.x) { std::swap(A,B); }
        for (int j = A.x;j <= B.x;j++)
        {
            image.set(j,y,color);
        }
    }  
}
*/

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color) 
{
    if (t0.y==t1.y&&t0.y==t2.y) { return; }
    if (t0.y > t1.y) std::swap(t0,t1);
    if (t0.y > t2.y) std::swap(t0, t2);
    if (t1.y>t2.y)std::swap(t1, t2);
    int total_height = t2.y - t0.y;
    for (int i = 0;i < total_height;i++)
    {
        bool second_half = i > t1.y - t0.y || t1.y == t0.y;
        int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height;
        Vec2i A = t0 + (t2 - t0) * alpha;
        Vec2i B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0)*beta;
        if (A.x > B.x) std::swap(A,B);
        for (int j=A.x;j<=B.x;j++) 
        {
            image.set(j, t0.y + i,color);
        }
    }

}

int main(int argc, char** argv) 
{
    //Model model{"obj/african_head.obj"};
    TGAImage image(width,height,TGAImage::RGB);


    Vec2i t0[3] = {Vec2i(10,70),Vec2i(50,160),Vec2i(70,80)};
    Vec2i t1[3] = {Vec2i(180,50),Vec2i(150,1),Vec2i(70,180)};
    Vec2i t2[3] = {Vec2i(180,150),Vec2i(120,160),Vec2i(130,180)};

    triangle(t0[0],t0[1],t0[2],image,red);
    triangle(t1[0], t1[1], t1[2], image, white);
    triangle(t2[0], t2[1], t2[2], image, green);

  
    image.flip_vertically();
    image.write_tga_file("OutputTriangle.tga");
    return 0;
}







/*
#include <iostream> // 添加调试输出

int main(int argc, char** argv) {
    std::string obj_path = (argc == 2) ? argv[1] : "obj/african_head.obj";
    std::cout << "Loading model: " << obj_path << std::endl;

    model = new Model(obj_path.c_str());
    if (model->nfaces() == 0) {
        std::cerr << "Error: Model has no faces!" << std::endl;
        return 1;
    }

    TGAImage image(width, height, TGAImage::RGB);
    std::cout << "Rendering " << model->nfaces() << " faces..." << std::endl;

    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        for (int j = 0; j < 3; j++) {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j + 1) % 3]);

            int x0 = (int)((v0.x + 1.) * width / 2.);
            int y0 = (int)((v0.y + 1.) * height / 2.);
            int x1 = (int)((v1.x + 1.) * width / 2.);
            int y1 = (int)((v1.y + 1.) * height / 2.);

            // 调试输出
            if (i < 5) { // 打印前5个面
                std::cout << "Face " << i << ": (" << x0 << "," << y0
                    << ") to (" << x1 << "," << y1 << ")" << std::endl;
            }

            line(x0, y0, x1, y1, image, white);
        }
    }

    if (!image.write_tga_file("output.tga")) {
        std::cerr << "Failed to write image!" << std::endl;
        return 1;
    }
    std::cout << "Render completed." << std::endl;
    delete model;
    system("pause");
    return 0;
}*/