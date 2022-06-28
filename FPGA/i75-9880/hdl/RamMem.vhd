library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity RamMem is
  Port ( 
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
end RamMem;

architecture Behavioral of RamMem is

signal testbyte : std_logic_vector(7 downto 0);
signal rev1,rev2,rev3,rev4 : std_logic_vector(7 downto 0);

begin
--/program REV/-----------------------
--/75-9880 rev -/---------------------
rev1 <= X"75";
rev2 <= X"98";
rev3 <= X"80";
rev4 <= X"2D";
--------------------------------------

Write_Data:
process(MClk,Rst,Wr,Address,AddrData)
begin
  if (Rst = '1') then
    testbyte <= X"77";
  elsif (rising_edge(MClk)) then
    if (Wr = '0') then
      case (Address) is 
        when X"01" => testbyte <= AddrData;
        when X"80" => DAC_Data_Out(7 downto 0) <= AddrData;
        when X"81" => DAC_Data_Out(11 downto 8) <= AddrData(3 downto 0);
        when X"82" => Enc_PB_Rtn <= AddrData(0);
        when others => null;
      end case;
    end if;
  end if;
end process Write_Data;

Read_Data:
process(MClk,Rd,Address,rev1,rev2,rev3,rev4,testbyte)
begin
  if (MClk'event and MClk = '1') then
    if (Rd = '0') then
      case (Address) is 
        when X"01" => AddrData <= testbyte;
        when X"02" => AddrData <= rev1;--program number 
        when X"03" => AddrData <= rev2;-- 
        when X"04" => AddrData <= rev3;-- 
        when X"05" => AddrData <= rev4;--
        
        when X"30" => AddrData(5 downto 0) <= PB_Data_1thru6;--
        when X"31" => AddrData(4 downto 0) <= PB_Data_7thru11;--
        when X"32" => AddrData(5 downto 0) <= PB_Data_12thru17;-- 
        
        when X"33" => AddrData(0) <= Brt_Pot_PB;--
        when X"34" => AddrData(0) <= Enc_PB;--

        when X"35" => AddrData(7 downto 0) <= datacounter1;
        
        when others => AddrData <= X"00";
        end case;
    else
      AddrData <= (others => 'Z');
    end if;
  end if;
end process Read_Data;

end Behavioral;
