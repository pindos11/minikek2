class random_gen{
	private:
		unsigned x;
		unsigned y;
  		unsigned z;
  		unsigned w;
  	public:
  		random_gen(){
  			struct timeval tp;
			gettimeofday(&tp, NULL);
			long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
			std::string seed = std::to_string(ms);
			x = std::stoi(seed.substr(0,1));
  			//x = 7;
  			y = std::stoi(seed.substr(1,3));
  			//y = 345;
  			z = std::stoi(seed.substr(3,6));
  			//z = 634;
  			w = std::stoi( seed.substr(6,8) );
  			//w = 863;
		}
		unsigned get_rand_val(){
			unsigned t = x^(x<<11);
  			x = y;
			y = z;
			z = w;
			w = (w^(w>>19)) ^ (t^(t>>8));
			return w;
		}
		float get_floated(){
			double t = (double)get_rand_val();
			t = t/1000.0;
			double z = floor(t);
			return t-z;
			
		}
		float get_n_floated(int n){
			float tmp = get_floated();
			tmp *= (float)n;
			return tmp;
		}
};
random_gen rand_gen;
