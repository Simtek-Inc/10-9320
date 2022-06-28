library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity ClockDivider is
  Port (
    MClk : in std_logic;
    UcClk : out std_logic
    );
end ClockDivider;

architecture Behavioral of ClockDivider is

signal div20Count : unsigned (9 downto 0);

begin

--ClkDivProcess: process(Clk,div20Count,MasterCount)
ClkDivProcess: process(MClk,div20Count)
begin
  if (rising_edge(MClk)) then
    if (div20Count = 1333) then
      div20Count <= (others => '0');
    else
      div20Count <= div20Count + 1;
    end if;
  end if;
end process ClkDivProcess;
UcClk <= div20Count(2);

end Behavioral;

