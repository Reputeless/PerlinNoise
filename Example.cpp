# include <iostream>
# include <sstream>
# include "PerlinNoise.hpp"
# include "BMP.hpp"

template<typename Type>
inline const Type& Clamp( const Type& x, const Type& min, const Type& max )
{
	return (x < min) ? min : ((max < x) ? max : x);
}

int main()
{
	BMP<512,512> bmp;

	std::cout << "--------------------------------\n";
	std::cout << "*frequency [0.1 .. 8.0 .. 64.0] \n";
	std::cout << "*octaves   [1 .. 8 .. 16]       \n";
	std::cout << "*seed      [0 .. 2^32-1]        \n";
	std::cout << "--------------------------------\n";

	for(;;)
	{
		double frequency;
		std::cout << "double frequency = ";
		std::cin >> frequency;
		frequency = Clamp(frequency,0.1,64.0);

		int octaves;
		std::cout << "int octaves      = ";
		std::cin >> octaves;
		octaves = Clamp(octaves,1,16);

		unsigned seed;
		std::cout << "unsigned seed    = ";
		std::cin >> seed;

		const PerlinNoise perlin(seed);
		const double fx = bmp.width/frequency;
		const double fy = bmp.height/frequency;

		for(int y=0; y<bmp.height; ++y)
		{
			for(int x=0; x<bmp.width; ++x)
			{
				double n = perlin.octaveNoise(x/fx,y/fy,octaves);
				
				n = Clamp(n*0.5+0.5,0.0,1.0);
			
				const unsigned char gray = static_cast<unsigned char>(n*255.99);
				
				const RGB rgb = { gray, gray, gray };
			
				bmp.data[y][x] = rgb;
			}
		}

		std::stringstream ss;
		ss << 'f' << frequency << 'o' << octaves << '_' << seed << ".bmp";

		if(bmp.save(ss.str()))
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

		if(c!='y') break;

		std::cout << '\n';
	}
}
