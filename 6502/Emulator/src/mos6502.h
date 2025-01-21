#pragma once
#include <stdio.h>
#include <stdlib.h>

/** Reference: http://www.6502.org/users/obelisk/ */
namespace m6502 {
	typedef char SByte;
  typedef unsigned char Byte;
  typedef unsigned short Word;
	//using Byte  = unsigned char;
	//using Word  = unsigned short;

  typedef unsigned int u32;
  typedef signed int s32;
	//using u32   = unsigned int;
	//using s32   = signed int;

	struct Mem;
	struct CPU;
	struct StatusFlags;
}

struct m6502::Mem {
	static constexpr u32 MAX_MEM = 1024 * 64;
	Byte Data[MAX_MEM];

	void Initialise() {
		for (u32 i = 0; i < MAX_MEM; i++) {
			Data[i] = 0;
		}
	}

	/** read 1 byte */
	Byte operator[](u32 Address) const {
		// assert here Address is < MAX_MEM
		return Data[Address];
	}

	/** write 1 byte */
	Byte &operator[](u32 Address) {
		// assert here Address is < MAX_MEM
		return Data[Address];
	}
};

struct m6502::StatusFlags {	
	Byte C      : 1;  //0: Carry Flag	
	Byte Z      : 1;  //1: Zero Flag
	Byte I      : 1;  //2: Interrupt disable
	Byte D      : 1;  //3: Decimal mode
	Byte B      : 1;  //4: Break
	Byte Unused : 1;  //5: Unused
	Byte V      : 1;  //6: Overflow
	Byte N      : 1;  //7: Negative
};

struct m6502::CPU {
	Word PC;      //program counter
	Byte SP;      //stack pointer

	Byte A, X, Y;	//registers

	union { // processor status
		Byte PS;	
		StatusFlags Flag;
	};

	void Reset(Mem &memory) {
		Reset(0xFFFC, memory);
	}

	void Reset(Word ResetVector, Mem &memory) {
		PC = ResetVector;
		SP = 0xFF;
		Flag.C = Flag.Z = Flag.I = Flag.D = Flag.B = Flag.V = Flag.N = 0;
		A = X = Y = 0;
		memory.Initialise();
	}

	Byte FetchByte(s32 &Cycles, const Mem &memory) {
		Byte Data = memory[PC];
		PC++;
		Cycles--;

		return Data;
	}

	SByte FetchSByte(s32 &Cycles, const Mem &memory) {
		return FetchByte(Cycles, memory);
	}

	Word FetchWord(s32 &Cycles, const Mem &memory) {
		// 6502 is little endian
		Word Data = memory[PC];
		PC++;
		
		Data |= (memory[PC] << 8 );
		PC++;
		Cycles -= 2;

		return Data;
	}

	Byte ReadByte(s32 &Cycles, Word Address, const Mem &memory) {
		Byte Data = memory[Address];
		Cycles--;

		return Data;
	}

	Word ReadWord(s32 &Cycles, Word Address, const Mem &memory) {
		Byte LoByte = ReadByte(Cycles, Address, memory);
		Byte HiByte = ReadByte(Cycles, Address + 1, memory);

		return LoByte | (HiByte << 8);
	}

	/** write 1 byte to memory */
	void WriteByte(Byte Value, s32 &Cycles, Word Address, Mem &memory) {
		memory[Address] = Value;
		Cycles--;
	}

	/** write 2 bytes to memory */
	void WriteWord(Word Value, s32 &Cycles, Word Address, Mem &memory) {
		memory[Address] = Value & 0xFF;
		memory[Address + 1] = (Value >> 8);
		Cycles -= 2;
	}

	/** @return the stack pointer as a full 16-bit address (in the 1st page) */
	Word SPToAddress() const{
		return 0x100 | SP;
	}

	void PushWordToStack(s32 &Cycles, Mem &memory, Word Value) {
		WriteByte(Value >> 8, Cycles, SPToAddress(), memory);
		SP--;
		WriteByte(Value &0xFF, Cycles, SPToAddress(), memory);
		SP--;
	}

	/** Push the PC-1 onto the stack */
	void PushPCMinusOneToStack(s32 &Cycles, Mem &memory) {
		PushWordToStack(Cycles, memory, PC - 1);
	}

	/** Push the PC+1 onto the stack */
	void PushPCPlusOneToStack(s32 &Cycles, Mem &memory ) {
		PushWordToStack(Cycles, memory, PC + 1);
	}

	/** Push the PC onto the stack */
	void PushPCToStack(s32 &Cycles, Mem &memory ) {
		PushWordToStack(Cycles, memory, PC);
	}

	void PushByteOntoStack(s32 &Cycles, Byte Value, Mem &memory) {
		const Word SPWord = SPToAddress();
		memory[SPWord] = Value;
		Cycles--;
		SP--;
		Cycles--;
	}

	Byte PopByteFromStack(s32&Cycles, Mem&memory) {
		SP++;
		Cycles--;
		const Word SPWord = SPToAddress();
		Byte Value = memory[SPWord];
		Cycles--;

		return Value;
	}

	/** Pop a 16-bit value from the stack */
	Word PopWordFromStack(s32 &Cycles, Mem &memory) {
		Word ValueFromStack = ReadWord(Cycles, SPToAddress()+1, memory);
		SP += 2;
		Cycles--;

		return ValueFromStack;
	}

  /** Process status bits */
  static constexpr Byte
    NegativeFlagBit         = 0b10000000,
		OverflowFlagBit         = 0b01000000,
		BreakFlagBit            = 0b000010000,
		UnusedFlagBit           = 0b000100000,
		InterruptDisableFlagBit = 0b000000100,
		ZeroBit                 = 0b00000001;

  /** Instruction Set */
	static constexpr Byte
    /** Op-Codes */
		// LDA = Load Accumulator
		INS_LDA_IM   = 0xA9, // IM = Immediate (addressing)
		INS_LDA_ZP   = 0xA5, // ZP = Zero Page
		INS_LDA_ZPX  = 0xB5, // ZPX = Zero Page,X
		INS_LDA_ABS  = 0xAD, // ABS = Absolute
		INS_LDA_ABSX = 0xBD, // ABSX = Absolute,X
		INS_LDA_ABSY = 0xB9, // ABSY = Absolute,Y
		INS_LDA_INDX = 0xA1, // INDX = Indirect,X
		INS_LDA_INDY = 0xB1, // INDY = Indirect,Y

		// LDX = Load X Register
		INS_LDX_IM   = 0xA2,
		INS_LDX_ZP   = 0xA6,
		INS_LDX_ZPY  = 0xB6, // ZPY = Zero Page,Y
		INS_LDX_ABS  = 0xAE,
		INS_LDX_ABSY = 0xBE,

		// LDY = Load Y Register
		INS_LDY_IM   = 0xA0,
		INS_LDY_ZP   = 0xA4,
		INS_LDY_ZPX  = 0xB4,
		INS_LDY_ABS  = 0xAC,
		INS_LDY_ABSX = 0xBC,

		// STA = Store Accumulator
		INS_STA_ZP   = 0x85,
		INS_STA_ZPX  = 0x95,
		INS_STA_ABS  = 0x8D,
		INS_STA_ABSX = 0x9D,
		INS_STA_ABSY = 0x99,
		INS_STA_INDX = 0x81,
		INS_STA_INDY = 0x91,

		// STX = Store X Register
		INS_STX_ZP  = 0x86,
		INS_STX_ZPY = 0x96,
		INS_STX_ABS = 0x8E,

		// STY = Store Y Register
		INS_STY_ZP  = 0x84,
		INS_STY_ZPX = 0x94,
		INS_STY_ABS = 0x8C,

		INS_TSX = 0xBA, // TSX = Transfer Stack Pointer to X
		INS_TXS = 0x9A, // TXS = Transfer X to Stack Pointer
		INS_PHA = 0x48, // PHA = Push Accumulator
		INS_PLA = 0x68, // PLA = Pull Accumulator
		INS_PHP = 0x08, // PHP = Push Processor Status
		INS_PLP = 0x28, // PLP = Pull Processor Status
    
    INS_JSR     = 0x20, // JSR = Jump to Subroutine
		INS_RTS     = 0x60, // RTS = Return from Subroutine
		INS_JMP_ABS = 0x4C, // JMP_ABS = Jump, Absolute
		INS_JMP_IND = 0x6C, // JMP_IND = Jump, Indirect 
		

    /** Logical Operations */
		// AND = Logical AND
		INS_AND_IM   = 0x29,
		INS_AND_ZP   = 0x25,
		INS_AND_ZPX  = 0x35,
		INS_AND_ABS  = 0x2D,
		INS_AND_ABSX = 0x3D,
		INS_AND_ABSY = 0x39,
		INS_AND_INDX = 0x21,
		INS_AND_INDY = 0x31,

		// ORA = Logical Inclusive OR
		INS_ORA_IM   = 0x09,
		INS_ORA_ZP   = 0x05,
		INS_ORA_ZPX  = 0x15,
		INS_ORA_ABS  = 0x0D,
		INS_ORA_ABSX = 0x1D,
		INS_ORA_ABSY = 0x19,
		INS_ORA_INDX = 0x01,
		INS_ORA_INDY = 0x11,

		// EOR = Exclusive OR
		INS_EOR_IM   = 0x49,
		INS_EOR_ZP   = 0x45,
		INS_EOR_ZPX  = 0x55,
		INS_EOR_ABS  = 0x4D,
		INS_EOR_ABSX = 0x5D,
		INS_EOR_ABSY = 0x59,
		INS_EOR_INDX = 0x41,
		INS_EOR_INDY = 0x51,

		// BIT = Bit Test
		INS_BIT_ZP  = 0x24,
		INS_BIT_ABS = 0x2C,


    /** Transfer Registers */
		INS_TAX = 0xAA, // TAX = Transfer Accumulator to X
		INS_TAY = 0xA8, // TAY = Transfer Accumulator to Y
		INS_TXA = 0x8A, // TXA = Transfer X to Accumulator
		INS_TYA = 0x98, // TYA = Transfer Y to Accumulator


    /** Increments, Decrements */
		INS_INX = 0xE8, // INX = Increment X Register
		INS_INY = 0xC8, // INY = Increment Y Register
		INS_DEY = 0x88, // DEX = Decrement X Register
		INS_DEX = 0xCA, // DEY = Decrement Y Register

    // DEC = Decrement Memory
		INS_DEC_ZP   = 0xC6,
		INS_DEC_ZPX  = 0xD6,
		INS_DEC_ABS  = 0xCE,
		INS_DEC_ABSX = 0xDE,

    // INC = Increment Memory
		INS_INC_ZP   = 0xE6,
		INS_INC_ZPX  = 0xF6,
		INS_INC_ABS  = 0xEE,
		INS_INC_ABSX = 0xFE,


    /** Branches */
		INS_BEQ = 0xF0, // BEQ = Branch if Equal
		INS_BNE = 0xD0, // BNE = Branch if Not Equal
		INS_BCS = 0xB0, // BCS = Branch if Carry Set
		INS_BCC = 0x90, // BCC = Branch if Carry Clear
		INS_BMI = 0x30, // BMI = Branch if Minus
		INS_BPL = 0x10, // BPL = Branch if Positive
		INS_BVC = 0x50, // BVC = Branch if Overflow Clear
		INS_BVS = 0x70, // BVS = Branch if Overflow Set


    /** Status flag changes */
		INS_CLC = 0x18, // CLC = Clear Carry Flag
		INS_SEC = 0x38, // SEC = Set Carry Flag
		INS_CLD = 0xD8, // CLD = Clear Decimal Mode
		INS_SED = 0xF8, // SED = Set Decimal Flag
		INS_CLI = 0x58, // CLI = Clear Interrupt Disable
		INS_SEI = 0x78, // SEI = Set Interrupt Disable
		INS_CLV = 0xB8, // CLV = Clear Overflow Flag


    /** Arithmetical operations */
		// ADC = Add with Carry
		INS_ADC      = 0x69,
		INS_ADC_ZP   = 0x65,
		INS_ADC_ZPX  = 0x75,
		INS_ADC_ABS  = 0x6D,
		INS_ADC_ABSX = 0x7D,
		INS_ADC_ABSY = 0x79,
		INS_ADC_INDX = 0x61,
		INS_ADC_INDY = 0x71,

    // SBC = Subtract with Carry
		INS_SBC      = 0xE9,
		INS_SBC_ABS  = 0xED,
		INS_SBC_ZP   = 0xE5,
		INS_SBC_ZPX  = 0xF5,
		INS_SBC_ABSX = 0xFD,
		INS_SBC_ABSY = 0xF9,
		INS_SBC_INDX = 0xE1,
		INS_SBC_INDY = 0xF1,

		// CMP = Compare
    /** This instruction compares the contents of the accumulator with another memory held value 
      * and sets the zero and carry flags as appropriate. */
		INS_CMP      = 0xC9,
		INS_CMP_ZP   = 0xC5,
		INS_CMP_ZPX  = 0xD5,
		INS_CMP_ABS  = 0xCD,
		INS_CMP_ABSX = 0xDD,
		INS_CMP_ABSY = 0xD9,
		INS_CMP_INDX = 0xC1,
		INS_CMP_INDY = 0xD1,

    // CPX = Compare X Register
    /** This instruction compares the contents of the X register with another memory held value 
      * and sets the zero and carry flags as appropriate. */
		INS_CPX     = 0xE0,
		INS_CPX_ZP  = 0xE4,
		INS_CPX_ABS = 0xEC,
		
    // CPY = Compare Y Register
    INS_CPY     = 0xC0,
    INS_CPY_ZP  = 0xC4,
    INS_CPY_ABS = 0xCC,


    /** Shifts */
    // ASL = Arithmetic Shift Left
		INS_ASL      = 0x0A,
		INS_ASL_ZP   = 0x06,
		INS_ASL_ZPX  = 0x16,
		INS_ASL_ABS  = 0x0E,
		INS_ASL_ABSX = 0x1E,

    // LSR = Logical Shift Right
		INS_LSR      = 0x4A,
		INS_LSR_ZP   = 0x46,
		INS_LSR_ZPX  = 0x56,
		INS_LSR_ABS  = 0x4E,
		INS_LSR_ABSX = 0x5E,

		// ROL - Rotate Left
    INS_ROL      = 0x2A,
		INS_ROL_ZP   = 0x26,
		INS_ROL_ZPX  = 0x36,
		INS_ROL_ABS  = 0x2E,
		INS_ROL_ABSX = 0x3E,

		// ROR - Rotate Right
    INS_ROR      = 0x6A,
		INS_ROR_ZP   = 0x66,
		INS_ROR_ZPX  = 0x76,
		INS_ROR_ABS  = 0x6E,
		INS_ROR_ABSX = 0x7E,


    /** Misc */
    /** The NOP instruction causes no changes to the processor
      * other than the normal incrementing of the program counter to the next instruction. */
		INS_NOP = 0xEA, // NOP = No Operation
    /** The BRK instruction forces the generation of an interrupt request. 
      * The program counter and processor status are pushed on the stack 
      * then the IRQ interrupt vector at $FFFE/F is loaded into the PC
      * and the break flag in the status set to one. */
		INS_BRK = 0x00, // BRK = Force Interrupt ("break")
    /** The RTI instruction is used at the end of an interrupt processing routine.
      * It pulls the processor flags from the stack followed by the program counter. */
		INS_RTI = 0x40; // RTI = Return from Interrupt


	/** Sets the correct process status after 
    * a load register instruction LDA, LDX, LDY */
	void SetZeroAndNegativeFlags(Byte Register) {
		Flag.Z = (Register == 0);
		Flag.N = (Register & NegativeFlagBit) > 0;
	}

	/** @return the address that the program was loading into, or 0 if no program */
	Word LoadPrg(const Byte *Program, u32 NumBytes, Mem &memory) const;

	/** printf the registers, program counter etc */
	void PrintStatus() const;

	/** @return the number of cycles that were used */
	s32 Execute(s32 Cycles, Mem &memory);

	/** Addressing mode - Zero page */
	Word AddrZeroPage(s32 &Cycles, const Mem &memory);

	/** Addressing mode - Zero page with X offset */
	Word AddrZeroPageX(s32 &Cycles, const Mem &memory);

	/** Addressing mode - Zero page with Y offset */
	Word AddrZeroPageY(s32 &Cycles, const Mem &memory);

	/** Addressing mode - Absolute */
	Word AddrAbsolute(s32 &Cycles, const Mem &memory);

	/** Addressing mode - Absolute with X offset */
	Word AddrAbsoluteX(s32 &Cycles, const Mem &memory);

	/** Addressing mode - Absolute with X offset 
    * Always takes a cycle for the X page boundary) 
    * See "STA Absolute,X" */
	Word AddrAbsoluteX_5(s32 &Cycles, const Mem &memory);	

	/** Addressing mode - Absolute with Y offset */
	Word AddrAbsoluteY(s32 &Cycles, const Mem &memory);

	/** Addressing mode - Absolute with Y offset
    * Always takes a cycle for the Y page boundary)
    * See "STA Absolute,Y" */
	Word AddrAbsoluteY_5(s32 &Cycles, const Mem &memory);

	/** Addressing mode - Indirect X | Indexed Indirect */
	Word AddrIndirectX(s32 &Cycles, const Mem &memory);

	/** Addressing mode - Indirect Y | Indirect Indexed */
	Word AddrIndirectY(s32 &Cycles, const Mem &memory);

	/** Addressing mode - Indirect Y | Indirect Indexed
    * Always takes a cycle for the Y page boundary)
    * See "STA (Indirect,Y) */
	Word AddrIndirectY_6(s32 &Cycles, const Mem &memory);
};
