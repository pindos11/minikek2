#define HMAP_SIZEX 150
#define HMAP_SIZEY 20
#define START_POINTS_PERCENT 5.0
#define MAX_HGT 10.0
#define SMOOTH_N 1

class height_map_position{
	public:
		int id;
		float height;
		void set(int height){
			height = height;
		}
		height_map_position(){
			id = id_generator.get_new_id();
			height = -MAX_HGT - 1;
		}
};

class map_triangle{
	public:
		map_triangle(){
			x1 = -300;
			x2 = -300;
			x3 = -300;
			y1 = -300;
			y2 = -300;
			y3 = -300;
			z1 = -300;
			z2 = -300;
			z3 = -300;
		}
		float x1;
		float x2;
		float x3;
		
		float y1;
		float y2;
		float y3;
		
		float z1;
		float z2;
		float z3;
		
		float nx1;
		float ny1;
		float nz1;
		
		float nx2;
		float ny2;
		float nz2;
		
		float nx3;
		float ny3;
		float nz3;
};

class height_map{
	protected:
		height_map_position map[HMAP_SIZEX][HMAP_SIZEY];
		height_map_position map2[HMAP_SIZEX][HMAP_SIZEY];
	public:
		std::vector<map_triangle> triangles;
		float fill_triangles(){
			for(int i = 0; i<HMAP_SIZEX-1; i++){
				for(int j = 0; j<HMAP_SIZEY-1; j++){
					map_triangle triangle;
					triangle.x1 = i;
					triangle.x2 = i+1;
					triangle.x3 = i;
					
					triangle.y1 = j;
					triangle.y2 = j;
					triangle.y3 = j+1;
					
					triangle.z1 = map[i][j].height;
					triangle.z2 = map[i+1][j].height;
					triangle.z3 = map[i][j+1].height;
					
					float ax = triangle.x1 - triangle.x2;
					float ay = triangle.y1 - triangle.y2;
					float az = triangle.z1 - triangle.z2;
			
					float bx = triangle.x2 - triangle.x3;
					float by = triangle.y2 - triangle.y3;
					float bz = triangle.z2 - triangle.z3;
			
					triangle.nx1 = ay*bz - az*by;
					triangle.ny1 = az*bx - ax*bz;
					triangle.nz1 = ax*by - ay*bx;
					
					triangle.nx2 = triangle.nx1;
					triangle.ny2 = triangle.ny1;
					triangle.nz2 = triangle.nz1;
					
					triangle.nx3 = triangle.nx1;
					triangle.ny3 = triangle.ny1;
					triangle.nz3 = triangle.nz1;
					
					triangles.push_back(triangle);
					
					triangle.x1 = i+1;
					triangle.x2 = i+1;
					triangle.x3 = i;
					           
					triangle.y1 = j;
					triangle.y2 = j+1;
					triangle.y3 = j+1;
					           
					triangle.z1 = map[i+1][j].height;
					triangle.z2 = map[i+1][j+1].height;
					triangle.z3 = map[i][j+1].height;
					
					ax = triangle.x1 - triangle.x2;
					ay = triangle.y1 - triangle.y2;
					az = triangle.z1 - triangle.z2;
			                                 
					bx = triangle.x2 - triangle.x3;
					by = triangle.y2 - triangle.y3;
					bz = triangle.z2 - triangle.z3;
			  
					triangle.nx1 = ay*bz - az*by;
					triangle.ny1 = az*bx - ax*bz;
					triangle.nz1 = ax*by - ay*bx;
					
					triangle.nx2 = triangle.nx1;
					triangle.ny2 = triangle.ny1;
					triangle.nz2 = triangle.nz1;
					
					triangle.nx3 = triangle.nx1;
					triangle.ny3 = triangle.ny1;
					triangle.nz3 = triangle.nz1;
					
					triangles.push_back(triangle);
				}
			}
			for(int sm=0; sm<SMOOTH_N; sm++){
			//normals smoothening
				for(int i = 0; i<HMAP_SIZEX; i++){
					for(int j = 0; j<HMAP_SIZEY; j++){
						float tnx = 0.0;
						float tny = 0.0;
						float tnz = 0.0;
						float amt = 0.0;
						for(int n=0; n<triangles.size();n++){
							if(triangles[n].x1 == i && triangles[n].y1 == j){
								amt += 1;
								tnx += triangles[n].nx1;
								tny += triangles[n].ny1;
								tnz += triangles[n].nz1;
							}
							if(triangles[n].x2 == i && triangles[n].y2 == j){
								amt += 1;
								tnx += triangles[n].nx2;
								tny += triangles[n].ny2;
								tnz += triangles[n].nz2;
							}
							if(triangles[n].x3 == i && triangles[n].y3 == j){
								amt += 1;
								tnx += triangles[n].nx3;
								tny += triangles[n].ny3;
								tnz += triangles[n].nz3;
							}
						}
						//calculate medium
						if(amt==0.0){
							continue;
						}
						tnx = tnx/amt;
						tny = tny/amt;
						tnz = tnz/amt;
						//std::cout<<'x'<<tnx<<'y'<<tny<<'z'<<tnz<<'\n';
						//set new normals
						for(int n=0; n<triangles.size();n++){
							if(triangles[n].x1 == i && triangles[n].y1 == j){
								triangles[n].nx1 = tnx;
								triangles[n].ny1 = tny;
								triangles[n].nz1 = tnz;
							}
							if(triangles[n].x2 == i && triangles[n].y2 == j){
								triangles[n].nx2 = tnx;
								triangles[n].ny2 = tny;
								triangles[n].nz2 = tnz;
							}
							if(triangles[n].x3 == i && triangles[n].y3 == j){
								triangles[n].nx3 = tnx;
								triangles[n].ny3 = tny;
								triangles[n].nz3 = tnz;
							}
						}
					}
				}
			}
			
		}
		
		height_map(){
			for(int i = 0; i<HMAP_SIZEX; i++){
				for(int j = 0; j<HMAP_SIZEY; j++){
					map[i][j] = height_map_position();
					map2[i][j] = height_map_position();
				}
			}
		}
		float get_rand_height(){
			float result = rand_gen.get_folated();
			result = round(result*1000.0)/100.0;
			float change_sign = rand_gen.get_folated();
			if(change_sign >= 0.5){
				result *= -1;
			}
			if(result>MAX_HGT){
				result = MAX_HGT;
			}
			if(result<-MAX_HGT){
				result = -MAX_HGT;
			}
			return result;
		}
		float clamp(float a, float max, float min){
			if(a>max){
				return max;
			}
			if(a<min){
				return min;
			}
			return a;
		}
		
		
		float get_point_height(int x, int y){
			if(x>=0 && x<HMAP_SIZEX && y>=0 && y<HMAP_SIZEY){
				return map[x][y].height;
			}
			return -MAX_HGT - 1;
		}
		
		float get_point_height_walk(float x, float y){
			int x1 = floor(x);
			int x2 = ceil(x);
			
			int y1 = floor(y);
			int y2 = ceil(y);
			
			
			float tot_h = 0.0;
			float amt = 0.0;
			float temph = 0.0;
			temph = get_point_height(x1,y1);
			if(temph != -MAX_HGT - 1){
				amt+=1.0;
				tot_h+=temph;
			}
			temph = get_point_height(x1,y2);
			if(temph != -MAX_HGT - 1){
				amt+=1.0;
				tot_h+=temph;
			}
			temph = get_point_height(x2,y2);
			if(temph != -MAX_HGT - 1){
				amt+=1.0;
				tot_h+=temph;
			}
			temph = get_point_height(x2,y1);
			if(temph != -MAX_HGT - 1){
				amt+=1.0;
				tot_h+=temph;
			}
			tot_h = tot_h/amt;
			return tot_h;
		}
		
		float get_smooth_near(int x, int y){
			float tot_h;
			float amt;
			float tvalue;
			
			int ltx = x-1;
			int lty = y-1;
			
			int tx = x;
			int ty = y-1;
			
			int rtx = x+1;
			int rty = y-1;
			
			int rx = x+1;
			int ry = y;
			
			int rbx = x+1;
			int rby = y+1;
			
			int bx = x;
			int by = y+1;
			
			int lbx = x-1;
			int lby = y+1;
			
			int lx = x-1;
			int ly = y;
			amt = 1.0;
			tvalue = get_point_height(ltx,lty);
			if(tvalue != -MAX_HGT - 1){
				amt+=1.0;
				tot_h += tvalue;
			}
			tvalue = get_point_height(tx,ty);
			if(tvalue != -MAX_HGT - 1){
				amt+=1.0;
				tot_h += tvalue;
			}
			tvalue = get_point_height(rtx,rty);
			if(tvalue != -MAX_HGT - 1){
				amt+=1.0;
				tot_h += tvalue;
			}
			tvalue = get_point_height(rx,ry);
			if(tvalue != -MAX_HGT - 1){
				amt+=1.0;
				tot_h += tvalue;
			}
			tvalue = get_point_height(rbx,rby);
			if(tvalue != -MAX_HGT - 1){
				amt+=1.0;
				tot_h += tvalue;
			}
			tvalue = get_point_height(bx,by);
			if(tvalue != -MAX_HGT - 1){
				amt+=1.0;
				tot_h += tvalue;
			}
			tvalue = get_point_height(lbx,lby);
			if(tvalue != -MAX_HGT - 1){
				amt+=1.0;
				tot_h += tvalue;
			}
			tvalue = get_point_height(lx,ly);
			if(tvalue != -MAX_HGT - 1){
				amt+=1.0;
				tot_h += tvalue;
			}
			tot_h += get_point_height(x,y);
			tot_h = tot_h/amt;
			if(tot_h>MAX_HGT){
				tot_h = MAX_HGT;
			}
			if(tot_h<-MAX_HGT){
				tot_h = -MAX_HGT;
			}
			return tot_h;
		}
		void generate_map(int seed, float multiplier){		
			for(int i = 0; i<HMAP_SIZEX; i++){
				for(int j = 0; j<HMAP_SIZEY; j++){
					map[i][j].height = get_rand_height();
				}
			}
			//smooth shit
			for(int ctr = 0; ctr<13; ctr++){
				for(int i = 0; i<HMAP_SIZEX; i++){
					for(int j = 0; j<HMAP_SIZEY; j++){
						float newval = get_smooth_near(i,j);
						map2[i][j].height = newval;
					}
				}
				for(int i = 0; i<HMAP_SIZEX; i++){
					for(int j = 0; j<HMAP_SIZEY; j++){
						map[i][j].height = map2[i][j].height;
					}
				}
			}
			fill_triangles();
					
		}
		void write_map(){
			for(int i = 0; i<HMAP_SIZEX; i++){
				for(int j = 0; j<HMAP_SIZEY; j++){
					std::cout<<(map[i][j].height+1.0)/2.0<<' ';
				}
			//	std::cout<<'\n';
			}
		}
		
};
