#include "Test_Cases.h"
#include "../Hexagon/Hexagon_Interface.h"
#include "Data_Generator.h"
#include <assert.h>
#include <QDebug>

Test_Cases::Test_Cases(Hexagon_Interface *hexagonPlugin, const QString &applicationLocation) {
    this->hexagonPlugin = hexagonPlugin;
    this->applicationLocation = applicationLocation;
}

void Test_Cases::Run_Test_Cases() {
    int passed = 0;
    int failed = 0;
    Data_Generator dataGenerator(this->applicationLocation);
    if (!dataGenerator.Generate_Data()) {
        qDebug() << "Unable to generate test case data!";
        return;
    }

    qDebug() << "Running test cases...";
    qDebug() << "";

    if (this->Create_Test_Case_1()) {
        ++passed;
        qDebug() << "Create_Test_Case_1() passed!";
    } else {
        ++failed;
        qDebug() << "Create_Test_Case_1() failed!";
    }

    if (this->Create_Test_Case_2()) {
        ++passed;
        qDebug() << "Create_Test_Case_2() passed!";
    } else {
        ++failed;
        qDebug() << "Create_Test_Case_2() failed!";
    }

    if (this->Create_Test_Case_3()) {
        ++passed;
        qDebug() << "Create_Test_Case_3() passed!";
    } else {
        ++failed;
        qDebug() << "Create_Test_Case_3() failed!";
    }

    if (this->Create_Test_Case_4()) {
        ++passed;
        qDebug() << "Create_Test_Case_4() passed!";
    } else {
        ++failed;
        qDebug() << "Create_Test_Case_4() failed!";
    }

    if (this->Apply_Test_Case_1()) {
        ++passed;
        qDebug() << "Apply_Test_Case_1() passed!";
    } else {
        ++failed;
        qDebug() << "Apply_Test_Case_1() failed!";
    }

    if (this->Apply_Test_Case_2()) {
        ++passed;
        qDebug() << "Apply_Test_Case_2() passed!";
    } else {
        ++failed;
        qDebug() << "Apply_Test_Case_2() failed!";
    }

    if (this->Apply_Test_Case_3()) {
        ++passed;
        qDebug() << "Apply_Test_Case_3() passed!";
    } else {
        ++failed;
        qDebug() << "Apply_Test_Case_3() failed!";
    }

    if (this->Apply_Test_Case_4()) {
        ++passed;
        qDebug() << "Apply_Test_Case_4() passed!";
    } else {
        ++failed;
        qDebug() << "Apply_Test_Case_4() failed!";
    }

    if (this->Apply_Test_Case_5()) {
        ++passed;
        qDebug() << "Apply_Test_Case_5() passed!";
    } else {
        ++failed;
        qDebug() << "Apply_Test_Case_5() failed!";
    }

    if (this->Apply_Test_Case_6()) {
        ++passed;
        qDebug() << "Apply_Test_Case_6() passed!";
    } else {
        ++failed;
        qDebug() << "Apply_Test_Case_6() failed!";
    }

    if (this->Apply_Test_Case_7()) {
        ++passed;
        qDebug() << "Apply_Test_Case_7() passed!";
    } else {
        ++failed;
        qDebug() << "Apply_Test_Case_7() failed!";
    }

    if (this->Convert_To_Qt_Code_Test_Case_1()) {
        ++passed;
        qDebug() << "Convert_To_Qt_Code_Test_Case_1() passed!";
    } else {
        ++failed;
        qDebug() << "Convert_To_Qt_Code_Test_Case_1() failed!";
    }

    if (this->Convert_To_Qt_Code_Test_Case_2()) {
        ++passed;
        qDebug() << "Convert_To_Qt_Code_Test_Case_2() passed!";
    } else {
        ++failed;
        qDebug() << "Convert_To_Qt_Code_Test_Case_2() failed!";
    }

    if (this->Convert_To_Qt_Code_Test_Case_3()) {
        ++passed;
        qDebug() << "Convert_To_Qt_Code_Test_Case_3() passed!";
    } else {
        ++failed;
        qDebug() << "Convert_To_Qt_Code_Test_Case_3() failed!";
    }

    if (this->Convert_To_Hexp_Test_Case_1()) {
        ++passed;
        qDebug() << "Convert_To_Hexp_Test_Case_1() passed!";
    } else {
        ++failed;
        qDebug() << "Convert_To_Hexp_Test_Case_1() failed!";
    }

    if (this->Convert_To_Hexp_Test_Case_2()) {
        ++passed;
        qDebug() << "Convert_To_Hexp_Test_Case_2() passed!";
    } else {
        ++failed;
        qDebug() << "Convert_To_Hexp_Test_Case_2() failed!";
    }

    if (this->Check_For_Conflicts_Test_Case_1()) {
        ++passed;
        qDebug() << "Check_For_Conflicts_Test_Case_1() passed!";
    } else {
        ++failed;
        qDebug() << "Check_For_Conflicts_Test_Case_1() failed!";
    }

    if (this->Check_For_Conflicts_Test_Case_2()) {
        ++passed;
        qDebug() << "Check_For_Conflicts_Test_Case_2() passed!";
    } else {
        ++failed;
        qDebug() << "Check_For_Conflicts_Test_Case_2() failed!";
    }

    if (this->Check_For_Conflicts_Test_Case_3()) {
        ++passed;
        qDebug() << "Check_For_Conflicts_Test_Case_3() passed!";
    } else {
        ++failed;
        qDebug() << "Check_For_Conflicts_Test_Case_3() failed!";
    }

    qDebug() << "";
    qDebug().noquote() << "Passed: " << QString::number(passed);
    qDebug().noquote() << "Failed: " << QString::number(failed);
    qDebug().noquote() << " Total: " << QString::number(passed+failed);

    if (!dataGenerator.Clean_Up()) qDebug() << "Unable to clean up generated test case data!";
}

bool Test_Cases::Create_Test_Case_1() {
    return false;
}

bool Test_Cases::Create_Test_Case_2() {
    return false;
}

bool Test_Cases::Create_Test_Case_3() {
    return false;
}

bool Test_Cases::Create_Test_Case_4() {
    return false;
}

bool Test_Cases::Apply_Test_Case_1() {
    return false;
}

bool Test_Cases::Apply_Test_Case_2() {
    return false;
}

bool Test_Cases::Apply_Test_Case_3() {
    return false;
}

bool Test_Cases::Apply_Test_Case_4() {
    return false;
}

bool Test_Cases::Apply_Test_Case_5() {
    return false;
}

bool Test_Cases::Apply_Test_Case_6() {
    return false;
}

bool Test_Cases::Apply_Test_Case_7() {
    return false;
}

bool Test_Cases::Convert_To_Qt_Code_Test_Case_1() {
    return false;
}

bool Test_Cases::Convert_To_Qt_Code_Test_Case_2() {
    return false;
}

bool Test_Cases::Convert_To_Qt_Code_Test_Case_3() {
    return false;
}

bool Test_Cases::Convert_To_Hexp_Test_Case_1() {
    return false;
}

bool Test_Cases::Convert_To_Hexp_Test_Case_2() {
    return false;
}

bool Test_Cases::Check_For_Conflicts_Test_Case_1() {
    return false;
}

bool Test_Cases::Check_For_Conflicts_Test_Case_2() {
    return false;
}

bool Test_Cases::Check_For_Conflicts_Test_Case_3() {
    return false;
}
