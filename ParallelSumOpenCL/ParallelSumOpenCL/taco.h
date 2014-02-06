
class taco{
public:
	taco(int salsa, float lettuce[], char cheese){
		this->cheese = cheese;
		this->salsa = salsa;
		for (int i = 0; i < 3; i++){
			this->lettuce[i] = lettuce[i];
		}
	}

private:
	int salsa;
	float lettuce[3];
	char cheese;
};