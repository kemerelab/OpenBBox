
#include <unittest++/UnitTest++.h>

#include <../src/receiverbehaviortcp.h>

TEST(MyTest) {
    ReceiverBehaviorTCP * r = new ReceiverBehaviorTCP(3000);
    CHECK(r);
    delete r;
}

int main()
{
    return UnitTest::RunAllTests();
}


