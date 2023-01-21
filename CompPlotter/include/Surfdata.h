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
	double zc;
	double nx;
	double ny;
	double nz;
	double s1;
	double ds;
	double angle;
	double Cp;
	double Cf;
	double Cf2;
	double qw;
	double rho;
	double u;
	double v;
	double w;
	double et;
	double p;
	double T;
	double nvki;
	double drho;
	double du;
	double dv;
	double dw;
	double det;
	double dp;
	double dT;
	double dnvki;

	SurfacePoint(std::vector<double> input)
	{
		ie = input[0];
		xc = input[1]; //
		yc = input[2]; //
		zc = input[3]; //
		nx = input[4]; //
		ny = input[5];
		nz = input[6];
		s1 = input[7];
		ds = input[8]; //
		angle = input[9];
		Cp = input[10]; //
		Cf = input[11];
		Cf2 = input[12];
		qw = input[13];
		rho = input[14];
		u = input[15];
		v = input[16];
		w = input[17];
		et = input[18];
		p = input[19];
		T = input[20];
		nvki = input[21];
		drho = input[22];
		du = input[23];
		dv = input[24];
		dw = input[25];
		det = input[26];
		dp = input[27];
		dT = input[28];
		dnvki = input[29];

		// std::cout << "--------------------------------------------------" << std::endl;
		// std::cout << "ie = " << ie << std::endl;
		// std::cout << "xc = " << xc << std::endl;
		// std::cout << "yc = " << yc << std::endl;
		// std::cout << "zc = " << zc << std::endl;
		// std::cout << "nx = " << nx << std::endl;
		// std::cout << "ny = " << ny << std::endl;
		// std::cout << "nz = " << nz << std::endl;
		// std::cout << "s1 = " << s1 << std::endl;
		// std::cout << "ds = " << ds << std::endl;
		// std::cout << "angle = " << angle << std::endl;
		// std::cout << "Cp = " << Cp << std::endl;
		// std::cout << "Cf = " << Cf << std::endl;
		// std::cout << "Cf2 = " << Cf2 << std::endl;
		// std::cout << "qw = " << qw << std::endl;
		// std::cout << "rho = " << rho << std::endl;
		// std::cout << "u = " << u << std::endl;
		// std::cout << "v = " << v << std::endl;
		// std::cout << "w = " << w << std::endl;
		// std::cout << "et = " << et << std::endl;
		// std::cout << "p = " << p << std::endl;
		// std::cout << "T = " << T << std::endl;
		// std::cout << "nvki = " << nvki << std::endl;
		// std::cout << "drho = " << drho << std::endl;
		// std::cout << "du = " << du << std::endl;
		// std::cout << "dv = " << dv << std::endl;
		// std::cout << "dw = " << dw << std::endl;
		// std::cout << "det = " << det << std::endl;
		// std::cout << "dp = " << dp << std::endl;
		// std::cout << "dT = " << dT << std::endl;
		// std::cout << "dnvki = " << dnvki << std::endl;
		// std::cout << "--------------------------------------------------" << std::endl;
	}
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
