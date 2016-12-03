`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    10:36:59 12/02/2016 
// Design Name: 
// Module Name:    top 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module top(
    input           RESET,
    input           clock_generator_0_CLKIN_pin,
    input           iomodule_dlmb_UART_Rx_pin,
    output          iomodule_dlmb_UART_Tx_pin,

    //output [31:0]   iomodule_dlmb_GPO1_pin,
    //input [31:0]    iomodule_dlmb_GPI1_pin,
    //output          iomodule_ilmb_IO_Addr_Strobe_pin,
    //output          iomodule_ilmb_IO_Read_Strobe_pin,
    //output          iomodule_ilmb_IO_Write_Strobe_pin,
    //output [31:0]   iomodule_ilmb_IO_Address_pin,
    //output [3:0]    iomodule_ilmb_IO_Byte_Enable_pin,
    //output [31:0]   iomodule_ilmb_IO_Write_Data_pin,
    //input [31:0]    iomodule_ilmb_IO_Read_Data_pin,
    //input           iomodule_ilmb_IO_Ready_pin,
    //output          iomodule_dlmb_IO_Addr_Strobe_pin,
    //output          iomodule_dlmb_IO_Read_Strobe_pin,
    //output          iomodule_dlmb_IO_Write_Strobe_pin,
    //output [31:0]   iomodule_dlmb_IO_Address_pin,
    //output [3:0]    iomodule_dlmb_IO_Byte_Enable_pin,
    //output [31:0]   iomodule_dlmb_IO_Write_Data_pin,
    //input [31:0]    iomodule_dlmb_IO_Read_Data_pin,
    //input           iomodule_dlmb_IO_Ready_pin,
    output[3:0]     GPO_led,
    input[3:0]      GPI_key,
    output          IO_AE,
    output          IO_RE,
    output          IO_WE,
    output[31:0]    IO_SA,
    output[3:0]     IO_BYTES,
    output[31:0]    IO_DOUT,
    input[31:0]     IO_DIN,
    input           IO_READY
);

wire        clock_generator_0_CLKOUT0_pin;
wire        proc_sys_reset_0_Peripheral_Reset_pin;
wire[31:0]  iomodule_dlmb_GPO1_pin;
wire[31:0]  iomodule_dlmb_GPI1_pin;
wire        iomodule_ilmb_IO_Addr_Strobe_pin;
wire        iomodule_ilmb_IO_Read_Strobe_pin;
wire        iomodule_ilmb_IO_Write_Strobe_pin;
wire[31:0]  iomodule_ilmb_IO_Address_pin;
wire[3:0]   iomodule_ilmb_IO_Byte_Enable_pin;
wire[31:0]  iomodule_ilmb_IO_Write_Data_pin;
wire[31:0]  iomodule_ilmb_IO_Read_Data_pin;
wire        iomodule_ilmb_IO_Ready_pin;
wire        iomodule_dlmb_IO_Addr_Strobe_pin;
wire        iomodule_dlmb_IO_Read_Strobe_pin;
wire        iomodule_dlmb_IO_Write_Strobe_pin;
wire[31:0]  iomodule_dlmb_IO_Address_pin;
wire[3:0]   iomodule_dlmb_IO_Byte_Enable_pin;
wire[31:0]  iomodule_dlmb_IO_Write_Data_pin;
wire[31:0]  iomodule_dlmb_IO_Read_Data_pin;
wire        iomodule_dlmb_IO_Ready_pin;

wire clk;
wire rst;
parameter ST_IDLE   = 2'b00;
parameter ST_ILMB   = 2'b01;
parameter ST_DLMB   = 2'b10;
reg[1:0] state;

cpu cpu_inst (
    .RESET                                  (RESET),
    .clock_generator_0_CLKIN_pin            (clock_generator_0_CLKIN_pin),
    .clock_generator_0_CLKOUT0_pin          (clock_generator_0_CLKOUT0_pin),
    .proc_sys_reset_0_Peripheral_Reset_pin  (proc_sys_reset_0_Peripheral_Reset_pin),

    .iomodule_dlmb_UART_Rx_pin              (iomodule_dlmb_UART_Rx_pin),
    .iomodule_dlmb_UART_Tx_pin              (iomodule_dlmb_UART_Tx_pin),
    .iomodule_dlmb_GPO1_pin                 (iomodule_dlmb_GPO1_pin),
    .iomodule_dlmb_GPI1_pin                 (iomodule_dlmb_GPI1_pin),
    
    .iomodule_ilmb_IO_Addr_Strobe_pin       (iomodule_ilmb_IO_Addr_Strobe_pin),
    .iomodule_ilmb_IO_Read_Strobe_pin       (iomodule_ilmb_IO_Read_Strobe_pin),
    .iomodule_ilmb_IO_Write_Strobe_pin      (iomodule_ilmb_IO_Write_Strobe_pin),
    .iomodule_ilmb_IO_Address_pin           (iomodule_ilmb_IO_Address_pin),
    .iomodule_ilmb_IO_Byte_Enable_pin       (iomodule_ilmb_IO_Byte_Enable_pin),
    .iomodule_ilmb_IO_Write_Data_pin        (iomodule_ilmb_IO_Write_Data_pin),
    .iomodule_ilmb_IO_Read_Data_pin         (iomodule_ilmb_IO_Read_Data_pin),
    .iomodule_ilmb_IO_Ready_pin             (iomodule_ilmb_IO_Ready_pin),
    
    .iomodule_dlmb_IO_Addr_Strobe_pin       (iomodule_dlmb_IO_Addr_Strobe_pin),
    .iomodule_dlmb_IO_Read_Strobe_pin       (iomodule_dlmb_IO_Read_Strobe_pin),
    .iomodule_dlmb_IO_Write_Strobe_pin      (iomodule_dlmb_IO_Write_Strobe_pin),
    .iomodule_dlmb_IO_Address_pin           (iomodule_dlmb_IO_Address_pin),
    .iomodule_dlmb_IO_Byte_Enable_pin       (iomodule_dlmb_IO_Byte_Enable_pin),
    .iomodule_dlmb_IO_Write_Data_pin        (iomodule_dlmb_IO_Write_Data_pin),
    .iomodule_dlmb_IO_Read_Data_pin         (iomodule_dlmb_IO_Read_Data_pin),
    .iomodule_dlmb_IO_Ready_pin             (iomodule_dlmb_IO_Ready_pin)
);

assign clk = clock_generator_0_CLKOUT0_pin;
assign rst = proc_sys_reset_0_Peripheral_Reset_pin;

always@(posedge clk)
begin
    if (rst)
        state <= ST_IDLE;
    else if (iomodule_dlmb_IO_Addr_Strobe_pin)
        state <= ST_DLMB;
    else if (iomodule_ilmb_IO_Addr_Strobe_pin)
        state <= ST_ILMB;
    else
        state <= state;
end

assign IO_AE = (state == ST_ILMB)?iomodule_ilmb_IO_Addr_Strobe_pin:iomodule_dlmb_IO_Addr_Strobe_pin;
assign IO_RE = (state == ST_ILMB)?iomodule_ilmb_IO_Read_Strobe_pin:iomodule_dlmb_IO_Read_Strobe_pin;
assign IO_WE = (state == ST_ILMB)?iomodule_ilmb_IO_Write_Strobe_pin:iomodule_dlmb_IO_Write_Strobe_pin;
assign IO_SA = (state == ST_ILMB)?iomodule_ilmb_IO_Address_pin:iomodule_dlmb_IO_Address_pin;
assign IO_BYTES = (state == ST_ILMB)?iomodule_ilmb_IO_Byte_Enable_pin:iomodule_dlmb_IO_Byte_Enable_pin;
assign IO_DOUT = (state == ST_ILMB)?iomodule_ilmb_IO_Write_Data_pin:iomodule_dlmb_IO_Write_Data_pin;
assign iomodule_ilmb_IO_Read_Data_pin = IO_DIN;
assign iomodule_dlmb_IO_Read_Data_pin = IO_DIN;
assign iomodule_ilmb_IO_Ready_pin = (state == ST_ILMB)?IO_READY:1'b0;
assign iomodule_dlmb_IO_Ready_pin = (state == ST_DLMB)?IO_READY:1'b0;


endmodule
