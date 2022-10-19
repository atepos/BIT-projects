-- uart_fsm.vhd: UART controller - finite state machine
-- Author(s): Petr Ka?ka
--
library ieee;
use ieee.std_logic_1164.all;

-------------------------------------------------
entity UART_FSM is
port(
   CLK : in std_logic;
   RST : in std_logic;
   DIN : in std_logic;
   COUNTER_MID : in std_logic_vector(4 downto 0);
   COUNTER_BIT : in std_logic_vector(3 downto 0);
   COUNTER_READ_STOP : in std_logic_vector(3 downto 0);
   DATA_VALIDATION : out std_logic;
   COUNTERS_STARTED : out std_logic;
   DATA_READING : out std_logic
   );
end entity UART_FSM;

-------------------------------------------------
architecture behavioral of UART_FSM is type STATE_TYPE is (IDLE, WAIT_FOR, DATA_READ, STOP_BIT);
    signal state : STATE_TYPE := IDLE;
begin

  DATA_READING <= '1' when state = DATA_READ else '0';
  DATA_VALIDATION <= '1' when state = STOP_BIT else '0';
  COUNTERS_STARTED <= '1' when state = WAIT_FOR or state = DATA_READ else '0';
  
  process (CLK) begin 

    if (clk'event and clk = '1') then
  
      if RST = '1' then 
        state <= IDLE;
  
       else
  
          case state is 
    
          when IDLE => if DIN = '0' then state <= WAIT_FOR;
            end if;
          when WAIT_FOR => if COUNTER_MID = "10110" then state <= DATA_READ;
            end if;
          when DATA_READ => if COUNTER_BIT = "1000" then state <= STOP_BIT;
            end if;
          when STOP_BIT => if COUNTER_READ_STOP = "1000" then state <= IDLE;
            end if;
          when others => null;
    
        end case;
      end if;
    end if;

  end process;

end behavioral;
