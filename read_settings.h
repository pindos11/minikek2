class settings{
	public:
		float fov;
		float frame_time;
		float strafe_angle;
		float move_multiplier;
		float x_screen_div;
		float y_screen_mul;
		float cam_height;
		int map_size_x;
		int map_size_y;
		float jump_power;
		float jump_speed;
		float max_hgt;
		int sm_iter;
		int hgt_mul;
		void read_settings(){
			std::string line;
			std::fstream in("settings.txt");
			if(in.is_open()){
				while(getline(in,line)){
					if(line.substr(0,3)=="fov"){
						fov = std::stof(line.substr(4));
					}
					if(line.substr(0,3)=="ftm"){
						frame_time = std::stof(line.substr(4));
					}
					if(line.substr(0,3)=="tan"){
						strafe_angle = std::stof(line.substr(4));
					}
					if(line.substr(0,3)=="smu"){
						move_multiplier = std::stof(line.substr(4));
					}
					if(line.substr(0,3)=="rox"){
						x_screen_div = std::stof(line.substr(4));
					}
					if(line.substr(0,3)=="roy"){
						y_screen_mul = std::stof(line.substr(4));
					}
					if(line.substr(0,3)=="hgt"){
						cam_height = std::stof(line.substr(4));
					}
					if(line.substr(0,3)=="msx"){
						map_size_x = std::stoi(line.substr(4));
					}
					if(line.substr(0,3)=="msy"){
						map_size_y = std::stoi(line.substr(4));
						//std::cout<<map_size_y<<'\n';
					}
					if(line.substr(0,3)=="jmp"){
						jump_power = std::stof(line.substr(4));
					}
					if(line.substr(0,3)=="jms"){
						jump_speed = std::stof(line.substr(4));
					}
					if(line.substr(0,3)=="msz"){
						max_hgt = std::stof(line.substr(4));
					}
					if(line.substr(0,3)=="msm"){
						sm_iter = std::stoi(line.substr(4));
					}
					if(line.substr(0,3)=="mhm"){
						hgt_mul = std::stoi(line.substr(4));
					}
					
				}
			}
		}
};
settings setting;
