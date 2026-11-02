#include <iostream>
#include <cmath>    

using namespace std;
struct Term {
    float coef; // 係數 
    int exp;    // 指數 
};
class Polynomial {
private:
    Term* termArray; // 動態陣列
    int capacity;    // 容量
    int terms;       // 非零項數量
public:
    Polynomial() {
        capacity = 2;
        terms = 0;
        termArray = new Term[capacity];
    }

    Polynomial(const Polynomial& poly) {
        capacity = poly.capacity;
        terms = poly.terms;
        termArray = new Term[capacity];
        for (int k = 0; k < terms; k++) {
            termArray[k] = poly.termArray[k];
        }
    }
    Polynomial& operator=(const Polynomial& poly) {
            capacity = poly.capacity;
            terms = poly.terms;
            termArray = new Term[capacity];
            for (int k = 0; k < terms; k++) {
                termArray[k] = poly.termArray[k];
            }
        return *this;
    }
    void NewTerm(float c, int e) {
        if (c == 0) return; // 係數是0不儲存
        if (terms == capacity) {
            capacity *= 2;
            Term* temp = new Term[capacity];
            for (int k = 0; k < terms; k++) {
                temp[k] = termArray[k];
            }
            delete[] termArray; 
            termArray = temp;   // 將指標指向新記憶體
        }
        termArray[terms].coef = c;
        termArray[terms].exp = e;
        terms++;
    }

    Polynomial Add(Polynomial poly) {
        Polynomial c;
        int aPos = 0;
        int bPos = 0;

        while ((aPos < terms) && (bPos < poly.terms)) {
            if (termArray[aPos].exp == poly.termArray[bPos].exp) {
                float sum = termArray[aPos].coef + poly.termArray[bPos].coef;
                if (sum != 0) c.NewTerm(sum, termArray[aPos].exp);
                aPos++;
                bPos++;
            }
            else if (termArray[aPos].exp < poly.termArray[bPos].exp) {
                c.NewTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);
                bPos++;
            }
            else {
                c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
                aPos++;
            }
        }
        for (; aPos < terms; aPos++) {
            c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
        }
        for (; bPos < poly.terms; bPos++) {
            c.NewTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);
        }
        return c;
    }

    Polynomial Mult(Polynomial poly) {
        Polynomial result;
        for (int k = 0; k < terms; k++) {
            Polynomial temp;
            for (int j = 0; j < poly.terms; j++) {
                float c = termArray[k].coef * poly.termArray[j].coef;
                int e = termArray[k].exp + poly.termArray[j].exp;
                temp.NewTerm(c, e);
            }
            result = result.Add(temp);
        }
        return result;
    }

    float Eval(float f) {
        float result = 0.0;
        for (int k = 0; k < terms; k++) {
            result += termArray[k].coef * std::pow(f, termArray[k].exp);
        }
        return result;
    }

    friend ostream& operator<<(ostream& o, const Polynomial& poly) {
        if (poly.terms == 0) {
            o << "0";
            return o;
        }
        for (int k = 0; k < poly.terms; k++) {
            if (k > 0 && poly.termArray[k].coef > 0) {
                o << " + ";
            }
            else if (poly.termArray[k].coef < 0) {
                o << " - ";
            }

            float absCoef = std::abs(poly.termArray[k].coef);

            if (poly.termArray[k].exp == 0) {
                o << absCoef;
            }
            else if (poly.termArray[k].exp == 1) {
                if (absCoef != 1.0f) o << absCoef;
                o << "x";
            }
            else {
                if (absCoef != 1.0f) o << absCoef;
                o << "x^" << poly.termArray[k].exp;
            }
        }
        return o;
    }

    friend istream& operator>>(istream& i, Polynomial& poly) {
        int numTerms;
        cout << "請輸入該多項式的非零項數量";
        i >> numTerms;

        cout << "請從最高次方開始輸入";
        for (int j = 0; j < numTerms; j++) {
            float c;
            int e;
            cout << j + 1;
            i >> c >> e;
            poly.NewTerm(c, e);
        }
        return i;
    }
};

int main()
{
    Polynomial p1, p2;

    cin >> p1;

    cin >> p2;

    cout << "p1(x) = " << p1 << endl;
    cout << "p2(x) = " << p2 << endl;

    Polynomial sum = p1.Add(p2);
    cout << "Sum = " << sum << endl;

    Polynomial prod = p1.Mult(p2);
    cout << "Prod = " << prod << endl;
    float x_val = 0;
    cin >> x_val;
    cout << "p1 = " << p1.Eval(x_val) << endl;
    cout << "p1 = " << p2.Eval(x_val) << endl;

    return 0;
}