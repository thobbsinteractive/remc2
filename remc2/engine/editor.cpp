#include "editor.h"

bool changed = false;

int stage = 1000;
int minimapx = 0;
int minimapy = 0;

int maptype = 0;
int maptypefeat = 0;
int actlevel = 1;
int first_terrain_feature = 1;

float terrainzoom = 1;
int terrainbeginx = 0;
int terrainbeginy = 0;

float terrainzoomfeat = 1;
int terrainbeginxfeat = 0;
int terrainbeginyfeat = 0;

int edited_line = -1;
int edited_line2 = -1;

SDL_Surface* mapsurface;
SDL_Surface* mapsurfacefeat;
kiss_image mapimage;
kiss_image mapimagefeat;
SDL_Renderer* editor_renderer;

type_str_0x30311 temparray_0x30311[0x4b0];
bool temparray_0x30311_inactive[0x4b0];

void SetPixelMapSurface(int x,int y,int nx,int ny,Bit8u* adress) {
	if (nx < 0 || nx>255 || ny < 0 || ny>255)
	{
		Bit8u* scrbuff = (Bit8u*)mapsurface->pixels;
		scrbuff[4 * (y * 512 + x)] = 255;
		scrbuff[4 * (y * 512 + x) + 1] = 0;
		scrbuff[4 * (y * 512 + x) + 2] = 0;
		scrbuff[4 * (y * 512 + x) + 3] = 255;
		return;
	}
	int color = adress[nx+ny* 256];
	Bit8u* scrbuff = (Bit8u*)mapsurface->pixels;
	scrbuff[4 * (y * 512 + x)] = color;
	scrbuff[4 * (y * 512 + x) + 1] = color;
	scrbuff[4 * (y * 512 + x) + 2] = color;
	scrbuff[4 * (y * 512 + x) + 3] = 255;
}

void SetPixelMapSurfacefeat(int x, int y, int nx, int ny, Bit8u* adress) {
	if (nx < 0 || nx>255 || ny < 0 || ny>255)
	{
		Bit8u* scrbuff = (Bit8u*)mapsurfacefeat->pixels;
		scrbuff[4 * (y * 512 + x)] = 255;
		scrbuff[4 * (y * 512 + x) + 1] = 0;
		scrbuff[4 * (y * 512 + x) + 2] = 0;
		scrbuff[4 * (y * 512 + x) + 3] = 255;
		return;
	}
	int color = adress[nx + ny * 256];
	Bit8u* scrbuff = (Bit8u*)mapsurfacefeat->pixels;
	scrbuff[4 * (y * 512 + x)] = color;
	scrbuff[4 * (y * 512 + x) + 1] = color;
	scrbuff[4 * (y * 512 + x) + 2] = color;
	scrbuff[4 * (y * 512 + x) + 3] = 255;
}

void init_pal() {
	Bit8u temppal[0x300];
	for (int i = 0; i < 256; i++)
	{
		temppal[i * 3] = i;
		temppal[i * 3 + 1] = i;
		temppal[i * 3 + 2] = i;
	}
	VGA_Set_pallette2(temppal);
};

void clean_tarrain()
{
}

void editor_loop();
void terrain_recalculate();

int main_x(/*int argc, char** argv*/);

void loadlevel(int levelnumber) {
	sub_533B0_decompress_levels(levelnumber, &D41A0_BYTESTR_0.str_2FECE);
	memcpy(temparray_0x30311, D41A0_BYTESTR_0.str_2FECE.array_0x30311,sizeof(D41A0_BYTESTR_0.str_2FECE.array_0x30311));
	for(int i=0;i< 0x4b0;i++)
		temparray_0x30311_inactive[i]=0;
}

void editor_run()
{	

	sub_6EBF0(&filearray_2aa18c[filearrayindex_POINTERSDATTAB]);
	x_WORD_180660_VGA_type_resolution = 1;
	x_WORD_E29DA_type_resolution = 1;

	sub_6FC50(1);

	//save D41A0_BYTESTR_0
	D41A0_BYTESTR_0.str_2FECE.word_0x2FEE5 = 0;
	D41A0_BYTESTR_0.str_2FECE.word_0x2FEE9 = 0;
	D41A0_BYTESTR_0.str_2FECE.word_0x2FEED = 0;
	D41A0_BYTESTR_0.str_2FECE.word_0x2FEF1 = 0;
	D41A0_BYTESTR_0.str_2FECE.word_0x2FEFD = 0;
	D41A0_BYTESTR_0.str_2FECE.word_0x2FF01 = 0;
	D41A0_BYTESTR_0.str_2FECE.word_0x2FF0D = 0;
	D41A0_BYTESTR_0.str_2FECE.word_0x2FF11 = 0;
	//init_pal();
	clean_tarrain();
	loadlevel(0);
	//terrain_recalculate();

	main_x(/*int argc, char** argv*/);
	//editor_loop();

	//restore D41A0_BYTESTR_0
}

void terrain_recalculate() {
	int j = 0;
	for (int i = 0; i < 0x4b0;i++)
	{
		if (temparray_0x30311_inactive[i])
			j++;
		else
		{
			D41A0_BYTESTR_0.str_2FECE.array_0x30311[j]= temparray_0x30311[i];
			j++;
		}
	}


	x_WORD_17B4E0 = D41A0_BYTESTR_0.str_2FECE.word_0x2FEE5;
	D41A0_BYTESTR_0.dword_0x8 = D41A0_BYTESTR_0.str_2FECE.word_0x2FEE5;
	memset((void*)x_WORD_15B4E0_source, 0, 0x20000);
	sub_B5E70_decompress_terrain_map_level(x_WORD_17B4E0, D41A0_BYTESTR_0.str_2FECE.word_0x2FEE9, D41A0_BYTESTR_0.str_2FECE.word_0x2FEED, D41A0_BYTESTR_0.str_2FECE.word_0x2FEF1);
	sub_44DB0_truncTerrainHeight();//225db0 //trunc and create
	if (stage > 0)
	{
		memset((void*)x_WORD_15B4E0_source, 0, 0x20000);
		sub_44E40(D41A0_BYTESTR_0.str_2FECE.word_0x2FEF5, D41A0_BYTESTR_0.str_2FECE.word_0x2FEF9);//225e40 //add any fields
	}
	if (stage > 1)
	{
		sub_45AA0_setMax4Tiles();//226aa0
	}

	if (stage > 2)
	{
		sub_440D0(D41A0_BYTESTR_0.str_2FECE.word_0x2FF01);//2250d0
	}
	if (stage > 3)
	{
		sub_45060(D41A0_BYTESTR_0.str_2FECE.word_0x2FF05, D41A0_BYTESTR_0.str_2FECE.word_0x2FF09);//226060
	}
	if (stage > 4)
	{
		sub_44320();//225320
	}
	if (stage > 5)
	{
		sub_45210(D41A0_BYTESTR_0.str_2FECE.word_0x2FF05, D41A0_BYTESTR_0.str_2FECE.word_0x2FF09);//226210
	}
	if (stage > 6)
	{
		sub_454F0(D41A0_BYTESTR_0.str_2FECE.word_0x2FEFD, D41A0_BYTESTR_0.str_2FECE.word_0x2FF11);//2264f0
	}
	if (stage > 7)
	{
		sub_45600(D41A0_BYTESTR_0.str_2FECE.word_0x2FF0D);//226600
	}
	if (stage > 8)
	{
		sub_43FC0();//224fc0
	}
	if (stage > 9)
	{
		memset((void*)x_BYTE_10B4E0_terraintype, 0, 0x10000);
		sub_43970(0);//224970 // smooth terrain
	}
	if (stage > 10)
	{
		sub_43EE0();//224ee0 // add rivers
	}
	if (stage > 11)
	{
		sub_44580();//225580 //set angle of terrain
	}
	if (stage > 12)
	{
		if (x_BYTE_D41B6)
			sub_43B40();//224b40 //change angle of terrain
		else
			sub_43D50();//224d50 //change angle of terrain
	}
	if (stage > 13)
	{
		sub_44D00();//225d00
	}
	sub_49F30();//prepare events pointers
	if (stage > 14)
	{
		sub_49830(&D41A0_BYTESTR_0.str_2FECE);
	}
	if (stage > 15)
	{
		sub_49290(&D41A0_BYTESTR_0.str_2FECE, 1);
	}
	changed = false;
};

void drawmenus() {
};
void drawterrain(int x,int y) {
	int k = 0;	
		for (int j = 0; j < 256; j++)
			for (int i = 0; i < 256; i++)
			{
				switch (maptype)
				{
				case 0:
					x_DWORD_180628b_screen_buffer[(j + y) * 640 + (i + x)] = x_BYTE_10B4E0_terraintype[k];
					break;
				case 1:
					x_DWORD_180628b_screen_buffer[(j + y) * 640 + (i + x)] = x_BYTE_11B4E0_height[k];
					break;
				case 2:
					x_DWORD_180628b_screen_buffer[(j + y) * 640 + (i + x)] = x_BYTE_12B4E0_shading[k];
					break;
				case 3:
					x_DWORD_180628b_screen_buffer[(j + y) * 640 + (i + x)] = x_BYTE_13B4E0_angle[k];
					break;
				}
			k++;
		}
};
void fillterrain(/*int x, int y,*/float zoom,int beginx,int beginy) {
	Bit8u terrfeatlayer[256 * 256];
	for (int j = 0; j < 256; j++)
		for (int i = 0; i < 256; i++)
		{
			terrfeatlayer[i+j*256]=0;
		}
	for (int i = 0; i < 0x4B0; i++)
	{
		type_str_0x30311 actfeat = D41A0_BYTESTR_0.str_2FECE.array_0x30311[first_terrain_feature + i];
		if ((actfeat.axis3d_4.x > -1) && (actfeat.axis3d_4.x < 256) && (actfeat.axis3d_4.y > -1) && (actfeat.axis3d_4.y < 256))
			terrfeatlayer[actfeat.axis3d_4.x + actfeat.axis3d_4.y * 256] = actfeat.str_0x30311_word_0;//all entites
	}

	for (int j = 0; j < 512; j++)
		for (int i = 0; i < 512; i++)
		{
			int nx = beginx + i / (zoom * 2);
			int ny = beginy + j / (zoom * 2);
			if ((nx > -1 && nx<256 && ny > -1 && ny < 256))
			{
				Bit8u* scrbuff = (Bit8u*)mapsurface->pixels;
				switch (terrfeatlayer[nx + ny * 256])
				{
				case 0:
					switch (maptype)
					{
					case 0:
						SetPixelMapSurface(i, j, nx, ny, x_BYTE_10B4E0_terraintype);
						break;
					case 1:
						SetPixelMapSurface(i, j, nx, ny, x_BYTE_11B4E0_height);
						break;
					case 2:
						SetPixelMapSurface(i, j, nx, ny, x_BYTE_12B4E0_shading);
						break;
					case 3:
						SetPixelMapSurface(i, j, nx, ny, x_BYTE_13B4E0_angle);
						break;
					}
					break;

				case 0x5:
					scrbuff[4 * (j * 512 + i)] = 0;//setgreen
					scrbuff[4 * (j * 512 + i) + 1] = 255;
					scrbuff[4 * (j * 512 + i) + 2] = 0;
					scrbuff[4 * (j * 512 + i) + 3] = 255;
					break;
				case 0xa:
					scrbuff[4 * (j * 512 + i)] = 0;//setblue
					scrbuff[4 * (j * 512 + i) + 1] = 0;
					scrbuff[4 * (j * 512 + i) + 2] = 255;
					scrbuff[4 * (j * 512 + i) + 3] = 255;
					break;
				/*case 0xf0:
					scrbuff[4 * (j * 512 + i)] = 255;//setwhite
					scrbuff[4 * (j * 512 + i) + 1] = 255;
					scrbuff[4 * (j * 512 + i) + 2] = 255;
					scrbuff[4 * (j * 512 + i) + 3] = 255;*/
					break;
				case 0x2:
				case 0x3:
					scrbuff[4 * (j * 512 + i)] = 255;//set brow
					scrbuff[4 * (j * 512 + i) + 1] = 255;
					scrbuff[4 * (j * 512 + i) + 2] = 0;
					scrbuff[4 * (j * 512 + i) + 3] = 255;
					break;
				case 0xb:
					scrbuff[4 * (j * 512 + i)] = 0;//set cyan
					scrbuff[4 * (j * 512 + i) + 1] = 255;
					scrbuff[4 * (j * 512 + i) + 2] = 255;
					scrbuff[4 * (j * 512 + i) + 3] = 255;
					break;
				case 0xe:
					scrbuff[4 * (j * 512 + i)] = 255;//set magenta
					scrbuff[4 * (j * 512 + i) + 1] = 0;
					scrbuff[4 * (j * 512 + i) + 2] = 255;
					scrbuff[4 * (j * 512 + i) + 3] = 255;
					break;
				default:
					scrbuff[4 * (j * 512 + i)] = 255;//setred
					scrbuff[4 * (j * 512 + i) + 1] = 0;
					scrbuff[4 * (j * 512 + i) + 2] = 0;
					scrbuff[4 * (j * 512 + i) + 3] = 255;
					break;
				}
			}
		}
};
void fillterrainfeat(float zoom, int beginx, int beginy) {	
	Bit8u terrfeatlayer[256 * 256];
	for (int j = 0; j < 256; j++)
		for (int i = 0; i < 256; i++)
		{
			terrfeatlayer[i + j * 256] = 0;
		}
	for (int i = 0; i < 0x4B0; i++)
	{
		type_str_0x30311 actfeat = D41A0_BYTESTR_0.str_2FECE.array_0x30311[first_terrain_feature + i];
		if ((actfeat.axis3d_4.x > -1) && (actfeat.axis3d_4.x < 256) && (actfeat.axis3d_4.y > -1) && (actfeat.axis3d_4.y < 256))
			terrfeatlayer[actfeat.axis3d_4.x + actfeat.axis3d_4.y * 256] = actfeat.str_0x30311_word_0;//all entites
	}

	type_str_0x30311 actfeat = temparray_0x30311[edited_line2 + 1];
	if ((actfeat.axis3d_4.x > -1) && (actfeat.axis3d_4.x < 256) && (actfeat.axis3d_4.y > -1) && (actfeat.axis3d_4.y < 256))
		terrfeatlayer[actfeat.axis3d_4.x + actfeat.axis3d_4.y * 256] = 0xf0;//selected entity

	for (int j = 0; j < 512; j++)
		for (int i = 0; i < 512; i++)
		{
			int nx = beginx + i / (zoom * 2);
			int ny = beginy + j / (zoom * 2);
			if ((nx > -1 && nx<256 && ny > -1 && ny < 256))
			{
				Bit8u* scrbuff = (Bit8u*)mapsurfacefeat->pixels;
				switch (terrfeatlayer[nx + ny * 256])
				{
				case 0:
					switch (maptypefeat)
					{
					case 0:
						SetPixelMapSurfacefeat(i, j, nx, ny, x_BYTE_10B4E0_terraintype);
						break;
					case 1:
						SetPixelMapSurfacefeat(i, j, nx, ny, x_BYTE_11B4E0_height);
						break;
					case 2:
						SetPixelMapSurfacefeat(i, j, nx, ny, x_BYTE_12B4E0_shading);
						break;
					case 3:
						SetPixelMapSurfacefeat(i, j, nx, ny, x_BYTE_13B4E0_angle);
						break;
					}
					break;

				case 0x5:
					scrbuff[4 * (j * 512 + i)] = 0;//setgreen
					scrbuff[4 * (j * 512 + i) + 1] = 255;
					scrbuff[4 * (j * 512 + i) + 2] = 0;
					scrbuff[4 * (j * 512 + i) + 3] = 255;
					break;
				case 0xa:
					scrbuff[4 * (j * 512 + i)] = 0;//setblue
					scrbuff[4 * (j * 512 + i) + 1] = 0;
					scrbuff[4 * (j * 512 + i) + 2] = 255;
					scrbuff[4 * (j * 512 + i) + 3] = 255;
					break;
				case 0xf0:
					scrbuff[4 * (j * 512 + i)] = 255;//setwhite
					scrbuff[4 * (j * 512 + i) + 1] = 255;
					scrbuff[4 * (j * 512 + i) + 2] = 255;
					scrbuff[4 * (j * 512 + i) + 3] = 255;
					break;
				case 0x2:
				case 0x3:
					scrbuff[4 * (j * 512 + i)] = 255;//set brow
					scrbuff[4 * (j * 512 + i) + 1] = 255;
					scrbuff[4 * (j * 512 + i) + 2] = 0;
					scrbuff[4 * (j * 512 + i) + 3] = 255;
					break;
				case 0xb:
					scrbuff[4 * (j * 512 + i)] = 0;//set cyan
					scrbuff[4 * (j * 512 + i) + 1] = 255;
					scrbuff[4 * (j * 512 + i) + 2] = 255;
					scrbuff[4 * (j * 512 + i) + 3] = 255;
					break;
				case 0xe:
					scrbuff[4 * (j * 512 + i)] = 255;//set magenta
					scrbuff[4 * (j * 512 + i) + 1] = 0;
					scrbuff[4 * (j * 512 + i) + 2] = 255;
					scrbuff[4 * (j * 512 + i) + 3] = 255;
					break;
				default:
					scrbuff[4 * (j * 512 + i)] = 255;//setred
					scrbuff[4 * (j * 512 + i) + 1] = 0;
					scrbuff[4 * (j * 512 + i) + 2] = 0;
					scrbuff[4 * (j * 512 + i) + 3] = 255;
					break;
				}
			}
		}
};
void drawdetail(int x, int y,int size,int zoom) {
	int k = 0;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			bool notframe = true;
			int posx = (minimapx)+(i - size / 2) / zoom;
			int posy = (minimapy - 480 + 256) + (j - size / 2) / zoom;
			if (posx < 0) { posx = 0; notframe = false; }
			if (posy < 0) { posy = 0; notframe = false; }
			if (posx > 255) { posx = 255; notframe = false; }
			if (posy > 255) { posy = 255; notframe = false; }
			if (notframe)
			{
				k = 256 * (posy)+posx;
				switch (maptype)
				{
				case 0:
					x_DWORD_180628b_screen_buffer[(j + y) * 640 + (i + x)] = x_BYTE_10B4E0_terraintype[k];
					break;
				case 1:
					x_DWORD_180628b_screen_buffer[(j + y) * 640 + (i + x)] = x_BYTE_11B4E0_height[k];
					break;
				case 2:
					x_DWORD_180628b_screen_buffer[(j + y) * 640 + (i + x)] = x_BYTE_12B4E0_shading[k];
					break;
				case 3:
					x_DWORD_180628b_screen_buffer[(j + y) * 640 + (i + x)] = x_BYTE_13B4E0_angle[k];
					break;
				}
			}
			else
				x_DWORD_180628b_screen_buffer[(j + y) * 640 + (i + x)] = 255;
			//k++;
		}
};

Bit16s editor_regiondata[22 * (32+32+32+1+4+3+1)] = {
	//arrow buttons
                                	//position    //size              //type gr      under mou 
0x0000,0x0000,0x0000,0x0000,0x0000,0x00b0,0x0000,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00b8,0x0000,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c0,0x0000,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c8,0x0000,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0110,0x0000,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0118,0x0000,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0120,0x0000,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0128,0x0000,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,

0x0000,0x0000,0x0000,0x0000,0x0000,0x00b0,0x0010,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00b8,0x0010,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c0,0x0010,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c8,0x0010,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0110,0x0010,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0118,0x0010,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0120,0x0010,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0128,0x0010,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,

0x0000,0x0000,0x0000,0x0000,0x0000,0x00b0,0x0020,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00b8,0x0020,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c0,0x0020,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c8,0x0020,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0110,0x0020,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0118,0x0020,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0120,0x0020,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0128,0x0020,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,

0x0000,0x0000,0x0000,0x0000,0x0000,0x00b0,0x0030,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00b8,0x0030,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c0,0x0030,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c8,0x0030,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0110,0x0030,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0118,0x0030,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0120,0x0030,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0128,0x0030,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,

0x0000,0x0000,0x0000,0x0000,0x0000,0x00b0,0x0040,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00b8,0x0040,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c0,0x0040,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c8,0x0040,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0110,0x0040,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0118,0x0040,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0120,0x0040,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0128,0x0040,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,

0x0000,0x0000,0x0000,0x0000,0x0000,0x00b0,0x0050,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00b8,0x0050,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c0,0x0050,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c8,0x0050,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0110,0x0050,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0118,0x0050,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0120,0x0050,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0128,0x0050,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,

0x0000,0x0000,0x0000,0x0000,0x0000,0x00b0,0x0060,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00b8,0x0060,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c0,0x0060,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c8,0x0060,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0110,0x0060,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0118,0x0060,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0120,0x0060,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0128,0x0060,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,

0x0000,0x0000,0x0000,0x0000,0x0000,0x00b0,0x0070,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00b8,0x0070,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c0,0x0070,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c8,0x0070,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0110,0x0070,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0118,0x0070,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0120,0x0070,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0128,0x0070,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,

0x0000,0x0000,0x0000,0x0000,0x0000,0x00b0,0x0080,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00b8,0x0080,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c0,0x0080,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c8,0x0080,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0110,0x0080,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0118,0x0080,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0120,0x0080,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0128,0x0080,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,

0x0000,0x0000,0x0000,0x0000,0x0000,0x00b0,0x0090,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00b8,0x0090,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c0,0x0090,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c8,0x0090,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0110,0x0090,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0118,0x0090,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0120,0x0090,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0128,0x0090,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,

0x0000,0x0000,0x0000,0x0000,0x0000,0x00b0,0x00a0,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00b8,0x00a0,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c0,0x00a0,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c8,0x00a0,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0110,0x00a0,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0118,0x00a0,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0120,0x00a0,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0128,0x00a0,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,

0x0000,0x0000,0x0000,0x0000,0x0000,0x00b0,0x00b0,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00b8,0x00b0,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c0,0x00b0,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x00c8,0x00b0,0x0008,0x0010,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0110,0x00b0,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0118,0x00b0,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0120,0x00b0,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0128,0x00b0,0x0008,0x0010,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
//map click
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00e0,0x0100,0x0100,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,

//map type
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00be,0x0019,0x0019,0x0000,0x0102,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0020,0x00be,0x0019,0x0019,0x0000,0x0102,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0040,0x00be,0x0019,0x0019,0x0000,0x0102,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0060,0x00be,0x0019,0x0019,0x0000,0x0102,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
//load
0x0000,0x0000,0x0000,0x0000,0x0000,0x0140,0x0000,0x0019,0x0019,0x0000,0x0102,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0138,0x0000,0x0019,0x0019,0x0000,0x0304,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0159,0x0000,0x0019,0x0019,0x0000,0x0506,0x0101,0x00ff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,

0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
};

/*
Bit16s x_WORD_E1F84[68] =//in region?
{
	//                                   xmin  ymin   sizex	 sizey        type?
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0123,0x00CD,0x003C,0x003C,0x0000,0x2324,
*/

Bit8u cursor[8 * 8] = {
	0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
	0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,
	0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,
	0xff,0x00,0x00,0xff,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff
};

Bit8u triangleL[16 * 8] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
	0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,
	0x00,0x00,0x00,0x00,0x00,0xff,0x00,0xff,
	0x00,0x00,0x00,0x00,0xff,0x00,0x00,0xff,
	0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,
	0x00,0x00,0xff,0x00,0x00,0x00,0x00,0xff,
	0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,
	0xff,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
	0xff,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
	0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,
	0x00,0x00,0xff,0x00,0x00,0x00,0x00,0xff,
	0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,
	0x00,0x00,0x00,0x00,0xff,0x00,0x00,0xff,
	0x00,0x00,0x00,0x00,0x00,0xff,0x00,0xff,
	0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff
};

Bit8u triangleLfull[16 * 8] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
	0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,
	0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,
	0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,
	0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,
	0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,
	0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,
	0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,
	0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,
	0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,
	0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff
};

Bit8u triangleP[16 * 8] = {
	0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
	0xff,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
	0xff,0x00,0x00,0xff,0x00,0x00,0x00,0x00,
	0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
	0xff,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
	0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
	0xff,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
	0xff,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
	0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
	0xff,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
	0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
	0xff,0x00,0x00,0xff,0x00,0x00,0x00,0x00,
	0xff,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
	0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
	0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

Bit8u trianglePfull[16 * 8] = {
	0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
	0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,
	0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
	0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
	0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,
	0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
	0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
	0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
	0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,
	0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
	0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

Bit8u triangleU[8 * 16] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
	0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
	0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};

Bit8u triangleUfull[8 * 16] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
	0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
	0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};

Bit8u triangleD[8 * 16] = {
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
	0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
	0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00	
};

Bit8u triangleDfull[8 * 16] = {
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,
	0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
	0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

void drawx(int x,int y, int index,int type=0) {
	if (type > 100)
		;// sub_2BB40_draw_bitmap(x, y, xy_DWORD_17DED4_spritestr[index]);
	switch (index)
	{
	case 0://cursor
	{
		int width = 8;
		int height = 8;
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				if(cursor[i + 8 * j]==0xff)
					*(Bit8u*)(x_DWORD_180628b_screen_buffer + i + x + 640 * (y + j)) = 255;				
			}
		break;
	}
	case 1://rectangle 25
	{
		int width = 25;
		int height = 25;
		for (int i = 0; i < width; i++)
		{
			*(Bit8u*)(x_DWORD_180628b_screen_buffer + i + x + 640 * (y + 0)) = 255;
			*(Bit8u*)(x_DWORD_180628b_screen_buffer + i + x + 640 * (y + height - 1)) = 255;
		}
		for (int j = 0; j < height; j++)
		{
			*(Bit8u*)(x_DWORD_180628b_screen_buffer + x + 0 + 640 * (y + j)) = 255;
			*(Bit8u*)(x_DWORD_180628b_screen_buffer + x + width - 1 + 640 * (y + j)) = 255;
		}
		break;
	}
	case 2://filled rectangle
	{
		int width = 25;
		int height = 25;
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				*(Bit8u*)(x_DWORD_180628b_screen_buffer + i + x + 640 * (y + j)) = 255;
			}
		break;
	}
	case 3://triangleL
	{
		int width = 8;
		int height = 16;
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				if (triangleL[i + width * j] == 0xff)
					* (Bit8u*)(x_DWORD_180628b_screen_buffer + i + x + 640 * (y + j)) = 255;
			}
		break;
	}
	case 4://triangleLfull
	{
		int width = 8;
		int height = 16;
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				if (triangleLfull[i + width * j] == 0xff)
					* (Bit8u*)(x_DWORD_180628b_screen_buffer + i + x + 640 * (y + j)) = 255;
			}
		break;
	}
	case 5://triangleP
	{
		int width = 8;
		int height = 16;
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				if (triangleP[i + width * j] == 0xff)
					* (Bit8u*)(x_DWORD_180628b_screen_buffer + i + x + 640 * (y + j)) = 255;
			}
		break;
	}
	case 6://trianglePfull
	{
		int width = 8;
		int height = 16;
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				if (trianglePfull[i + width * j] == 0xff)
					* (Bit8u*)(x_DWORD_180628b_screen_buffer + i + x + 640 * (y + j)) = 255;
			}
		break;
	}
	}
}

void drawcursor() {
	drawx(x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DEE4_mouse_positiony, 0);
};

void drawtexts() {
	char temp[256];
	//VGA_GotoXY(0xc8/40, 6/25);
	//VGA_Draw_string((char*)"RANDOM SEED");	
	VGA_Draw_stringXYtoBuffer((char*)"RANDOM SEED:", 0, 0, x_DWORD_180628b_screen_buffer);
	sprintf(temp, "%04X", D41A0_BYTESTR_0.str_2FECE.word_0x2FEE5);
	VGA_Draw_stringXYtoBuffer(temp, 210, 0, x_DWORD_180628b_screen_buffer);

	VGA_Draw_stringXYtoBuffer((char*)"SHIFT:", 0, 16, x_DWORD_180628b_screen_buffer);
	sprintf(temp, "%04X", D41A0_BYTESTR_0.str_2FECE.word_0x2FEE9);
	VGA_Draw_stringXYtoBuffer(temp, 210, 16, x_DWORD_180628b_screen_buffer);

	VGA_Draw_stringXYtoBuffer((char*)"HEIGHT:", 0, 32, x_DWORD_180628b_screen_buffer);
	sprintf(temp, "%04X", D41A0_BYTESTR_0.str_2FECE.word_0x2FEED);
	VGA_Draw_stringXYtoBuffer(temp, 210, 32, x_DWORD_180628b_screen_buffer);

	VGA_Draw_stringXYtoBuffer((char*)"NOISE:", 0, 48, x_DWORD_180628b_screen_buffer);
	sprintf(temp, "%04X", D41A0_BYTESTR_0.str_2FECE.word_0x2FEF1);
	VGA_Draw_stringXYtoBuffer(temp, 210, 48, x_DWORD_180628b_screen_buffer);


	VGA_Draw_stringXYtoBuffer((char*)"RIVERS:", 0, 64, x_DWORD_180628b_screen_buffer);
	sprintf(temp, "%04X", D41A0_BYTESTR_0.str_2FECE.word_0x2FEF5);
	VGA_Draw_stringXYtoBuffer(temp, 210, 64, x_DWORD_180628b_screen_buffer);

	VGA_Draw_stringXYtoBuffer((char*)"RIVERSH:", 0, 80, x_DWORD_180628b_screen_buffer);
	sprintf(temp, "%04X", D41A0_BYTESTR_0.str_2FECE.word_0x2FEF9);
	VGA_Draw_stringXYtoBuffer(temp, 210, 80, x_DWORD_180628b_screen_buffer);

	VGA_Draw_stringXYtoBuffer((char*)"0x2FEFD:", 0, 96, x_DWORD_180628b_screen_buffer);
	sprintf(temp, "%04X", D41A0_BYTESTR_0.str_2FECE.word_0x2FEFD);
	VGA_Draw_stringXYtoBuffer(temp, 210, 96, x_DWORD_180628b_screen_buffer);

	VGA_Draw_stringXYtoBuffer((char*)"0x2FF01:", 0, 112, x_DWORD_180628b_screen_buffer);
	sprintf(temp, "%04X", D41A0_BYTESTR_0.str_2FECE.word_0x2FF01);
	VGA_Draw_stringXYtoBuffer(temp, 210, 112, x_DWORD_180628b_screen_buffer);

	VGA_Draw_stringXYtoBuffer((char*)"0x2FF05:", 0, 128, x_DWORD_180628b_screen_buffer);
	sprintf(temp, "%04X", D41A0_BYTESTR_0.str_2FECE.word_0x2FF05);
	VGA_Draw_stringXYtoBuffer(temp, 210, 128, x_DWORD_180628b_screen_buffer);

	VGA_Draw_stringXYtoBuffer((char*)"0x2FF09:", 0, 144, x_DWORD_180628b_screen_buffer);
	sprintf(temp, "%04X", D41A0_BYTESTR_0.str_2FECE.word_0x2FF09);
	VGA_Draw_stringXYtoBuffer(temp, 210, 144, x_DWORD_180628b_screen_buffer);

	VGA_Draw_stringXYtoBuffer((char*)"0x2FF0D:", 0, 160, x_DWORD_180628b_screen_buffer);
	sprintf(temp, "%04X", D41A0_BYTESTR_0.str_2FECE.word_0x2FF0D);
	VGA_Draw_stringXYtoBuffer(temp, 210, 160, x_DWORD_180628b_screen_buffer);

	VGA_Draw_stringXYtoBuffer((char*)"0x2FF11:", 0, 176, x_DWORD_180628b_screen_buffer);
	sprintf(temp, "%04X", D41A0_BYTESTR_0.str_2FECE.word_0x2FF11);
	VGA_Draw_stringXYtoBuffer(temp, 210, 176, x_DWORD_180628b_screen_buffer);


	sprintf(temp, "%d", actlevel);
	VGA_Draw_stringXYtoBuffer(temp, 324, 4, x_DWORD_180628b_screen_buffer);


	VGA_Draw_stringXYtoBuffer((char*)"DETAIL:", 256, 480-256-16, x_DWORD_180628b_screen_buffer);
	
	VGA_Draw_stringXYtoBuffer((char*)"TERAIN FEATURES:", 304, 16, x_DWORD_180628b_screen_buffer);

	//sprintf(temp, " 0  | 2  | X  | Y  | Z  | 10 | 12 | 14 | 16 | 18");
	sprintf(temp, " 0  | 2  | X  | Y  | Z");
	VGA_Draw_stringXYtoBuffer(temp, 304, 32, x_DWORD_180628b_screen_buffer);
	for (int i = 0; i < 10; i++)
	{
		type_str_0x30311 actfeat= D41A0_BYTESTR_0.str_2FECE.array_0x30311[first_terrain_feature+i];
		//sprintf(temp, "%04X|%04X|%04X|%04X|%04X|%04X|%04X|%04X|%04X|%04X", actfeat.str_0x30311_word_0, actfeat.str_0x30311_word_2, actfeat.axis3d_4.x, actfeat.axis3d_4.y, actfeat.axis3d_4.z, actfeat.word_10, actfeat.word_12, actfeat.word_14, actfeat.word_16, actfeat.word_18);
		sprintf(temp, "%04X|%04X|%04X|%04X|%04X", actfeat.str_0x30311_word_0, actfeat.str_0x30311_word_2, actfeat.axis3d_4.x, actfeat.axis3d_4.y, actfeat.axis3d_4.z, actfeat.word_10, actfeat.word_12, actfeat.word_14, actfeat.word_16, actfeat.word_18);
		VGA_Draw_stringXYtoBuffer(temp, 304, i*16+48, x_DWORD_180628b_screen_buffer);	
	}
	
	//word_0x2FEFD
	//word_0x2FF01
	//word_0x2FF0D
	//word_0x2FF11
	

	sprintf(temp, "X:%d,Y:%d", x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DEE4_mouse_positiony);
	VGA_Draw_stringXYtoBuffer(temp, 450,480-16 , x_DWORD_180628b_screen_buffer);


}

void editor_mouse_events()//25f0e0
{
	int v3; // edx
	Bit16s v0 =0;

	Bit32u i = 0;
	do
	{		
		if (!(x_WORD_17DEEE & 1) || v0)
		{
			if ((editor_regiondata[i + 12] & 0xff) && sub_7B200_in_region(&editor_regiondata[i], x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DEE4_mouse_positiony))//confirm language
				v3 = editor_regiondata[i + 10] & 0xff;
			else
				v3 = (editor_regiondata[i + 10] & 0xff00) >> 8;
			drawx(editor_regiondata[i + 5], editor_regiondata[i + 6], v3);
			//sub_2BB40_draw_bitmap(x_WORD_E1F84[i + 5], x_WORD_E1F84[i + 6], xy_DWORD_17DED4_spritestr[v3]);
		}
		else if (sub_7B200_in_region(&editor_regiondata[i], x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DEE4_mouse_positiony))//change language
		{
			//sub_8F100_sound_proc19(0, 14, 127, 64, 0x64u, 0, 3u);
			//sub_2BB40_draw_bitmap(editor_regiondata[i + 5], editor_regiondata[i + 6], xy_DWORD_17DED4_spritestr[editor_regiondata[i + 10] & 0xff]);//asi vykresleni stisknuteho tlacitka
			drawx(editor_regiondata[i + 5], editor_regiondata[i + 6], editor_regiondata[i + 10] & 0xff);
			v0 = editor_regiondata[i + 11] & 0xff;
			switch (i / 22)
			{
			case 0: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEE5 -= 4096; changed = true; break; }
			case 1: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEE5 -= 256; changed = true; break; }
			case 2: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEE5-=16; changed = true; break; }
			case 3: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEE5--; changed = true; break; }
			case 4: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEE5++; changed = true; break; }
			case 5: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEE5+=16; changed = true; break; }
			case 6: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEE5+=256; changed = true; break; }
			case 7: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEE5 += 4096; changed = true; break; }

			case 8: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEE9 -= 4096; changed = true; break; }
			case 9: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEE9 -= 256; changed = true; break; }
			case 10: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEE9 -= 16; changed = true; break; }
			case 11: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEE9--; changed = true; break; }
			case 12: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEE9++; changed = true; break; }
			case 13: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEE9 += 16; changed = true; break; }
			case 14: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEE9 += 256; changed = true; break; }
			case 15: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEE9 += 4096; changed = true; break; }

			case 16: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEED -= 4096; changed = true; break; }
			case 17: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEED -= 256; changed = true; break; }
			case 18: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEED -= 16; changed = true; break; }
			case 19: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEED--; changed = true; break; }
			case 20: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEED++; changed = true; break; }
			case 21: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEED += 16; changed = true; break; }
			case 22: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEED += 256; changed = true; break; }
			case 23: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEED += 4096; changed = true; break; }

			case 24: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF1 -= 4096; changed = true; break; }
			case 25: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF1 -= 256; changed = true; break; }
			case 26: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF1 -= 16; changed = true; break; }
			case 27: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF1--; changed = true; break; }
			case 28: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF1++; changed = true; break; }
			case 29: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF1 += 16; changed = true; break; }
			case 30: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF1 += 256; changed = true; break; }
			case 31: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF1 += 4096; changed = true; break; }

			case 32: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF5 -= 4096; changed = true; break; }
			case 33: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF5 -= 256; changed = true; break; }
			case 34: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF5 -= 16; changed = true; break; }
			case 35: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF5--; changed = true; break; }
			case 36: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF5++; changed = true; break; }
			case 37: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF5 += 16; changed = true; break; }
			case 38: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF5 += 256; changed = true; break; }
			case 39: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF5 += 4096; changed = true; break; }

			case 40: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF9 -= 4096; changed = true; break; }
			case 41: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF9 -= 256; changed = true; break; }
			case 42: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF9 -= 16; changed = true; break; }
			case 43: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF9--; changed = true; break; }
			case 44: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF9++; changed = true; break; }
			case 45: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF9 += 16; changed = true; break; }
			case 46: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF9 += 256; changed = true; break; }
			case 47: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEF9 += 4096; changed = true; break; }

			case 48: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEFD -= 4096; changed = true; break; }
			case 49: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEFD -= 256; changed = true; break; }
			case 50: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEFD -= 16; changed = true; break; }
			case 51: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEFD--; changed = true; break; }
			case 52: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEFD++; changed = true; break; }
			case 53: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEFD += 16; changed = true; break; }
			case 54: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEFD += 256; changed = true; break; }
			case 55: {D41A0_BYTESTR_0.str_2FECE.word_0x2FEFD += 4096; changed = true; break; }

			case 56: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF01 -= 4096; changed = true; break; }
			case 57: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF01 -= 256; changed = true; break; }
			case 58: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF01 -= 16; changed = true; break; }
			case 59: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF01--; changed = true; break; }
			case 60: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF01++; changed = true; break; }
			case 61: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF01 += 16; changed = true; break; }
			case 62: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF01 += 256; changed = true; break; }
			case 63: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF01 += 4096; changed = true; break; }

			case 64: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF05 -= 4096; changed = true; break; }
			case 65: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF05 -= 256; changed = true; break; }
			case 66: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF05 -= 16; changed = true; break; }
			case 67: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF05--; changed = true; break; }
			case 68: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF05++; changed = true; break; }
			case 69: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF05 += 16; changed = true; break; }
			case 70: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF05 += 256; changed = true; break; }
			case 71: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF05 += 4096; changed = true; break; }

			case 72: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF09 -= 4096; changed = true; break; }
			case 73: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF09 -= 256; changed = true; break; }
			case 74: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF09 -= 16; changed = true; break; }
			case 75: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF09--; changed = true; break; }
			case 76: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF09++; changed = true; break; }
			case 77: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF09 += 16; changed = true; break; }
			case 78: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF09 += 256; changed = true; break; }
			case 79: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF09 += 4096; changed = true; break; }

			case 80: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF0D -= 4096; changed = true; break; }
			case 81: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF0D -= 256; changed = true; break; }
			case 82: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF0D -= 16; changed = true; break; }
			case 83: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF0D--; changed = true; break; }
			case 84: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF0D++; changed = true; break; }
			case 85: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF0D += 16; changed = true; break; }
			case 86: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF0D += 256; changed = true; break; }
			case 87: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF0D += 4096; changed = true; break; }

			case 88: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF11 -= 4096; changed = true; break; }
			case 89: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF11 -= 256; changed = true; break; }
			case 90: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF11 -= 16; changed = true; break; }
			case 91: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF11--; changed = true; break; }
			case 92: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF11++; changed = true; break; }
			case 93: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF11 += 16; changed = true; break; }
			case 94: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF11 += 256; changed = true; break; }
			case 95: {D41A0_BYTESTR_0.str_2FECE.word_0x2FF11 += 4096; changed = true; break; }

			case 96: {
				minimapx = x_DWORD_17DEE4_mouse_positionx;
				minimapy = x_DWORD_17DEE4_mouse_positiony;
				break;
			}
			case 97: {maptype = 0; changed = true; break; }
			case 98: {maptype = 1; changed = true; break; }
			case 99: {maptype = 2; changed = true; break; }
			case 100: {maptype = 3; changed = true; break; }				

			//case 101: {loadlevel(0); changed = true; break; }
			case 102: {
				actlevel--;
				if (actlevel < 0)actlevel =0;
				loadlevel(actlevel); changed = true; break;
			}
			case 103: {
				actlevel++;
				if (actlevel > 24)actlevel = 24;
				loadlevel(actlevel); changed = true; break;
			}

			}
			/*
				D41A0_BYTESTR_0.str_2FECE.word_0x2FEE5 = 128;
				D41A0_BYTESTR_0.str_2FECE.word_0x2FEE9 = 128;
				D41A0_BYTESTR_0.str_2FECE.word_0x2FEED = 128;
				D41A0_BYTESTR_0.str_2FECE.word_0x2FEF1 = 128;
			*/
		}
		else
			drawx(editor_regiondata[i + 5], editor_regiondata[i + 6], (editor_regiondata[i + 10] & 0xff00) >> 8);
		i += 22;
	} while (editor_regiondata[i + 10] & 0xff);
}

void editor_loop()
{
	bool runnning = true;
	while (runnning)
	{
		if (x_BYTE_17DF10_get_key_scancode == 59)
		{
			runnning = false;
		}
		memset(x_DWORD_180628b_screen_buffer, 0, 640 * 480);
		
		editor_mouse_events();//25f0e0 adress 258ca1 - zmeni tlacitko, vrati stav kliknuti
		if (changed)
			terrain_recalculate();
		//4527b9=[ebp+66]+6 4527bf 161545005835->58e2e2e2e2e2
		//[ebp+66]+6//save1
		drawmenus();
		drawterrain(0,480-256);
		drawdetail(256,480-256,256,8);
		drawtexts();
		drawcursor();
		sub_75200_VGA_Blit640(480);//vykresleni //256200		
		sub_7A060_get_mouse_and_keyboard_events();//25b060 adress 258ddf
	}
}

/*
  kiss_sdl widget toolkit
  Copyright (c) 2016 Tarvo Korrovits <tkorrovi@mail.com>

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
	 claim that you wrote the original software. If you use this software
	 in a product, an acknowledgment in the product documentation would
	 be appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not
	 be misrepresented as being the original software.
  3. This notice may not be removed or altered from any source
	 distribution.

  kiss_sdl version 1.2.0
*/

static void text_reset(kiss_textbox* textbox, kiss_vscrollbar* vscrollbar)
{
	qsort(textbox->array->data, textbox->array->length, sizeof(void*),
		kiss_string_compare);
	vscrollbar->step = 0.;
	if (textbox->array->length - textbox->maxlines > 0)
		vscrollbar->step = 1. / (textbox->array->length -
			textbox->maxlines);
	textbox->firstline = 0;
	textbox->highlightline = -1;
	vscrollbar->fraction = 0.;
}

/* Read directory entries into the textboxes */
/*static void dirent_read(kiss_textbox* textbox1, kiss_vscrollbar* vscrollbar1,
	kiss_textbox* textbox2, kiss_vscrollbar* vscrollbar2,
	kiss_label* label_sel)
{
	kiss_dirent* ent;
	kiss_stat s;
	kiss_dir* dir;
	char buf[KISS_MAX_LENGTH], ending[2];

	kiss_array_free(textbox1->array);
	kiss_array_free(textbox2->array);
	kiss_array_new(textbox1->array);
	kiss_array_new(textbox2->array);
	

	kiss_getcwd(buf, KISS_MAX_LENGTH);
	strcpy(ending, "/");
	if (buf[0] == 'C') strcpy(ending, "\\");
	if (!strcmp(buf, "/") || !strcmp(buf, "C:\\")) strcpy(ending, "");
	kiss_string_copy(label_sel->text, (2 * textbox1->rect.w +2 * kiss_up.w) / kiss_textfont.advance, buf, ending);
#ifdef _MSC_VER
	dir = kiss_opendir((char*)"*");
#else
	dir = kiss_opendir(".");
#endif
	while ((ent = kiss_readdir(dir))) {
		if (!ent->d_name) continue;
		kiss_getstat(ent->d_name, &s);		
		if (kiss_isdir(s))
			kiss_array_appendstring(textbox1->array, 0,
				ent->d_name, (char*)"/");
		else if (kiss_isreg(s))
			kiss_array_appendstring(textbox2->array, 0,
				ent->d_name, NULL);
	}
	kiss_closedir(dir);
	text_reset(textbox1, vscrollbar1);
	text_reset(textbox2, vscrollbar2);
}*/

/* Read directory entries into the textboxes */
static void terrain_feat_append(kiss_textbox* textbox1, kiss_vscrollbar* vscrollbar1){
	kiss_array_free(textbox1->array);
	kiss_array_new(textbox1->array);
	char temp[256];
	//VGA_Draw_stringXYtoBuffer(temp, 304, 32, x_DWORD_180628b_screen_buffer);
	for (int i = 0; i < 0x4B0; i++)
	{
		type_str_0x30311 actfeat = temparray_0x30311[first_terrain_feature + i];//D41A0_BYTESTR_0.str_2FECE.array_0x30311[first_terrain_feature + i];
		sprintf(temp, "%04X|%04X|%04X|%04X|%04X|%04X|%04X|%04X|%04X|%04XA", actfeat.str_0x30311_word_0, actfeat.str_0x30311_word_2, actfeat.axis3d_4.x, actfeat.axis3d_4.y, actfeat.axis3d_4.z, actfeat.word_10, actfeat.word_12, actfeat.word_14, actfeat.word_16, actfeat.word_18);		
		if(temparray_0x30311_inactive[first_terrain_feature + i])temp[strlen(temp) - 1] = 'I';//set last char as inactive
		kiss_array_appendstring(textbox1->array, 0, (char*)"", temp);
	}		
	//text_reset(textbox1, vscrollbar1);
}

/* The widget arguments are widgets that this widget talks with */
static int textbox1_event(kiss_textbox* textbox, SDL_Event* e,	kiss_vscrollbar* vscrollbar1,int mousex,int mousey,int* draw)
{
	int index;
	int result = -1;

	if (kiss_textbox_event(textbox, e, mousex,mousey,draw)) {
		index = textbox->firstline + textbox->selectedline;
		if (strcmp((char*)kiss_array_data(textbox->array, index),"")) {
			textbox->selectedline = -1;
			kiss_chdir((char*)kiss_array_data(textbox->array,index));
			result = index;
			//dirent_read(textbox, vscrollbar1, textbox2,	vscrollbar2, label_sel);
			*draw = 1;
		}
	}
	return result;
}

static void vscrollbar1_event(kiss_vscrollbar* vscrollbar, SDL_Event* e,
	kiss_textbox* textbox1, int* draw)
{
	int firstline;

	if (kiss_vscrollbar_event(vscrollbar, e, draw) &&
		textbox1->array->length - textbox1->maxlines > 0) {
		firstline = (int)((textbox1->array->length -
			textbox1->maxlines) * vscrollbar->fraction + 0.5);
		if (firstline >= 0) textbox1->firstline = firstline;
		*draw = 1;
	}
}
/*
static void textbox2_event(kiss_textbox* textbox, SDL_Event* e,
	kiss_vscrollbar* vscrollbar2, kiss_entry* entry, int* draw)
{
	int index;

	if (kiss_textbox_event(textbox, e, draw)) {
		index = textbox->firstline + textbox->selectedline;
		if (strcmp((char*)kiss_array_data(textbox->array, index),
			"")) {
			kiss_string_copy(entry->text,
				entry->textwidth / kiss_textfont.advance,
				(char*)kiss_array_data(textbox->array,
					index), NULL);
			*draw = 1;
		}
	}
}*/

static void vscrollbar2_event(kiss_vscrollbar* vscrollbar, SDL_Event* e,
	kiss_textbox* textbox2, int* draw)
{
	int firstline;

	if (kiss_vscrollbar_event(vscrollbar, e, draw) &&
		textbox2->array->length) {
		firstline = (int)((textbox2->array->length -
			textbox2->maxlines) * vscrollbar->fraction + 0.5);
		if (firstline >= 0) textbox2->firstline = firstline;
		*draw = 1;
	}
}

static void button_ok1_event(kiss_button* button, SDL_Event* e,	kiss_window* window1, kiss_window* window2, kiss_label* label_sel,
	kiss_entry* entry, kiss_label* label_res,
	kiss_progressbar* progressbar, int* draw)
{
	char buf[KISS_MAX_LENGTH];

	if (kiss_button_event(button, e, draw)) {
		kiss_string_copy(buf, kiss_maxlength(kiss_textfont,
			window2->rect.w - 2 * kiss_vslider.w,
			label_sel->text, entry->text),
			label_sel->text, entry->text);
		kiss_string_copy(label_res->text, KISS_MAX_LABEL,
			(char*)"The following path was selected:\n", buf);
		window2->visible = 1;
		window2->focus = 1;
		window1->focus = 0;
		button->prelight = 0;
		progressbar->fraction = 0.;
		progressbar->run = 1;
		*draw = 1;
	}
}

kiss_window window1, window2;
kiss_hex4edit hex4edit1feat = { 0 };
kiss_hex4edit hex4edit2feat = { 0 };
kiss_hex4edit hex4edit3feat = { 0 };
kiss_hex4edit hex4edit4feat = { 0 };
kiss_hex4edit hex4edit5feat = { 0 };
kiss_hex4edit hex4edit6feat = { 0 };
kiss_hex4edit hex4edit7feat = { 0 };
kiss_hex4edit hex4edit8feat = { 0 };
kiss_hex4edit hex4edit9feat = { 0 };
kiss_hex4edit hex4edit10feat = { 0 };

static void button_ok_event(kiss_button* button, SDL_Event* e,
	int* quit, int* draw)
{
	if (kiss_button_event(button, e, draw))
	{
		window2.visible = 0;
		window2.focus = 0;
		window1.focus = 1;
	}//*quit = 1;
}

static int button_del_event(kiss_button* button, SDL_Event* e,	int* quit, int* draw)
{
	if (kiss_button_event(button, e, draw))
	{
		for (int i = edited_line2 + 2; i < 0x4b0; i++)
		{		
			temparray_0x30311[i - 1] = temparray_0x30311[i];
			temparray_0x30311_inactive[i-1] = temparray_0x30311_inactive[i];
		}
		window2.visible = 0;
		window2.focus = 0;
		window1.focus = 1;
		return 1;
	}	
	return 0;
}

static int button_add_event(kiss_button* button, SDL_Event* e,	int* quit, int* draw)
{
	if (kiss_button_event(button, e, draw))
	{
		for (int i = 0x4b0 - 1; i > edited_line2 + 1; i--)
		{
			temparray_0x30311[i] = temparray_0x30311[i - 1];
			temparray_0x30311_inactive[i] = temparray_0x30311_inactive[i-1];
		}
		//memset(&temparray_0x30311[edited_line2+1], 0, sizeof(temparray_0x30311[edited_line2+1]));
		temparray_0x30311_inactive[edited_line2 + 1] = false;
		kiss_hex4edit_update_adress(&hex4edit1feat, &temparray_0x30311[edited_line2 + 1].str_0x30311_word_0);
		kiss_hex4edit_update_adress(&hex4edit2feat, &temparray_0x30311[edited_line2 + 1].str_0x30311_word_2);
		kiss_hex4edit_update_adress(&hex4edit3feat, &temparray_0x30311[edited_line2 + 1].axis3d_4.x);
		kiss_hex4edit_update_adress(&hex4edit4feat, &temparray_0x30311[edited_line2 + 1].axis3d_4.y);
		kiss_hex4edit_update_adress(&hex4edit5feat, &temparray_0x30311[edited_line2 + 1].axis3d_4.z);
		kiss_hex4edit_update_adress(&hex4edit6feat, &temparray_0x30311[edited_line2 + 1].word_10);
		kiss_hex4edit_update_adress(&hex4edit7feat, &temparray_0x30311[edited_line2 + 1].word_12);
		kiss_hex4edit_update_adress(&hex4edit8feat, &temparray_0x30311[edited_line2 + 1].word_14);
		kiss_hex4edit_update_adress(&hex4edit9feat, &temparray_0x30311[edited_line2 + 1].word_16);
		kiss_hex4edit_update_adress(&hex4edit10feat, &temparray_0x30311[edited_line2 + 1].word_18);
		return 1;
	}
	return 0;
}

static int button_clean_event(kiss_button* button, SDL_Event* e, int* quit, int* draw)
{
	if (kiss_button_event(button, e, draw))
	{
		memset(&temparray_0x30311[edited_line2 + 1], 0, sizeof(temparray_0x30311[edited_line2 + 1]));
		temparray_0x30311_inactive[edited_line2 + 1] = false;
		kiss_hex4edit_update_adress(&hex4edit1feat, &temparray_0x30311[edited_line2 + 1].str_0x30311_word_0);
		kiss_hex4edit_update_adress(&hex4edit2feat, &temparray_0x30311[edited_line2 + 1].str_0x30311_word_2);
		kiss_hex4edit_update_adress(&hex4edit3feat, &temparray_0x30311[edited_line2 + 1].axis3d_4.x);
		kiss_hex4edit_update_adress(&hex4edit4feat, &temparray_0x30311[edited_line2 + 1].axis3d_4.y);
		kiss_hex4edit_update_adress(&hex4edit5feat, &temparray_0x30311[edited_line2 + 1].axis3d_4.z);
		kiss_hex4edit_update_adress(&hex4edit6feat, &temparray_0x30311[edited_line2 + 1].word_10);
		kiss_hex4edit_update_adress(&hex4edit7feat, &temparray_0x30311[edited_line2 + 1].word_12);
		kiss_hex4edit_update_adress(&hex4edit8feat, &temparray_0x30311[edited_line2 + 1].word_14);
		kiss_hex4edit_update_adress(&hex4edit9feat, &temparray_0x30311[edited_line2 + 1].word_16);
		kiss_hex4edit_update_adress(&hex4edit10feat, &temparray_0x30311[edited_line2 + 1].word_18);
		return 1;
	}
	return 0;
}

static void button_ok2_event(kiss_button* button, SDL_Event* e,
	kiss_window* window1, kiss_window* window2,
	kiss_progressbar* progressbar, int* draw)
{
	if (kiss_button_event(button, e, draw)) {
		window2->visible = 0;
		window1->focus = 1;
		button->prelight = 0;
		progressbar->fraction = 0.;
		progressbar->run = 0;
		*draw = 1;
	}
}

static int select1_event(kiss_selectbutton* select1, SDL_Event* e, kiss_selectbutton* select2, kiss_selectbutton* select3, kiss_selectbutton* select4, int* draw)
{
	if (kiss_selectbutton_event(select1, e, draw))
	{
		select2->selected = select1->selected^1;
		select3->selected = select1->selected ^ 1;
		select4->selected = select1->selected ^ 1;
		maptype = 0;
		return 1;
	}
	return 0;
}

static int select2_event(kiss_selectbutton* select2, SDL_Event* e, kiss_selectbutton* select1, kiss_selectbutton* select3, kiss_selectbutton* select4, int* draw)
{
	if (kiss_selectbutton_event(select2, e, draw))
	{
		select1->selected = select2->selected ^ 1;
		select3->selected = select2->selected ^ 1;
		select4->selected = select2->selected ^ 1;
		maptype = 1;
		return 1;
	}
	return 0;
}

static int select3_event(kiss_selectbutton* select3, SDL_Event* e, kiss_selectbutton* select1, kiss_selectbutton* select2, kiss_selectbutton* select4, int* draw)
{
	if (kiss_selectbutton_event(select3, e, draw))
	{
		select1->selected = select3->selected ^ 1;
		select2->selected = select3->selected ^ 1;
		select4->selected = select3->selected ^ 1;
		maptype = 2;
		return 1;
	}
	return 0;
}

static int select4_event(kiss_selectbutton* select4, SDL_Event* e, kiss_selectbutton* select1, kiss_selectbutton* select2, kiss_selectbutton* select3, int* draw)
{
	if (kiss_selectbutton_event(select4, e, draw))
	{
		select1->selected = select4->selected ^ 1;
		select2->selected = select4->selected ^ 1;
		select3->selected = select4->selected ^ 1;
		maptype = 3;
		return 1;
	}
	return 0;
}

static int select1_feat_event(kiss_selectbutton* select, SDL_Event* e, kiss_array* objects, int* draw)
{
	if (kiss_selectbutton_event(select, e, draw))
	{
		//char** buff = (char**)(&objects->data[edited_line2]);
		if (select->selected)
			temparray_0x30311_inactive[edited_line2 + 1] = false;
			//*buff[strlen(*buff)] = 'A';
		else
			temparray_0x30311_inactive[edited_line2 + 1] = true;
			//*buff[strlen(*buff)] = 'I';
		return 1;
	}
	return 0;
}

bool first = true;
int main_x(/*int argc, char** argv*/)
{
	SDL_Event e;
	kiss_array objects/*, a1, a2*/;	
	//kiss_label label2 = { 0 },label_res = { 0 };

	kiss_label label_terfeat = { 0 };
	kiss_label label_terfeat2 = { 0 };
	//kiss_button button_ok1 = { 0 }, button_ok2 = { 0 }, button_cancel = { 0 };
	kiss_textbox textbox1 = { 0 };
	kiss_vscrollbar vscrollbar1 = { 0 };
	kiss_progressbar progressbar = { 0 };
	kiss_entry entry = { 0 };
	int textbox_width, textbox_height, window2_width, window2_height, draw, quit;

	kiss_hex4edit hex4edit1 = { 0 };
	kiss_hex4edit hex4edit2 = { 0 };
	kiss_hex4edit hex4edit3 = { 0 };
	kiss_hex4edit hex4edit4 = { 0 };
	kiss_hex4edit hex4edit5 = { 0 };
	kiss_hex4edit hex4edit6 = { 0 };
	kiss_hex4edit hex4edit7 = { 0 };
	kiss_hex4edit hex4edit8 = { 0 };
	kiss_hex4edit hex4edit9 = { 0 };
	kiss_hex4edit hex4edit10 = { 0 };
	kiss_hex4edit hex4edit11 = { 0 };
	kiss_hex4edit hex4edit12 = { 0 };

	kiss_terrain terrain1 = { 0 };
	kiss_terrain terrainfeat = { 0 };

	kiss_label labelSel = { 0 };
	kiss_selectbutton select1 = { 0 };
	kiss_selectbutton select2 = { 0 };
	kiss_selectbutton select3 = { 0 };
	kiss_selectbutton select4 = { 0 };

	kiss_dec1edit levelSel = { 0 };

	kiss_label labelXY = { 0 };

	const int count_features = 35;
	/*kiss_button button_del[count_features];
	for (int i = 0; i < count_features; i++) button_del[i] = { 0 };
	kiss_button button_add[count_features];
	for (int i = 0; i < count_features; i++) button_add[i] = { 0 };*/

	kiss_label levelSelFeat = { 0 };
	kiss_selectbutton select1feat = { 0 };
	kiss_button button_ok1feat = { 0 }, button_plusfeat = { 0 } , button_deletefeat = { 0 }, button_cleanfeat = { 0 };
	kiss_label label_plusFeat = { 0 };

	SDL_ShowCursor(true);
	quit = 0;
	draw = 1;
	textbox_width = 460;
	textbox_height = count_features * 18 + 12;
	window2_width = 532;
	window2_height = 740;//300;
	editor_renderer = kiss_init((char*)"REMC2 Editor", &objects, 1024, 768);
	if (!editor_renderer) return 1;
	//kiss_array_new(&a1);
	//kiss_array_append(&objects, ARRAY_TYPE, &a1);
	//kiss_array_new(&a2);
	//kiss_array_append(&objects, ARRAY_TYPE, &a2);
	kiss_array_new(&objects);
	/* Arrange the widgets nicely relative to each other */
	kiss_window_new(&window1, NULL, 1, 0, 0, kiss_screen_width, kiss_screen_height);
	kiss_textbox_new(&textbox1, &window1, 1, &objects, 530, 50, textbox_width, textbox_height);


#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	Uint32 rmask = 0xff000000;
	Uint32 gmask = 0x00ff0000;
	Uint32 bmask = 0x0000ff00;
	Uint32 amask = 0x000000ff;
#else
	Uint32 rmask = 0x000000ff;
	Uint32 gmask = 0x0000ff00;
	Uint32 bmask = 0x00ff0000;
	Uint32 amask = 0xff000000;
#endif
	mapsurface = SDL_CreateRGBSurface(SDL_SWSURFACE, 512, 512, 32, rmask, gmask, bmask, amask);
	if (mapsurface == NULL) {
		fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
		exit(1);
	}
	mapsurfacefeat = SDL_CreateRGBSurface(SDL_SWSURFACE, 512, 512, 32, rmask, gmask, bmask, amask);
	if (mapsurfacefeat == NULL) {
		fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
		exit(1);
	}
	/*Bit8u* scrbuff = (Bit8u*)mapsurface->pixels;
	for (int j = 0; j < 256; j++)
		for (int i = 0; i < 256; i++) {
			scrbuff[4 * (i * 256 + j)] = 128;
			scrbuff[4 * (i * 256 + j)+1] = 128;
			scrbuff[4 * (i * 256 + j)+2] = 128;
			scrbuff[4 * (i * 256 + j)+3] = 128;
		}*/
	mapimage.w = 512;
	mapimage.h = 512;
	mapimage.magic = KISS_MAGIC;
	mapimage.image = SDL_CreateTextureFromSurface(editor_renderer, mapsurface);

	mapimagefeat.w = 512;
	mapimagefeat.h = 512;
	mapimagefeat.magic = KISS_MAGIC;
	mapimagefeat.image = SDL_CreateTextureFromSurface(editor_renderer, mapsurfacefeat);

	char path2[512];
	kiss_image img_delete_normal;
	FixDir(path2, (char*)"kiss\\kiss_delete_normal.png");
	img_delete_normal.w = 16;
	img_delete_normal.h = 16;
	img_delete_normal.magic = KISS_MAGIC;
	img_delete_normal.image = IMG_LoadTexture(editor_renderer, path2);

	kiss_image img_delete_prelight;
	FixDir(path2, (char*)"kiss\\kiss_delete_prelight.png");
	img_delete_prelight.w = 16;
	img_delete_prelight.h = 16;
	img_delete_prelight.magic = KISS_MAGIC;
	img_delete_prelight.image = IMG_LoadTexture(editor_renderer, path2);

	kiss_image img_clean_normal;
	FixDir(path2, (char*)"kiss\\kiss_clean_normal.png");
	img_clean_normal.w = 16;
	img_clean_normal.h = 16;
	img_clean_normal.magic = KISS_MAGIC;
	img_clean_normal.image = IMG_LoadTexture(editor_renderer, path2);

	kiss_image img_clean_prelight;
	FixDir(path2, (char*)"kiss\\kiss_clean_prelight.png");
	img_clean_prelight.w = 16;
	img_clean_prelight.h = 16;
	img_clean_prelight.magic = KISS_MAGIC;
	img_clean_prelight.image = IMG_LoadTexture(editor_renderer, path2);

	kiss_image img_add_normal;
	FixDir(path2, (char*)"kiss\\kiss_add_normal.png");
	img_add_normal.w = 16;
	img_add_normal.h = 16;
	img_add_normal.magic = KISS_MAGIC;
	img_add_normal.image = IMG_LoadTexture(editor_renderer, path2);

	kiss_image img_add_prelight;
	FixDir(path2, (char*)"kiss\\kiss_add_prelight.png");
	img_add_prelight.w = 16;
	img_add_prelight.h = 16;
	img_add_prelight.magic = KISS_MAGIC;
	img_add_prelight.image = IMG_LoadTexture(editor_renderer, path2);

	kiss_vscrollbar_new(&vscrollbar1, &window1, textbox1.rect.x + textbox_width, textbox1.rect.y, textbox_height);
	//kiss_textbox_new(&textbox2, &window1, 1, &a2,vscrollbar1.uprect.x + kiss_up.w, textbox1.rect.y,	textbox_width, textbox_height);
	//kiss_vscrollbar_new(&vscrollbar2, &window1, textbox2.rect.x +textbox_width, vscrollbar1.uprect.y, textbox_height);
	kiss_label_new(&label_terfeat, &window1, (char*)"TERRAN FEATURES:", 13 + textbox1.rect.x + kiss_edge, 10);
	kiss_label_new(&label_terfeat2, &window1, (char*)" 0  | 2  | X  | Y  | Z  | 10 | 12 | 14 | 16 | 18", 13 + textbox1.rect.x + kiss_edge, textbox1.rect.y - kiss_textfont.lineheight);
	//kiss_label_new(&label2, &window1, (char*)"Files", textbox2.rect.x +	kiss_edge, textbox1.rect.y - kiss_textfont.lineheight);
	//kiss_label_new(&label_sel, &window1, (char*)"", textbox1.rect.x +kiss_edge, textbox1.rect.y + textbox_height +kiss_normal.h);
	//kiss_entry_new(&entry, &window1, 1, (char*)"kiss", textbox1.rect.x,label_sel.rect.y + kiss_textfont.lineheight,2 * textbox_width + 2 * kiss_up.w + kiss_edge);
	//kiss_button_new(&button_cancel, &window1, (char*)"Cancel",entry.rect.x + entry.rect.w - kiss_edge - kiss_normal.w,entry.rect.y + entry.rect.h + kiss_normal.h);
	//kiss_button_new(&button_ok1, &window1, (char*)"OK", button_cancel.rect.x -2 * kiss_normal.w, button_cancel.rect.y);

	/*for (int i = 0; i < count_features; i++)
	{
		kiss_button_new(&button_del[i], &window1, (char*)" ", 13 + textbox1.rect.x + textbox1.rect.w, 2 + textbox1.rect.y + i * 18, &img_delete_normal);
		button_del[i].prelightimg = img_delete_prelight;
	}
	for (int i = 0; i < count_features; i++)
	{
		kiss_button_new(&button_add[i], &window1, (char*)" ", 13+16 + textbox1.rect.x + textbox1.rect.w, 2 + textbox1.rect.y + i * 18, &img_add_normal);
		button_add[i].prelightimg = img_add_prelight;
	}*/

	kiss_window_new(&window2, NULL, 1, kiss_screen_width / 2 -window2_width / 2, kiss_screen_height / 2 -window2_height / 2, window2_width, window2_height);
	
	//kiss_label_new(&label_res, &window2, (char*)"", window2.rect.x +kiss_up.w, window2.rect.y + kiss_vslider.h);
	//label_res.textcolor = kiss_blue;
	kiss_progressbar_new(&progressbar, &window2, window2.rect.x +kiss_up.w - kiss_edge, window2.rect.y + window2.rect.h / 2 -kiss_bar.h / 2 - kiss_border,window2.rect.w - 2 * kiss_up.w + 2 * kiss_edge);
	//kiss_button_new(&button_ok2, &window2, (char*)"OK", window2.rect.x +window2.rect.w / 2 - kiss_normal.w / 2,progressbar.rect.y + progressbar.rect.h +2 * kiss_vslider.h);
	
	kiss_hex4edit_new(&hex4edit1, &window1, &D41A0_BYTESTR_0.str_2FECE.word_0x2FEE5, (char*)"RANDOM SEED:", 10, 10);
	kiss_hex4edit_new(&hex4edit2, &window1, &D41A0_BYTESTR_0.str_2FECE.word_0x2FEE9, (char*)"SHIFT:", 10, 30);
	kiss_hex4edit_new(&hex4edit3, &window1, &D41A0_BYTESTR_0.str_2FECE.word_0x2FEED, (char*)"HEIGHT:", 10, 50);
	kiss_hex4edit_new(&hex4edit4, &window1, &D41A0_BYTESTR_0.str_2FECE.word_0x2FEF1, (char*)"NOISE:", 10, 70);
	kiss_hex4edit_new(&hex4edit5, &window1, &D41A0_BYTESTR_0.str_2FECE.word_0x2FEF5, (char*)"RIVERS:", 10, 90);
	kiss_hex4edit_new(&hex4edit6, &window1, &D41A0_BYTESTR_0.str_2FECE.word_0x2FEF9, (char*)"RIVERSH:", 10, 110);
	kiss_hex4edit_new(&hex4edit7, &window1, &D41A0_BYTESTR_0.str_2FECE.word_0x2FEFD, (char*)"0x2FEFD:", 10, 130);
	kiss_hex4edit_new(&hex4edit8, &window1, &D41A0_BYTESTR_0.str_2FECE.word_0x2FF01, (char*)"0x2FF01:", 10, 150);
	kiss_hex4edit_new(&hex4edit9, &window1, &D41A0_BYTESTR_0.str_2FECE.word_0x2FF05, (char*)"0x2FF05:", 10, 170);
	kiss_hex4edit_new(&hex4edit10, &window1, &D41A0_BYTESTR_0.str_2FECE.word_0x2FF09, (char*)"0x2FF09:", 10, 190);
	kiss_hex4edit_new(&hex4edit11, &window1, &D41A0_BYTESTR_0.str_2FECE.word_0x2FF0D, (char*)"0x2FF0D:", 10, 210);
	kiss_hex4edit_new(&hex4edit12, &window1, &D41A0_BYTESTR_0.str_2FECE.word_0x2FF11, (char*)"0x2FF11:", 10, 230);

	//drawterrain2(0, window1.rect.h - mapimage.h, 10, 0, 0);
	kiss_terrain_new(&terrain1, &window1, mapsurface, kiss_border, window1.rect.h - mapimage.h- kiss_border,&terrainzoom,&terrainbeginx,&terrainbeginy);
	kiss_array combo1ar;

	//kiss_selectbutton_new(&selectbutton1, &window2,50, 50);
	kiss_label_new(&labelSel, &window1, (char*)"Map layer:",290, 226);
	kiss_selectbutton_new(&select1, &window1, labelSel.rect.x+110, labelSel.rect.y);
	kiss_selectbutton_new(&select2, &window1, select1.rect.x+20, labelSel.rect.y);
	kiss_selectbutton_new(&select3, &window1, select2.rect.x + 20, labelSel.rect.y);
	kiss_selectbutton_new(&select4, &window1, select3.rect.x + 20, labelSel.rect.y);


	kiss_dec1edit_new(&levelSel, &window1, &actlevel,(char*)"Level:", 1,24,290, 206);

	kiss_label_new(&labelXY, &window1, (char*)"", 900, 740);

	Bit16u temp_var;
	kiss_hex4edit_new(&hex4edit1feat, &window2, &temp_var, (char*)" 0:", window2.rect.x + kiss_up.w, window2.rect.y+ 10);
	kiss_hex4edit_new(&hex4edit2feat, &window2, &temp_var, (char*)" 2:", window2.rect.x + kiss_up.w, window2.rect.y + 30);
	kiss_hex4edit_new(&hex4edit3feat, &window2, &temp_var, (char*)" X:", window2.rect.x + kiss_up.w, window2.rect.y + 50);
	kiss_hex4edit_new(&hex4edit4feat, &window2, &temp_var, (char*)" Y:", window2.rect.x + kiss_up.w, window2.rect.y + 70);
	kiss_hex4edit_new(&hex4edit5feat, &window2, &temp_var, (char*)" Z:", window2.rect.x + kiss_up.w, window2.rect.y + 90);
	kiss_hex4edit_new(&hex4edit6feat, &window2, &temp_var, (char*)"10:", window2.rect.x + kiss_up.w, window2.rect.y + 110);
	kiss_hex4edit_new(&hex4edit7feat, &window2, &temp_var, (char*)"12:", window2.rect.x + kiss_up.w, window2.rect.y + 130);
	kiss_hex4edit_new(&hex4edit8feat, &window2, &temp_var, (char*)"14:", window2.rect.x + kiss_up.w, window2.rect.y + 150);
	kiss_hex4edit_new(&hex4edit9feat, &window2, &temp_var, (char*)"16:", window2.rect.x + kiss_up.w, window2.rect.y + 170);
	kiss_hex4edit_new(&hex4edit10feat, &window2, &temp_var, (char*)"18:", window2.rect.x + kiss_up.w, window2.rect.y + 190);

	kiss_label_new(&levelSelFeat, &window2, (char*)"Active:", 300+window2.rect.x + kiss_up.w, window2.rect.y + 10);
	kiss_selectbutton_new(&select1feat, &window2, 300+window2.rect.x + kiss_up.w+80, window2.rect.y+10);
	kiss_label_new(&label_plusFeat, &window2, (char*)"Add:", 300 + window2.rect.x + kiss_up.w, window2.rect.y + 30);
	kiss_button_new(&button_plusfeat, &window2, (char*)" ", 300 + window2.rect.x + kiss_up.w+80, window2.rect.y + 30, &img_add_normal);
	button_plusfeat.prelightimg = img_add_prelight;
	kiss_button_new(&button_deletefeat, &window2, (char*)" ", 300 + window2.rect.x + kiss_up.w + 80, window2.rect.y + 50, &img_delete_normal);
	button_deletefeat.prelightimg = img_delete_prelight;
	kiss_button_new(&button_cleanfeat, &window2, (char*)" ", 300 + window2.rect.x + kiss_up.w + 80, window2.rect.y + 70, &img_clean_normal);
	button_cleanfeat.prelightimg = img_clean_prelight;
	kiss_button_new(&button_ok1feat, &window2, (char*)"OK", 300 + window2.rect.x + kiss_up.w + 80, window2.rect.y + 110);
	//kiss_button_new(&button_cancelfeat, &window2, (char*)"CANCEL", window2.rect.x + window2.rect.w-100-kiss_normal.w / 2, window2.rect.y + 250);
	kiss_terrain_new(&terrainfeat, &window2, mapsurfacefeat, window2.rect.x+ kiss_border, window2.rect.y+window2.rect.h - mapimagefeat.h- kiss_border, &terrainzoomfeat, &terrainbeginxfeat, &terrainbeginyfeat);
	


	//dirent_read(&textbox1, &vscrollbar1, &textbox2, &vscrollbar2,&label_sel);
	
	terrain_feat_append(&textbox1, &vscrollbar1);

	select1.selected = 1;
	/* Do that, and all widgets associated with the window will show */
	window1.visible = 1;

	while (!quit) {

		/* Some code may be written here */
		bool changed = false;
		bool changed2 = false;
		bool zoomchanged = false;
		bool zoomchangedfeat = false;
		SDL_Delay(10);
		while (SDL_PollEvent(&e)) {

			int mousex;
			int mousey;
			SDL_GetMouseState(&mousex, &mousey);
			//updatexystr((char**)&xystr,mousex, mousey);
			sprintf(labelXY.text, "%d,%d", mousex, mousey);

			if (e.type == SDL_QUIT) quit = 1;
			
			kiss_window_event(&window2, &e, &draw);
			kiss_window_event(&window1, &e, &draw);
			edited_line=textbox1_event(&textbox1, &e, &vscrollbar1,	mousex,mousey,&draw);

			if (edited_line>=0)
			{
				edited_line2 = edited_line;
				window2.visible = 1;
				window2.focus = 1;
				window1.focus = 0;
				kiss_hex4edit_update_adress(&hex4edit1feat,&temparray_0x30311[edited_line2+1].str_0x30311_word_0);
				kiss_hex4edit_update_adress(&hex4edit2feat,&temparray_0x30311[edited_line2 + 1].str_0x30311_word_2);
				kiss_hex4edit_update_adress(&hex4edit3feat,&temparray_0x30311[edited_line2 + 1].axis3d_4.x);
				kiss_hex4edit_update_adress(&hex4edit4feat,&temparray_0x30311[edited_line2 + 1].axis3d_4.y);
				kiss_hex4edit_update_adress(&hex4edit5feat,&temparray_0x30311[edited_line2 + 1].axis3d_4.z);
				kiss_hex4edit_update_adress(&hex4edit6feat,&temparray_0x30311[edited_line2 + 1].word_10);
				kiss_hex4edit_update_adress(&hex4edit7feat,&temparray_0x30311[edited_line2 + 1].word_12);
				kiss_hex4edit_update_adress(&hex4edit8feat,&temparray_0x30311[edited_line2 + 1].word_14);
				kiss_hex4edit_update_adress(&hex4edit9feat,&temparray_0x30311[edited_line2 + 1].word_16);
				kiss_hex4edit_update_adress(&hex4edit10feat,&temparray_0x30311[edited_line2 + 1].word_18);
				char* textbuff = (char*)objects.data[edited_line2];
				if (textbuff[strlen(textbuff) - 1] == 'I')
					select1feat.selected = false;
				else
					select1feat.selected = true;
				changed2 = true;
				terrainzoomfeat = 4;
				float cursorpixx = temparray_0x30311[edited_line2 + 1].axis3d_4.x;
				float cursorpixy = temparray_0x30311[edited_line2 + 1].axis3d_4.y;
				terrainbeginxfeat = cursorpixx - (terrainfeat.rect.w/2) / (terrainzoomfeat * 2);
				terrainbeginyfeat = cursorpixy - (terrainfeat.rect.h / 2) / (terrainzoomfeat * 2);
			}

			vscrollbar1_event(&vscrollbar1, &e, &textbox1,	&draw);
			//textbox2_event(&textbox2, &e, &vscrollbar2, &entry,	&draw);
			//vscrollbar2_event(&vscrollbar2, &e, &textbox2, &draw);
			//button_ok1_event(&button_ok1, &e, &window1, &window2,&label_sel, &entry, &label_res, &progressbar,&draw);
			
			kiss_entry_event(&entry, &e, &draw);
			//button_ok2_event(&button_ok2, &e, &window1, &window2,&progressbar, &draw);			
			if (kiss_hex4edit_event(&hex4edit1, &e, &draw) > 10)changed = true;
			if (kiss_hex4edit_event(&hex4edit2, &e, &draw) > 10)changed = true;
			if (kiss_hex4edit_event(&hex4edit3, &e, &draw) > 10)changed = true;
			if (kiss_hex4edit_event(&hex4edit4, &e, &draw) > 10)changed = true;
			if (kiss_hex4edit_event(&hex4edit5, &e, &draw) > 10)changed = true;
			if (kiss_hex4edit_event(&hex4edit6, &e, &draw) > 10)changed = true;
			if (kiss_hex4edit_event(&hex4edit7, &e, &draw) > 10)changed = true;
			if (kiss_hex4edit_event(&hex4edit8, &e, &draw) > 10)changed = true;
			if (kiss_hex4edit_event(&hex4edit9, &e, &draw) > 10)changed = true;
			if (kiss_hex4edit_event(&hex4edit10, &e, &draw) > 10)changed = true;
			if (kiss_hex4edit_event(&hex4edit11, &e, &draw) > 10)changed = true;
			if (kiss_hex4edit_event(&hex4edit12, &e, &draw) > 10)changed = true;

			if(select1_event(&select1, &e, &select2, &select3, &select4, &draw))zoomchanged = true;
			if(select2_event(&select2, &e, &select1, &select3, &select4, &draw))zoomchanged = true;
			if(select3_event(&select3, &e, &select1, &select2, &select4, &draw))zoomchanged = true;
			if(select4_event(&select4, &e, &select1, &select2, &select3, &draw))zoomchanged = true;
			//window2
			if (kiss_hex4edit_event(&hex4edit1feat, &e, &draw) > 10)changed2 = true;
			if (kiss_hex4edit_event(&hex4edit2feat, &e, &draw) > 10)changed2 = true;
			if (kiss_hex4edit_event(&hex4edit3feat, &e, &draw) > 10)changed2 = true;
			if (kiss_hex4edit_event(&hex4edit4feat, &e, &draw) > 10)changed2 = true;
			if (kiss_hex4edit_event(&hex4edit5feat, &e, &draw) > 10)changed2 = true;
			if (kiss_hex4edit_event(&hex4edit6feat, &e, &draw) > 10)changed2 = true;
			if (kiss_hex4edit_event(&hex4edit7feat, &e, &draw) > 10)changed2 = true;
			if (kiss_hex4edit_event(&hex4edit8feat, &e, &draw) > 10)changed2 = true;
			if (kiss_hex4edit_event(&hex4edit9feat, &e, &draw) > 10)changed2 = true;
			if (kiss_hex4edit_event(&hex4edit10feat, &e, &draw) > 10)changed2 = true;			

			button_ok_event(&button_ok1feat, &e, &quit, &draw);

			/*for(int i=0;i<count_features;i++)
				button_del_event(&button_del[i], &e, &quit, &draw);

			for (int i = 0; i < count_features; i++)
				button_add_event(&button_add[i], &e, &quit, &draw);*/

			int terev=kiss_terrain_event(&terrain1, &e, &draw, mousex, mousey);

			if (kiss_dec1edit_event(&levelSel, &e, &draw) > 10) { loadlevel(actlevel-1); changed = true; }

			if (terev >= 10)
			{
				zoomchanged = true;				
			}			

			int tersetposx, tersetposy;
			int terevfeat = kiss_terrain_eventfeat(&terrainfeat, &e, &draw, mousex, mousey,&tersetposx, &tersetposy);
			if (terevfeat == 20)
			{
				temparray_0x30311[edited_line2 + 1].axis3d_4.x= tersetposx;
				temparray_0x30311[edited_line2 + 1].axis3d_4.y= tersetposy;
				kiss_hex4edit_update_adress(&hex4edit3feat, &temparray_0x30311[edited_line2 + 1].axis3d_4.x);
				kiss_hex4edit_update_adress(&hex4edit4feat, &temparray_0x30311[edited_line2 + 1].axis3d_4.y);
				changed2 = true;
			}

			if (terevfeat >= 10)
			{
				zoomchangedfeat = true;
			}

			if(button_del_event(&button_deletefeat, &e, &quit, &draw))
				changed2=true;
			if(button_add_event(&button_plusfeat, &e, &quit, &draw))
				changed2=true;
			if (button_clean_event(&button_cleanfeat, &e, &quit, &draw))
				changed2 = true;
			if (select1_feat_event(&select1feat, &e, &objects,&draw))
				changed2 = true;

			if (changed2)terrain_feat_append(&textbox1, &vscrollbar1);
		}
		if (first) { changed = true; changed2 = true;}
		if (changed|| changed2)
			terrain_recalculate();

		vscrollbar1_event(&vscrollbar1, NULL, &textbox1, &draw);
		//vscrollbar2_event(&vscrollbar2, NULL, &textbox2, &draw);
		kiss_progressbar_event(&progressbar, NULL, &draw);

		if (!draw) continue;
		SDL_RenderClear(editor_renderer);

		

		
		kiss_window_draw(&window1, editor_renderer);
		kiss_label_draw(&label_terfeat, editor_renderer);
		kiss_label_draw(&label_terfeat2, editor_renderer);
		//kiss_label_draw(&label2, editor_renderer);
		kiss_textbox_draw(&textbox1, editor_renderer);
		kiss_vscrollbar_draw(&vscrollbar1, editor_renderer);
		//kiss_textbox_draw(&textbox2, editor_renderer);
		//kiss_vscrollbar_draw(&vscrollbar2, editor_renderer);
		//kiss_label_draw(&label_sel, editor_renderer);
		kiss_entry_draw(&entry, editor_renderer);
		//kiss_button_draw(&button_ok1, editor_renderer);
		//kiss_button_draw(&button_cancel, editor_renderer);		
		//kiss_label_draw(&label_res, editor_renderer);
		kiss_progressbar_draw(&progressbar, editor_renderer);
		//kiss_button_draw(&button_ok2, editor_renderer);

		kiss_hex4edit_draw(&hex4edit1, editor_renderer);
		kiss_hex4edit_draw(&hex4edit2, editor_renderer);
		kiss_hex4edit_draw(&hex4edit3, editor_renderer);
		kiss_hex4edit_draw(&hex4edit4, editor_renderer);
		kiss_hex4edit_draw(&hex4edit5, editor_renderer);
		kiss_hex4edit_draw(&hex4edit6, editor_renderer);
		kiss_hex4edit_draw(&hex4edit7, editor_renderer);
		kiss_hex4edit_draw(&hex4edit8, editor_renderer);
		kiss_hex4edit_draw(&hex4edit9, editor_renderer);
		kiss_hex4edit_draw(&hex4edit10, editor_renderer);
		kiss_hex4edit_draw(&hex4edit11, editor_renderer);
		kiss_hex4edit_draw(&hex4edit12, editor_renderer);

		kiss_selectbutton_draw(&select1, editor_renderer);
		kiss_selectbutton_draw(&select2, editor_renderer);
		kiss_selectbutton_draw(&select3, editor_renderer);
		kiss_selectbutton_draw(&select4, editor_renderer);
		kiss_label_draw(&labelSel, editor_renderer);

		kiss_dec1edit_draw(&levelSel, editor_renderer);

		kiss_label_draw(&labelXY, editor_renderer);

		/*for (int i = 0; i < count_features; i++)
			kiss_button_draw(&button_del[i], editor_renderer);

		for (int i = 0; i < count_features; i++)
			kiss_button_draw(&button_add[i], editor_renderer);*/

		if(changed||zoomchanged)
			fillterrain(/*0, window1.rect.h-mapimage.h, */terrainzoom, terrainbeginx, terrainbeginy);//set terrain

		if (changed2 || zoomchangedfeat)
			fillterrainfeat(terrainzoomfeat, terrainbeginxfeat, terrainbeginyfeat);//set terrain2

		kiss_terrain_draw(&terrain1, editor_renderer);

		kiss_window_draw(&window2, editor_renderer);
		kiss_hex4edit_draw(&hex4edit1feat, editor_renderer);
		kiss_hex4edit_draw(&hex4edit2feat, editor_renderer);
		kiss_hex4edit_draw(&hex4edit3feat, editor_renderer);
		kiss_hex4edit_draw(&hex4edit4feat, editor_renderer);
		kiss_hex4edit_draw(&hex4edit5feat, editor_renderer);
		kiss_hex4edit_draw(&hex4edit6feat, editor_renderer);
		kiss_hex4edit_draw(&hex4edit7feat, editor_renderer);
		kiss_hex4edit_draw(&hex4edit8feat, editor_renderer);
		kiss_hex4edit_draw(&hex4edit9feat, editor_renderer);
		kiss_hex4edit_draw(&hex4edit10feat, editor_renderer);

		kiss_selectbutton_draw(&select1feat, editor_renderer);
		kiss_button_draw(&button_ok1feat, editor_renderer);
		//kiss_button_draw(&button_cancelfeat, editor_renderer);
		kiss_button_draw(&button_plusfeat, editor_renderer);	
		kiss_button_draw(&button_deletefeat, editor_renderer);		
		kiss_button_draw(&button_cleanfeat, editor_renderer);
		kiss_label_draw(&levelSelFeat, editor_renderer);
		kiss_label_draw(&label_plusFeat, editor_renderer);
		kiss_terrain_draw(&terrainfeat, editor_renderer);


		/*SDL_Rect r; r.x = 50; r.y = 50; r.w = 50; r.h = 50; SDL_Color color = { 0, 0, 0 };
		kiss_fillrect(editor_renderer, &r, color);*/
		
		SDL_RenderPresent(editor_renderer);
		draw = 0;
		first = false;
	}
	kiss_clean(&objects);
	return 0;
}
