#include "stdafx.h"
#include "CppUnitTest.h"
#include "../VSProj/VSProj.cpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(BenchPressSerialization)
		{
			// TODO: Your test code here
			Benchpress* machine1 = new Benchpress;
			machine1->name = "test1";

			rapidjson::Document d = machine1->serialize();
			rapidjson::StringBuffer buf;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
			d.Accept(writer);
			
			Assert::AreEqual(buf.GetString(), "{\"name\":\"test1\",\"instanceType\":\"BenchPress\"}");

		}

		TEST_METHOD(LegPressSerialization)
		{
			// TODO: Your test code here
			Legpress* machine1 = new Legpress;
			machine1->name = "test1";

			rapidjson::Document d = machine1->serialize();
			rapidjson::StringBuffer buf;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
			d.Accept(writer);

			Assert::AreEqual(buf.GetString(), "{\"name\":\"test1\",\"instanceType\":\"LegPress\"}");

		}



		TEST_METHOD(DeserialTest)
		{
			vector <Equipment*> loaded = loadSerializedData("UnitTest.json");

			vector <Equipment*> list;
			list.push_back(Factory::make_Equipment(1, "Machine1"));
			list.push_back(Factory::make_Equipment(2, "Machine1"));
			list.push_back(Factory::make_Equipment(1, "Machine2"));
			list.push_back(Factory::make_Equipment(1, "Machine3"));
			list.push_back(Factory::make_Equipment(2, "Machine2"));
			list.push_back(Factory::make_Equipment(2, "Machine3"));

			Assert::AreEqual(loaded.size(), list.size());

			for (int i = 0; i < loaded.size(); i++) {
				Assert::AreEqual(loaded[i]->getEquipmentType(), list[i]->getEquipmentType());
				Assert::AreEqual(loaded[i]->name, list[i]->name);
			}
		}

		
		TEST_METHOD(DeserialSerializeTest)
		{
			vector <Equipment*> list;
			list.push_back(Factory::make_Equipment(1, "Machine1"));
			list.push_back(Factory::make_Equipment(2, "Machine1"));
			list.push_back(Factory::make_Equipment(1, "Machine2"));
			list.push_back(Factory::make_Equipment(1, "Machine3"));
			list.push_back(Factory::make_Equipment(2, "Machine2"));
			list.push_back(Factory::make_Equipment(2, "Machine3"));

			
			serializeData(list, "UnitTest2.json");
			vector <Equipment*> loaded = loadSerializedData("UnitTest2.json");

			Assert::AreEqual(loaded.size(), list.size());

			for (int i = 0; i < loaded.size(); i++) {
				Assert::AreEqual(loaded[i]->getEquipmentType(), list[i]->getEquipmentType());
				Assert::AreEqual(loaded[i]->name, list[i]->name);
			}
		}
	};
}