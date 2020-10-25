struct vec3{
	float x;
	float y;
	float z;
	vec3(){
		x = y = z = -1;
	}
	vec3(float ix, float iy, float iz){
		x = ix;
		y = iy;
		z = iz;
	}
	void set(float ix, float iy, float iz){
		x = ix;
		y = iy;
		z = iz;
	}
};

struct mesh3{
	int v1;
	int v2;
	int v3;
	
	int t1;
	int t2;
	int t3;
	
	int n1;
	int n2;
	int n3;
};

struct mesh4{
	int v1;
	int v2;
	int v3;
	int v4;
	
	int t1;
	int t2;
	int t3;
	int t4;
	
	int n1;
	int n2;
	int n3;
	int n4;
};

struct model{
	std::vector<vec3> verticles;
	std::vector<vec3> normals;
	std::vector<vec3> textures;
	std::vector<mesh3> meshes3;
	std::vector<mesh4> meshes4;
	
};
model bodies[20];
int loader( const char * filename, int ind ){
	std::ifstream file(filename);
	if(!file.is_open()){
		return -1;
	}
	
	for(std::string line; std::getline(file,line);){
		if(line[line.size()-1]==' '){
			line = line.substr(0,line.size()-1);
		}
		if(line[0]=='v'){
			if(line[1]==' '){
				vec3 d;
				int spos = 2;
				while(line[spos]==' '){
					spos+=1;
				}
				int pos_s = line.find(' ',spos);
				d.x = std::stof(line.substr(spos,pos_s-1));
				int pos_s2 = line.find(' ',pos_s+1);
				d.y = std::stof(line.substr(pos_s+1,pos_s2-pos_s-1));
				pos_s = pos_s2;
				d.z = std::stof(line.substr(pos_s+1,line.size()-pos_s-1));
				
				//std::cout<<"vert x: "<<d.x<<" y: "<<d.y<<" z: "<<d.z<<'\n';
				
				bodies[ind].verticles.push_back(d);
			}
			if(line[1]=='t'){
				vec3 d;
				d.z = 0.0;
				int pos_s = line.find(' ',3);
				d.x = std::stof(line.substr(3,pos_s-1));
				d.y = std::stof(line.substr(pos_s+1,line.size()-pos_s-1));
				
				//std::cout<<"tex x: "<<d.x<<" y: "<<d.y<<" z: "<<d.z<<'\n';
				
				bodies[ind].textures.push_back(d);
			}
			if(line[1]=='n'){
				vec3 d;
				
				int pos_s = line.find(' ',3);
				d.x = std::stof(line.substr(3,pos_s-1));
				int pos_s2 = line.find(' ',pos_s+1);
				d.y = std::stof(line.substr(pos_s+1,pos_s2-pos_s-1));
				pos_s = pos_s2;
				d.z = std::stof(line.substr(pos_s+1,line.size()-pos_s-1));
				
				//std::cout<<"norm x: "<<d.x<<" y: "<<d.y<<" z: "<<d.z<<'\n';
				
				bodies[ind].normals.push_back(d);
			}
		}
		if(line[0]=='f'){
			std::vector<std::string> verts;
			std::vector<vec3> verts_v;
			int pos = 1;
			int pos2 = 1;
			while(1){
				pos = line.find(' ',pos+1);
				if(pos!=std::string::npos){
					std::string vert;
					vert = line.substr(pos2,pos-pos2);
					pos2 = pos;
					verts.push_back(vert);
				}
				else{
					pos=line.size();
					std::string vert;
					vert = line.substr(pos2,pos-pos2);
					pos2 = pos;
					verts.push_back(vert);
					break;
				}
			}
			
			for(int i=0; i<verts.size(); i++){
				std::string vert = verts[i];
				vec3 vert_numbers;
				vert_numbers.x = vert_numbers.y = vert_numbers.y = -1;
				//position
				pos = vert.find('/',0);
				pos2 = 0;
				if(pos==std::string::npos){
					vert_numbers.x = std::stof(vert);
					verts_v.push_back(vert_numbers);
					continue;
				}
				vert_numbers.x = std::stof(vert.substr(pos2,pos-pos2))-1.0;
				//texture
				pos2 = pos;
				pos = vert.find('/',pos2+1);
				if(pos-pos2>1){
					vert_numbers.y = std::stof(vert.substr(pos2+1,pos-pos2-1))-1.0;
				}
				//normal
				pos2 = pos;
				pos = vert.size();
				if(pos-pos2>1){
					vert_numbers.z = std::stof(vert.substr(pos2+1,vert.size()))-1.0;
				}
				verts_v.push_back(vert_numbers);
			}
			if(verts_v.size()==3){
				mesh3 m;
				m.v1 = verts_v[0].x;
				m.v2 = verts_v[1].x;
				m.v3 = verts_v[2].x;
				
				if(verts_v[0].y!=-1){
					m.t1 = verts_v[0].y;
					m.t2 = verts_v[1].y;
					m.t3 = verts_v[2].y;
				}
				
				if(verts_v[0].z!=-1){
					m.n1 = verts_v[0].z;
					m.n2 = verts_v[1].z;
					m.n3 = verts_v[2].z;
				}
				//std::cout<<"mesh3 v1: "<<m.v1<<" v2: "<<m.v2<<" v3: "<<m.v3<<'\n';
				//std::cout<<"mesh3 t1: "<<m.t1<<" n2: "<<m.t2<<" n3: "<<m.t3<<'\n';
				//std::cout<<"mesh3 n1: "<<m.n1<<" n2: "<<m.n2<<" n3: "<<m.n3<<'\n'<<'\n';
				bodies[ind].meshes3.push_back(m);
			}
			
			if(verts_v.size()==4){
				mesh4 m;
				m.v1 = verts_v[0].x;
				m.v2 = verts_v[1].x;
				m.v3 = verts_v[2].x;
				m.v4 = verts_v[3].x;
				
				if(verts_v[0].y!=-1){
					m.t1 = verts_v[0].y;
					m.t2 = verts_v[1].y;
					m.t3 = verts_v[2].y;
					m.t4 = verts_v[3].y;
				}
				
				if(verts_v[0].z!=-1){
					m.n1 = verts_v[0].z;
					m.n2 = verts_v[1].z;
					m.n3 = verts_v[2].z;
					m.n4 = verts_v[3].z;
				}
				//std::cout<<"mesh4 v1: "<<m.v1<<" v2: "<<m.v2<<" v3: "<<m.v3<<" v4: "<<m.v4<<'\n';
				//std::cout<<"mesh4 t1: "<<m.t1<<" t2: "<<m.t2<<" t3: "<<m.t3<<" t4: "<<m.t4<<'\n';
				//std::cout<<"mesh4 n1: "<<m.n1<<" n2: "<<m.n2<<" n3: "<<m.n3<<" n4: "<<m.n4<<'\n'<<'\n';
				bodies[ind].meshes4.push_back(m);
			}
		}
//		if(strcmp(line,"f")==0){
//			unsigned int vert[4],tex[4],norm[4];
//			std::string pattern()
//			int matches = snprintf(line, sizeof(line), "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vert[0],&norm[0],&tex[0],&vert[1],&norm[1],&tex[1],&vert[2],&norm[2],&tex[2],&vert[3],&norm[3],&tex[3]);
//			//std::cout<<matches<<' ';
//			if(matches==8){
//				mesh4 m;
//				m.v1 = vert[0]-1;
//				m.v2 = vert[1]-1;
//				m.v3 = vert[2]-1;
//				m.v4 = vert[3]-1;
//				
//				m.t1 = tex[0]-1;
//				m.t2 = tex[1]-1;
//				m.t3 = tex[2]-1;
//				m.t4 = tex[3]-1;
//				bodies[ind].meshes4.push_back(m);
//			}
//			if(matches==9){
//				mesh3 m;
//				m.v1 = vert[0]-1;
//				m.v2 = vert[1]-1;
//				m.v3 = vert[2]-1;
//				
//				m.t1 = tex[0]-1;
//				m.t2 = tex[1]-1;
//				m.t3 = tex[2]-1;
//				
//				m.n1 = norm[0]-1;
//				m.n2 = norm[1]-1;
//				m.n3 = norm[2]-1;
//				bodies[ind].meshes3.push_back(m);
//			}
//			matches = snprintf(line, sizeof(line), "%d//%d %d//%d %d//%d\n", &vert[0],&norm[0],&vert[1],&norm[1],&vert[2],&norm[2],&vert[3],&norm[3]);
//			std::cout<<matches<<' ';
//		}
//				
		//std::cout<<"kek\n";
		}
		
	}
	
