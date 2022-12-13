#include <bits/stdc++.h>
#include <list>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <netinet/in.h>

#include "hotel_room.h"

using namespace CppUnit;
using namespace std;

//-----------------------------------------------------------------------------

class Testhotel : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(Testhotel);
    CPPUNIT_TEST(testinit);
    CPPUNIT_TEST(testreserveRoom);
    CPPUNIT_TEST(testunreserveRoom);
    CPPUNIT_TEST(testAvailableRoom);
    CPPUNIT_TEST(testAvailableRoomWithPrice);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void);
    void tearDown(void);

protected:
    void testinit(void);
    void testreserveRoom(void);
    void testunreserveRoom(void);
    void testAvailableRoom(void);
    void testAvailableRoomWithPrice(void);

private:

      hotel_room  *mTestObj;
      hotel_rooms *mTestObj1;
};

//-----------------------------------------------------------------------------


void
Testhotel::testinit(void)
{
	//adding customer
    //sunny testcases
    CPPUNIT_ASSERT(1 == mTestObj1->init("hotelrooms.csv"));

    //rainy testcases
}
void
Testhotel::testreserveRoom(void)
{
	//adding customer
    //sunny testcases
     CPPUNIT_ASSERT(true == mTestObj1->reserveRoom(101));

    //rainy testcases
    CPPUNIT_ASSERT(false == mTestObj1->reserveRoom(200));

}
void
Testhotel::testunreserveRoom(void)
{
	//adding customer
    //sunny testcases
    CPPUNIT_ASSERT(true == mTestObj1->unreserveRoom(101));

    //rainy testcases
}
void
Testhotel::testAvailableRoom(void)
{
	//adding customer
    //sunny testcases
    CPPUNIT_ASSERT(true == mTestObj1->getAvailableRoom("AC"));

    //rainy testcases
}
void
Testhotel::testAvailableRoomWithPrice(void)
{
	//adding customer
    //sunny testcases
    CPPUNIT_ASSERT(true == mTestObj1->getAvailableRoomWithPrice("AC",3));

    //rainy testcases
}
void Testhotel::setUp(void)
{
    mTestObj = new hotel_room();
    mTestObj1 = new hotel_rooms();
}

void Testhotel::tearDown(void)
{
    delete mTestObj;
    delete mTestObj1;
}

//-----------------------------------------------------------------------------

CPPUNIT_TEST_SUITE_REGISTRATION( Testhotel );

int main(int argc, char* argv[])
{
    // informs test-listener about testresults
    CPPUNIT_NS::TestResult testresult;

    // register listener for collecting the test-results
    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    // register listener for per-test progress output
    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener (&progress);

    // insert test-suite at test-runner by registry
    CPPUNIT_NS::TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest ());
    testrunner.run(testresult);

    // output results in compiler-format
    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write ();

    // Output XML for Jenkins CPPunit plugin
    ofstream xmlFileOut("Testhotel.xml");
    XmlOutputter xmlOut(&collectedresults, xmlFileOut);
    xmlOut.write();

    // return 0 if tests were successful
    return collectedresults.wasSuccessful() ? 0 : 1;
}


