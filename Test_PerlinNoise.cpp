//--------------------------------------------------------------------
//
//	Perlin noise library for modern C++
//
//	Copyright (C) 2013-2016 Ryo Suzuki <reputeless@gmail.com>
//
//	For the license information refer to PerlinNoise.hpp.
//
//--------------------------------------------------------------------

# include <iostream>
# include <fstream>
# include <sstream>
# include "PerlinNoise.hpp"

# pragma pack (push, 1)
struct BMPHeader
{
	std::uint16_t bfType;
	std::uint32_t bfSize;
	std::uint16_t bfReserved1;
	std::uint16_t bfReserved2;
	std::uint32_t bfOffBits;
	std::uint32_t biSize;
	std::int32_t  biWidth;
	std::int32_t  biHeight;
	std::uint16_t biPlanes;
	std::uint16_t biBitCount;
	std::uint32_t biCompression;
	std::uint32_t biSizeImage;
	std::int32_t  biXPelsPerMeter;
	std::int32_t  biYPelsPerMeter;
	std::uint32_t biClrUsed;
	std::uint32_t biClrImportant;
};

struct RGB
{
	double r = 0.0;
	double g = 0.0;
	double b = 0.0;
	constexpr RGB() = default;
	constexpr RGB(double _rgb)
		: r(_rgb), g(_rgb), b(_rgb) {}
	constexpr RGB(double _r, double _g, double _b)
		: r(_r), g(_g), b(_b) {}
};

static_assert(sizeof(BMPHeader) == 54, "sizeof(BMPHeader) != 54");
# pragma pack ( pop )

class Image
{
private:

	std::vector<RGB> m_data;
	
	std::int32_t m_width = 0, m_height = 0;

	bool inBounds(std::int32_t y, std::int32_t x) const
	{
		return (0 <= y) && (y < m_height) && (0 <= x) && (x < m_width);
	}

	static constexpr std::uint8_t ToUint8(double x)
	{
		return x >= 1.0 ? 255 : x <= 0.0 ? 0 : static_cast<std::uint8_t>(x * 255.0 + 0.5);
	}

public:

	Image() = default;

	Image(std::int32_t width, std::int32_t height)
		: m_data(width * height)
		, m_width(width)
		, m_height(height) {}

	void set(std::int32_t x, std::int32_t y, const RGB& color)
	{
		if (!inBounds(y, x))
		{
			return;
		}

		m_data[y * m_width + x] = color;
	}

	int width() const
	{
		return m_width;
	}

	int height() const
	{
		return m_height;
	}

	bool saveBMP(const std::string& path)
	{
		const std::int32_t  rowSize = m_width * 3 + m_width % 4;
		const std::uint32_t bmpsize = rowSize * m_height;
		const BMPHeader header =
		{
			0x4d42,
			static_cast<std::uint32_t>(bmpsize + sizeof(BMPHeader)),
			0,
			0,
			sizeof(BMPHeader),
			40,
			m_width,
			m_height,
			1,
			24,
			0,
			bmpsize,
			0,
			0,
			0,
			0
		};

		if (std::ofstream ofs{ path, std::ios_base::binary })
		{
			ofs.write((const char*)&header, sizeof(header));

			std::vector<std::uint8_t> line(rowSize);

			for (std::int32_t y = m_height - 1; -1 < y; --y)
			{
				size_t pos = 0;

				for (std::int32_t x = 0; x < m_width; ++x)
				{
					const RGB& col = m_data[y * m_width + x];
					line[pos++] = ToUint8(col.b);
					line[pos++] = ToUint8(col.g);
					line[pos++] = ToUint8(col.r);
				}

				ofs.write((const char*)line.data(), line.size());
			}

			return true;
		}
		else
		{
			return false;
		}
	}
};

template <class Type>
inline constexpr const Type& Clamp(const Type& x, const Type& min, const Type& max)
{
	return (x < min) ? min : ((max < x) ? max : x);
}

int main()
{
	Image image(512, 512);

	std::cout << "---------------------------------\n";
	std::cout << "* frequency [0.1 .. 8.0 .. 64.0] \n";
	std::cout << "* octaves   [1 .. 8 .. 16]       \n";
	std::cout << "* seed      [0 .. 2^32-1]        \n";
	std::cout << "---------------------------------\n";

	for (;;)
	{
		double frequency;
		std::cout << "double frequency = ";
		std::cin >> frequency;
		frequency = Clamp(frequency, 0.1, 64.0);

		int octaves;
		std::cout << "int32 octaves    = ";
		std::cin >> octaves;
		octaves = Clamp(octaves, 1, 16);

		std::uint32_t seed;
		std::cout << "uint32 seed      = ";
		std::cin >> seed;

		const siv::PerlinNoise perlin(seed);
		const double fx = image.width() / frequency;
		const double fy = image.height() / frequency;

		for (int y = 0; y < image.height(); ++y)
		{
			for (int x = 0; x < image.width(); ++x)
			{
				const RGB color(perlin.octaveNoise0_1(x / fx, y / fy, octaves));

				image.set(x, y, color);
			}
		}

		std::stringstream ss;
		ss << 'f' << frequency << 'o' << octaves << '_' << seed << ".bmp";

		if (image.saveBMP(ss.str()))
		{
			std::cout << "...saved \"" << ss.str() << "\"\n";
		}
		else
		{
			std::cout << "...failed\n";
		}

		char c;
		std::cout << "continue? [y/n] >";
		std::cin >> c;
		if (c != 'y') break;
		std::cout << '\n';
	}
}
