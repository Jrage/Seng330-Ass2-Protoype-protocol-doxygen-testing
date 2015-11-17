// VSProj.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <stdlib.h>     //for using the function sleep
using namespace std;

/*! Equipment is the base class for several subclasses. */
class Equipment {
public:
	virtual Equipment* clone() = 0;        /*!< All subclasses of Equipment must have clone functionality */
	virtual void printEquipment() = 0;    /*!< prints the instance type and instance name */
	virtual string getEquipmentType() { return "default"; };	/*!< returns the instance type as string*/
 
	/*!< serializes the data inside the object */
	rapidjson::Document serialize(){
		rapidjson::Document d;
		d.SetObject();
		rapidjson::Value str;

		str.SetString(name.c_str(), name.length(), d.GetAllocator());
		d.AddMember("name", str, d.GetAllocator());
		str.SetString(this->getEquipmentType().c_str(), getEquipmentType().length(), d.GetAllocator());
		d.AddMember("instanceType", str, d.GetAllocator());

		return d;
	}
	string name = "Unnamed";    /*!< Name of equipments instance, by default equipment is unnamed */
};


	/*! Legpress is a subclass of Equipment. */
	class Legpress : public Equipment {
	public:
		Equipment*   clone() { return new Legpress; };/*!< Returns a clone */

		string getEquipmentType() { return "LegPress"; }

		void printEquipment() {
			cout << "Legpress: " << name << "\n";
		}
	};
	class Benchpress : public Equipment {
	public:
		Equipment*   clone() { return new Benchpress; };  /*!< Returns a clone */
		string getEquipmentType() { return "BenchPress"; }

		void printEquipment() {
			cout << "Benchpress: " << name << "\n";
		}
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

		static Equipment* prototypes[3]; /*!< Contains the registered prototyped classes. */
	};


	/*! Factory's registered prototypes*/
	Equipment* Factory::prototypes[] = {
		0, new Legpress, new Benchpress
	};

	Equipment* Factory::make_Equipment(int choice, string name) {
		Equipment * item = prototypes[choice]->clone();
		item->name = name;
		return item;
	}


	//! Reads the given list of equipment, serialises the objects and saves to the given file
	/*!
	\param list of equipment to become serialized data
	\param file name of file to save the serialized data to
	*/
	void serializeData(vector <Equipment*> list, string file) {
		rapidjson::Document d;
		d.SetObject();
		rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

		rapidjson::Value array(rapidjson::kArrayType);
		for (int i = 0; i < list.size(); ++i) {
			rapidjson::Value val(list[i]->serialize(), allocator); // need to copy
			array.PushBack(val, allocator) ;
		}

		d.AddMember("equipment", array, allocator);

		rapidjson::StringBuffer buf;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
		d.Accept(writer);
		cout << "File Saved\n";

		ofstream myfile;
		myfile.open(file);
		myfile << buf.GetString();
		myfile.close();
	}

	//! Reads the given file of serialized data, creates the objects and returns them as if they have been created manually
	/*!
	\param file name of file with the serialized data
	\return the new list of prototyped object created from the serialized data.
	*/
	vector <Equipment*> loadSerializedData(string file) {

		ifstream inFile;
		inFile.open(file);//open the input file

		stringstream strStream;
		strStream << inFile.rdbuf();//read the file
		string str = strStream.str();//str holds the content of the file
		inFile.close();
	

		rapidjson::Document d;
		d.Parse(str.c_str());

		vector<Equipment*> list;

		const rapidjson::Value& b = d["equipment"];

		for (rapidjson::SizeType i = 0; i < b.Size(); i++)
		{
			const rapidjson::Value& c = b[i];
			//printf("%s, %s \n", c["name"].GetString(), c["instanceType"].GetString());
			int choice;
			if (strcmp( c["instanceType"].GetString() , "LegPress") == 0)
				choice = 1;
			else if (strcmp(c["instanceType"].GetString() , "BenchPress") == 0)
				choice = 2;
			else 
				continue;
			
			list.push_back(Factory::make_Equipment(choice, c["name"].GetString()));
		}

		cout << "File Loaded\n";
		return list;
	}


	void run() {

		vector<Equipment*> list;
		int             choice;


		while (true) {
			cout << "\n(0) finish,\n(1) Add Legpress,\n(2) Add Benchpress,\n(3) Save Serialized Data,\n(4) Load Serialized Data,\n(5) print Equipment\nenter( 0,1,2,3,4,5) :";
			cin >> choice;

			//Cancel
			if (choice == 0)
				break;

			//Save
			else if (choice == 3)
				serializeData(list, "equipment.json");

			//Load
			else if (choice == 4)
				list = loadSerializedData("equipment.json");

			//Print
			else if (choice == 5) {
				cout << "\n";
				for (int i = 0; i < list.size(); ++i) {
					list[i]->printEquipment();
				}

				// Manually add
			}
			else {
				string name;
				cout << "\nEnter Instance Name (no spaces): ";
				cin >> name;

				list.push_back(Factory::make_Equipment(choice, name));
			}
		}

		for (int i = 0; i < list.size(); ++i)
			delete list[i];

		system("PAUSE");
	}


	int main() {
		run();
	}

