#ifndef GEIGER_H_
#define GEIGER_H_

class Geiger {
public:
    Geiger();
	virtual ~Geiger();

	void setup(int pin);
    void setup(int pin, double factor);
	void loop();
private:
    int pin;
    double factor = 0.0067;
};


#endif /* GEIGER_H_ */