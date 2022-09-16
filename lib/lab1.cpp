#define ll long long

#include <stdio.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>
#include <vector>
#include <map>

using namespace std;

ll modularExponentiation(ll x, ll y, ll p) {
	ll a = 1;
	while (y) {
		if (y & 1) {
			a = (a * x) % p;
		}
		y >>= 1;
		x = (x * x) % p;
	}
	return a;
}

ll* eucleadian(ll a, ll b) {
	ll static u[]{a, 1, 0}, v[]{b, 0, 1}, t[]{0, 0, 0};
	ll q = 0;
	while (v[0]) {
		q = u[0] / v[0];
		t[0] = u[0] % v[0]; 
		t[1] = u[1] - q * v[1]; 
		t[2] = u[2] - q * v[2];
		u[0] = v[0]; u[1] = v[1]; u[2] = v[2];
		v[0] = t[0]; v[1] = t[1]; v[2] = t[2];

	}
	//cout << u[0] << " " << u[1] << " " << u[2] << endl;
	return u;
}

bool isPrime(ll p, int k) {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<ll> dist(1, p - 1);

	if (p == 2) {
		return true;
	}
	if (!(p & 1)) {
		return false;
	}
	for (int i = 0; i < k; i++) {
		ll a = dist(mt);
		if (eucleadian(a, p)[0] != 1 || modularExponentiation(a, p - 1, p) != 1) {
			//cout << a << p << endl;
			return false;
		}
	}
	return true;
}

ll simpleNumberRandom(ll left, ll right) {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<ll> dist(left, right);

	while (1) {
		ll x = dist(mt);
		if (isPrime(x, 100)) {
			return x;
		}
	}
}

ll diffieHellman() {
	std::random_device rd;
	std::mt19937 mt(rd());
	
	ll p = 0, q = 0;
	while (1) {
		p = simpleNumberRandom(0, 1 << 22);
		if (isPrime((p - 1) / 2, 100)) {
			q = (p - 1) / 2; 
			break;
		}
	}
	ll g = 0;
	std::uniform_int_distribution<ll> dist(1, p - 1);
	while (modularExponentiation(g, q, p) != 1) {
		g = dist(mt);
	}
	cout << "p = " << p << "; q = " << q << "; g = " << g << endl;
	ll Xa = simpleNumberRandom(1, p);
	ll Xb = simpleNumberRandom(1, p);
	cout << "Secret keys: " << Xa << ", " << Xb << endl;

	ll Ya = modularExponentiation(g, Xa, p);
	ll Yb = modularExponentiation(g, Xb, p);
	cout << "Open keys: " << Ya << ", " << Yb << endl;
	cout << modularExponentiation(Yb, Xa, p) << " == " << modularExponentiation(Ya, Xb, p) << endl;
	return g;
}

ll babyGiantStep(ll a, ll y, ll p) {
	//mk > p
	ll m = 0, k = 0;
	m = (int)sqrt(p + .0) + 1;  k = m;
	map<ll,int> b;

	for (int i = 0; i < m; i++) {
		ll tmp = modularExponentiation(a, i, p) * y % p;
		//if (!b.count(tmp)) {
			b[tmp] = i;
		//}
		cout << modularExponentiation(a, i, p) * y % p << " ";
	}
	cout << endl;
	for (int i = 1; i <= k; i++) {
		ll tmp = modularExponentiation(a, (m * i), p);
		cout << tmp << " ";
		if (b.count(tmp))
		{
			cout << endl;
			return i * m - b[tmp];
		}
	}
	cout << endl;
	return -1;
}


int main() {
	cout << modularExponentiation(3, 1000000000, 11) << endl;
	ll* answer = eucleadian(28, 19);
	cout << "gcd = " << answer[0] << "; x = " << answer[1] << "; y = " << answer[2] << endl;
	cout << isPrime(2147483647, 100) << endl;
	cout << isPrime(67217, 100) << endl;
	cout << simpleNumberRandom(1, 10 << 22) << endl;
	cout << diffieHellman() << endl;
	cout << endl << babyGiantStep(17, 438, 509) << endl;
}