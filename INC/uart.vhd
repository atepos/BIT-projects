-- uart.vhd: UART controller - receiving part
-- Author(s): Petr Ka?ka
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

-------------------------------------------------
entity UART_RX is
port(	
    CLK: 	    in std_logic;
	RST: 	    in std_logic;
	DIN: 	    in std_logic;
	DOUT: 	    out std_logic_vector(7 downto 0);
	DOUT_VLD: 	out std_logic
);
end UART_RX;  

-------------------------------------------------
architecture behavioral of UART_RX is
  signal counter : std_logic_vector(4 downto 0);
  signal bit_counter : std_logic_vector(3 downto 0) := "0000";
  signal stop_counter : std_logic_vector(3 downto 0) := "0000";
  signal data_valid : std_logic := '1';
  signal start_counting: std_logic;
  signal validation: std_logic := '0';  
  signal read_data: std_logic;

begin
  
  UART_FSM: entity work.UART_FSM(behavioral) port map(
    DIN => DIN,
    RST => RST,
    CLK => CLK,
    COUNTER_MID => counter,
    COUNTER_BIT => bit_counter,
    COUNTER_READ_STOP => stop_counter,
    DATA_VALIDATION => data_valid,
    DATA_READING => read_data,
    COUNTERS_STARTED => start_counting
  ); 

process(CLK) begin
    
    if (clk'event and clk = '1') then
    
       DOUT_VLD <= validation;
      
         if rst = '1' then -- kdyz prijde na vstup reset, tak se vsechny signaly nastavy na pocatecni hodnotu
            validation <= '0';
            DOUT <= "00000000";
            bit_counter <= "0000";
            stop_counter <= "0000";
         
         elsif stop_counter = "1000" then -- counter napocita do 8, tak se vysle validacni signal a country se nastavy na pocatecni hodnotu
            DOUT_VLD <= '1';
            bit_counter <= "0000";
            stop_counter <= "0000";
         else 
    
            if(start_counting = '1') then 
                counter <= counter + '1';
            else 
                counter <= "00001";
            end if;
    
            if bit_counter = "1000" then 
                stop_counter <= stop_counter + "1";
    
            end if;
    
            if (read_data = '1' and counter(4) = '1')then--samotne cteni ze vstupu
      
                counter <= "00001";
        
                if(bit_counter) = "0000" then DOUT(0) <= DIN;
                elsif(bit_counter) = "0001" then DOUT(1) <= DIN;
                elsif(bit_counter) = "0010" then DOUT(2) <= DIN;
                elsif(bit_counter) = "0011" then DOUT(3) <= DIN;
                elsif(bit_counter) = "0100" then DOUT(4) <= DIN;
                elsif(bit_counter) = "0101" then DOUT(5) <= DIN;
                elsif(bit_counter) = "0110" then DOUT(6) <= DIN;
                elsif(bit_counter) = "0111" then DOUT(7) <= DIN;
      
            end if;
      
            bit_counter <= bit_counter + "1";
        
          end if;
        end if;
    end if;

  end process;

end behavioral;