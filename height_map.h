#define START_POINTS_PERCENT 5.0
#define SMOOTH_N 1

class grass_object{
	public:
		vec3 scale;
		vec3 rotate_angs;
		vec3 translate;
		int model;
		int texture;
	grass_object(){
		model = rand_gen.get_rand_val();
		model %= 5;
		
		texture = 7;
		float minusx = rand_gen.get_floated();
		float minusy = rand_gen.get_floated();
		
		translate.x = rand_gen.get_rand_val()%(setting.map_size_x-2)+1;
		translate.y = rand_gen.get_rand_val()%(setting.map_size_y-2)+1;
		
		if(minusx>=0.5){
			translate.x-=rand_gen.get_floated();
		}
		else{
			translate.x+=rand_gen.get_floated();
		}
		
		if(minusy>=0.5){
			translate.y-=rand_gen.get_floated();
		}
		else{
			translate.y+=rand_gen.get_floated();
		}
		//std::cout<<"grass x: "<<translate.x<<" y: "<<translate.y<<'\n';
		
		
		scale.x = scale.y  = 0.01;
		scale.z = 0.01;
		rotate_angs.x = 90.0;
		rotate_angs.y = rand_gen.get_rand_val()%270;
		rotate_angs.z = 0;
	}
	int am_i_between_coords(int xmin, int ymin, int xmax, int ymax){
		if(xmin>translate.x){
			return 0;
		}
		if(ymin>translate.y){
			return 0;
		}
		if(xmax<translate.x){
			return 0;
		}
		if(ymax<translate.y){
			return 0;
		}
		return 1;
	}
};

class height_map_position{
	public:
		int id;
		float height;
		void set(int height){
			height = height;
		}
		height_map_position(){
			id = id_generator.get_new_id();
			height = -setting.max_hgt - 1;
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
		int is_point_on_me(float x, float y){
			float l1 = (x1-x)*(y2-y1)-(x2-x1)*(y1-y);
			if(l1<0){
				return 0;
			}
			
			l1 = (x2-x)*(y3-y2)-(x3-x2)*(y2-y);
			if(l1<0){
				return 0;
			}
			
			l1 = (x3-x)*(y1-y3)-(x1-x3)*(y3-y);
			if(l1<0){
				return 0;
			}
			return 1;
		}
		float height_on_me(float x, float y){
			float div1 = nx*x1-nx*x+ny*y1-ny*y+nz*z1;
			float result = div1/nz;
			return result;
		}
		float get_medium_height(){
			return (z1+z2+z3)/3.0;
		}
		int am_i_between_coords(int xmin, int ymin, int xmax, int ymax){
			if(xmin>x1 || xmin>x2 || xmin>x3){
				return 0;
			}
			if(ymin>y1 || ymin>y2 || ymin>y3){
				return 0;
			}
			if(xmax<x1 || xmax<x2 || xmax<x3){
				return 0;
			}
			if(ymax<y1 || ymax<y2 || ymax<y3){
				return 0;
			}
			return 1;
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
		
		float tx1;
		float ty1;
		
		float tx2;
		float ty2;
		
		float tx3;
		float ty3;
		
		float nx;
		float ny;
		float nz;
};

class triangles_pack{
	public:
		std::vector<map_triangle> data;
};

class grasses_pack{
	public:
		std::vector<grass_object> data;
};

class height_map{
	protected:
		std::vector<std::vector<height_map_position>> map;
		std::vector<std::vector<height_map_position>> map2;
		std::vector<std::vector<height_map_position>> plains;
		float max_hgt;
		//height_map_position map[HMAP_SIZEX][HMAP_SIZEY];
		//height_map_position map2[HMAP_SIZEX][HMAP_SIZEY];
	public:
		std::vector<map_triangle> triangles;
		std::vector<grass_object> grasses;
		triangles_pack buf_triag;
		grasses_pack buf_grass;
		int draw_x = 0;
		int draw_y = 0;
		
		int draw_xg = 0;
		int draw_yg = 0;
		
		int buf_t_filled = 0;
		int buf_g_filled = 0;
		int buf_refilled = 0;
		float fill_triangles(){
			for(int i = 0; i<setting.map_size_x-1; i++){
				for(int j = 0; j<setting.map_size_y-1; j++){
					map_triangle triangle;
					height_map_position a;
					height_map_position b;
					height_map_position c;
					triangle.x1 = i;
					triangle.x2 = i+1;
					triangle.x3 = i;
					
					triangle.y1 = j;
					triangle.y2 = j;
					triangle.y3 = j+1;
					
					a = map[i][j];
					b = map[i+1][j]; //at(i+1).at(j)
					c = map[i][j+1]; //at(i).at(j+1)
					
					triangle.z1 = a.height;
					triangle.z2 = b.height;
					triangle.z3 = c.height;
					
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
					
					triangle.nx = triangle.nx1;
					triangle.ny = triangle.ny1;
					triangle.nz = triangle.nz1;
					
					float topz = triangle.z1;
					if(triangle.z2>topz){
						topz = triangle.z2;
						if(triangle.z3>topz){
							topz = triangle.z3;
							triangle.tx1 = 1;
							triangle.tx2 = 0;
							triangle.tx3 = 0.5;
						}
						else{
							triangle.tx1 = 0;
							triangle.tx2 = 0.5;
							triangle.tx3 = 1;
						}
					}
					else{
						triangle.tx1 = 0.5;
						triangle.tx2 = 0;
						triangle.tx3 = 1;
					}
					
					triangle.ty1 = triangle.z1/float(max_hgt);
					triangle.ty2 = triangle.z2/float(max_hgt);
					triangle.ty3 = triangle.z3/float(max_hgt);
					
					if(triangle.ty1 == triangle.ty2 && triangle.ty2 == triangle.ty3){
						triangle.ty1 = 1;
						triangle.ty2 = 0.8;
						triangle.ty3 = 0.8;
					}
					
					if(triangle.ty1>1){
						triangle.ty1 = 1;
					}
					if(triangle.ty1<0){
						triangle.ty1 = 0;
					}
					
					if(triangle.ty2>1){
						triangle.ty2 = 1;
					}
					if(triangle.ty2<0){
						triangle.ty2 = 0;
					}
					
					if(triangle.ty3>1){
						triangle.ty3 = 1;
					}
					if(triangle.ty3<0){
						triangle.ty3 = 0;
					}
					
					
					//std::cout<<"x: "<<triangle.tx1<<" y: "<<triangle.ty1<<'\n';
					
					triangles.push_back(triangle);
					
					triangle.x1 = i+1;
					triangle.x2 = i+1;
					triangle.x3 = i;
					           
					triangle.y1 = j;
					triangle.y2 = j+1;
					triangle.y3 = j+1;
					
					a = map[i+1][j];
					b = map[i+1][j+1];
					c = map[i][j+1];
					           
					triangle.z1 = a.height;
					triangle.z2 = b.height;
					triangle.z3 = c.height;
					
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
					
					triangle.nx = triangle.nx1;
					triangle.ny = triangle.ny1;
					triangle.nz = triangle.nz1;
					
					topz = triangle.z1;
					if(triangle.z2>topz){
						topz = triangle.z2;
						if(triangle.z3>topz){
							topz = triangle.z3;
							triangle.tx1 = 1;
							triangle.tx2 = 0;
							triangle.tx3 = 0.5;
						}
						else{
							triangle.tx1 = 0;
							triangle.tx2 = 0.5;
							triangle.tx3 = 1;
						}
					}
					else{
						triangle.tx1 = 0.5;
						triangle.tx2 = 0;
						triangle.tx3 = 1;
					}
					triangle.ty1 = triangle.z1/float(max_hgt);
					triangle.ty2 = triangle.z2/float(max_hgt);
					triangle.ty3 = triangle.z3/float(max_hgt);
					
					if(triangle.ty1 == triangle.ty2 && triangle.ty2 == triangle.ty3){
						triangle.ty1 = 1;
						triangle.ty2 = 0.8;
						triangle.ty3 = 0.8;
					}
					
					if(triangle.ty1>1){
						triangle.ty1 = 1;
					}
					if(triangle.ty1<0){
						triangle.ty1 = 0;
					}
					
					if(triangle.ty2>1){
						triangle.ty2 = 1;
					}
					if(triangle.ty2<0){
						triangle.ty2 = 0;
					}
					
					if(triangle.ty3>1){
						triangle.ty3 = 1;
					}
					if(triangle.ty3<0){
						triangle.ty3 = 0;
					}
					
					
					triangles.push_back(triangle);
				}
			}
			for(int sm=0; sm<SMOOTH_N; sm++){
			//normals smoothening
				for(int i = 0; i<setting.map_size_x; i++){
					for(int j = 0; j<setting.map_size_y; j++){
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
			setting.read_settings();
			for(int i = 0; i<setting.map_size_x; i++){
				std::vector<height_map_position> line;
				std::vector<height_map_position> line2;
				std::vector<height_map_position> line3;
				for(int j = 0; j<setting.map_size_y; j++){
					height_map_position f;
					height_map_position d;
					height_map_position s;
					line.push_back(f);
					line2.push_back(d);
					line3.push_back(s);
				}
				map.push_back(line);
				map2.push_back(line2);
				plains.push_back(line3);
			}
			std::cout<<"filled"<<'\n';
			
			for(int i=10; i<25;i++){
				for(int j=10; j<25;j++){
					plains.at(i).at(j).height = 2;
				}
			}
		}
		
		float get_rand_height(){
			float result = rand_gen.get_n_floated(setting.hgt_mul);
			result = round(result*1000.0)/100.0;
			float change_sign = rand_gen.get_n_floated(setting.hgt_mul);
			if(change_sign >= 0.6){
				result *= -1;
			}
			if(result>setting.max_hgt){
				result = setting.max_hgt;
			}
			if(result<0){
				result = 0;
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
		
		triangles_pack get_to_draw(float x, float y){
			int draw_quad = 50;
			int draw_hold = 10;
			int do_draw = 0;
			if(abs(draw_x-x)>draw_hold){
				do_draw = 1;
			}
			if(abs(draw_y-y)>draw_hold){
				do_draw = 1;
			}
			if(do_draw==0 && buf_t_filled == 1){
				//std::cout<<"use_buffer"<<'\n';
				buf_refilled = 0;
				return buf_triag;
			}
			buf_refilled = 1;
			draw_y = y;
			draw_x = x;
			
			int minx = x - draw_quad/2;
			int maxx = x + draw_quad/2;
			int miny = y - draw_quad/2;
			int maxy = y + draw_quad/2;
			if(minx<0){
				minx = 0;
			}
			if(miny<0){
				miny = 0;
			}
			if(maxx>=setting.map_size_x){
				maxx = setting.map_size_x-1;
			}
			if(maxy>=setting.map_size_y){
				maxy = setting.map_size_y-1;
			}
			
			triangles_pack items;
			for(int i = 0; i<triangles.size(); i++){
				map_triangle tri = triangles[i];
				if(tri.am_i_between_coords(minx,miny,maxx,maxy)==1){
					items.data.push_back(tri);
				}
			}
			buf_t_filled = 1;
			buf_triag = items;
			return items;	
		}
		
		grasses_pack get_grass_to_draw(float x, float y){
			int draw_quad = 50;
			int draw_hold = 10;
			int do_draw = 0;
			if(abs(draw_xg-x)>draw_hold){
				do_draw = 1;
			}
			if(abs(draw_yg-y)>draw_hold){
				do_draw = 1;
			}
			if(do_draw==0 && buf_g_filled == 1){
				return buf_grass;
			}
			buf_refilled = 1;
			draw_yg = y;
			draw_xg = x;
			
			int minx = x - draw_quad/2;
			int maxx = x + draw_quad/2;
			int miny = y - draw_quad/2;
			int maxy = y + draw_quad/2;
			if(minx<0){
				minx = 0;
			}
			if(miny<0){
				miny = 0;
			}
			if(maxx>=setting.map_size_x){
				maxx = setting.map_size_x-2;
			}
			if(maxy>=setting.map_size_y){
				maxy = setting.map_size_y-2;
			}
			
			grasses_pack items;
			for(int i = 0; i<grasses.size(); i++){
				grass_object grass1 = grasses[i];
				if(grass1.am_i_between_coords(minx,miny,maxx,maxy)==1){
					items.data.push_back(grass1);
				}
			}
			buf_g_filled = 1;
			buf_grass = items;
			return items;
		}
		
		float get_point_height(int x, int y){
			if(x>=0 && x<setting.map_size_x && y>=0 && y<setting.map_size_y){
				height_map_position f;
				f = map[x][y];
				return f.height;
			}
			return -setting.max_hgt - 1;
		}
		
		float get_point_height_walk(float x, float y){
			for(int i=0; i<triangles.size();i++){
				map_triangle a = triangles[i];
				if(a.is_point_on_me(x,y)==1){
					return a.height_on_me(x,y);
				}
			}
			
		}
		
		void add_grass(){
			int grass = (setting.map_size_x * setting.map_size_y)/0.5;
			for(int i = 0; i<grass; i++){
				grass_object t;
				t.translate.z = get_point_height_walk(t.translate.x, t.translate.y);
				if(t.translate.z>=max_hgt/3){
					grasses.push_back(t);
				}
				
			}	
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
			if(tvalue != -setting.max_hgt - 1){
				amt+=1.0;
				tot_h += tvalue;
			}
			tvalue = get_point_height(tx,ty);
			if(tvalue != -setting.max_hgt - 1){
				amt+=1.0;
				tot_h += tvalue;
			}
			tvalue = get_point_height(rtx,rty);
			if(tvalue != -setting.max_hgt - 1){
				amt+=1.0;
				tot_h += tvalue;
			}
			tvalue = get_point_height(rx,ry);
			if(tvalue != -setting.max_hgt - 1){
				amt+=1.0;
				tot_h += tvalue;
			}
			tvalue = get_point_height(rbx,rby);
			if(tvalue != -setting.max_hgt - 1){
				amt+=1.0;
				tot_h += tvalue;
			}
			tvalue = get_point_height(bx,by);
			if(tvalue != -setting.max_hgt - 1){
				amt+=1.0;
				tot_h += tvalue;
			}
			tvalue = get_point_height(lbx,lby);
			if(tvalue != -setting.max_hgt - 1){
				amt+=1.0;
				tot_h += tvalue;
			}
			tvalue = get_point_height(lx,ly);
			if(tvalue != -setting.max_hgt - 1){
				amt+=1.0;
				tot_h += tvalue;
			}
			tot_h += get_point_height(x,y);
			tot_h = tot_h/amt;
			//tot_h = get_point_height(x,y) - tot_h/2.0;
			if(tot_h>setting.max_hgt){
				tot_h = setting.max_hgt;
			}
			if(tot_h<0){
				tot_h = 0;
			}
			return tot_h;
		}
		
		int is_on_plain(int x, int y){
			if(plains.at(x).at(y).height != -setting.max_hgt - 1){
				return 1;
			}
			return 0;
		}
		
		void apply_plains(){
			for(int i = 0; i<setting.map_size_x; i++){
				for(int j = 0; j<setting.map_size_y; j++){
					if(is_on_plain(i,j)==1){
						map.at(i).at(j).height = plains.at(i).at(j).height;
					}
				}
			}
		}
		
		void set_max_hgt(){
			max_hgt = map.at(0).at(0).height;
			for(int i = 0; i<setting.map_size_x; i++){
				for(int j = 0; j<setting.map_size_y; j++){
					if(map.at(i).at(j).height > max_hgt){
						max_hgt = map.at(i).at(j).height;
					}
				}
			}
		}
		
		void generate_map(int seed, float multiplier){		
			for(int i = 0; i<setting.map_size_x; i++){
				for(int j = 0; j<setting.map_size_y; j++){
					map.at(i).at(j).height = get_rand_height();
				}
			}
			apply_plains();
			std::cout<<"generated"<<'\n';
			//smooth shit
			for(int ctr = 0; ctr<setting.sm_iter; ctr++){
				for(int i = 0; i<setting.map_size_x; i++){
					for(int j = 0; j<setting.map_size_y; j++){
						if(is_on_plain(i,j)==1){
							continue;
						}
						float newval = get_smooth_near(i,j);
						map2.at(i).at(j).height = newval;
					}
				}
				for(int i = 0; i<setting.map_size_x; i++){
					for(int j = 0; j<setting.map_size_y; j++){
						if(is_on_plain(i,j)==1){
							continue;
						}
						height_map_position f;
						f = map2[i][j];
						map.at(i).at(j).height = f.height;
					}
				}
			}
			set_max_hgt();
			std::cout<<"smoothed"<<'\n';
			fill_triangles();
			
			add_grass();
					
		}
		
};
