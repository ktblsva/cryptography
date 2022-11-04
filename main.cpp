#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <exception>
#include <random>
#include <algorithm>

#define MAX(x, y) (x)>(y)?(x):(y)
#define MIN(x, y) (x)<(y)?(x):(y)
#define N 52
#define ll long long
using namespace std;

ll modularExponentiation(ll x, ll ex, ll p) {
    ll result = 1;
    for (ll na = abs(ex); na > 0; na >>= 1) {
        if (na % 2 == 1) {
            result = (result * x) % p;
        }
        x = (x * x) % p;
    }
    return result % p;
}

ll gcd(ll a, ll b, ll *x, ll *y) {
    ll U_array[] = {MAX(a, b), 1, 0};
    ll V_array[] = {MIN(a, b), 0, 1};
    ll T_array[3];
    ll q, *swop_p, *U, *V, *T;

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

ll inversion(ll *c, ll *d, ll p) {
    ll x, y;
    ll b_c, b_d, b_p;

    do {
        *c = rand() + 1;
    } while (gcd(*c, p, &x, &y) != 1);

    b_c = *c;
    b_p = p;

    *d = x < 0 ? p + x : x;
    b_d = *d;

    return (b_c * b_d) % b_p;
}

bool ferma(ll x) {
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

ll *eucleadian(ll a, ll b) {
    ll static u[]{a, 1, 0}, v[]{b, 0, 1}, t[]{0, 0, 0};
    ll q = 0;
    while (v[0]) {
        q = u[0] / v[0];
        t[0] = u[0] % v[0];
        t[1] = u[1] - q * v[1];
        t[2] = u[2] - q * v[2];
        u[0] = v[0];
        u[1] = v[1];
        u[2] = v[2];
        v[0] = t[0];
        v[1] = t[1];
        v[2] = t[2];

    }
    //cout << u[0] << " " << u[1] << " " << u[2] << endl;
    return u;
}

bool isPrime(ll p, int k) {

    if (p == 2) {
        return true;
    }
    if (!(p & 1)) {
        return false;
    }
    for (int i = 0; i < k; i++) {
        ll a = rand() % (p - 2) + 1;
        if (eucleadian(a, p)[0] != 1 || modularExponentiation(a, p - 1, p) != 1) {
            //cout << a << p << endl;
            return false;
        }
    }
    return true;
}

ll primeNumberRandom(ll left, ll right) {
    ll cnt = 0;
    while (1) {
        ll x = (rand() * (ll) rand() + rand()) % (right - left) + left;
        x |= 1;
        //std::cout << x << std::endl;
        if (isPrime(x, 100)) {
            return x;
        }
    }
}

//clubs, diamonds, hearts, spades
vector<pair<ll, string>> generateDeck() {
    vector<string> suits{"C", "D", "H", "S"};
    vector<string> faces{"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    int i = 2;
    vector<pair<ll, string>> deck;

    for (string suit: suits) {
        for (string face: faces) {
            deck.push_back(make_pair(i, suit + face));
            i++;
        }
    }
//    map<int, string>::iterator it = cards.begin();
//    for (int i = 0; it != cards.end(); it++, i++) {
//        cout << it->first << " " << it->second << endl;
//    }

    for (auto const &p: deck) {
        cout << p.first << " " << p.second << endl;
    }
    return deck;
}

void shuffle(vector<pair<ll, string>> &cards) {
    random_device random_device;
    mt19937 generator(random_device());
    shuffle(cards.begin(), cards.end(), generator);
}

void deckEncryption(vector<ll> &c, vector<ll> &d, int pN, ll p, vector<pair<ll, string>> &deck) {
    for (auto const &p: deck) {
        cout << p.first << " " << p.second << "; ";
    }
    cout << endl;
    for (int i = 0; i < pN; i++) {
        ll c_, d_;
        inversion(&c_, &d_, p - 1);
        c.at(i) = c_;
        d.at(i) = d_;
        for (int i = 0; i < deck.size(); i++) {
            pair<ll, string> card = deck.at(i);
            deck.at(i) = make_pair(modularExponentiation(card.first, c_, p), card.second);
        }
        for (auto const &p: deck) {
            cout << p.first << " " << p.second << "; ";
        }
        cout << endl;
    }

}

void cardsDistribution(vector<pair<ll, string>> &deck, int pN, map<int, vector<pair<ll, string>>> &players) {
    for (int i = 0; i < pN; i++) {
        vector<pair<ll, string>>::const_iterator first = deck.begin() + 0;
        vector<pair<ll, string>>::const_iterator last = deck.begin() + 2;
        vector<pair<ll, string>> toPlayer(first, last);
        deck.erase(first, last);
        players[i] = toPlayer;
    }

    vector<pair<ll, string>>::const_iterator first = deck.begin() + 0;
    vector<pair<ll, string>>::const_iterator last = deck.begin() + 5;
    vector<pair<ll, string>> table(first, last);
    deck.erase(first, last);
    players[-1] = table;
}

void cardsDecode(map<int, vector<pair<ll, string>>> &players, vector<ll> c, vector<ll> d, int pN, ll p) {
    vector<pair<ll, string>> table = players[-1];
    cout << endl << endl;
    cout << "__________________________________" << endl;
    for (int i = 0; i < pN; i++) {
        ll key = d.at(i);
        for (int j = 0; j < table.size(); j++) {
            ll card = table.at(j).first;
            table.at(j) = make_pair(modularExponentiation(card, key, p), table.at(j).second);
        }
    }
    cout << "Table: ";
    for (auto const &p: table) {
        cout << p.first << " " << p.second << "; ";
    }
    cout << endl;

    for (int i = 0; i < pN; i++) {
        vector<pair<ll, string>> playerCards = players[i];
        for (int j = 0; j < pN; j++) {
            ll key = d.at(j);
            if (i != j) {
                for (int k = 0; k < playerCards.size(); k++) {
                    ll card = playerCards.at(k).first;
                    playerCards.at(k) = make_pair(modularExponentiation(card, key, p), playerCards.at(k).second);
                }
            }
        }
        ll key = d.at(i);
        for (int k = 0; k < playerCards.size(); k++) {
            ll card = playerCards.at(k).first;
            playerCards.at(k) = make_pair(modularExponentiation(card, key, p), playerCards.at(k).second);
        }
        players[i] = playerCards;
    }

    for (int i = 0; i < pN; i++) {
        cout << "Player " << i << " has cards: ";
        for (auto card: players[i]) {
            cout << card.first << " - " << card.second << " ";
        }
        cout << endl;
    }

}

int main() {
    std::cout << "Hello, World!" << std::endl;
    srand(42);
    vector<pair<ll, string>> deck = generateDeck();;

    int pN = 0;
    cout << "Enter number of players: " << endl;
    try {
        cin >> pN;
        if (pN < 0 || pN > 24) {
            throw runtime_error("Check players number");
        }
    } catch (runtime_error e) {
        cerr << e.what() << endl;
    }
//    for (auto const &p: deck) {
//        cout << p.first << " " << p.second << endl;
//    }

    shuffle(deck);

    ll p, q;
    while (1) {
        q = primeNumberRandom(1 << 20, 1 << 30);
        p = 2 * q + 1;
        if (isPrime(p, 100)) {
            break;
        }
    }
    cout << "P = " << p << ", Q = " << q << endl;

    vector<ll> c(pN), d(pN);
    deckEncryption(c, d, pN, p, deck);
    map<int, vector<pair<ll, string>>> players;
    cardsDistribution(deck, pN, players);
    cardsDecode(players, c, d, pN, p);
    return 0;
}
