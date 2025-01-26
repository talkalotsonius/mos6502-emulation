#include <gtest/gtest.h>
#include "mos6502.h"

class ShiftTest : public testing::Test {
public:
	m6502::Mem mem;
	m6502::CPU cpu;

	virtual void SetUp() {
		cpu.Reset(mem);
	}

	virtual void TearDown()	{}
};

TEST_F(ShiftTest, ASLCanShiftTheValueOfOne) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.A = 1;
	mem[0xFF00] = CPU::INS_ASL;
	constexpr s32 EXPECTED_CYCLES = 2;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 2);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);	
}

TEST_F(ShiftTest, ASLCanShiftANegativeValue) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	cpu.A = 0b11000010;
	mem[0xFF00] = CPU::INS_ASL;
	constexpr s32 EXPECTED_CYCLES = 2;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0b10000100);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}

TEST_F(ShiftTest, ASLZeroPageCanShiftTheValueOfOne) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_ASL_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 1;
	constexpr s32 EXPECTED_CYCLES = 5;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0042], 2);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, ASLZeroPageCanShiftANegativeValue) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_ASL_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 0b11000010;
	constexpr s32 EXPECTED_CYCLES = 5;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0042], 0b10000100);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}

TEST_F(ShiftTest, ASLZeroPageXCanShiftTheValueOfOne) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ASL_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042 + 0x10] = 1;
	constexpr s32 EXPECTED_CYCLES = 6;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0042 + 0x10], 2);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, ASLZeroPageXCanShiftANegativeValue) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ASL_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042 + 0x10] = 0b11000010;
	constexpr s32 EXPECTED_CYCLES = 6;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0042 + 0x10], 0b10000100);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}

TEST_F(ShiftTest, ASLAbsCanShiftTheValueOfOne) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_ASL_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 1;
	constexpr s32 EXPECTED_CYCLES = 6;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8000], 2);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, ASLAbsCanShiftANegativeValue) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_ASL_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 0b11000010;
	constexpr s32 EXPECTED_CYCLES = 6;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8000], 0b10000100);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}

TEST_F(ShiftTest, ASLAbsXCanShiftTheValueOfOne) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ASL_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000 + 0x10] = 1;
	constexpr s32 EXPECTED_CYCLES = 7;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8000 + 0x10], 2);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, ASLAbsXCanShiftANegativeValue) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ASL_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000+0x10] = 0b11000010;
	constexpr s32 EXPECTED_CYCLES = 7;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8000 + 0x10], 0b10000100);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}

TEST_F(ShiftTest, LSRCanShiftTheValueOfOne) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.A = 1;
	mem[0xFF00] = CPU::INS_LSR;
	constexpr s32 EXPECTED_CYCLES = 2;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, LSRCanShiftAZeroIntoTheCarryFlag) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.A = 8;
	mem[0xFF00] = CPU::INS_LSR;
	constexpr s32 EXPECTED_CYCLES = 2;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 4);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, LSRZeroPageCanShiftTheValueOfOne) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_LSR_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 1;
	constexpr s32 EXPECTED_CYCLES = 5;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0042], 0);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, LSRZeroPageCanShiftAZeroIntoTheCarryFlag) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_LSR_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 8;
	constexpr s32 EXPECTED_CYCLES = 5;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0042], 4);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, LSRZeroPageXCanShiftTheValueOfOne) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_LSR_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042+ 0x10] = 1;
	constexpr s32 EXPECTED_CYCLES = 6;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0042+0x10], 0);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, LSRZeroPageXCanShiftAZeroIntoTheCarryFlag) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_LSR_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042+ 0x10] = 8;
	constexpr s32 EXPECTED_CYCLES = 6;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0042+ 0x10], 4);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, LSRAbsCanShiftTheValueOfOne) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_LSR_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 1;
	constexpr s32 EXPECTED_CYCLES = 6;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8000], 0);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, LSRAbsCanShiftAZeroIntoTheCarryFlag) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_LSR_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 8;
	constexpr s32 EXPECTED_CYCLES = 6;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8000], 4);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, LSRAbsXCanShiftTheValueOfOne) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_LSR_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000+0x10] = 1;
	constexpr s32 EXPECTED_CYCLES = 7;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8000 + 0x10], 0);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, LSRAbsXCanShiftAZeroIntoTheCarryFlag) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_LSR_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000 + 0x10] = 8;
	constexpr s32 EXPECTED_CYCLES = 7;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8000 + 0x10], 4);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

 // ------------ ROL ----------------

TEST_F(ShiftTest, ROLCanShiftABitOutOfTheCarryFlag) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.A = 0;
	mem[0xFF00] = CPU::INS_ROL;
	constexpr s32 EXPECTED_CYCLES = 2;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 1);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, ROLCanShiftABitIntoTheCarryFlag) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.A = 0b10000000;
	mem[0xFF00] = CPU::INS_ROL;
	constexpr s32 EXPECTED_CYCLES = 2;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, ROLCanShiftZeroWithNoCarry) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.A = 0;
	mem[0xFF00] = CPU::INS_ROL;
	constexpr s32 EXPECTED_CYCLES = 2;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, ROLCanShiftAValueThatResultInANegativeValue) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	cpu.A = 0b01110011;
	mem[0xFF00] = CPU::INS_ROL;
	constexpr s32 EXPECTED_CYCLES = 2;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0b11100111);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}

// ---------- Zero Page -------------

TEST_F(ShiftTest, ROLZeroPageCanShiftABitOutOfTheCarryFlag) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_ROL_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 0;
	constexpr s32 EXPECTED_CYCLES = 5;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0042], 1);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, ROLZeroPageCanShiftABitIntoTheCarryFlag) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_ROL_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 0b10000000;
	constexpr s32 EXPECTED_CYCLES = 5;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0042], 0);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, ROLZeroPageCanShiftZeroWithNoCarry) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_ROL_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 0;
	constexpr s32 EXPECTED_CYCLES = 5;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0042], 0);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, ROLZeroPageCanShiftAValueThatResultInANegativeValue) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_ROL_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 0b01110011;
	constexpr s32 EXPECTED_CYCLES = 5;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0042], 0b11100111);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}

// ------------- Zero Page X --------------

TEST_F(ShiftTest, ROLZeroPageXCanShiftABitOutOfTheCarryFlag) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROL_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042 + 0x10] = 0;
	constexpr s32 EXPECTED_CYCLES = 6;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0042+0x10], 1);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, ROLZeroPageXCanShiftABitIntoTheCarryFlag) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROL_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042+0x10] = 0b10000000;
	constexpr s32 EXPECTED_CYCLES = 6;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0042 + 0x10], 0);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, ROLZeroPageXCanShiftZeroWithNoCarry) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROL_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042 + 0x10] = 0;
	constexpr s32 EXPECTED_CYCLES = 6;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0042 + 0x10], 0);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, ROLZeroPageXCanShiftAValueThatResultInANegativeValue) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROL_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042 + 0x10] = 0b01110011;
	constexpr s32 EXPECTED_CYCLES = 6;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0042 + 0x10], 0b11100111);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}

// ------------- Absolute --------------

TEST_F(ShiftTest, ROLAbsoluteCanShiftABitOutOfTheCarryFlag) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_ROL_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 0;
	constexpr s32 EXPECTED_CYCLES = 6;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8000], 1);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, ROLAbsoluteCanShiftABitIntoTheCarryFlag) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_ROL_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 0b10000000;
	constexpr s32 EXPECTED_CYCLES = 6;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8000], 0);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, ROLAbsoluteCanShiftZeroWithNoCarry) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_ROL_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 0;
	constexpr s32 EXPECTED_CYCLES = 6;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8000], 0);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, ROLAbsoluteCanShiftAValueThatResultInANegativeValue) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_ROL_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 0b01110011;
	constexpr s32 EXPECTED_CYCLES = 6;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8000], 0b11100111);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}

// ------------- Absolute X --------------

TEST_F(ShiftTest, ROLAbsoluteXCanShiftABitOutOfTheCarryFlag) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROL_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000+0x10] = 0;
	constexpr s32 EXPECTED_CYCLES = 7;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8000 + 0x10], 1);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, ROLAbsoluteXCanShiftABitIntoTheCarryFlag) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROL_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000 + 0x10] = 0b10000000;
	constexpr s32 EXPECTED_CYCLES = 7;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8000 + 0x10], 0);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, ROLAbsoluteXCanShiftZeroWithNoCarry) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROL_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000 + 0x10] = 0;
	constexpr s32 EXPECTED_CYCLES = 7;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8000 + 0x10], 0);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, ROLAbsoluteXCanShiftAValueThatResultInANegativeValue) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROL_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000 + 0x10] = 0b01110011;
	constexpr s32 EXPECTED_CYCLES = 7;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8000 + 0x10], 0b11100111);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}

// --------------- ROR --------------------

TEST_F(ShiftTest, RORCanShiftTheCarryFlagIntoTheOperand) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	cpu.A = 0;
	mem[0xFF00] = CPU::INS_ROR;
	constexpr s32 EXPECTED_CYCLES = 2;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0b10000000);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}

TEST_F(ShiftTest, RORCanShiftAValueIntoTheCarryFlag) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	cpu.A = 1;
	mem[0xFF00] = CPU::INS_ROR;
	constexpr s32 EXPECTED_CYCLES = 2;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, RORCanRotateANumber) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	cpu.A = 0b01101101;
	mem[0xFF00] = CPU::INS_ROR;
	constexpr s32 EXPECTED_CYCLES = 2;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0b10110110);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}

// ZeroPage

TEST_F(ShiftTest, RORZeroPageCanShiftTheCarryFlagIntoTheOperand) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_ROR_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 0;
	constexpr s32 EXPECTED_CYCLES = 5;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0042], 0b10000000);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}

TEST_F(ShiftTest, RORZeroPageCanShiftAValueIntoTheCarryFlag) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_ROR_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 1;
	constexpr s32 EXPECTED_CYCLES = 5;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0042], 0);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, RORZeroPageCanRotateANumber) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_ROR_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 0b01101101;
	constexpr s32 EXPECTED_CYCLES = 5;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0042], 0b10110110);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}

// Zero Page X

TEST_F(ShiftTest, RORZeroXPageCanShiftTheCarryFlagIntoTheOperand) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROR_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042 + 0x10] = 0;
	constexpr s32 EXPECTED_CYCLES = 6;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0042 + 0x10], 0b10000000);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}

TEST_F(ShiftTest, RORZeroXPageCanShiftAValueIntoTheCarryFlag) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROR_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042+0x10] = 1;
	constexpr s32 EXPECTED_CYCLES = 6;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0042 + 0x10], 0);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, RORZeroXPageCanRotateANumber) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROR_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042 + 0x10] = 0b01101101;
	constexpr s32 EXPECTED_CYCLES = 6;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0042 + 0x10], 0b10110110);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}

// Absolute

TEST_F(ShiftTest, RORAbsolutePageCanShiftTheCarryFlagIntoTheOperand) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_ROR_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 0;
	constexpr s32 EXPECTED_CYCLES = 6;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8000], 0b10000000);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}

TEST_F(ShiftTest, RORAbsolutePageCanShiftAValueIntoTheCarryFlag) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_ROR_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 1;
	constexpr s32 EXPECTED_CYCLES = 6;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8000], 0);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, RORAbsolutePageCanRotateANumber) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_ROR_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 0b01101101;
	constexpr s32 EXPECTED_CYCLES = 6;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8000], 0b10110110);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}

// Absolute X

TEST_F(ShiftTest, RORAbsoluteXPageCanShiftTheCarryFlagIntoTheOperand) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROR_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000 + 0x10] = 0;
	constexpr s32 EXPECTED_CYCLES = 7;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8000 + 0x10], 0b10000000);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}

TEST_F(ShiftTest, RORAbsoluteXPageCanShiftAValueIntoTheCarryFlag) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROR_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000+0x10] = 1;
	constexpr s32 EXPECTED_CYCLES = 7;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8000 + 0x10], 0);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(ShiftTest, RORAbsoluteXPageCanRotateANumber) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROR_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000 + 0x10] = 0b01101101;
	constexpr s32 EXPECTED_CYCLES = 7;
	// CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8000 + 0x10], 0b10110110);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}
