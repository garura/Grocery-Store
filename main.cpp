//============================================================================
// File Name   : Assignment 6
// Author      : Chris Jordan
// Description : Online Fruit-Ordering Application
// Revision History: None
// Date             Version     Change ID    Author          Comment
// 06-25-15          1.1         NA           Chris Jordan    None
//============================================================================

#include <iostream>
#include <iomanip>

using namespace std;

int const gSize = 16;


class Fruit {
private:
   string FruitName;
   double Weight;
   double UnitPrice;

public:
   Fruit();
   Fruit(string FruitName, double Weight, double UnitPrice);
   ~Fruit();

   void SetName(string name);
   string GetName();
   void SetWeight(double weight);
   double GetWeight();
   void SetPrice(double price);
   double GetPrice();
   double FruitOrder(double fruitWeight);
   void Display();
};

Fruit::Fruit(): FruitName(""),Weight(0.0),UnitPrice(0.0) {}

Fruit::Fruit(string FruitName, double Weight, double UnitPrice) {
   this->FruitName = FruitName;
   this->Weight = Weight;
   this->UnitPrice = UnitPrice;
}

Fruit::~Fruit() {
   cout << "Destroyed Fruit " << FruitName << ", Weight = ";
   cout << Weight << "." << endl;
}

void Fruit::SetName(string name) {
   FruitName = name;
}

string Fruit::GetName() {
   return FruitName;
}

void Fruit::SetWeight(double weight) {
   if (weight >0) {
      Weight = weight;
   }
}

double Fruit::GetWeight() {
   return Weight;
}

void Fruit::SetPrice(double price) {
   if (price > 0) {
      UnitPrice = price;
   }
}

double Fruit::GetPrice() {
   return UnitPrice;
}

double Fruit::FruitOrder(double fruitWeight) {
   if (fruitWeight > Weight) {
      return -1;
   }
   else {
      Weight -= fruitWeight;
      return fruitWeight * UnitPrice;
   }
}

void Fruit::Display() {
   cout << fixed << setprecision(2);
   cout << left << setw(10) << FruitName;
   cout << right << setw(10) << Weight << " lbs";
   cout << "\t  $ " << setw(4) << UnitPrice << "/lb\n";

}



class OnlineSuperMarket {
private:
   Fruit *pFruitList;
   string MarketName;
   string WebAddress;
   static double TaxRate;

public:
   OnlineSuperMarket();
   OnlineSuperMarket(string Marketname, string WebAddress);
   ~OnlineSuperMarket();

   void Init(Fruit *fruit);
   void Sort();
   void Run();
   Fruit * FindFruit(string fruitname);
   void Quit();
   static double GetTaxRate();

   friend void ListAllFruits(OnlineSuperMarket *pMarket);
};

double OnlineSuperMarket::TaxRate = .085;

double OnlineSuperMarket::GetTaxRate() {
   return TaxRate;
}

OnlineSuperMarket::OnlineSuperMarket():
                        pFruitList(NULL),MarketName(""),WebAddress("") {}

OnlineSuperMarket::OnlineSuperMarket(string MarketName, string WebAddress) {
   this->MarketName = MarketName;
   this->WebAddress = WebAddress;
   this->pFruitList = NULL;
}

OnlineSuperMarket::~OnlineSuperMarket() {
   cout << "\nOnline market (" << MarketName;
   cout << " " << WebAddress << ") is closed.\n" << endl;
}

void OnlineSuperMarket::Init(Fruit *fruit) {
   pFruitList = fruit;
   Sort();
}
// dont call this by itself, pointer might not be at first element anymore!
void OnlineSuperMarket::Sort() {
   bool unsorted = true;
   Fruit FruitBuffer; // deconstructor called when this leaves scope
   while (unsorted) {
      unsorted = false;
      for (int i=0; i < (gSize-1); i++) {
         if (pFruitList[i].GetName() > pFruitList[i+1].GetName()) {
            unsorted = true;
            FruitBuffer = pFruitList[i];
            pFruitList[i] = pFruitList[i+1];
            pFruitList[i+1] = FruitBuffer;
         }
      }
   }
}

void OnlineSuperMarket::Run() {
   int MenuChoice = 0;
   string FruitName;
   double FruitWeight = 0.0;
   double CustFruitOrder = 0.0;
   Fruit *pFruit;
   cout << "Your most convenient and time saving way to order food and fruit from";
   cout << "\n\n\t\t\t" << MarketName << endl;
   cout << "\t\t\t" << WebAddress << endl;
   bool shopping = true;
   while (shopping) {
      cout << "\n\t\t\tSelect Option:\n\n\t\t\t1. Order Fruit\n\t\t\t2. Quit\n";
      cin >> MenuChoice;
      /* no user error checking for any of the inputs (choice, fruit name, weight)
      if (cin.fail()) {
         cin.clear();
         while (cin.peek() != '\n') {
            cin.ignore();
         }
         MenuChoice = 0;
      }
       */
      switch(MenuChoice){
         case 1:
            ListAllFruits(this);
            cout << "\nEnter a fruit name: ";
            cin >> FruitName;
            cout << "\nEnter weight in lbs: ";
            cin >> FruitWeight;
            pFruit = FindFruit(FruitName);
            if (!pFruit) {
               cout << "Fruit not found.";
            }
            else {
               CustFruitOrder = pFruit->FruitOrder(FruitWeight);
               if (CustFruitOrder == -1) {
                  cout << "Quantity Unavailable";
               }
               else {
                  CustFruitOrder = CustFruitOrder * (1 + OnlineSuperMarket::GetTaxRate());
                  cout << "Total cost: $" << fixed << setprecision(2) << CustFruitOrder;
                  cout << " (tax included)\n\n";
               }
            }
            break;
         case 2:
            Quit();
            shopping = false;
            break;
         default:
            cout << "Please Choose 1 or 2.";
      }
   }
}

// binary search
Fruit * OnlineSuperMarket::FindFruit(string fruitname) {
   int First = 0;
   int Last = gSize-1;
   int Mid = 0;
   while (First <= Last) {
      Mid = (First+Last)/2;
      if (pFruitList[Mid].GetName() == fruitname) {
         return & pFruitList[Mid];   // found a match, return the index where the match is found
      }
      else if (fruitname > pFruitList[Mid].GetName()) {
         First = Mid+1;  // order must be in upper half of the list
      }
      else {
         Last = Mid-1;   // order must be in lower half of the list
      }
   }
   return NULL;  // not found
}





void ListAllFruits(OnlineSuperMarket *pOSM) {

    for (int i = 0; i < gSize; i++) {
    (pOSM->pFruitList)[i].Display();
    }

}

void OnlineSuperMarket::Quit() {
   ListAllFruits(this);
   cout << "\nThank you, please visit again.";
}

int main() {
   Fruit FruitArray[gSize] = {Fruit ("Apple", 145.50, 1.50),
                              Fruit ("Orange", 5.50, 2.99),
                              Fruit ("Pear", 47.50, 1.90),
                              Fruit ("Mango", 75.50, 2.25),
                              Fruit ("Peach", 25.50, 4.00),
                              Fruit ("Coconut", 310.50, 1.00),
                              Fruit ("Plum", 98.50, 3.25),
                              Fruit ("Cherry", 56.50, 5.50),
                              Fruit ("Watermelon", 60, 1.50),
                              Fruit ("Banana", 42, .50),
                              Fruit ("Lime", 48.5, 2.50),
                              Fruit ("Lemon", 49.50, 2.50),
                              Fruit ("Cantaloupe", 60.50, 1.50),
                              Fruit ("Papaya", 80.50, 3.50),
                              Fruit ("Apricot", 64.50, 4.50),
                              Fruit ("Grape", 60.7, 2.50)};
   OnlineSuperMarket FoothillMarket = OnlineSuperMarket("Foothill Supermarket","www.foothillmarket.com");
   OnlineSuperMarket *pMarket = NULL;
   pMarket = &FoothillMarket;
   pMarket->Init(FruitArray);
   // ListAllFruits(pMarket);
// ListAllFruits(&FoothillMarket);  // this is equivalent to previous statement, function can be called in either manner
   pMarket->Run();
   return 0;
}







