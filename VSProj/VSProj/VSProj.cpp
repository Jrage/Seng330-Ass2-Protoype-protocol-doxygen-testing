// VSProj.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>     //for using the function sleep
using namespace std;

/*! Equipment is the base class for several subclasses. */
class Equipment {
public:
	virtual Equipment* clone() = 0;        /*!< All subclasses of Equipment must have clone functionality */
	virtual void printEquipment() = 0;    /*!< prints the instance type and instance name */

	string name = "Unnamed";    /*!< Name of equipments instance, by default equipment is unnamed */
};


/*! Factory, holds a series of registered prototype classes. These classes are all subclasses of Equipment. Factory is used to generate new instances with clone() */
class Factory {
	public:

		//! A prototype creation Method taking two arguments and returning an class of Equipment* type.
		/*!
		\param choice an integer argument Indicating which registered prototype to use.
		\param name used to set the instance name of the newly generated object.
		\return the new prototyped object, with instance name set.
		*/
		static Equipment* make_Equipment(int choice, string name); /*!< All subclasses of Equipment must have clone functionality */


	private:

		static Equipment* s_prototypes[3]; /*!< Contains the registered prototyped classes. */
	};

	int main() {
		vector<Equipment*> roles;
		int             choice;

		while (true) {
			cout << "Legpress(1) Benchpress(2) Go(0): ";
			cin >> choice;
			if (choice == 0)
				break;
			else {
				string name;
				cout << "Enter Instance Name: ";
				cin >> name;

				roles.push_back(
					Factory::make_Equipment(choice, name));
			}
		}

		for (int i = 0; i < roles.size(); ++i) {
			roles[i]->printEquipment();
			cout << roles[i]->name;
		}
		for (int i = 0; i < roles.size(); ++i)
			delete roles[i];

		system("PAUSE");
	}

	/*! Legpress is a subclass of Equipment. */
	class Legpress : public Equipment {
	public:
		Equipment*   clone() { return new Legpress; } /*!< Returns a clone */


		void printEquipment() {
			cout << "Legpress: " << name << "\n";
		}
	};
	class Benchpress : public Equipment {
	public:
		Equipment*   clone() { return new Benchpress; }  /*!< Returns a clone */
		void printEquipment() {
			cout << "Benchpress: " << name << "\n";
		}
	};

	Equipment* Factory::s_prototypes[] = {
		0, new Legpress, new Benchpress
	};
	Equipment* Factory::make_Equipment(int choice, string name) {
		Equipment * item = s_prototypes[choice]->clone();
		item->name = name;
		return item;
	}

