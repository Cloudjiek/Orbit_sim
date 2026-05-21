#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <cmath>
#include <thread>
#include <chrono>

using namespace std;

// constants
const double G = 6.674e-11; // N*m^2/kg^2

const double Earth_mass = 5.972e24; // kg
const double Earth_a = 149600000000; // m
const double Earth_e = 0.0167;
const double Earth_T = 365; // days

const int dt = 1.0; // days

const double Sun_mass = 1.989e30; // kg

// Celestial bodies
struct Planet {
	string name;
	double mass;
	double a;
	double e;
	double T;
};

struct Sun {
	string name;
	double mass;
};

// Calculates gravitational force between a given planet and a given sun (distance = a)
double calc_g_force(const Planet &p, const Sun &s, double distance) {
	// F = (G*M*m)/(R*R) ---> N = (N*m^2/kg^2)*(kg*kg)/(m*m) = N*m^2*kg^2/kg^2*m^2 = N

	double g_force = (G * p.mass * s.mass) / (distance*distance);

	return g_force;
}

double calc_velocity(const Planet &p, const Sun &s, double distance) {
	// v = sqrt(G * M * (2/R) - (1/a))

	double velocity = sqrt(G * s.mass * ((2.0 / distance) - (1.0 / p.a)));
	return velocity;
}

// Calculates the eccentric anomaly E;
double calc_E(const Planet& p, const Sun& s, double t) {
	// Calculates the mean anomaly M
	double M = (2.0 * M_PI * t) / p.T;

	// Initial guess for the eccentric anomaly E;
	double E = M;

	// Iteratively solve for E using Newton's method
	for (int i = 0; i < 100; i++) {
		E = E - (E - p.e * sin(E) - M) / (1.0 - p.e * cos(E));
	}

	return E;
}


// Calculates the distance for a given time t
double calc_R(const Planet &p, const Sun &s, double E) {

		// Calculates distance from the sun
		double R = p.a * (1.0 - p.e * cos(E));

		return R;
}

// Calculates X coordinate for a given planet
double calc_x(const Planet& p, double E) {
	double x = p.a * (cos(E) - p.e);
	return x;
}

// Calculates Y coordinate for a given planet
double calc_y(const Planet& p, double E) {
	double y = p.a * sqrt(1 - (p.e * p.e)) * sin(E);
	return y;
}

// Everything used here is explained above
void simulate_orbit(const Planet &p, const Sun &s) {
	for (int t = 0; t < p.T; t = t + dt) {
		double E = calc_E(p, s, t);

		double R = calc_R(p, s, E);

		double g_force = calc_g_force(p, s, R);

		double v = calc_velocity(p, s, R);

		double x = calc_x(p, E);
		double y = calc_y(p, E);

		cout << "Force: " << g_force << " [|] Distance: " << R << " [|] Velocity: " << v << " [|] [x, y]: [" << x << ", " << y << " ]" << endl;

		this_thread::sleep_for(chrono::milliseconds(100));
	}
}

int main() {
	Planet earth = { "Earth", Earth_mass, Earth_a, Earth_e, Earth_T };
	Sun sun = { "Sun", Sun_mass };

	Planet mars = { "Mars", 6.39e23, 227939200000, 0.0934, 687 };

	//double g_force = calc_g_force(earth, sun);
	//cout << g_force << " N" << endl;

	simulate_orbit(earth, sun);

	//simulate_orbit(mars, sun);

	return 0;
}

// Cloudjiek :)