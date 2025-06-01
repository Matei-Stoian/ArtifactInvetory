#include <QtTest>

// Include test class headers
#include "TestArtifact.h"
#include "TestRepository.h"
#include "TestFilters.h"
#include "TestController.h"

int main(int argc, char *argv[])
{
    int status = 0;

    {
        TestArtifact tc;
        status |= QTest::qExec(&tc, argc, argv);
    }

    {
        TestRepository tc;
        status |= QTest::qExec(&tc, argc, argv);
    }

    {
        TestFilters tc;
        status |= QTest::qExec(&tc, argc, argv);
    }

    {
        TestController tc;
        status |= QTest::qExec(&tc, argc, argv);
    }

    return status;
}
