#include <gtest/gtest.h>
#include "mos6502.h"

class AddSubWithCarryTest : public testing::Test {
public:
    m6502::Mem mem;
    m6502::CPU cpu;

    virtual void SetUp() {
        cpu.Reset(mem);
    }

    virtual void TearDown() {}

    void ExpectUnaffectedRegisters(m6502::CPU CPUBefore) {
        EXPECT_EQ(CPUBefore.Flag.I, cpu.Flag.I);
        EXPECT_EQ(CPUBefore.Flag.D, cpu.Flag.D);
        EXPECT_EQ(CPUBefore.Flag.B, cpu.Flag.B);
    }

    struct ADCTestData {
        bool Carry;
        m6502::Byte A;
        m6502::Byte Operand;
        m6502::Byte Answer;

        bool ExpectC;
        bool ExpectZ;
        bool ExpectN;
        bool ExpectV;
    };

    enum class EOperation	{
        Add, Subtract
    };

    void TestSBCAbsolute(ADCTestData Test) {		
        TestADCAbsolute(Test, EOperation::Subtract);
    }

    void TestADCAbsolute(ADCTestData Test, 
                         EOperation Operation = EOperation::Add) {
        // given:
        using namespace m6502;
        cpu.Reset(0xFF00, mem);
        cpu.Flag.C = Test.Carry;
        cpu.A = Test.A;
        cpu.Flag.Z = !Test.ExpectZ;
        cpu.Flag.N = !Test.ExpectN;
        cpu.Flag.V = !Test.ExpectV;
        mem[0xFF00] = (Operation == EOperation::Add) ? 
                  CPU::INS_ADC_ABS : CPU::INS_SBC_ABS;
        mem[0xFF01] = 0x00;
        mem[0xFF02] = 0x80;
        mem[0x8000] = Test.Operand;
        constexpr s32 EXPECTED_CYCLES = 4;
        CPU CPUCopy = cpu;

        // when:
        const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

        // then:
        EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
        EXPECT_EQ(cpu.A, Test.Answer);
        EXPECT_EQ(cpu.Flag.C, Test.ExpectC);
        EXPECT_EQ(cpu.Flag.Z, Test.ExpectZ);
        EXPECT_EQ(cpu.Flag.N, Test.ExpectN);
        EXPECT_EQ(cpu.Flag.V, Test.ExpectV);
        ExpectUnaffectedRegisters(CPUCopy);
    }

    void TestSBCAbsoluteX(ADCTestData Test)	{
        TestADCAbsoluteX(Test, EOperation::Subtract);
    }

    void TestADCAbsoluteX(ADCTestData Test,
                          EOperation Operation = EOperation::Add) {
        // given:
        using namespace m6502;
        cpu.Reset(0xFF00, mem);
        cpu.Flag.C = Test.Carry;
        cpu.X = 0x10;
        cpu.A = Test.A;
        cpu.Flag.Z = !Test.ExpectZ;
        cpu.Flag.N = !Test.ExpectN;
        cpu.Flag.V = !Test.ExpectV;
        mem[0xFF00] = (Operation == EOperation::Add) ?
                  CPU::INS_ADC_ABSX : CPU::INS_SBC_ABSX;
        mem[0xFF01] = 0x00;
        mem[0xFF02] = 0x80;
        mem[0x8000+0x10] = Test.Operand;
        constexpr s32 EXPECTED_CYCLES = 4;
        CPU CPUCopy = cpu;

        // when:
        const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

        // then:
        EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
        EXPECT_EQ(cpu.A, Test.Answer);
        EXPECT_EQ(cpu.Flag.C, Test.ExpectC);
        EXPECT_EQ(cpu.Flag.Z, Test.ExpectZ);
        EXPECT_EQ(cpu.Flag.N, Test.ExpectN);
        EXPECT_EQ(cpu.Flag.V, Test.ExpectV);
        ExpectUnaffectedRegisters(CPUCopy);
    }

    void TestSBCAbsoluteY(ADCTestData Test)	{
        TestADCAbsoluteY(Test, EOperation::Subtract);
    }

    void TestADCAbsoluteY( ADCTestData Test,
        EOperation Operation = EOperation::Add )
    {
        // given:
        using namespace m6502;
        cpu.Reset(0xFF00, mem);
        cpu.Flag.C = Test.Carry;
        cpu.Y = 0x10;
        cpu.A = Test.A;
        cpu.Flag.Z = !Test.ExpectZ;
        cpu.Flag.N = !Test.ExpectN;
        cpu.Flag.V = !Test.ExpectV;
        mem[0xFF00] =
            (Operation == EOperation::Add) ?
            CPU::INS_ADC_ABSY : CPU::INS_SBC_ABSY;
        mem[0xFF01] = 0x00;
        mem[0xFF02] = 0x80;
        mem[0x8000 + 0x10] = Test.Operand;
        constexpr s32 EXPECTED_CYCLES = 4;
        CPU CPUCopy = cpu;

        // when:
        const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

        // then:
        EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
        EXPECT_EQ(cpu.A, Test.Answer);
        EXPECT_EQ(cpu.Flag.C, Test.ExpectC);
        EXPECT_EQ(cpu.Flag.Z, Test.ExpectZ);
        EXPECT_EQ(cpu.Flag.N, Test.ExpectN);
        EXPECT_EQ(cpu.Flag.V, Test.ExpectV);
        ExpectUnaffectedRegisters(CPUCopy);
    }

    void TestSBCImmediate(ADCTestData Test)	{
        TestADCImmediate(Test, EOperation::Subtract);
    }

    void TestADCImmediate(ADCTestData Test, 
                        EOperation Operation = EOperation::Add)	{
        // given:
        using namespace m6502;
        cpu.Reset(0xFF00, mem);
        cpu.Flag.C = Test.Carry;
        cpu.A = Test.A;
        cpu.Flag.Z = !Test.ExpectZ;
        cpu.Flag.N = !Test.ExpectN;
        cpu.Flag.V = !Test.ExpectV;
        mem[0xFF00] = (Operation == EOperation::Add) ?
                      CPU::INS_ADC : CPU::INS_SBC;
        mem[0xFF01] = Test.Operand;
        constexpr s32 EXPECTED_CYCLES = 2;
        CPU CPUCopy = cpu;

        // when:
        const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

        // then:
        EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
        EXPECT_EQ(cpu.A, Test.Answer);
        EXPECT_EQ(cpu.Flag.C, Test.ExpectC);
        EXPECT_EQ(cpu.Flag.Z, Test.ExpectZ);
        EXPECT_EQ(cpu.Flag.N, Test.ExpectN);
        EXPECT_EQ(cpu.Flag.V, Test.ExpectV);
        ExpectUnaffectedRegisters(CPUCopy);
    }

    void TestSBCZeroPage(ADCTestData Test) {
        TestADCZeroPage(Test, EOperation::Subtract);
    }

    void TestADCZeroPage(ADCTestData Test,
                       EOperation Operation = EOperation::Add) {
        // given:
        using namespace m6502;
        cpu.Reset(0xFF00, mem);
        cpu.Flag.C = Test.Carry;
        cpu.A = Test.A;
        cpu.Flag.Z = !Test.ExpectZ;
        cpu.Flag.N = !Test.ExpectN;
        cpu.Flag.V = !Test.ExpectV;
        mem[0xFF00] = (Operation == EOperation::Add) ?
                  CPU::INS_ADC_ZP : CPU::INS_SBC_ZP;
        mem[0xFF01] = 0x42;
        mem[0x0042] = Test.Operand;
        constexpr s32 EXPECTED_CYCLES = 3;
        CPU CPUCopy = cpu;

        // when:
        const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

        // then:
        EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
        EXPECT_EQ(cpu.A, Test.Answer);
        EXPECT_EQ(cpu.Flag.C, Test.ExpectC);
        EXPECT_EQ(cpu.Flag.Z, Test.ExpectZ);
        EXPECT_EQ(cpu.Flag.N, Test.ExpectN);
        EXPECT_EQ(cpu.Flag.V, Test.ExpectV);
        ExpectUnaffectedRegisters(CPUCopy);
    }

    void TestSBCZeroPageX(ADCTestData Test)	{
        TestADCZeroPageX(Test, EOperation::Subtract);
    }

    void TestADCZeroPageX(ADCTestData Test, 
                          EOperation Operation = EOperation::Add)	{
        // given:
        using namespace m6502;
        cpu.Reset(0xFF00, mem);
        cpu.Flag.C = Test.Carry;
        cpu.X = 0x10;
        cpu.A = Test.A;
        cpu.Flag.Z = !Test.ExpectZ;
        cpu.Flag.N = !Test.ExpectN;
        cpu.Flag.V = !Test.ExpectV;
        mem[0xFF00] = (Operation == EOperation::Add) ?
                      CPU::INS_ADC_ZPX : CPU::INS_SBC_ZPX;
        mem[0xFF01] = 0x42;
        mem[0x0042+0x10] = Test.Operand;
        constexpr s32 EXPECTED_CYCLES = 4;
        CPU CPUCopy = cpu;

        // when:
        const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

        // then:
        EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
        EXPECT_EQ(cpu.A, Test.Answer);
        EXPECT_EQ(cpu.Flag.C, Test.ExpectC);
        EXPECT_EQ(cpu.Flag.Z, Test.ExpectZ);
        EXPECT_EQ(cpu.Flag.N, Test.ExpectN);
        EXPECT_EQ(cpu.Flag.V, Test.ExpectV);
        ExpectUnaffectedRegisters(CPUCopy);
    }

    void TestSBCIndirectX(ADCTestData Test)	{
        TestADCIndirectX(Test, EOperation::Subtract);
    }

    void TestADCIndirectX(ADCTestData Test, 
                          EOperation Operation = EOperation::Add)	{
        // given:
        using namespace m6502;
        cpu.Reset(0xFF00, mem);
        cpu.Flag.C = Test.Carry;
        cpu.X = 0x04;
        cpu.A = Test.A;
        cpu.Flag.Z = !Test.ExpectZ;
        cpu.Flag.N = !Test.ExpectN;
        cpu.Flag.V = !Test.ExpectV;
        mem[0xFF00] =	(Operation == EOperation::Add) ?
                      CPU::INS_ADC_INDX : CPU::INS_SBC_INDX;
        mem[0xFF01] = 0x02;
        mem[0x0006] = 0x00;	//0x2 + 0x4
        mem[0x0007] = 0x80;
        mem[0x8000] = Test.Operand;
        constexpr s32 EXPECTED_CYCLES = 6;
        CPU CPUCopy = cpu;

        // when:
        const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

        // then:
        EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
        EXPECT_EQ(cpu.A, Test.Answer);
        EXPECT_EQ(cpu.Flag.C, Test.ExpectC);
        EXPECT_EQ(cpu.Flag.Z, Test.ExpectZ);
        EXPECT_EQ(cpu.Flag.N, Test.ExpectN);
        EXPECT_EQ(cpu.Flag.V, Test.ExpectV);
        ExpectUnaffectedRegisters(CPUCopy);
    }

    void TestSBCIndirectY(ADCTestData Test)	{
        TestADCIndirectY(Test, EOperation::Subtract);
    }

    void TestADCIndirectY(ADCTestData Test, 
                          EOperation Operation = EOperation::Add)	{
        // given:
        using namespace m6502;
        cpu.Reset(0xFF00, mem);
        cpu.Flag.C = Test.Carry;
        cpu.Y = 0x04;
        cpu.A = Test.A;
        cpu.Flag.Z = !Test.ExpectZ;
        cpu.Flag.N = !Test.ExpectN;
        cpu.Flag.V = !Test.ExpectV;
        mem[0xFF00] =
            (Operation == EOperation::Add) ?
            CPU::INS_ADC_INDY : CPU::INS_SBC_INDY;
        mem[0xFF01] = 0x02;
        mem[0x0002] = 0x00;
        mem[0x0003] = 0x80;
        mem[0x8000 + 0x04] = Test.Operand;
        constexpr s32 EXPECTED_CYCLES = 5;
        CPU CPUCopy = cpu;

        // when:
        const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

        // then:
        EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
        EXPECT_EQ(cpu.A, Test.Answer);
        EXPECT_EQ(cpu.Flag.C, Test.ExpectC);
        EXPECT_EQ(cpu.Flag.Z, Test.ExpectZ);
        EXPECT_EQ(cpu.Flag.N, Test.ExpectN);
        EXPECT_EQ(cpu.Flag.V, Test.ExpectV);
        ExpectUnaffectedRegisters(CPUCopy);
    }
};

#define BYTE( A ) ( (m6502::Byte)A )

TEST_F(AddSubWithCarryTest, ADCAbsCanAddZeroToZeroAndGetZero) {
    ADCTestData Test;
    Test.Carry = false;
    Test.A = 0;
    Test.Operand = 0;
    Test.Answer = 0;
    Test.ExpectC = false;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = true;
    TestADCAbsolute(Test);
}

TEST_F(AddSubWithCarryTest, ADCAbsCanAddCarryAndZeroToZeroAndGetOne) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 0;
    Test.Operand = 0;
    Test.Answer = 1;
    Test.ExpectC = false;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestADCAbsolute(Test);
}

TEST_F(AddSubWithCarryTest, ADCAbsCanAddTwoUnsignedNumbers) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = 17;
    Test.Answer = 38;
    Test.ExpectC = false;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestADCAbsolute(Test);
}

TEST_F(AddSubWithCarryTest, ADCAbsCanAddAPositiveAndNegativeNumber) {
    // A: 00010100 20   
    // O: 11101111 -17
    // =: 00000011
    // C:1 N:0 V:0 Z:0

    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = BYTE(-17);
    Test.Answer = 4;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestADCAbsolute(Test);
}

TEST_F(AddSubWithCarryTest, ADCAbsCanAddOneToFFAndItWillCauseACarry) {
    ADCTestData Test;
    Test.Carry = false;
    Test.A = 0xFF;			
    Test.Operand = 1;
    Test.Answer = 0;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = true;
    TestADCAbsolute(Test);
}

TEST_F(AddSubWithCarryTest, ADCAbsWillSetTheNegativeFlagWhenTheResultIsNegative) {
    ADCTestData Test;
    Test.Carry = false;
    Test.A = 0;
    Test.Operand = BYTE(-1);
    Test.Answer = BYTE(-1);
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestADCAbsolute(Test);
}

TEST_F(AddSubWithCarryTest, ADCAbsWillSetTheOverflowFlagWhenSignedNegativeAdditionFails) {
    // A: 10000000 -128
    // O: 11111111 -1
    // =: 01111111 127
    // C:1 N:0 V:1 Z:0

    ADCTestData Test;
    Test.Carry = false;
    Test.A = BYTE( -128 );
    Test.Operand = BYTE( -1 );
    Test.Answer = 127;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = true;
    Test.ExpectZ = false;
    TestADCAbsolute(Test);
}

TEST_F(AddSubWithCarryTest, 
       ADCAbsWillSetTheOverflowFlagWhenSignedNegativeAdditionPassedDueToInitalCarryFlag){
    // C: 00000001
    // A: 10000000 -128
    // O: 11111111 -1
    // =: 10000000 -128
    // C:1 N:1 V:0 Z:0

    ADCTestData Test;
    Test.Carry = true;
    Test.A = BYTE( -128 );
    Test.Operand = BYTE( -1 );
    Test.Answer = BYTE(-128);
    Test.ExpectC = true;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestADCAbsolute(Test);
}

TEST_F(AddSubWithCarryTest, ADCAbsWillSetTheOverflowFlagWhenSignedPositiveAdditionFails) {
    // A: 01111111 127   
    // O: 00000001 1
    // =: 10000000 128
    // C:0 N:1 V:1 Z:0

    ADCTestData Test;
    Test.Carry = false;
    Test.A = 127;
    Test.Operand = 1;
    Test.Answer = 128;
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = true;
    Test.ExpectZ = false;
    TestADCAbsolute(Test);
}

TEST_F(AddSubWithCarryTest, ADCImmediateCanAddTwoUnsignedNumbers) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = 17;
    Test.Answer = 38;
    Test.ExpectC = false;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestADCImmediate(Test);
}

TEST_F(AddSubWithCarryTest, ADCImmediateCanAddAPositiveAndNegativeNumber) {
    // A: 00010100 20   
    // O: 11101111 -17
    // =: 00000011
    // C:1 N:0 V:0 Z:0

    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = BYTE( -17 );
    Test.Answer = 4;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestADCImmediate(Test);
}

TEST_F(AddSubWithCarryTest, ADCZeroPageCanAddTwoUnsignedNumbers){
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = 17;
    Test.Answer = 38;
    Test.ExpectC = false;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestADCZeroPage(Test);
}

TEST_F(AddSubWithCarryTest, ADCZeroPageCanAddAPositiveAndNegativeNumber){
    // A: 00010100 20   
    // O: 11101111 -17
    // =: 00000011
    // C:1 N:0 V:0 Z:0

    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = BYTE( -17 );
    Test.Answer = 4;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestADCZeroPage(Test);
}

TEST_F(AddSubWithCarryTest, ADCZeroPageXCanAddTwoUnsignedNumbers) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = 17;
    Test.Answer = 38;
    Test.ExpectC = false;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestADCZeroPageX(Test);
}

TEST_F(AddSubWithCarryTest, ADCZeroPageXCanAddAPositiveAndNegativeNumber) {
    // A: 00010100 20   
    // O: 11101111 -17
    // =: 00000011
    // C:1 N:0 V:0 Z:0

    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = BYTE( -17 );
    Test.Answer = 4;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestADCZeroPageX(Test);
}

TEST_F(AddSubWithCarryTest, ADCAbsXCanAddTwoUnsignedNumbers) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = 17;
    Test.Answer = 38;
    Test.ExpectC = false;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestADCAbsoluteX(Test);
}

TEST_F(AddSubWithCarryTest, ADCAbsXCanAddAPositiveAndNegativeNumber) {
    // A: 00010100 20   
    // O: 11101111 -17
    // =: 00000011
    // C:1 N:0 V:0 Z:0

    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = BYTE( -17 );
    Test.Answer = 4;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestADCAbsoluteX(Test);
}

TEST_F(AddSubWithCarryTest, ADCAbsYCanAddTwoUnsignedNumbers) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = 17;
    Test.Answer = 38;
    Test.ExpectC = false;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestADCAbsoluteY(Test);
}

TEST_F(AddSubWithCarryTest, ADCAbsYCanAddAPositiveAndNegativeNumber) {
    // A: 00010100 20   
    // O: 11101111 -17
    // =: 00000011
    // C:1 N:0 V:0 Z:0

    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = BYTE( -17 );
    Test.Answer = 4;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestADCAbsoluteY(Test);
}

TEST_F(AddSubWithCarryTest, ADCIndXCanAddTwoUnsignedNumbers) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = 17;
    Test.Answer = 38;
    Test.ExpectC = false;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestADCIndirectX(Test);
}

TEST_F(AddSubWithCarryTest, ADCIndXCanAddAPositiveAndNegativeNumber) {
    // A: 00010100 20   
    // O: 11101111 -17
    // =: 00000011
    // C:1 N:0 V:0 Z:0

    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = BYTE( -17 );
    Test.Answer = 4;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestADCIndirectX(Test);
}

TEST_F(AddSubWithCarryTest, ADCIndYCanAddTwoUnsignedNumbers) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = 17;
    Test.Answer = 38;
    Test.ExpectC = false;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestADCIndirectY(Test);
}

TEST_F(AddSubWithCarryTest, ADCIndYCanAddAPositiveAndNegativeNumber) {
    // A: 00010100 20   
    // O: 11101111 -17
    // =: 00000011
    // C:1 N:0 V:0 Z:0

    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = BYTE( -17 );
    Test.Answer = 4;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestADCIndirectY(Test);
}

// SBC Abs --------------

TEST_F(AddSubWithCarryTest, SBCAbsCanSubtractZeroFromZeroAndGetZero) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 0;
    Test.Operand = 0;
    Test.Answer = 0;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = true;
    TestSBCAbsolute(Test);
}

TEST_F(AddSubWithCarryTest, SBCAbsCanSubtractZeroFromZeroAndCarryAndGetMinusOne) {
    ADCTestData Test;
    Test.Carry = false;
    Test.A = 0;
    Test.Operand = 0;
    Test.Answer = BYTE(-1);
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCAbsolute(Test);
}

TEST_F(AddSubWithCarryTest, SBCAbsCanSubtractOneFromZeroAndGetMinusOne) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 0;
    Test.Operand = 1;
    Test.Answer = BYTE(-1);
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCAbsolute(Test);
}

TEST_F(AddSubWithCarryTest, SBCAbsCanSubtractOneFromZeroWithCarryAndGetMinusTwo) {
    ADCTestData Test;
    Test.Carry = false;
    Test.A = 0;
    Test.Operand = 1;
    Test.Answer = BYTE( -2 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCAbsolute(Test);
}

TEST_F(AddSubWithCarryTest, SBCAbsCanSubtractTwoNegativeNumbersAndGetSignedOverflow) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = BYTE(-128);
    Test.Operand = 1;
    Test.Answer = 127;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = true;
    Test.ExpectZ = false;
    TestSBCAbsolute(Test);
}

TEST_F(AddSubWithCarryTest, SBCAbsCanSubtractAPositiveAndNegativeNumbersAndGetSignedOverflow) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 127;
    Test.Operand = BYTE(-1);
    Test.Answer = 128;
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = true;
    Test.ExpectZ = false;
    TestSBCAbsolute(Test);
}

TEST_F(AddSubWithCarryTest, SBCAbsCanSubtractTwoUnsignedNumbers) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = 17;
    Test.Answer = 3;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCAbsolute(Test);
}

TEST_F(AddSubWithCarryTest, SBCAbsCanSubtractTwoNegativeNumbers) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = BYTE(-20);
    Test.Operand = BYTE(-17);
    Test.Answer = BYTE(-3);
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCAbsolute(Test);
}

// SBC Zero Page

TEST_F(AddSubWithCarryTest, SBCZeroPageCanSubtractZeroFromZeroAndGetZero) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 0;
    Test.Operand = 0;
    Test.Answer = 0;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = true;
    TestSBCZeroPage(Test);
}

TEST_F(AddSubWithCarryTest, SBCZeroPageCanSubtractZeroFromZeroAndCarryAndGetMinusOne) {
    ADCTestData Test;
    Test.Carry = false;
    Test.A = 0;
    Test.Operand = 0;
    Test.Answer = BYTE( -1 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCZeroPage(Test);
}

TEST_F(AddSubWithCarryTest, SBCZeroPageCanSubtractOneFromZeroAndGetMinusOne) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 0;
    Test.Operand = 1;
    Test.Answer = BYTE( -1 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCZeroPage(Test);
}

TEST_F(AddSubWithCarryTest, SBCZeroPageCanSubtractOneFromZeroWithCarryAndGetMinusTwo) {
    ADCTestData Test;
    Test.Carry = false;
    Test.A = 0;
    Test.Operand = 1;
    Test.Answer = BYTE( -2 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCZeroPage(Test);
}

TEST_F(AddSubWithCarryTest, SBCZeroPageCanSubtractTwoNegativeNumbersAndGetSignedOverflow) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = BYTE( -128 );
    Test.Operand = 1;
    Test.Answer = 127;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = true;
    Test.ExpectZ = false;
    TestSBCZeroPage(Test);
}

TEST_F(AddSubWithCarryTest, SBCZeroPageCanSubtractAPositiveAndNegativeNumbersAndGetSignedOverflow) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 127;
    Test.Operand = BYTE( -1 );
    Test.Answer = 128;
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = true;
    Test.ExpectZ = false;
    TestSBCZeroPage(Test);
}

TEST_F(AddSubWithCarryTest, SBCZeroPageCanSubtractTwoUnsignedNumbers) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = 17;
    Test.Answer = 3;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCZeroPage(Test);
}

TEST_F(AddSubWithCarryTest, SBCZeroPageCanSubtractTwoNegativeNumbers) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = BYTE( -20 );
    Test.Operand = BYTE( -17 );
    Test.Answer = BYTE( -3 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCZeroPage(Test);
}

// SBC Immediate ---------

TEST_F(AddSubWithCarryTest, SBCImmediateCanSubtractZeroFromZeroAndGetZero) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 0;
    Test.Operand = 0;
    Test.Answer = 0;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = true;
    TestSBCImmediate(Test);
}

TEST_F(AddSubWithCarryTest, SBCImmediateCanSubtractZeroFromZeroAndCarryAndGetMinusOne) {
    ADCTestData Test;
    Test.Carry = false;
    Test.A = 0;
    Test.Operand = 0;
    Test.Answer = BYTE( -1 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCImmediate(Test);
}

TEST_F(AddSubWithCarryTest, SBCImmediateCanSubtractOneFromZeroAndGetMinusOne) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 0;
    Test.Operand = 1;
    Test.Answer = BYTE( -1 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCImmediate(Test);
}

TEST_F(AddSubWithCarryTest, SBCImmediateCanSubtractOneFromZeroWithCarryAndGetMinusTwo) {
    ADCTestData Test;
    Test.Carry = false;
    Test.A = 0;
    Test.Operand = 1;
    Test.Answer = BYTE( -2 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCImmediate(Test);
}

TEST_F(AddSubWithCarryTest, SBCImmediateCanSubtractTwoNegativeNumbersAndGetSignedOverflow) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = BYTE( -128 );
    Test.Operand = 1;
    Test.Answer = 127;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = true;
    Test.ExpectZ = false;
    TestSBCImmediate(Test);
}

TEST_F(AddSubWithCarryTest, SBCImmediateCanSubtractAPositiveAndNegativeNumbersAndGetSignedOverflow) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 127;
    Test.Operand = BYTE( -1 );
    Test.Answer = 128;
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = true;
    Test.ExpectZ = false;
    TestSBCImmediate(Test);
}

TEST_F(AddSubWithCarryTest, SBCImmediateCanSubtractTwoUnsignedNumbers) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = 17;
    Test.Answer = 3;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCImmediate(Test);
}

TEST_F(AddSubWithCarryTest, SBCImmediateCanSubtractTwoNegativeNumbers) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = BYTE( -20 );
    Test.Operand = BYTE( -17 );
    Test.Answer = BYTE( -3 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCImmediate(Test);
}

// SBC Zero Page, X -------

TEST_F(AddSubWithCarryTest, SBCZeroPageXCanSubtractZeroFromZeroAndGetZero) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 0;
    Test.Operand = 0;
    Test.Answer = 0;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = true;
    TestSBCZeroPageX(Test);
}

TEST_F(AddSubWithCarryTest, SBCZeroPageXCanSubtractZeroFromZeroAndCarryAndGetMinusOne) {
    ADCTestData Test;
    Test.Carry = false;
    Test.A = 0;
    Test.Operand = 0;
    Test.Answer = BYTE( -1 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCZeroPageX(Test);
}

TEST_F(AddSubWithCarryTest, SBCZeroPageXCanSubtractOneFromZeroAndGetMinusOne) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 0;
    Test.Operand = 1;
    Test.Answer = BYTE( -1 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCZeroPageX(Test);
}

TEST_F(AddSubWithCarryTest, SBCZeroPageXCanSubtractOneFromZeroWithCarryAndGetMinusTwo) {
    ADCTestData Test;
    Test.Carry = false;
    Test.A = 0;
    Test.Operand = 1;
    Test.Answer = BYTE( -2 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCZeroPageX(Test);
}

TEST_F(AddSubWithCarryTest, SBCZeroPageXCanSubtractTwoNegativeNumbersAndGetSignedOverflow) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = BYTE( -128 );
    Test.Operand = 1;
    Test.Answer = 127;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = true;
    Test.ExpectZ = false;
    TestSBCZeroPageX(Test);
}

TEST_F(AddSubWithCarryTest, SBCZeroPageXCanSubtractAPositiveAndNegativeNumbersAndGetSignedOverflow) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 127;
    Test.Operand = BYTE( -1 );
    Test.Answer = 128;
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = true;
    Test.ExpectZ = false;
    TestSBCZeroPageX(Test);
}

TEST_F(AddSubWithCarryTest, SBCZeroPageXCanSubtractTwoUnsignedNumbers) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = 17;
    Test.Answer = 3;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCZeroPageX(Test);
}

TEST_F(AddSubWithCarryTest, SBCZeroPageXCanSubtractTwoNegativeNumbers) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = BYTE( -20 );
    Test.Operand = BYTE( -17 );
    Test.Answer = BYTE( -3 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCZeroPageX(Test);
}

// SBC Absolute, X -------

TEST_F(AddSubWithCarryTest, SBCAbsoluteXCanSubtractZeroFromZeroAndGetZero) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 0;
    Test.Operand = 0;
    Test.Answer = 0;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = true;
    TestSBCAbsoluteX(Test);
}

TEST_F(AddSubWithCarryTest, SBCAbsoluteXCanSubtractZeroFromZeroAndCarryAndGetMinusOne) {
    ADCTestData Test;
    Test.Carry = false;
    Test.A = 0;
    Test.Operand = 0;
    Test.Answer = BYTE( -1 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCAbsoluteX(Test);
}

TEST_F(AddSubWithCarryTest, SBCAbsoluteXCanSubtractOneFromZeroAndGetMinusOne) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 0;
    Test.Operand = 1;
    Test.Answer = BYTE( -1 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCAbsoluteX(Test);
}

TEST_F(AddSubWithCarryTest, SBCAbsoluteXCanSubtractOneFromZeroWithCarryAndGetMinusTwo) {
    ADCTestData Test;
    Test.Carry = false;
    Test.A = 0;
    Test.Operand = 1;
    Test.Answer = BYTE( -2 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCAbsoluteX(Test);
}

TEST_F(AddSubWithCarryTest, SBCAbsoluteXCanSubtractTwoNegativeNumbersAndGetSignedOverflow) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = BYTE( -128 );
    Test.Operand = 1;
    Test.Answer = 127;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = true;
    Test.ExpectZ = false;
    TestSBCAbsoluteX(Test);
}

TEST_F(AddSubWithCarryTest, SBCAbsoluteXCanSubtractAPositiveAndNegativeNumbersAndGetSignedOverflow) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 127;
    Test.Operand = BYTE( -1 );
    Test.Answer = 128;
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = true;
    Test.ExpectZ = false;
    TestSBCAbsoluteX(Test);
}

TEST_F(AddSubWithCarryTest, SBCAbsoluteXCanSubtractTwoUnsignedNumbers) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = 17;
    Test.Answer = 3;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCAbsoluteX(Test);
}

TEST_F(AddSubWithCarryTest, SBCAbsoluteXCanSubtractTwoNegativeNumbers) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = BYTE( -20 );
    Test.Operand = BYTE( -17 );
    Test.Answer = BYTE( -3 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCAbsoluteX(Test);
}

// SBC Absolute, Y

TEST_F(AddSubWithCarryTest, SBCAbsoluteYCanSubtractZeroFromZeroAndGetZero) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 0;
    Test.Operand = 0;
    Test.Answer = 0;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = true;
    TestSBCAbsoluteY(Test);
}

TEST_F(AddSubWithCarryTest, SBCAbsoluteYCanSubtractZeroFromZeroAndCarryAndGetMinusOne) {
    ADCTestData Test;
    Test.Carry = false;
    Test.A = 0;
    Test.Operand = 0;
    Test.Answer = BYTE( -1 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCAbsoluteY(Test);
}

TEST_F(AddSubWithCarryTest, SBCAbsoluteYCanSubtractOneFromZeroAndGetMinusOne) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 0;
    Test.Operand = 1;
    Test.Answer = BYTE( -1 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCAbsoluteY(Test);
}

TEST_F(AddSubWithCarryTest, SBCAbsoluteYCanSubtractOneFromZeroWithCarryAndGetMinusTwo) {
    ADCTestData Test;
    Test.Carry = false;
    Test.A = 0;
    Test.Operand = 1;
    Test.Answer = BYTE( -2 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCAbsoluteY(Test);
}

TEST_F(AddSubWithCarryTest, SBCAbsoluteYCanSubtractTwoNegativeNumbersAndGetSignedOverflow) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = BYTE( -128 );
    Test.Operand = 1;
    Test.Answer = 127;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = true;
    Test.ExpectZ = false;
    TestSBCAbsoluteY(Test);
}

TEST_F(AddSubWithCarryTest, SBCAbsoluteYCanSubtractAPositiveAndNegativeNumbersAndGetSignedOverflow) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 127;
    Test.Operand = BYTE( -1 );
    Test.Answer = 128;
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = true;
    Test.ExpectZ = false;
    TestSBCAbsoluteY(Test);
}

TEST_F(AddSubWithCarryTest, SBCAbsoluteYCanSubtractTwoUnsignedNumbers) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = 17;
    Test.Answer = 3;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCAbsoluteY(Test);
}

TEST_F(AddSubWithCarryTest, SBCAbsoluteYCanSubtractTwoNegativeNumbers) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = BYTE( -20 );
    Test.Operand = BYTE( -17 );
    Test.Answer = BYTE( -3 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCAbsoluteY(Test);
}

// SBC Indirect, X ---------

TEST_F(AddSubWithCarryTest, SBCIndirectXCanSubtractZeroFromZeroAndGetZero) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 0;
    Test.Operand = 0;
    Test.Answer = 0;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = true;
    TestSBCIndirectX(Test);
}

TEST_F(AddSubWithCarryTest, SBCIndirectXCanSubtractZeroFromZeroAndCarryAndGetMinusOne) {
    ADCTestData Test;
    Test.Carry = false;
    Test.A = 0;
    Test.Operand = 0;
    Test.Answer = BYTE( -1 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCIndirectX(Test);
}

TEST_F(AddSubWithCarryTest, SBCIndirectXCanSubtractOneFromZeroAndGetMinusOne) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 0;
    Test.Operand = 1;
    Test.Answer = BYTE( -1 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCIndirectX(Test);
}

TEST_F(AddSubWithCarryTest, SBCIndirectXCanSubtractOneFromZeroWithCarryAndGetMinusTwo) {
    ADCTestData Test;
    Test.Carry = false;
    Test.A = 0;
    Test.Operand = 1;
    Test.Answer = BYTE( -2 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCIndirectX(Test);
}

TEST_F(AddSubWithCarryTest, SBCIndirectXCanSubtractTwoNegativeNumbersAndGetSignedOverflow) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = BYTE( -128 );
    Test.Operand = 1;
    Test.Answer = 127;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = true;
    Test.ExpectZ = false;
    TestSBCIndirectX(Test);
}

TEST_F(AddSubWithCarryTest, SBCIndirectXCanSubtractAPositiveAndNegativeNumbersAndGetSignedOverflow) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 127;
    Test.Operand = BYTE( -1 );
    Test.Answer = 128;
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = true;
    Test.ExpectZ = false;
    TestSBCIndirectX(Test);
}

TEST_F(AddSubWithCarryTest, SBCIndirectXCanSubtractTwoUnsignedNumbers) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = 17;
    Test.Answer = 3;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCIndirectX(Test);
}

TEST_F(AddSubWithCarryTest, SBCIndirectXCanSubtractTwoNegativeNumbers) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = BYTE( -20 );
    Test.Operand = BYTE( -17 );
    Test.Answer = BYTE( -3 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCIndirectX(Test);
}

// SBC Indirect, Y -----------

TEST_F(AddSubWithCarryTest, SBCIndirectYCanSubtractZeroFromZeroAndGetZero) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 0;
    Test.Operand = 0;
    Test.Answer = 0;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = true;
    TestSBCIndirectY(Test);
}

TEST_F(AddSubWithCarryTest, SBCIndirectYCanSubtractZeroFromZeroAndCarryAndGetMinusOne) {
    ADCTestData Test;
    Test.Carry = false;
    Test.A = 0;
    Test.Operand = 0;
    Test.Answer = BYTE( -1 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCIndirectY(Test);
}

TEST_F(AddSubWithCarryTest, SBCIndirectYCanSubtractOneFromZeroAndGetMinusOne) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 0;
    Test.Operand = 1;
    Test.Answer = BYTE( -1 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCIndirectY(Test);
}

TEST_F(AddSubWithCarryTest, SBCIndirectYCanSubtractOneFromZeroWithCarryAndGetMinusTwo) {
    ADCTestData Test;
    Test.Carry = false;
    Test.A = 0;
    Test.Operand = 1;
    Test.Answer = BYTE( -2 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCIndirectY(Test);
}

TEST_F(AddSubWithCarryTest, SBCIndirectYCanSubtractTwoNegativeNumbersAndGetSignedOverflow) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = BYTE( -128 );
    Test.Operand = 1;
    Test.Answer = 127;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = true;
    Test.ExpectZ = false;
    TestSBCIndirectY(Test);
}

TEST_F(AddSubWithCarryTest, SBCIndirectYCanSubtractAPositiveAndNegativeNumbersAndGetSignedOverflow) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 127;
    Test.Operand = BYTE( -1 );
    Test.Answer = 128;
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = true;
    Test.ExpectZ = false;
    TestSBCIndirectY(Test);
}

TEST_F(AddSubWithCarryTest, SBCIndirectYCanSubtractTwoUnsignedNumbers) {
    ADCTestData Test;
    Test.Carry = true;
    Test.A = 20;
    Test.Operand = 17;
    Test.Answer = 3;
    Test.ExpectC = true;
    Test.ExpectN = false;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCIndirectY(Test);
}

TEST_F(AddSubWithCarryTest, SBCIndirectYCanSubtractTwoNegativeNumbers ){
    ADCTestData Test;
    Test.Carry = true;
    Test.A = BYTE( -20 );
    Test.Operand = BYTE( -17 );
    Test.Answer = BYTE( -3 );
    Test.ExpectC = false;
    Test.ExpectN = true;
    Test.ExpectV = false;
    Test.ExpectZ = false;
    TestSBCIndirectY(Test);
}
