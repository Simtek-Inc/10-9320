--//;=========================================================================
--//	SIMTEK, INC
--//	PART #:75-9880
--//	DESCRIPTION: SIMTEK TESTBOX
--//	SIMTEK PART # 10-9320
--//	by: John Morrison
--//	June 28, 2022
--//
--//	*BASED ON:  PART #:75-8903 DESCRIPTION: , SIMTEK PART # 10-9186 by: david bradshaw Oct. 13, 2021
--//;=========================================================================
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity top_level is
  Port (
    MClk : in std_logic;
    UcClk : out std_logic;
    Rst : in std_logic;
    AddrData : inout std_logic_vector(7 downto 0);
    Rd : in std_logic;
    Wr : in std_logic;
    Ale : in std_logic;
    Max_DE : out std_logic;             -- max 422 drive enable
    Max_422_232_Sel : out std_logic;
    PB_Data_1thru6 : in std_logic_vector(5 downto 0);
    PB_Data_7thru11 : in std_logic_vector(4 downto 0);
    PB_Data_12thru17 : in std_logic_vector(5 downto 0);
    Brt_Pot_PB : in std_logic;
    Enc_PB : in std_logic;
    Enc_PB_Rtn : out std_logic;
    DAC_Data_Out : out std_logic_vector(11 downto 0);
    A1 : in std_logic;
	B1 : in std_logic
    );
end top_level;

architecture Behavioral of top_level is

--Clock divider--------------------------------------
COMPONENT ClockDivider 
PORT (
    MClk : in std_logic;
    UcClk : out std_logic
    );
END COMPONENT;
--Address latch------------------------------------------
COMPONENT address_latch
PORT(
	MClk : IN std_logic;
	Rst : IN std_logic;
	Ale : IN std_logic;
	AddrData : IN std_logic_vector(7 downto 0);          
	Address : OUT std_logic_vector(7 downto 0)
	);
END COMPONENT;
--get data from microcontroller--------------------------
COMPONENT RamMem
PORT(
    MClk : in std_logic;
    Rst : in std_logic;
    Rd : in std_logic;
    Wr : in std_logic;
    AddrData : inout std_logic_vector(7 downto 0);
    Address : in std_logic_vector(7 downto 0);
    PB_Data_1thru6 : in std_logic_vector(5 downto 0);
    PB_Data_7thru11 : in std_logic_vector(4 downto 0);
    PB_Data_12thru17 : in std_logic_vector(5 downto 0);
    Brt_Pot_PB : in std_logic;
    Enc_PB : in std_logic;
    Enc_PB_Rtn : out std_logic;
    DAC_Data_Out : out std_logic_vector(11 downto 0);
    datacounter1 : in std_logic_vector(7 downto 0)
	);
END COMPONENT;

COMPONENT state_machine
Port ( 
    master_clock : in std_logic;
    logic_reset : in std_logic;
    A : in std_logic;
    B : in std_logic;
    dir : out std_logic;
    tclk : out std_logic
    );
END COMPONENT;

COMPONENT filter
Port (
    master_clock : in std_logic;
    logic_reset : in std_logic;
    A : in std_logic;
    B : in std_logic;
    outA : out std_logic;
    outB : out std_logic
	);
END COMPONENT;

COMPONENT ucounter
Port (     
    master_clock : in std_logic;
    logic_reset : in std_logic;
    dir : in std_logic;
    ce : in std_logic;
    Dout : out std_logic_vector(7 downto 0)
    );
END COMPONENT;

signal Address :std_logic_vector(7 downto 0);

signal outA1,outB1,dir1,tclk1 : std_logic;
signal datacounter1 : std_logic_vector(7 downto 0);	

begin
--set com to RS232 to microcontroller--------------------
Max_422_232_Sel <= '0';     
Max_DE          <= '0'; 
--Enc_PB_Rtn      <= '0';    
--get data from microcontroller--------------------------
Inst_RamMem: RamMem PORT MAP(
  MClk  => MClk,
  Rst   => Rst,
  Rd    => Rd,
  Wr    => Wr,
  AddrData       => AddrData,
  Address        => Address,
  PB_Data_1thru6   => PB_Data_1thru6,
  PB_Data_7thru11  => PB_Data_7thru11,
  PB_Data_12thru17 => PB_Data_12thru17,
  Brt_Pot_PB => Brt_Pot_PB,
  Enc_PB => Enc_PB,
  Enc_PB_Rtn => Enc_PB_Rtn,
  DAC_Data_Out => DAC_Data_Out,
  datacounter1 => datacounter1
  );
----------------------------------------------------------
Inst_ClockDivider_1 : ClockDivider PORT MAP(
    MClk => MClk,
    UcClk => UcClk
    );
-------------------------------------------------------
--Address latch for data input from microcontroller------
Inst_address_latch: address_latch PORT MAP(
  MClk => MClk,
  Rst => Rst,
  Ale => Ale,
  AddrData => AddrData,
  Address => Address
);
----------------------------------------------------------
	Inst_state_machine_1: state_machine PORT MAP(
		master_clock => MClk,
		logic_reset => RST,
		A => outA1,
		B => outB1,
		dir => dir1,
		tclk => tclk1
	);

	Inst_ucounter_1: ucounter PORT MAP(
		master_clock => MClk,
		logic_reset => RST,
		dir => dir1,
		ce => tclk1,
		Dout => datacounter1 
	);

	Inst_filter_1: filter PORT MAP(
		master_clock => MClk,
		logic_reset => RST,
		A => A1,
		B => B1,
		outA => outA1,
		outB => outB1
	);
----------------------------------------------------------
end Behavioral;