#include <iostream>
#include <fstream>
#include <vector>
#include <string>

typedef struct color_map {
	color_map(){}
	color_map(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha=255) {
		r = red; g = green; b = blue; a = alpha;
	}
	color_map(int red, int green, int blue, int alpha=255) {
		try{
			if((red > 255) || (green > 255) || (blue > 255) || (alpha > 255))
				throw(1);
			if((red < 0) || (green < 0) || (blue < 0) || (alpha < 0))
				throw(2);
			r = red; g = green; b = blue; a = alpha;
		}
		catch(int err){
			std::cout << "ERROR:invalid color";
			switch(err) {
				case 1:
				std::cout << "(>255)";

				case 2:
				std::cout << "(<255)";
			}
		}

	}

	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
}color_map;

void drop_image(const std::string filename, std::vector<color_map> &image, const int height, const int width) {
	try {
		if(image.size() != (height * width))
			throw(image.size());
		std::ofstream ofs(filename);
		ofs << "P6\n" << width << " " << height << "\n255\n";
		for (auto i = 0; i < width*height; ++i) {
		    ofs << static_cast<char>(image[i].r) 
				<< static_cast<char>(image[i].g) 
				<< static_cast<char>(image[i].b);
		}
		ofs.close();
	}
	catch(unsigned long size) {
		std::cout << "ERROR:ivalid size of image, image.size()=" << size << " need:" << height * width;
	}


}

void draw_rectangle(std::vector<color_map> &img, const int img_w, const int img_h, const int x, const int y, 
		const int w, const int h, const color_map color){
	try {
		if(img.size() != (img_h * img_w))
			throw(1);
		
		for (auto i=0; i<w; i++) {
			for (auto j=0; j<h; j++) {
				auto cx = x+i;
				auto cy = y+j;
				if(cx>img_w && cy>img_h)
					throw(2);
				img[cx + cy*img_w] = color;
			}
		}
	}
	catch(int err) {
		std::cout << "ERROR draw_rectangle()";
		switch(err)
		{
			case 1:
				std::cout << ":ivalid size of image, image.size()" << std::endl;
			break;

			case 2:
				std::cout << ":ivalid value x or y" << std::endl;
			break;
			
			default:
				std::cout << ":code-" << err;
			break;
		}
	}
}
int main() {
    const int width = 512; // image width
    const int height = 512; // image height
    std::vector<color_map> framebuffer(width*height);

	const size_t map_w = 16; // map width
    const size_t map_h = 16; // map height
    const char map[] = "0000222222220000"\
                       "1              0"\
                       "1      11111   0"\
                       "1     0        0"\
                       "0     0  1110000"\
                       "0     3        0"\
                       "0   10000      0"\
                       "0   0   11100  0"\
                       "0   0   0      0"\
                       "0   0   1  00000"\
                       "0       1      0"\
                       "2       1      0"\
                       "0       0      0"\
                       "0 0000000      0"\
                       "0              0"\
                       "0002222222200000"; 

    for (auto j = 0; j<height; j++) { // fill the screen with color gradients
        for (auto i = 0; i<width; i++) {
            color_map color(255*j/float(width), 255*i/float(width), 0);
            framebuffer[i+j*width] = color;
        }
    }

	auto rect_w = width/map_w;
    auto rect_h = height/map_h;
    for (auto j=0; j<map_h; j++) { // draw the map
        for (auto i=0; i<map_w; i++) {
            if (map[i+j*map_w]!=' ') {// skip empty spaces
            auto rect_x = i*rect_w;
            auto rect_y = j*rect_h;
            draw_rectangle(framebuffer, width, height, rect_x, rect_y, rect_w, rect_h, color_map(100, 100, 100));
			}
		}
    }

    drop_image("./out.ppm", framebuffer, width, height);

    return 0;
}
