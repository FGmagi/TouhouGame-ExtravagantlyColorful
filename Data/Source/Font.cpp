//============================================================
//	头文件声明
//============================================================
#include <stdafx.h>
#include <Font.h>
#include <fstream>
#include <sstream>
#include <Bitmap.h>
#include <OpenGL.h>
#include <Ref.h>
#include <DeBug.h>
//============================================================
//	数据结构
//============================================================
typedef unsigned char uc;
Font::ivec3 get_symbol(int id);
int fuck(int id);
class Font1 {
public:
	Font1(int w,int h):bitmap(w,h) {};
	Bitmap bitmap;//字表
	std::map<WORD, Font::ivec3> table;
}static* ptr;
//============================================================
//	Font 字库
//============================================================
Font::Font() {// 猜猜这鬼东西会溢出吗？
    //载入字库
	int w, h;
	int format = GL_RED;
	int* data = (int*)stbi_load("Data/Fonts/字库.png", &w, &h, &format, 0);
	ptr = new Font1(w, h);
	ptr->bitmap.Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);

	//写入纹理大小UBO
	Shader::code[5].Bind();
	Uniform(2i, TexSize, w, h);
	Uniform(1i, bold, 0);
	Uniform(1i, italic, 0);
	Uniform(1i, slender, 0);
	Uniform(4f, outlineColor, 1.0f, 1.0f, 1.0f, 1.0f);//粗体颜色
	Shader::code[0].Bind();
	//建立map索引
    std::string* text = new std::string;
    read_file(*text, "Data/Fonts/字库.txt");
    WORD character = 0;
    int x = 0;
    int y = 0;
	int le = 0;//当前统计到第几个字
    for (auto n = text->begin(); n != text->end();) {
        //确认 字
        if (*n == 10) { //忽略\n
            n++;
            continue;
        }
		if (uc(*n) <= 127) {
			character = uc(*n);
			n++;
		}else{
			character = uc(*n) << 8; n++;
			character |= uc(*n); n++;
		}

		// 储存字符供之后使用
        Font::ivec3 vec = { x, y,48 };
        if (y == 0) {
			vec = get_symbol(le);
        }
        ptr->table.insert(std::pair<DWORD, Font::ivec3>(character, vec));
        //偏移处理
         //第一行专给符号
        if (y == 0) {
			x += vec.w;
            if (le >= 118) {
                y += 55;
                x = 0;
            }
			le++;
			continue;
        }
        if (x == 48 * 99) {
            y += 50;
            x = 0;
            if (y == 38 * 50 + 10) {
                break;
            }
        } else {
            x += 48;
        }
		le++;
    }
    //清除内存
    delete text;
}
Font::~Font() {
	delete ptr;
}
Font::ivec3 Font::find(WORD n) {
    return ptr->table[n];
}

void Font::Bind(){
	ptr->bitmap.Bind();
}

const char* Font::read_file(std::string& str, const char* src) {
    std::ifstream file;
    file.exceptions(std::ifstream::badbit);
    try {
        file.open(src);
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        str = stream.str();
    }
    catch (std::ifstream::failure e) {
        print("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
    }
    return str.c_str();
};

Font::ivec3 get_symbol(int id) {
	Font::ivec3 i;
	if (id == 15) {
		i = { 420,0,18 };
		return i;
	}
	if (id == 18) {
		i = { 512,0,12 };
		return i;
	}
	i = { fuck(id) ,0,fuck(id + 1) - fuck(id) };
	return i;
}
Font::ivec2 Font::get_wh() {
	return {ptr->bitmap.get_width(), ptr->bitmap.get_height()};
}
int fuck(int id) {
	switch (id) {
	case 0://0
		return 0;
	case 1://1
		return 28;
	case 2://2
		return 56;
	case 3://3
		return 84;
	case 4://4
		return 112;
	case 5://5
		return 140;
	case 6://6
		return 168;
	case 7://7
		return 196;
	case 8://8
		return 224;
	case 9://9
		return 252;
	case 10://a
		return 280;
	case 11://b
		return 308;
	case 12://c
		return 338;
	case 13://d
		return 362;
	case 14://e
		return 393;
	case 15://f
		return 420;// 438
	case 16://g
		return 451;
	case 17://h
		return 482;
	case 18://i
		return 512;// 524
	case 19://j
		return 534;
	case 20://k
		return 551;
	case 21://l
		return 577;
	case 22://m
		return 590;
	case 23://n
		return 635;
	case 24://o
		return 664;
	case 25://p
		return 695;
	case 26://q
		return 726;
	case 27://r
		return 756;
	case 28://s
		return 775;
	case 29://t
		return 797;
	case 30://u
		return 815;
	case 31://v
		return 844;
	case 32://w
		return 870;
	case 33://x
		return 907;
	case 34://y
		return 931;
	case 35://z
		return 957;
	case 36://A
		return 981;
	case 37://B
		return 1015;
	case 38://C
		return 1045;
	case 39://D
		return 1077;
	case 40://E
		return 1114;
	case 41://F
		return 1140;
	case 42://G
		return 1165;
	case 43://H
		return 1201;
	case 44://I
		return 1238;
	case 45://J
		return 1252;
	case 46://K
		return 1271;
	case 47://L
		return 1301;
	case 48://M
		return 1326;
	case 49://N
		return 1373;
	case 50://O
		return 1412;
	case 51://P
		return 1452;
	case 52://Q
		return 1481;
	case 53://R
		return 1520;
	case 54://S
		return 1552;
	case 55://T
		return 1579;
	case 56://U
		return 1607;
	case 57://V
		return 1642;
	case 58://W
		return 1675;
	case 59://X
		return 1724;
	case 60://Y
		return 1755;
	case 61://Z
		return 1784;
	case 62://,
		return 1814;
	case 63://.
		return 1824;
	case 64://:
		return 1836;
	case 65://;
		return 1848;
	case 66://`
		return 1860;
	case 67://~
		return 1874;
	case 68://!
		return 1909;
	case 69://?
		return 1924;
	case 70://@
		return 1947;
	case 71://#
		return 1997;
	case 72://$
		return 2028;
	case 73://%
		return 2056;
	case 74://^
		return 2099;
	case 75://&
		return 2134;
	case 76://*
		return 2176;
	case 77://|
		return 2198;
	case 78://(
		return 2211;
	case 79://)
		return 2227;
	case 80://[
		return 2242;
	case 81://]
		return 2258;
	case 82://{
		return 2274;
	case 83://}
		return 2291;
	case 84:// (\)
		return 2305;
	case 85:///
		return 2327;
	case 86://<
		return 2347;
	case 87://>
		return 2383;
	case 88://"
		return 2418;
	case 89://'
		return 2439;
	case 90://+
		return 2452;
	case 91://-
		return 2487;
	case 92://*
		return 2508;
	case 93://	
		return 2530;
	case 94://·
		return 2543;
	case 95://！
		return 2556;
	case 96://￥
		return 2603;
	case 97://…
		return 2651;
	case 98://…
		return 2690;
	case 99://（
		return 2730;
	case 100://）
		return 2778;
	case 101://【
		return 2826;
	case 102://】
		return 2874;
	case 103://：	
		return 2922;
	case 104://；
		return 2970;
	case 105://“
		return 3017;
	case 106://”
		return 3065;
	case 107://‘
		return 3113;
	case 108://’
		return 3162;
	case 109://《
		return 3210;
	case 110://》
		return 3258;
	case 111://，
		return 3305;
	case 112://。
		return 3353;
	case 113://？
		return 3402;
	case 114://、
		return 3450;
	case 115://『
		return 3497;
	case 116://』
		return 3546;
	case 117://「
		return 3593;
	case 118://」
		return 3642;
	case 119://结尾
		return 3690;
	default:
		0;
	};
}