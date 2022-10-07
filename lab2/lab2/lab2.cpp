#define _CRT_SECURE_NO_WARNINGS
#define ll long long
#define MAX(x,y) (x)>(y)?(x):(y)
#define MIN(x,y) (x)<(y)?(x):(y)
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/istreamwrapper.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>
#include <vector>
#include <fstream>
#include <map>

using namespace rapidjson;
using namespace std;
const int BUFFERSIZE = 4096;

ll modularExponentiation(ll x, ll ex, ll p)
{
    ll result = 1;
    for (ll na = abs(ex); na > 0; na >>= 1) {
        if (na % 2 == 1) {
            result = (result * x) % p;
        }
        x = (x * x) % p;
    }
    return result % p;
}

ll* eucleadian(ll a, ll b) {
    ll static u[]{ a, 1, 0 }, v[]{ b, 0, 1 }, t[]{ 0, 0, 0 };
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

ll random()
{
    srand(time(nullptr));
    return rand() + 1;
}

ll gcd(ll a, ll b, ll* x, ll* y)
{
    ll U_array[] = { MAX(a, b), 1, 0 };
    ll V_array[] = { MIN(a, b), 0, 1 };
    ll T_array[3];
    ll q, * swop_p, * U, * V, * T;

    q = MAX(a, b);
    if (q != a) {
        swop_p = x;
        x = y;
        y = swop_p;
    }

    U = U_array;
    V = V_array;
    T = T_array;
    while (V[0] != 0) {
        q = U[0] / V[0];
        T[0] = U[0] % V[0];
        T[1] = U[1] - q * V[1];
        T[2] = U[2] - q * V[2];
        swop_p = U;
        U = V;
        V = T;
        T = swop_p;
    }
    if (x != NULL) {
        *x = U[1];
    }
    if (y != NULL) {
        *y = U[2];
    }
    return U[0];
}

ll inversion(ll* c, ll* d, ll p)
{
    ll x, y;
    ll big_c, big_d, big_p;

    do {
        *c = random();
    } while (gcd(*c, p, &x, &y) != 1);

    big_c = *c;
    big_p = p;

    *d = x < 0 ? p + x : x;
    big_d = *d;

    return (big_c * big_d) % big_p;
}

bool ferma(ll x)
{
    ll a;

    if (!(x % 2)) {
        return false;
    }
    for (int i = 0; i < 100; i++) {
        a = (rand() % (x - 2)) + 2;
        if (gcd(a, x, NULL, NULL) != 1)
            return false;
        if (modularExponentiation(a, x - 1, x) != 1)
            return false;
    }
    return true;
}

vector<ll> Shamir(vector<unsigned char> m) {
    ll c1, d1, p, x, x1, x2, x3;
    do {
        p = simpleNumberRandom(0, 1 << 20);
    } while (p <= 65500);
    inversion(&c1, &d1, p - 1);
    ll c2, d2;
    inversion(&c2, &d2, p - 1);

    
    vector<ll> encode;
    for (unsigned char a : m) {
        x = modularExponentiation(a, c1, p);

        /* B1 */

        x1 = modularExponentiation(x, c2, p);

        /* A2 */
        x2 = modularExponentiation(x1, d1, p);

        /* B2 */
        x3 = modularExponentiation(x2, d2, p);
        //4ый шаг
        //x = modularExponentiation(x, Db, p);
        //cout << (long long)a << " == " << x3 << "  x2 = " << x2 << " == " << static_cast<unsigned char>(x2) << endl;
        encode.push_back(x2);
    }

    Document d;
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
    d.SetObject();
    d.AddMember("p", p, allocator);
    d.AddMember("C1", c1, allocator);
    d.AddMember("D1", d1, allocator);
    d.AddMember("C2", c2, allocator);
    d.AddMember("D2", d2, allocator);
    ofstream foutK("shamirKeys.json", ios::binary);
    OStreamWrapper out(foutK);
    Writer<OStreamWrapper> writer(out);
    d.Accept(writer);
    return encode;
}

vector<unsigned char> ShamirDecode(vector <ll> m) {
    std::ifstream ifs("shamirKeys.json");
    IStreamWrapper isw{ ifs };

    Document doc{};
    doc.ParseStream(isw);

    StringBuffer buffer{};
    Writer<StringBuffer> writer{ buffer };
    doc.Accept(writer);
    const std::string jsonStr{ buffer.GetString() };

    std::cout << jsonStr << '\n';
    vector<unsigned char> result;
    ll x = 0;
    ll p = doc["p"].GetInt64();
    ll Db = doc["D2"].GetInt64();
    for (ll a : m) {
        //cout << a << " " << static_cast<long long> (a) <<  endl;
        x = modularExponentiation(a, Db, p);
        result.push_back(x);
        //cout << (long long) a << " " << x << " " << (unsigned char)x << endl;
    }
    return result;
}

vector<ll> ElGamal(vector<unsigned char> m) {
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
    ll Xb = simpleNumberRandom(1, p);
    ll Yb = modularExponentiation(g, Xb, p); //открытое число

    ll k = random() % (p - 2);
    ll a = modularExponentiation(g, k, p);
    vector<ll> res;
    for (unsigned char x : m) {
        ll b =  x * modularExponentiation(Yb, k, p) % p;
        res.push_back(b);
    }
    Document d;
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
    d.SetObject();
    d.AddMember("p", p, allocator);
    d.AddMember("Xb", Xb, allocator);
    d.AddMember("a", a, allocator);
    ofstream foutK("elGamalKeys.json", ios::binary);
    OStreamWrapper out(foutK);
    Writer<OStreamWrapper> writer(out);
    d.Accept(writer);
    return res;
}

vector<unsigned char> ElGamalDecode(vector <ll> m) {
    std::ifstream ifs("elGamalKeys.json");
    IStreamWrapper isw{ ifs };

    Document doc{};
    doc.ParseStream(isw);

    StringBuffer buffer{};
    Writer<StringBuffer> writer{ buffer };
    doc.Accept(writer);
    const std::string jsonStr{ buffer.GetString() };

    std::cout << jsonStr << '\n';
    vector<unsigned char> result;
    ll x = doc["Xb"].GetInt64();
    ll p = doc["p"].GetInt64();
    ll a = doc["a"].GetInt64();
    for (ll b : m) {
        result.push_back(b * modularExponentiation(a, p - 1 - x, p) % p);
    }
    return result;
}

vector<ll> rsa(vector<unsigned char> m) {
    ll p = simpleNumberRandom(0, 1 << 15);
    ll q = simpleNumberRandom(0, 1 << 15);

    ll n = p * q;

    ll phi = (p - 1) * (q - 1);
    ll c, d_;

    inversion(&c, &d_, phi);
    vector<ll> result;
    for (unsigned char x : m) {
        ll e = modularExponentiation(x, d_, n);
        result.push_back(e);
    }
    Document d;
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
    d.SetObject();
    d.AddMember("c", c, allocator);
    d.AddMember("n", n, allocator);
    ofstream foutK("rsaKeys.json", ios::binary);
    OStreamWrapper out(foutK);
    Writer<OStreamWrapper> writer(out);
    d.Accept(writer);
    return result;
}

vector<unsigned char> rsadecode(vector <ll> m) {
    std::ifstream ifs("rsaKeys.json");
    IStreamWrapper isw{ ifs };

    Document doc{};
    doc.ParseStream(isw);

    StringBuffer buffer{};
    Writer<StringBuffer> writer{ buffer };
    doc.Accept(writer);
    const std::string jsonStr{ buffer.GetString() };

    std::cout << jsonStr << '\n';
    vector<unsigned char> result;
    ll n = doc["n"].GetInt64();
    ll c = doc["c"].GetInt64();
    for (ll b : m) {
        result.push_back(modularExponentiation(b, c, n));
    }
    return result;
}

vector<ll> Vernam(vector<unsigned char> m){
    vector<int> codes(m.size());
    vector<ll> result(m.size());
    for (ll i = 0; i < m.size(); i++) {
        codes.at(i) = rand() % 256;
    }
    for (ll i = 0; i < m.size(); i++) {
        result.at(i) = m.at(i) ^ codes.at(i);
    }
    Document d;
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
    d.SetObject();
    Value array(kArrayType);
    for (int i = 0; i < m.size(); i++) {
        array.PushBack(codes.at(i), allocator);
    }
    d.AddMember("codes", array, allocator);
    ofstream foutK("vernamKeys.json", ios::binary);
    OStreamWrapper out(foutK);
    Writer<OStreamWrapper> writer(out);
    d.Accept(writer);
    return result;
}

vector<unsigned char> VernamDecode(vector <ll> m) {
    std::ifstream ifs("vernamKeys.json");
    IStreamWrapper isw{ ifs };

    Document doc{};
    doc.ParseStream(isw);

    StringBuffer buffer{};
    Writer<StringBuffer> writer{ buffer };
    doc.Accept(writer);
    const std::string jsonStr{ buffer.GetString() };

    //std::cout << jsonStr << '\n';
    vector<unsigned char> result;
    vector<ll> codes;
    auto& codesJson = doc["codes"];
    for (auto&& item : codesJson.GetArray()) {
        codes.push_back(item.GetInt64());
    } 
    for (ll i = 0; i < m.size(); i++) {
        result.push_back(m.at(i) ^ codes.at(i));
    }
    return result;
}

void writeToFile(vector<ll> a, string filename) {
    ofstream fout(filename, ios::out | ios::binary);
    size_t size = a.size();
    fout.write(reinterpret_cast<const char*>(&a[0]), a.size() * sizeof(ll));
    fout.close();
}

void writeToFile(vector<unsigned char> a, string filename) {
    ofstream fout(filename, ios::out | ios::binary);
    size_t size = a.size();
    fout.write(reinterpret_cast<const char*>(&a[0]), a.size() * sizeof(unsigned char));
    fout.close();
}

vector<ll> readFromFile(string filename, ll N) {
    ifstream fin(filename, ios::out | ios::binary);
    const size_t count = fin.tellg() / sizeof(ll);
    std::vector<ll> vec;
    vec.resize(N);
    fin.read(reinterpret_cast<char*>(&vec[0]), N*sizeof(ll));
    fin.close();
    return vec;
}


int main() {
   
    std::ifstream input("original.png", std::ios::binary);
    std::ofstream output("123l1.txt", std::ios::binary);
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

   /* vector<ll> v = Shamir(buffer);
    writeToFile(v, "shamir.encode");
    vector <ll> out = readFromFile("shamir.encode", v.size());
    cout << v.size() << " " << out.size() << endl;
    writeToFile(ShamirDecode(out), "shamir.png");*/

  
    //vector<ll> elGamal = ElGamal(buffer);
    //writeToFile(elGamal, "elgamal.encode");
    //vector <ll> out = readFromFile("elgamal.encode", elGamal.size());
    //cout << elGamal.size() << " " << out.size() << endl;
    //writeToFile(ElGamalDecode(out), "elgamaldecode.png");
  
    /*vector<ll> rsaE = rsa(buffer);
    writeToFile(rsaE, "rsa.encode");
    vector <ll> out = readFromFile("rsa.encode", rsaE.size());
    cout << rsaE.size() << " " << out.size() << endl;
    writeToFile(rsadecode(out), "rsadecode.png");*/

    vector<ll> vernam = Vernam(buffer);
    writeToFile(vernam, "vernam.encode");
    vector <ll> out = readFromFile("vernam.encode", vernam.size());
    cout << vernam.size() << " " << out.size() << endl;
    writeToFile(VernamDecode(out), "vernamdecode.png");
    cout << endl;
    return 0;
}