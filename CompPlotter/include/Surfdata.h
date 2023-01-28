 #pragma once
#include <iostream>
#include <vector>
#include <iterator>

#include <iostream>
#include <vector>

struct SurfacePoint
{
	double ie;
	double xc;
	double yc;
#if threedim
	double zc;
#endif
	double nx;
	double ny;
#if threedim
	double nz;
#endif
	double s1;
	double ds;
	double angle;
	double Cp;
	double Cf;
#if threedim
	double Cf2;
#endif
	double qw;
	double rho;
	double u;
	double v;
#if threedim
	double w;
#endif
	double et;
	double p;
	double T;
	double nvki;
	double drho;
	double du;
	double dv;
#if threedim
	double dw;
#endif
	double det;
	double dp;
	double dT;
	double dnvki;
};

std::vector<SurfacePoint> extractDataFromFile(const std::string &filename);
class Surfdata
{
public:
	// Variables
	double CD = 0.0;

	// Functions
	Surfdata(){};
	Surfdata(const std::string &filePath, const bool &hasJet = true);
	std::vector<SurfacePoint> points;

private:
	// Variables
	bool m_hasJet;

	// Functions
	void CalculateCD();
};
