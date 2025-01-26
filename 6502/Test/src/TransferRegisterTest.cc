#include <gtest/gtest.h>
#include "mos6502.h"

class TransferRegisterTest : public testing::Test {
public:
	m6502::Mem mem;
	m6502::CPU cpu;

	virtual void SetUp() {
		cpu.Reset(mem);
	}

	virtual void TearDown()	{}

	void ExpectUnaffectedRegisters(m6502::CPU CPUBefore) {
		EXPECT_EQ(CPUBefore.Flag.C, cpu.Flag.C);
		EXPECT_EQ(CPUBefore.Flag.I, cpu.Flag.I);
		EXPECT_EQ(CPUBefore.Flag.D, cpu.Flag.D);
		EXPECT_EQ(CPUBefore.Flag.B, cpu.Flag.B);
		EXPECT_EQ(CPUBefore.Flag.V, cpu.Flag.V);
	}
};

TEST_F(TransferRegisterTest, TAXCanTransferANonNegativeNonZeroValue) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.A = 0x42;
	cpu.X = 0x32;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_TAX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x42);
	EXPECT_EQ(cpu.X, 0x42);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	ExpectUnaffectedRegisters(CPUCopy);
}

TEST_F(TransferRegisterTest, TAXCanTransferANonNegativeZeroValue) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.A = 0x0;
	cpu.X = 0x32;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_TAX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x0);
	EXPECT_EQ(cpu.X, 0x0);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	ExpectUnaffectedRegisters(CPUCopy);
}

TEST_F(TransferRegisterTest, TAXCanTransferANegativeValue) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.A = 0b10001011;
	cpu.X = 0x32;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_TAX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0b10001011);
	EXPECT_EQ(cpu.X, 0b10001011);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
	ExpectUnaffectedRegisters(CPUCopy);
}

TEST_F(TransferRegisterTest, TAYCanTransferANonNegativeNonZeroValue) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.A = 0x42;
	cpu.Y = 0x32;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_TAY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x42);
	EXPECT_EQ(cpu.Y, 0x42);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	ExpectUnaffectedRegisters(CPUCopy);
}

TEST_F(TransferRegisterTest, TAYCanTransferANonNegativeZeroValue) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.A = 0x0;
	cpu.Y = 0x32;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_TAY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x0);
	EXPECT_EQ(cpu.Y, 0x0);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	ExpectUnaffectedRegisters(CPUCopy);
}

TEST_F(TransferRegisterTest, TAYCanTransferANegativeValue) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.A = 0b10001011;
	cpu.Y = 0x32;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_TAY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0b10001011);
	EXPECT_EQ(cpu.Y, 0b10001011);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
	ExpectUnaffectedRegisters(CPUCopy);
}

TEST_F(TransferRegisterTest, TXACanTransferANonNegativeNonZeroValue) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.X = 0x42;
	cpu.A = 0x32;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_TXA;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.X, 0x42);
	EXPECT_EQ(cpu.A, 0x42);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	ExpectUnaffectedRegisters(CPUCopy);
}

TEST_F(TransferRegisterTest, TXACanTransferANonNegativeZeroValue) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.X = 0x0;
	cpu.A = 0x32;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_TXA;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.X, 0x0);
	EXPECT_EQ(cpu.A, 0x0);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	ExpectUnaffectedRegisters(CPUCopy);
}

TEST_F(TransferRegisterTest, TXACanTransferANegativeValue) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.X = 0b10001011;
	cpu.A = 0x32;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_TXA;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.X, 0b10001011);
	EXPECT_EQ(cpu.A, 0b10001011);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
	ExpectUnaffectedRegisters(CPUCopy);
}

TEST_F(TransferRegisterTest, TYACanTransferANonNegativeNonZeroValue) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Y = 0x42;
	cpu.A = 0x32;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_TYA;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.Y, 0x42);
	EXPECT_EQ(cpu.A, 0x42);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	ExpectUnaffectedRegisters(CPUCopy);
}

TEST_F(TransferRegisterTest, TYACanTransferANonNegativeZeroValue) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Y = 0x0;
	cpu.A = 0x32;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_TYA;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.Y, 0x0);
	EXPECT_EQ(cpu.A, 0x0);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	ExpectUnaffectedRegisters(CPUCopy);
}

TEST_F(TransferRegisterTest, TYACanTransferANegativeValue) {
	// given:
	using namespace m6502;
	cpu.Reset(0xFF00, mem);
	cpu.Y = 0b10001011;
	cpu.A = 0x32;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_TYA;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.Y, 0b10001011);
	EXPECT_EQ(cpu.A, 0b10001011);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
	ExpectUnaffectedRegisters(CPUCopy);
}
