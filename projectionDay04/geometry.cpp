#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <fstream>
#include "geometry.h"

#pragma pack(push,1)
struct TGA_Header {
	char idlength;
	char colormaptype;
	char datatypecode;
	short colormaporigin;
	short colormaplength;
	char colormapdepth;
	short x_origin;
	short y_origin;
	short width;
	short height;
	char  bitsperpixel;
	char  imagedescriptor;
};
#pragma pack(pop)



struct TGAColor {
	union {
		struct {
			unsigned char b, g, r, a;
		};
		unsigned char raw[4];
		unsigned int val;
	};
	int bytespp;

	TGAColor() : val(0), bytespp(1) {
	}

	TGAColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A) : b(B), g(G), r(R), a(A), bytespp(4) {
	}

	TGAColor(int v, int bpp) : val(v), bytespp(bpp) {
	}

	TGAColor(const TGAColor& c) : val(c.val), bytespp(c.bytespp) {
	}

	TGAColor(const unsigned char* p, int bpp) : val(0), bytespp(bpp) {
		for (int i = 0; i < bpp; i++) {
			raw[i] = p[i];
		}
	}

	TGAColor& operator =(const TGAColor& c) {
		if (this != &c) {
			bytespp = c.bytespp;
			val = c.val;
		}
		return *this;
	}
};


class TGAImage {
protected:
	unsigned char* data;
	int width;
	int height;
	int bytespp;

	bool   load_rle_data(std::ifstream& in);
	bool unload_rle_data(std::ofstream& out);
public:
	enum Format {
		GRAYSCALE = 1, RGB = 3, RGBA = 4
	};

	TGAImage();
	TGAImage(int w, int h, int bpp);
	TGAImage(const TGAImage& img);
	bool read_tga_file(const char* filename);
	bool write_tga_file(const char* filename, bool rle = true);
	bool flip_horizontally();
	bool flip_vertically();
	bool scale(int w, int h);
	TGAColor get(int x, int y);
	bool set(int x, int y, TGAColor c);
	~TGAImage();
	TGAImage& operator =(const TGAImage& img);
	int get_width();
	int get_height();
	int get_bytespp();
	unsigned char* buffer();
	void clear();
};

#endif //__IMAGE_H__

Matrix::Matrix(int r, int c) : rows(r), cols(c), m(std::vector<std::vector<float>>(r, std::vector<float>(c, 0.f))) {}

Matrix Matrix::identity(int dim)
{
	Matrix m(dim, dim);
	for (int i=0;i<dim;i++) 
	{
		m[i][i] = 1.f;
	}
	return m;
}

std::vector<float>& Matrix::operator[](int i) {
	return m[i];
}

// geometry.cpp ÖÐÌí¼Ó
template<>
template<>
Vec3<int>::Vec3(const Vec3<float>& v) : raw(), x(raw[0]), y(raw[1]), z(raw[2]) {
	x = static_cast<int>(v.x);
	y = static_cast<int>(v.y);
	z = static_cast<int>(v.z);
}
template<>
template<>
Vec3<float>::Vec3(const Vec3<int>& v) : raw(), x(raw[0]), y(raw[1]), z(raw[2]) {
	x = static_cast<float>(v.x);
	y = static_cast<float>(v.y);
	z = static_cast<float>(v.z);
}
Matrix Matrix::operator*(const Matrix& a) {
	Matrix result(rows, a.cols);
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < a.cols; ++j) {
			for (int k = 0; k < cols; ++k) {
				result[i][j] += m[i][k] * a.m[k][j];
			}
		}
	}
	return result;
}