# 41443132

## 申論及開發報告

## 解題說明

主要運用2個類別來完成 一個是多項式類別 一個是係數與指數 要自己寫一個複製建構子不然淺複製會出問題因為有指標 然後加法的部分是用一個對一個乘法利用雙重迴圈來完成分配律 求值是把用for回全把數字帶進去

## 程式實作

```cpp
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
```

## 效能分析

* **加法運算 (Add)**：時間複雜度為 $O(n + m)$，其中 $n$ 與 $m$ 分別為兩個多項式的非零項數量。空間複雜度為 $O(n + m)$ 用於儲存合併後的新多項式。

* **乘法運算 (Mult)**：時間複雜度為 $O(n \times m \cdot (n + m))$，透過雙重迴圈將每一項相乘後再逐步呼叫 `Add` 合併同類項。空間複雜度為 $O(n \times m)$。

* **求值運算 (Eval)**：時間複雜度為 $O(n \cdot \log(\text{exp}))$ 或取決於 `std::pow` 的運算，對每一項進行次方與係數乘加。空間複雜度為 $O(1)$。

## 測試與驗證

* **指定運算子自我賦值問題**：若未加上自我檢查，在進行 `a = a` 時會先將自身的動態記憶體 `delete[]` 釋放，導致後續複製時存取到已被釋放的無效記憶體而崩潰。

## 效能量測 (Measuring)

請輸入該多項式的非零項數量2
請從最高次方開始輸入11 3
22 4
請輸入該多項式的非零項數量2
請從最高次方開始輸入14 2
23 3
p1（x） = ×^3 + 2x^4
p2(x) = 4x^2 + 3x^3
Sum = x^3 + 2x^4 + 4x^2 + 3x^3
Prod = 8x^6 + 6x^7 + 4x^5 + 3x^6
2
01 = 40
01 = 40

## 心得討論

雖然是國中數學但要用C++寫出來還是需要漫長的思考