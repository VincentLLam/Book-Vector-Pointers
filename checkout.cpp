// Name: Vincent Lam
// Section #3
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "person.cpp"
#include "book.cpp"

using namespace std;

void printMenu() {
    cout << "----------Library Book Rental System----------" << endl;
    cout << "1.  Book checkout" << endl;
    cout << "2.  Book return" << endl;
    cout << "3.  View all available books" << endl;
    cout << "4.  View all outstanding rentals" << endl;
    cout << "5.  View outstanding rentals for a cardholder" << endl;
    cout << "6.  Open new library card" << endl;
    cout << "7.  Close library card" << endl;
    cout << "8.  Exit system" << endl;
    cout << "Please enter a choice: ";
}

void readBooks(vector<Book*> &myBooks) {
    ifstream inFile;
    int idNum;
    string title, author, category, extra;
    inFile.open("books.txt");

    while(inFile >> idNum)
    {
      getline(inFile, extra);
      getline(inFile, title);
      getline(inFile, author);
      getline(inFile, category);
      myBooks.emplace_back(new Book (idNum, title, author, category));
    }
/*
    for (int i = 0; i < myBooks.size(); i++)
    {
      Book temp = *myBooks.at(i);
      cout << temp.getTitle() << endl;

      //cout << myBooks.at(i)->getTitle() << endl;
    }
*/
    inFile.close();
    return;
}

int readPersons(vector<Person*> &myCardHolders) {
    ifstream inFile;
    int id, acti;
    int newID = 0;
    string fName, lName, extra;
    inFile.open("persons.txt");

    while (inFile >> id >> acti >> fName >> lName)
    {
      getline(inFile, extra);
      myCardHolders.emplace_back(new Person (id, acti, fName, lName));
    }

    inFile.close();

    for (int i = 0; i < myCardHolders.size(); i++)
    {
      /*
      if (newID < (*myCardHolders.at(i)).getId())
        newID = (*myCardHolders.at(i)).getId();
*/
      if(newID < myCardHolders.at(i)->getId())
        newID = myCardHolders.at(i)->getId();
    }
    return newID + 1;
}

void readRentals(vector<Book*> &myBooks, vector<Person*> myCardHolders) {
    ifstream inFile;
    int book, Id;
    inFile.open("rentals.txt");

    while(inFile >> book >> Id)
    {
      for(int i = 0; i < myBooks.size(); i++)
      {
        if(book == myBooks.at(i)->getId())
        {
          for(int j = 0; j < myCardHolders.size(); j++)
          {
            if(Id == myCardHolders.at(j)->getId())
            {
              myBooks.at(i)->setPersonPtr(myCardHolders.at(j));
            }
          }
        }
      }
    }
    inFile.close();
    /*int test = 69859;
    for(int testCounter = 0; testCounter < myBooks.size(); testCounter++)
      if(test == myBooks.at(testCounter)->getId())
      {
        Person *testing = myBooks.at(testCounter)->getPersonPtr();
        cout << testing->fullName() << endl;
      }*/
    return;
}

int openCard(vector<Person*> &myCardHolders, int nextID) {
    string firstName = "";
    string lastName = "";
    string fullName = "";

    cout << "Please enter the first name: ";
    cin >> firstName;
    cout << "Please enter the last name: ";
    cin >> lastName;
    fullName = firstName + " " + lastName;

    for(int i = 0; i < myCardHolders.size(); i++)
    {
      if (fullName == myCardHolders.at(i)->fullName())
        {
          myCardHolders.at(i)->setActive(true);
          cout << "Card ID " << myCardHolders.at(i)->getId() << " active"
            << endl << "Cardholder: " << fullName << endl;
          return nextID;
        }
      else if(i == myCardHolders.size() - 1)
      {
        myCardHolders.push_back(new Person(nextID, true, firstName, lastName));
        cout << "Card ID " << nextID << " active" << endl
          << "Cardholder: " << fullName << endl;
        return nextID + 1;
      }
    }
}

void closeCard(vector<Person*> &myCardHolders) {
  int userPersonId = 0;
  char choice;

  cout << "Please enter the card ID: ";
  cin >> userPersonId;

  for (int i = 0; i < myCardHolders.size(); i++)
  {
    if (userPersonId == myCardHolders.at(i)->getId())
    {
      if(myCardHolders.at(i)->isActive() == true)
      {
        cout << "Cardholder: " << myCardHolders.at(i)->fullName() << endl
          << "Are you sure you want to deactivate(y/n)?";
        cin >> choice;
        switch(toupper(choice))
        {
          case 'Y':
            {
              myCardHolders.at(i)->setActive(false);
              cout << "Card ID deactivated" << endl;
            }
          case 'N':
              return;
          default:
            {
              cout << "Wrong input... Returning to main menu" << endl;
              return;
            }
        }
        return;
      }
      else
      {
        cout << "Card is already inactive active" << endl;
        return;
      }
    }
    else if (i == myCardHolders.size() - 1)
    {
      cout << "Card ID not found" << endl;
      return;
    }
  }
}

int validUserID(int inputPerson, vector<Person*> &myCardHolders) {
  for (int i = 0; i < myCardHolders.size(); i++)
  {
    if (inputPerson == myCardHolders.at(i)->getId())
    {
      if(myCardHolders.at(i)->isActive() == true)
      {
        cout << "Cardholder: " << myCardHolders.at(i)->fullName() << endl;
        return 2;
      }
      else
      {
        cout << "Card is not active" << endl;
        return 1;
      }
    }
    else if (i == myCardHolders.size() - 1)
    {
      cout << "Card ID not found" << endl;
      return 1;
    }
  }
}

int validBookID(int inputBook, vector<Book*> &myBooks) {
  for(int i = 0; i < myBooks.size(); i++)
  {
    if(inputBook == myBooks.at(i)->getId())
    {
      Person *temp = myBooks.at(i)->getPersonPtr();
      if(temp != 0)
      {
        cout << "Book is already checked out!" << endl;
        return 1;
      }
      else
      {
        cout << "Title: " << myBooks.at(i)->getTitle() << endl;
        return 2;
      }
    }
    else if (i == myBooks.size() - 1)
    {
      cout << "Book ID not found" << endl;
      return 1;
    }
  }
}

void checkoutBooks(vector<Book*> &myBooks, vector<Person*> &myCardHolders) {
  int inputPerson = 0;
  int inputBook = 0;
  int checkPerson = 0;
  int checkBook = 0;
  cout << "Please enter the card ID: ";
  cin >> inputPerson;
  checkPerson = validUserID(inputPerson, myCardHolders);
  switch(checkPerson)
  {
    case 1:
      return;
    case 2:
      cout << "Please enter the book ID: ";
      cin >> inputBook;
      checkBook = validBookID(inputBook, myBooks);
      switch(checkBook)
      {
        case 1:
          return;
        case 2:
          for(int i = 0; i < myBooks.size(); i++)
          {
            if(inputBook == myBooks.at(i)->getId())
            {
              for(int j = 0; j < myCardHolders.size(); j++)
              {
                if(inputPerson == myCardHolders.at(j)->getId())
                {
                  myBooks.at(i)->setPersonPtr(myCardHolders.at(j));
                  cout << "Rental complete" << endl;
                  return;
                }
              }
            }
          }
        default:
          return;
      }
      return;
    default:
      cout << "Looks like we made a wrong turn somewhere...";
      break;
  }
}

void returnBooks(vector<Book*> &myBooks) {
  int userBookId = 0;

  cout << "Please enter the book ID to return: ";
  cin >> userBookId;
  for (int j = 0; j < myBooks.size(); j++)
  {

  }
  for (int i = 0; i < myBooks.size(); i++)
  {
    if (userBookId == myBooks.at(i)->getId())
    {
      cout << "Title: " << myBooks.at(i)->getTitle() << endl;
      if (myBooks.at(i)->getPersonPtr() == nullptr)
      {
        cout << "Book is not checked out..." << endl;
        return;
      }
      else
      {
        myBooks.at(i)->setPersonPtr(nullptr);
        cout << "Return Completed." << endl;
        return;
      }
    }
    else if (i == myBooks.size() - 1)
    {
      cout << "Book ID not found";
      return;
    }
  }
}

void viewAvaBooks(vector<Book*> &myBooks) {
  for (int i = 0; i < myBooks.size(); i++)
  {
    if (myBooks.at(i)->getPersonPtr() == nullptr)
    {
      cout << "Book ID: " << myBooks.at(i)->getId() << endl
        << "Title: " << myBooks.at(i)->getTitle() << endl
        << "Author: " << myBooks.at(i)->getAuthor() << endl
        << "Category: " << myBooks.at(i)->getCategory() << endl << endl;
    }
    else if (i == myBooks.size() - 1)
    {
      if (myBooks.at(i)->getPersonPtr() != nullptr)
        return;
      else
      {
        cout << "No available books." << endl;
        return;
      }
    }
  }
}

void viewOutRentals(vector<Book*> &myBooks, vector<Person*> &myCardHolders) {
  int counter = 0;
  for(int i = 0; i < myBooks.size(); i++)
  {
    if(myBooks.at(i)->getPersonPtr() != nullptr)
    {
      Person *temp = myBooks.at(i)->getPersonPtr();
      cout << "Book ID: " << myBooks.at(i)->getId() << endl
        << "Title: " << myBooks.at(i)->getTitle() << endl
        << "Author: " << myBooks.at(i)->getAuthor() << endl
        << "Card ID: " << temp->getId() << endl << endl;
      counter++;
    }
  }
  if (counter == 0)
    cout << "No outstanding rentals" << endl;
}

void viewCardRental(vector<Book*> &myBooks, vector<Person*> &myCardHolders) {
  int userPersonId = 0;
  int check = 0;
  int counter = 0;

  cout << "Please enter the card number: ";
  cin >> userPersonId;

  check = validUserID(userPersonId, myCardHolders);
  switch(check)
  {
    case 1:
      return;
    case 2:
    {
      for(int i = 0; i < myBooks.size(); i++)
      {
        if (myBooks.at(i)->getPersonPtr() != nullptr)
        {
          Person *temp = myBooks.at(i)->getPersonPtr();
          if (userPersonId == temp->getId())
          {
            cout << endl <<  "Book ID:" << myBooks.at(i)->getId() << endl
              << "Title: " << myBooks.at(i)->getTitle() << endl
              << "Author: " << myBooks.at(i)->getAuthor() << endl;
            counter++;
          }
        }
      }
      if (counter == 0)
        cout << "No books currently checked out" << endl;
    }
  }
      return;
}

void updateFiles(vector<Book*> &myBooks, vector<Person*> &myCardHolders) {
  ofstream outFile;
  outFile.open("persons.txt");

  for(int i = 0; i < myCardHolders.size(); i++)
  {
    outFile << myCardHolders.at(i)->getId() << " ";
    if(myCardHolders.at(i)->isActive() == true)
      outFile << "1" << " ";
    else
      outFile << "0" << " ";
    outFile << myCardHolders.at(i)->fullName() << endl;
  }
  outFile.close();

  outFile.open("rentals.txt");
  for(int i = 0; i < myBooks.size(); i++)
  {
    if(myBooks.at(i)->getPersonPtr() != nullptr)
    {
      Person *temp = myBooks.at(i)->getPersonPtr();
      outFile << myBooks.at(i)->getId() << " " << temp->getId() << endl;
    }
  }
  outFile.close();
}

void deleteVectors(vector<Book*> &myBooks, vector<Person*> &myCardHolders) {
  for(int i = 0; i < myBooks.size(); i++)
  {
    delete myBooks.at(i);
    myBooks.at(i) = nullptr;
  }

  for(int j = 0; j < myCardHolders.size(); j++)
  {
    delete myCardHolders.at(j);
    myCardHolders.at(j) = nullptr;
  }
}

int main()
{
    vector<Book*> myBooks;
    vector<Person*> myCardHolders;
    int nextID = 0;

    readBooks(myBooks);
    nextID = readPersons(myCardHolders);
    readRentals(myBooks, myCardHolders);

    int choice;
    do
    {
        printMenu();
        cin >> choice;
        switch(choice)
        {
            case 1:
                // Book checkout
                checkoutBooks(myBooks, myCardHolders);
                break;

            case 2:
                // Book return
                returnBooks(myBooks);
                break;

            case 3:
                // View all available books
                viewAvaBooks(myBooks);
                break;

            case 4:
                // View all outstanding rentals
                viewOutRentals(myBooks, myCardHolders);
                break;

            case 5:
                // View outstanding rentals for a cardholder
                viewCardRental(myBooks, myCardHolders);
                break;

            case 6:
                // Open new library card
                nextID = openCard(myCardHolders, nextID);
                break;

            case 7:
                // Close library card
                closeCard(myCardHolders);
                break;

            case 8:
                // Must update records in files here before exiting the program
                updateFiles(myBooks, myCardHolders);
                deleteVectors(myBooks, myCardHolders);
                break;

            default:
                cout << "Invalid entry" << endl;
                break;
        }
        cout << endl;
   } while(choice != 8);
      return 0;
}
