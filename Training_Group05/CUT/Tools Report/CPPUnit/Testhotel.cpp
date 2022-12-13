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

#include "hotel.h"

using namespace CppUnit;
using namespace std;

//-----------------------------------------------------------------------------

class Testhotel : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(Testhotel);
    CPPUNIT_TEST(testroom);
    CPPUNIT_TEST(testrooms);
    CPPUNIT_TEST(testcancel);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void);
    void tearDown(void);

protected:
    void testroom(void);
    void testrooms(void);
    void testcancel(void);

private:

      hotel  *mTestObj;
};

//-----------------------------------------------------------------------------


void
Testhotel::testroom(void)
{
	//adding customer
    //sunny testcases
    CPPUNIT_ASSERT(1 == mTestObj->setroom("ac",2000,1));
    CPPUNIT_ASSERT(1 == mTestObj->setroom("non_ac",1000,3));
    CPPUNIT_ASSERT(1 == mTestObj->setroom("non_ac",1000,5));
    CPPUNIT_ASSERT(1 == mTestObj->setroom("delux",3000,7));
    CPPUNIT_ASSERT(1 == mTestObj->setroom("suite",4000,10));

    //rainy testcases
    CPPUNIT_ASSERT(0 == mTestObj->setroom("Non_ac",1000,13));
    CPPUNIT_ASSERT(0 == mTestObj->setroom("ac",2000,12));
    CPPUNIT_ASSERT(0 == mTestObj->setroom("delux",3000,15));
    CPPUNIT_ASSERT(0 == mTestObj->setroom("suite",4000,16));
    CPPUNIT_ASSERT(0 == mTestObj->setroom("suite",4000,19));
}
void
Testhotel::testrooms(void)
{
	//Rooms occupied
    CPPUNIT_ASSERT(1 == mTestObj->rooms());
    CPPUNIT_ASSERT(1 == mTestObj->rooms());
    CPPUNIT_ASSERT(1 == mTestObj->rooms());
    CPPUNIT_ASSERT(1 == mTestObj->rooms());
    CPPUNIT_ASSERT(1 == mTestObj->rooms());
}
void
Testhotel::testcancel(void)
{
	//cancelling Reservation IF USER IS PRESENT
	//sunny testcases
	
    CPPUNIT_ASSERT(1 == mTestObj->cancel(1));


    //USER IS NOT PRESENT
    //rainy testcases
    CPPUNIT_ASSERT(0 == mTestObj->cancel(2));
    CPPUNIT_ASSERT(0 == mTestObj->cancel(4));
    CPPUNIT_ASSERT(0 == mTestObj->cancel(6));
    CPPUNIT_ASSERT(0 == mTestObj->cancel(8));
    CPPUNIT_ASSERT(0 == mTestObj->cancel(13));
}

void Testhotel::setUp(void)
{
    mTestObj = new hotel();
}

void Testhotel::tearDown(void)
{
    delete mTestObj;
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


