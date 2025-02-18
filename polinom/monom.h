#pragma once
#include <iostream>
#include <string.h>
#include <string>
#include <cmath>
#include <cctype>
struct Monom
{
    double coeff;
    int x, y, z;
public:
    Monom(double c = 0, int x_ = 0, int y_ = 0, int z_ = 0)
        : coeff(c), x(x_), y(y_), z(z_) {}
    /*friend std::istream& operator>>(std::istream& in, Monom& m) {
        in >> m.coeff >> m.x >> m.y >> m.z;
        return in;
    }*/
    friend std::ostream& operator<<(std::ostream& out, const Monom& m)
    {
        out << m.coeff << "x^" << m.x << "y^" << m.y << "z^" << m.z;
        return out;
    }
    bool operator<(const Monom& other) const;
    bool operator>(const Monom& other) const;
    bool operator==(const Monom& other) const;
};

bool Monom :: operator<(const Monom& other) const
{
    if (x != other.x) return x < other.x;
    if (y != other.y) return y < other.y;
    return z < other.z;
}
bool Monom :: operator>(const Monom& other) const
{
    return other < *this;
}
bool Monom::operator==(const Monom& other) const
{
    return x == other.x && y == other.y && z == other.z;
}

template <typename T>
struct Node
{
    T val;
    Node* pNext;
};
template <typename T>
class List {
protected:
    Node <T>* pFirst, * pLast;
    int sz;
public:
    void InsFirst(T elem);
    void DelFirst();
    List();
    ~List();
    void Clear();
    List(const List& other);
    List& operator=(const List& other);
    void InsLast(T elem);
    void DelLast();
    friend std::ostream& operator<<(std::ostream& out, const List<T>& list) {
        Node<T>* current = list.pFirst;
        while (current) {
            out << current->val << " -> ";
            current = current->pNext;
        }
        out << "NULL";
        return out;
    }

    friend std::istream& operator>>(std::istream& in, List<T>& list) {
        T value;
        std::cout << "Введите элементы списка (для завершения введите 'q'): ";

        while (true) {
            if (!(in >> value)) {  // Проверяем ввод
                in.clear();         // Сбрасываем ошибочное состояние
                std::string stop;
                in >> stop;         // Считываем строку
                if (stop == "q") break;  // Если "q" — выходим
                else {
                    std::cerr << "Ошибка ввода! Попробуйте снова: ";
                    continue;
                }
            }
            list.InsLast(value);
        }
        return in;
    }
};

template <typename T>
void List<T>::InsFirst(T elem) {
    Node <T>* tmp = new Node <T>;
    tmp->val = elem;
    tmp->pNext = pFirst;
    if (pFirst == nullptr) {
        pLast = tmp;
        pFirst = tmp;
    }
    sz++;
}

template <typename T>
void List<T>::DelFirst() {
    if (pFirst == nullptr) return;
    Node <T>* tmp = pFirst;
    pFirst = pFirst->pNext;
    delete tmp;
    if (pFirst == nullptr)
        pLast = nullptr;
    sz--;
}

template <typename T>
void List<T>::Clear() {
    while (pFirst) {
        DelFirst();
    }
}

template <typename T>
void List<T>::InsLast(T elem) {
    Node<T>* tmp = new Node<T>;
    tmp->val = elem;
    if (pLast) {
        pLast->pNext = tmp;
    }
    else {
        pFirst = tmp;
    }
    pLast = tmp;
    sz++;
}

template <typename T>
void List<T>::DelLast() {
    if (pFirst == nullptr) return;
    if (pFirst == pLast) {
        delete pFirst;
        pFirst = pLast = nullptr;
    }
    else {
        Node<T>* current = pFirst;
        while (current->pNext != pLast) {
            current = current->pNext;
        }
        delete pLast;
        pLast = current;
        pLast->pNext = nullptr;
    }
    sz--;
}

template <typename T>
List<T>::List() : pFirst(nullptr), pLast(nullptr), sz(0) {}

template <typename T>
List<T>::~List() {
    Clear();
}

template <typename T>
List<T>::List(const List& other) : pFirst(nullptr), pLast(nullptr), sz(0) {
    Node<T>* current = other.pFirst;
    while (current) {
        InsLast(current->val);
        current = current->pNext;
    }
}

template <typename T>
List<T>& List<T>:: operator=(const List& other) {
    if (this == &other) return *this;
    Clear();
    Node<T>* current = other.pFirst;
    while (current) {
        InsLast(current->val);
        current = current->pNext;
    }
    return *this;
}



class Polynom : public List<Monom> {
public:
    Polynom() = default;
    Polynom(const Polynom& other) : List<Monom>(other) {}

    Polynom& operator=(const Polynom& other) {
        List<Monom>::operator=(other);
        return *this;
    }


    void Simplify() {
        if (!pFirst) return;
        Node<Monom>* current = pFirst;
        while (current && current->pNext) {
            if (current->val == current->pNext->val) {
                current->val.coeff += current->pNext->val.coeff;
                Node<Monom>* tmp = current->pNext;
                current->pNext = tmp->pNext;
                delete tmp;
                sz--;
            }
            else {
                current = current->pNext;
            }
        }
    }


    Polynom operator+(const Polynom& other) const {
        Polynom result = *this;
        Node<Monom>* current = other.pFirst;
        while (current) {
            result.InsLast(current->val);
            current = current->pNext;
        }
        result.Simplify();
        return result;
    }


    Polynom operator-(const Polynom& other) const {
        Polynom result = *this;
        Node<Monom>* current = other.pFirst;
        while (current) {
            result.InsLast(Monom(-current->val.coeff, current->val.x, current->val.y, current->val.z));
            current = current->pNext;
        }
        result.Simplify();
        return result;
    }


    friend std::ostream& operator<<(std::ostream& out, const Polynom& poly) {
        Node<Monom>* current = poly.pFirst;
        if (!current) {
            out << "0";
            return out;
        }
        while (current) {
            if (current != poly.pFirst && current->val.coeff > 0)
                out << " + ";
            out << current->val;
            current = current->pNext;
        }
        return out;
    }


    friend std::istream& operator>>(std::istream& in, Polynom& poly) {
        std::cout << "Введите коэффициент и степени x, y, z (ввод завершится при нечисловом вводе):\n";
        Monom m;
        while (in >> m.coeff >> m.x >> m.y >> m.z) {
            poly.InsLast(m);
        }
        //in.clear();
        //in.ignore();
        poly.Simplify();
        return in;
    }
};




/*Необходимый код : (можете поправлять)
класс список со всеми необходимыми методами
класс моном :
операции ввода, вывода, сравнения
класс полином : унаследоваться от списка<Моном>
конструкторы
операции ввода, вывода
остальные методы унаследованы

Баркалов сказал, что на след паре будем писать метод, добавляющий моном в полином
*/
