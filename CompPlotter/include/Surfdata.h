#pragma once
#include <iostream>
#include <vector>
#include <iterator>

struct SurfacePoint{
	double ie;
	double xc;
	double yc;
	double nx;
	double ny;
	double s1;
	double ds;
	double angle;
	double Cp;
	double Cf;
	double qw;
	double rho;
	double u;
	double v;
	double et;
	double p;
	double T;
	double nvki;
	double drho;
	double du;
	double dv;
	double det;
	double dp;
	double dT;
	double dnvki;

	std::vector<double> data;

	SurfacePoint(std::vector<double> input){
		ie = input[0];
		xc = input[1];
		yc = input[2];
		nx = input[3];
		ny = input[4];
		s1 = input[5];
		ds = input[6];
		angle = input[7];
		Cp = input[8];
		Cf = input[9];
		qw = input[10];
		rho = input[11];
		u = input[12];
		v = input[13];
		et = input[14];
		p = input[15];
		T = input[16];
		nvki = input[17];
		drho = input[18];
		du = input[19];
		dv = input[20];
		det = input[21];
		dp = input[22];
		dT = input[23];
		dnvki = input[24];

		data = input;
	}

};

std::ostream& operator<<(std::ostream& os, const SurfacePoint& sp)
{
    os << "\t";
	std::copy(sp.data.begin(), sp.data.end(), std::ostream_iterator<double>(os, "\t"));
	os << "\t";
    return os;
}

class Surfdata
{
public:
	Surfdata() {};
	Surfdata(std::string filePath);
	std::vector<SurfacePoint> points;

};

