
# pragma once
# include <vector>
# include <array>
# include <string>
# include <fstream>

# pragma pack ( push, 1 )
struct BMPHeader
{
	unsigned short bfType;
	unsigned bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned bfOffBits;
	unsigned biSize;
	int      biWidth;
	int      biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned biCompression;
	unsigned biSizeImage;
	int      biXPelsPerMeter;
	int      biYPelsPerMeter;
	unsigned biClrUsed;
	unsigned biClrImportant;
};

struct RGB
{
	unsigned char r,g,b;
};
# pragma pack ( pop )

template<int Width,int Height>
struct BMP
{
	enum { width = Width, height = Height };

	std::vector<std::array<RGB,Width>> data;

	BMP()
		:	data(Height)
	{

	}

	bool save( const std::string& path )
	{
		std::ofstream ofs(path,std::ios_base::binary);

		if(!ofs)
		{
			return false;
		}

		const int rowSize = Width*3 + Width%4;
		const unsigned bmpsize = rowSize*Height;

		const BMPHeader header =
		{
			0x4d42,
			bmpsize+sizeof(BMPHeader),
			0,
			0,
			sizeof(BMPHeader),
			40,
			Width,
			Height,
			1,
			24,
			0,
			bmpsize,
			0,
			0,
			0,
			0
		};

		ofs.write(static_cast<const char*>(static_cast<const void*>(&header)),sizeof(header));

		std::vector<unsigned char> line(rowSize);

		for(int y=Height-1; -1<y; --y)
		{
			int pos = 0;

			for(int x=0; x<Width; ++x)
			{
				line[pos++] = data[y][x].b;
				line[pos++] = data[y][x].g;
				line[pos++] = data[y][x].r;
			}

			ofs.write(static_cast<const char*>(static_cast<const void*>(line.data())),line.size());
		}

		return true;
	}
};
