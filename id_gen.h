class id_gen{
	private:
		int current_id;
	public:
		int get_new_id(){
			current_id += 1;
			return current_id;
		}
		id_gen(){
			current_id = 0;
		}
};
id_gen id_generator;
