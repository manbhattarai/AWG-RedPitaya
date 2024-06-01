
module arb
    #(parameter DAC_DATA_WIDTH  = 14,
    parameter ADDR_WIDTH = 16)
    (input dac_clk,
    input wclk,
    input we,re,
    input [ADDR_WIDTH-1:0]waddr,
    input [DAC_DATA_WIDTH-1:0] data_in,
    input rst_n,
    input ddr_clk,
    input locked,
    output dac_clk_o,dac_wrt_o,dac_sel_o,dac_rst_o,
    output [13:0] dac_dat_o    
    );
    
reg [DAC_DATA_WIDTH-1:0] awg_rom [0:2**ADDR_WIDTH-1];
/*
initial
begin
    $readmemh("memory_test_hex.mem", awg_rom,0,2**ADDR_WIDTH-1);
end
*/
//initialize memrory
always@(posedge wclk)
begin
    if (we)
    begin
        awg_rom[waddr] <= data_in;
    end
end


reg [ADDR_WIDTH-1:0]raddr = 0;
reg [13:0] dac_out;
reg [13:0] dac_out_pipe0;
reg [13:0] dac_out_pipe1;
always @(posedge dac_clk)
begin
    if(!rst_n)
        raddr <= 0;
    else
    begin
        if(re)
        begin
            dac_out_pipe0 <= awg_rom[raddr];
            dac_out_pipe1 <= dac_out_pipe0;
            dac_out <= dac_out_pipe1;
            raddr <= raddr + 1;
        end
    end
end

reg  dac_rst;
always @(posedge dac_clk)
    dac_rst <= ~locked;


ODDR ODDR_rst (.Q(dac_rst_o), .D1(dac_rst), .D2(dac_rst), .C(  dac_clk  ), .CE(1'b1), .R( 1'b0  ), .S(1'b0)); 
ODDR ODDR_sel (.Q(dac_sel_o), .D1( 1'b0  ), .D2( 1'b1  ), .C(  dac_clk  ), .CE(1'b1), .R(1'b0), .S(1'b0)); 
ODDR ODDR_wrt (.Q(dac_wrt_o), .D1( 1'b0  ), .D2( 1'b1  ), .C(ddr_clk), .CE(1'b1), .R( 1'b0  ), .S(1'b0));
ODDR ODDR_clk (.Q(dac_clk_o), .D1( 1'b0  ), .D2( 1'b1  ), .C(ddr_clk), .CE(1'b1), .R( 1'b0  ), .S(1'b0));

genvar j;
generate
    for(j = 0; j < DAC_DATA_WIDTH; j = j + 1)
    begin : DAC_DAT
      ODDR ODDR_inst(
        .Q(dac_dat_o[j]),
        .D1(dac_out[j]),
        .D2(dac_out[j]),
        .C(dac_clk),
        .CE(1'b1),
        .R(1'b0),
        .S(1'b0)
      );
    end
  endgenerate

        
    
endmodule
