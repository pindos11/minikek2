class random_gen{
	private:
		unsigned x;
		unsigned y;
  		unsigned z;
  		unsigned w;
  	public:
  		random_gen(){
  			x = 7;
  			y = 345;
  			z = 634;
  			w = 863;
		}
		unsigned get_rand_val(){
			unsigned t = x^(x<<11);
  			x = y;
			y = z;
			z = w;
			w = (w^(w>>19)) ^ (t^(t>>8));
			return w;
		}
		float get_folated(){
			double t = (double)get_rand_val();
			t = t/1000.0;
			double z = floor(t);
			return t-z;
			
		}
};
random_gen rand_gen;
