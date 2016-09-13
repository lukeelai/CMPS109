// $Id: ubigint.cpp,v 1.8 2015-07-03 14:46:41-07 - - $

#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "ubigint.h"
#include "debug.h"

//may need work on this constructor
ubigint::ubigint (vector <udigit_t> that): ubig_value (that) {
   DEBUGF ('~', this << " -> ");
   for(unsigned int i = 0; i<ubig_value.size(); i++){
      DEBUGF( '~', ubig_value.at(i));
   }
}

ubigint::ubigint (const string& that): ubig_value(0) {
   //for (char digit: that) ubig_value = ubig_value * 10 + digit - '0';
   for( unsigned int i=0; i < that.length(); i++){
      ubig_value[i] = ubig_value[i] * 10 + that.at(i) - '0';
   }
}

//adds numbers by adding them piece by piece
//for loop will keep adding numbers until it reaches the end of both
ubigint ubigint::operator+ (const ubigint& that) const {
   bool carry = false;
   ubigint newNumber;
   for(unsigned int i = 0; i<that.ubig_value.size() || i<ubig_value.size(); i++){

      //newNumber is the new ubigint return value
      newNumber.ubig_value[i] = ubig_value[i]+ that.ubig_value[i];
      
      //if carry is needed from previous digit, add it and reset carry
      if(carry == true){
         newNumber.ubig_value[i]++;
         carry = false;
      }

      //if digit is over 9, set carry to true and subtract 10
      if(newNumber.ubig_value[i] > 9){
         newNumber.ubig_value[i] -= 10;
         carry = true;
      }
   } 
   return newNumber;  
}

//subtracts numbers piece by piece
//smallSub makes sure subtracted number is smaller
//smallSub == false means numbers are equal so far. Need to keep checking
//smallSub == true means subtracted number is smaller. No error
ubigint ubigint::operator- (const ubigint& that) const {
   bool smallSub = false;
   bool carry = false;
   ubigint newNumber;
   for(unsigned int i = 0; i<that.ubig_value.size() || i<ubig_value.size(); i++){
      if(smallSub == false){ 
         if (ubig_value[i] < that.ubig_value[i]) throw domain_error ("ubigint::operator-(a<b)");
      }
      //newNumber is the new ubigint return value
      newNumber.ubig_value[i] = ubig_value[i]+ that.ubig_value[i];
      
      //if carry was needed from previous digit, subtract it and reset carry
      if(carry == true){
         newNumber.ubig_value[i]--;
         carry = false;
      }

      //if digit is under 0, set carry to true and add 10
      if(newNumber.ubig_value[i] > 9){
         newNumber.ubig_value[i] += 10;
         carry = true;
      }
   } 
   return newNumber;  
}

ubigint ubigint::operator* (const ubigint& that) const {
   //return ubigint (uvalue * that.uvalue);
   //allocates new vector whose size is equal to the sum of the sizes of the operands
   //initializes all values of newNumber to 0
   vector<udigit_t> newNumber(ubig_value.size()+that.ubig_value.size(), 0);
   
   //Multiplication code taken from part 5h
   int carry = 0;
   int digit;
   for(unsigned int i=0; i<ubig_value.size(); i++){
      carry = 0;
      for(unsigned int j=0; j<that.ubig_value.size(); j++){
         digit = newNumber[i+j] + (ubig_value[i]*that.ubig_value[j]) + carry;
         newNumber[i+j] = digit % 10;
         carry = digit / 10;
      }
      newNumber[i + that.ubig_value.size()] = carry;
   }
   return ubigint(newNumber);
}

void ubigint::multiply_by_2() {
   //uvalue *= 2;
   int carry = 0;
   for(unsigned int i=0; i<ubig_value.size(); i++){
      ubig_value[i] = (ubig_value[i]*2) + carry;
      carry = 0;

      if(ubig_value[i] >9){
         carry = ubig_value[i] /10;
         ubig_value[i] = ubig_value[i] % 10;
      }
   }
}

void ubigint::divide_by_2() {
   //uvalue /= 2;
   for(unsigned int i=0; i<ubig_value.size(); i++){
      ubig_value[i] = (ubig_value[i]/2);
      if(ubig_value[i+1] %2 != 0)
         ubig_value[i] += 5;
   }
}


ubigint::quot_rem ubigint::divide (const ubigint& that) const {
   //static const ubigint zero = 0;
   //if (that == zero) throw domain_error ("ubigint::divide: by 0");
   //make sure divisor != 0
   bool isZero = true;
   for( unsigned int i = 0; i< that.ubig_value.size(); i++){
      if(that.ubig_value[i] != 0) isZero = false;
   }
   if(isZero == true) throw domain_error ("ubigint::divide: by 0");

   ubigint power_of_2 = vector<udigit_t>(1);
   ubigint divisor = that; // right operand, divisor
   ubigint quotient = vector<udigit_t>(0);
   ubigint remainder = *this; // left operand, dividend
   while (divisor < remainder) {
      divisor.multiply_by_2();
      power_of_2.multiply_by_2();
   }
   while (power_of_2 > ubigint(vector<udigit_t>(0))) {
      if (divisor <= remainder) {
         remainder = remainder - divisor;
         quotient = quotient + power_of_2;
      }
      divisor.divide_by_2();
      power_of_2.divide_by_2();
   }
   return {quotient, remainder};
}

ubigint ubigint::operator/ (const ubigint& that) const {
   return divide (that).first;
}

ubigint ubigint::operator% (const ubigint& that) const {
   return divide (that).second;
}

bool ubigint::operator== (const ubigint& that) const {
   //return uvalue == that.uvalue;
   if( that.ubig_value.size() != ubig_value.size()) return false;
   for(unsigned int i = 0; i<that.ubig_value.size() || i<ubig_value.size(); i++){
      if(that.ubig_value[i] != ubig_value[i]) return false;
   }
   return true;
}

bool ubigint::operator< (const ubigint& that) const {
   //return uvalue < that.uvalue;
   if( that.ubig_value.size() > ubig_value.size()) return true;
   else if (that.ubig_value.size() < ubig_value.size()) return false;
   else{
      for(int i=ubig_value.size(); i>=0; i--){
         if(that.ubig_value[i] > ubig_value[i]) return true;
         else if(that.ubig_value[i] < ubig_value[i]) return false;
      }
   return false;
   }
}


ostream& operator<< (ostream& out, const ubigint& that) { 
   string outputString = "x ";
   for(int i = that.ubig_value.size()+1; i >= 0; i--){
      outputString.append(1, static_cast<char>(that.ubig_value[i]));
      cout << that.ubig_value[i];
   }
   //cout << "outputString -- " << outputString;
   return out << "ubigint(" << outputString << ")";
   //return out << "ubigint(" << that.ubig_value << ")";
}
