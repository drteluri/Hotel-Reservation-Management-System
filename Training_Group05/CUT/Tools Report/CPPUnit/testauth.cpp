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

#include "Auth.h"

using namespace CppUnit;
using namespace std;

//-----------------------------------------------------------------------------

class testauth : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(testauth);
    CPPUNIT_TEST(testcheckpasswd);
    CPPUNIT_TEST(testvaliduser);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void);
    void tearDown(void);

protected:
    void testcheckpasswd(void);
    void testvaliduser(void);

private:

    Auth *mTestObj;
};

//-----------------------------------------------------------------------------


void
testauth::testcheckpasswd(void)
{
	//sunny testcases
    CPPUNIT_ASSERT(1 == mTestObj->checkpasswd("Sunitha12"));
    CPPUNIT_ASSERT(1 == mTestObj->checkpasswd("Jack123"));
    CPPUNIT_ASSERT(1 == mTestObj->checkpasswd("Chan@234"));
    CPPUNIT_ASSERT(1 == mTestObj->checkpasswd("VeronIca45"));
    CPPUNIT_ASSERT(1 == mTestObj->checkpasswd("MaX43@89"));

    //rainy testcases
    CPPUNIT_ASSERT(0== mTestObj->checkpasswd("Sunityyyyyyyyyygghhha12"));
    CPPUNIT_ASSERT(0 == mTestObj->checkpasswd("ac"));
    CPPUNIT_ASSERT(0 == mTestObj->checkpasswd("JSHDNDJSK"));
    CPPUNIT_ASSERT(0 == mTestObj->checkpasswd("123456"));
    CPPUNIT_ASSERT(0 == mTestObj->checkpasswd("Laaaaw"));

}
void
testauth::testvaliduser(void)
{
	//sunny testcases
    CPPUNIT_ASSERT(1==mTestObj->validuser("565a"));
    CPPUNIT_ASSERT(1==mTestObj->validuser("I2CIC"));
    CPPUNIT_ASSERT(1==mTestObj->validuser("a@r2it"));
    CPPUNIT_ASSERT(1==mTestObj->validuser("app12"));
    CPPUNIT_ASSERT(1==mTestObj->validuser("fo2!"));


    //rainy testcases
    CPPUNIT_ASSERT(0==mTestObj->validuser("asr"));
    CPPUNIT_ASSERT(0==mTestObj->validuser("assert"));
    CPPUNIT_ASSERT(0==mTestObj->validuser("assertion"));
    CPPUNIT_ASSERT(0==mTestObj->validuser(""));
    CPPUNIT_ASSERT(0==mTestObj->validuser("ctb@r%"));
}


void testauth::setUp(void)
{
   mTestObj = new Auth();
}

void testauth::tearDown(void)
{
    delete mTestObj;
}

//-----------------------------------------------------------------------------

CPPUNIT_TEST_SUITE_REGISTRATION( testauth );

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
    ofstream xmlFileOut("AuthResults.xml");
    XmlOutputter xmlOut(&collectedresults, xmlFileOut);
    xmlOut.write();

    // return 0 if tests were successful
    return collectedresults.wasSuccessful() ? 0 : 1;
}


