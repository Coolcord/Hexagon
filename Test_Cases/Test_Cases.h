#ifndef TEST_CASES_H
#define TEST_CASES_H

class Hexagon_Interface;

class Test_Cases {
public:
    Test_Cases(Hexagon_Interface *hexagonPlugin);
    ~Test_Cases() {}
    void Run_Test_Cases();

    bool Create_Test_Case_1(); //standard test
    bool Create_Test_Case_2(); //no comments
    bool Create_Test_Case_3(); //no checksum
    bool Create_Test_Case_4(); //compareSize 1 vs compareSize 5
    bool Apply_Test_Case_1(); //standard apply
    bool Apply_Test_Case_2(); //no comments
    bool Apply_Test_Case_3(); //no checksum
    bool Apply_Test_Case_4(); //invalid checksum
    bool Apply_Test_Case_5(); //parse error
    bool Apply_Test_Case_6(); //invalid data in offset
    bool Apply_Test_Case_7(); //invalid data in value
    bool Convert_To_Qt_Code_Test_Case_1(); //standard convert
    bool Convert_To_Qt_Code_Test_Case_2(); //no comments
    bool Convert_To_Qt_Code_Test_Case_3(); //extra code
    bool Convert_To_Hexp_Test_Case_1(); //standard convert
    bool Convert_To_Hexp_Test_Case_2(); //no comments
    bool Check_For_Conflicts_Test_Case_1(); //no conflicts
    bool Check_For_Conflicts_Test_Case_2(); //1 conflict out of 3
    bool Check_For_Conflicts_Test_Case_3(); //3 conflicts out of 3

private:
    Hexagon_Interface *hexagonPlugin;
};

#endif // TEST_CASES_H
