#include <gtest/gtest.h>
#include "mos6502.h"

class StackOperationsTest : public testing::Test {
public:	
	m6502::Mem mem;
	m6502::CPU cpu;

	virtual void SetUp() {
		cpu.Reset(mem);
	}

	virtual void TearDown()	{}
};

TEST_F(StackOperationsTest, TSXCanTransferTheStackPointerToXRegister) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.Z = cpu.Flag.N = true;
	cpu.X = 0x00;
	cpu.SP = 0x01;
	mem[0xFF00] = CPU::INS_TSX;
	constexpr s32 EXPECTED_CYCLES = 2;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.X, 0x01);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(StackOperationsTest, TSXCanTransferAZeroStackPointerToXRegister) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.Z = cpu.Flag.N = true;
	cpu.X = 0x00;
	cpu.SP = 0x00;
	mem[0xFF00] = CPU::INS_TSX;
	constexpr s32 EXPECTED_CYCLES = 2;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.X, 0x00);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(StackOperationsTest, TSXCanTransferANegativeStackPointerToXRegister) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.Z = cpu.Flag.N = false;
	cpu.X = 0x00;
	cpu.SP = 0b10000000;
	mem[0xFF00] = CPU::INS_TSX;
	constexpr s32 EXPECTED_CYCLES = 2;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.X, 0b10000000);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}

TEST_F(StackOperationsTest, TXSCanTransferXRegisterToTheStackPointer) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.X = 0xFF;
	cpu.SP = 0;
	mem[0xFF00] = CPU::INS_TXS;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.SP, 0xFF);
	EXPECT_EQ(cpu.PS, CPUCopy.PS);
}

TEST_F(StackOperationsTest, PHACanPushARegsiterOntoTheStack) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.A = 0x42;
	mem[0xFF00] = CPU::INS_PHA;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[cpu.SPToAddress()+1] ,cpu.A);
	EXPECT_EQ(cpu.PS, CPUCopy.PS);
	EXPECT_EQ(cpu.SP, 0xFE);
}

TEST_F(StackOperationsTest, PLACanPullAValueFromTheStackIntoTheARegsiter) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.A = 0x00;
	cpu.SP = 0xFE;
	mem[0x01FF] = 0x42;
	mem[0xFF00] = CPU::INS_PLA;
	constexpr s32 EXPECTED_CYCLES = 4;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x42);
	EXPECT_EQ(cpu.SP, 0xFF);
}

TEST_F(StackOperationsTest, PLACanPullAZeroValueFromTheStackIntoTheARegsiter) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.A = 0x42;
	cpu.SP = 0xFE;
	mem[0x01FF] = 0x00;
	mem[0xFF00] = CPU::INS_PLA;
	constexpr s32 EXPECTED_CYCLES = 4;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x00);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_EQ(cpu.SP, 0xFF);
}

TEST_F(StackOperationsTest, PLACanPullANegativeValueFromTheStackIntoTheARegsiter) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.N = false;
	cpu.Flag.Z = true;
	cpu.A = 0x42;
	cpu.SP = 0xFE;
	mem[0x01FF] = 0b10000001;
	mem[0xFF00] = CPU::INS_PLA;
	constexpr s32 EXPECTED_CYCLES = 4;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0b10000001);
	EXPECT_TRUE(cpu.Flag.N);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_EQ(cpu.SP, 0xFF);
}

TEST_F(StackOperationsTest, PHPCanPushProcessorStatusOntoTheStack) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.PS = 0xCC;
	mem[0xFF00] = CPU::INS_PHP;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[cpu.SPToAddress() + 1], 
		0xCC | CPU::UnusedFlagBit | CPU::BreakFlagBit);
	EXPECT_EQ(cpu.PS, CPUCopy.PS);
	EXPECT_EQ(cpu.SP, 0xFE);
}

TEST_F(StackOperationsTest, PHPAlwaysSetsBits4And5OnTheStack) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.PS = 0x0;
	mem[0xFF00] = CPU::INS_PHP;
	constexpr s32 EXPECTED_CYCLES = 3;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	Word AddPSOnStack = cpu.SPToAddress() + 1;
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);


	/* https://wiki.nesdev.com/w/index.php/Status_flags
	 * Two interrupts (/IRQ and /NMI) and two instructions (PHP and BRK) push 
	 * the flags to the stack. If the byte pushed, bit 5 is always set to 1, 
	 * and bit 4 is 1 if from an instruction (PHP or BRK) or 0 if from an 
	 * interrupt line being pulled low (/IRQ or /NMI). This is the only time 
	 * and place where the B flag actually exists: not in the status register 
	 * itself, but in bit 4 of the copy that is written to the stack. */
	const Byte FlagsOnStack = CPU::UnusedFlagBit | CPU::BreakFlagBit;
	EXPECT_EQ(mem[AddPSOnStack], FlagsOnStack);
}

TEST_F(StackOperationsTest, PLPCanPullAValueFromTheStackIntoTheProcessorStatus) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.SP = 0xFE;
	cpu.PS = 0;
	mem[0x01FF] = 0x42 | CPU::BreakFlagBit | CPU::UnusedFlagBit;
	mem[0xFF00] = CPU::INS_PLP;
	constexpr s32 EXPECTED_CYCLES = 4;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS, 0x42);
}

TEST_F(StackOperationsTest, PLPClearsBits4And5WhenPullingFromTheStack) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.SP = 0xFE;
	cpu.PS = 0;
	mem[0x01FF] = CPU::BreakFlagBit | CPU::UnusedFlagBit;
	mem[0xFF00] = CPU::INS_PLP;
	constexpr s32 EXPECTED_CYCLES = 4;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS, 0);
}
