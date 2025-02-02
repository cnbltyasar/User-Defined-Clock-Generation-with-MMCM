library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD.ALL;

entity freq_counter is
    Port (
        clk_ref   : in  std_logic;         -- 100 MHz reference clock
        clk_user  : in  std_logic;         -- Unknown user clock
        rst       : in  std_logic;         -- Reset signal
        count_clk_ref: out std_logic_vector(31 downto 0); --Reference Clock counter
        count_clk_user: out std_logic_vector(31 downto 0) --User defined Clock counter 
    );
end freq_counter;

architecture Behavioral of freq_counter is
signal count_ref: std_logic_vector(31 downto 0) := (others => '0');
signal count_user: std_logic_vector(31 downto 0) := (others => '0');

begin

    -- Divide the reference clock to 1 Hz
    process(clk_ref, rst)
    begin
        if rst = '1' then
            count_ref <= (others => '0'); --set all bits as 0
        elsif rising_edge(clk_ref) then
            count_ref <= count_ref + 1;
        end if;
    end process;

    -- Count the user clock edges
    process(clk_user, rst)
    begin
        if rst = '1' then
            count_user <= (others => '0');----set all bits as 0
        elsif rising_edge(clk_user) then
            count_user <= count_user + 1;
        end if;
    end process;
    
    count_clk_ref <= count_ref;
    count_clk_user <= count_user;

end Behavioral;
