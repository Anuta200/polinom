#include <iostream>
#include "Monom.h"
int main()
{
    setlocale(LC_ALL, "Russian");
    Polynom p1, p2;
    std::cin >> p1;
    std::cin >> p2;
    // Polynom sum = p1 + p2;  
     //std::cout << "Ñóììà: " << sum << std::endl;
    std::cout << p1 << std::endl;
    std::cout << p2 << std::endl;
    return 0;

}
/*
* int main(){
* vector <int> v={1,2,3,4,5};
* vector <int>::iterator i;
* for(i=v.begin(); i!=v.end();i++){
* cout<<*i<<" ";
* }
* cout << endl;
*
*
*
*
*/
