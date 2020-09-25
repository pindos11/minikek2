struct bmp_header{
   char type[2];
   char size[4];
   char r1[2];
   char r2[2];
   char offset[4];
};
struct info_header{
	char size[4];
	char width[4];
	char height[4];
	char planes[2];
	char planes[2];
	char bitCount[2];	
	char compression[4];
	char sizeImage[4];
	char xpelsPerMeter[4];
	char xpelsPerMeter[4];
	char colorsUsed[4];
	char colorsImportant[4];
};
class bmp_image_save{
	private:
		bmp_header header;
		info_header info_h;
		char image[HMAP_SIZEX*HMAP_SIZEX*3];
	public:
		void prepare_image_by_map(height_map map){
			header.type[0]='B';
			header.type[1]='M';
			header.size[3]='1';
			
		}
};
