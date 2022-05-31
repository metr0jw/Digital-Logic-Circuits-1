module rippe_adder(input_x, input_y, S, Co);
	`include "fulladder.v"
 input [31:0] input_x, input_y;	// 2 Input variables
 output [31:0] S;						// Sum out
 output Co;								// Carry out
 wire [31:0]

    assign out_carry = carry[31];
    
    full_adder u0  (out_sum[0],  carry[0],  in_a[0],  in_b[0],  in_carry);
    full_adder u1 (out_sum[1],  carry[1],  in_a[1],  in_b[1],  carry[0]);
    full_adder u2 (out_sum[2],  carry[2],  in_a[2],  in_b[2],  carry[1]);
    full_adder u3 (out_sum[3],  carry[3],  in_a[3],  in_b[3],  carry[2]);
    full_adder u4 (out_sum[4],  carry[4],  in_a[4],  in_b[4],  carry[3]);
    full_adder u5 (out_sum[5],  carry[5],  in_a[5],  in_b[5],  carry[4]);
    full_adder u6 (out_sum[6],  carry[6],  in_a[6],  in_b[6],  carry[5]);
    full_adder u7 (out_sum[7],  carry[7],  in_a[7],  in_b[7],  carry[6]);
    full_adder u8 (out_sum[8],  carry[8],  in_a[8],  in_b[8],  carry[7]);
    full_adder u9 (out_sum[9],  carry[9],  in_a[9],  in_b[9],  carry[8]);
    full_adder u10 (out_sum[10],  carry[10],  in_a[10],  in_b[10],  carry[9]);
    full_adder u11 (out_sum[11],  carry[11],  in_a[11],  in_b[11],  carry[10]);
    full_adder u12 (out_sum[12],  carry[12],  in_a[12],  in_b[12],  carry[11]);
    full_adder u13 (out_sum[13],  carry[13],  in_a[13],  in_b[13],  carry[12]);
    full_adder u14 (out_sum[14],  carry[14],  in_a[14],  in_b[14],  carry[13]);
    full_adder u15 (out_sum[15],  carry[15],  in_a[15],  in_b[15],  carry[14]);
    full_adder u16 (out_sum[16],  carry[16],  in_a[16],  in_b[16],  carry[15]);
    full_adder u17 (out_sum[17],  carry[17],  in_a[17],  in_b[17],  carry[16]);
    full_adder u18 (out_sum[18],  carry[18],  in_a[18],  in_b[18],  carry[17]);
    full_adder u19 (out_sum[19],  carry[19],  in_a[19],  in_b[19],  carry[18]);
    full_adder u20 (out_sum[20],  carry[20],  in_a[20],  in_b[20],  carry[19]);
    full_adder u21 (out_sum[21],  carry[21],  in_a[21],  in_b[21],  carry[20]);
    full_adder u22 (out_sum[22],  carry[22],  in_a[22],  in_b[22],  carry[21]);
    full_adder u23 (out_sum[23],  carry[23],  in_a[23],  in_b[23],  carry[22]);
    full_adder u24 (out_sum[24],  carry[24],  in_a[24],  in_b[24],  carry[23]);
    full_adder u25 (out_sum[25],  carry[25],  in_a[25],  in_b[25],  carry[24]);
    full_adder u26 (out_sum[26],  carry[26],  in_a[26],  in_b[26],  carry[25]);
    full_adder u27 (out_sum[27],  carry[27],  in_a[27],  in_b[27],  carry[26]);
    full_adder u28 (out_sum[28],  carry[28],  in_a[28],  in_b[28],  carry[27]);
    full_adder u29 (out_sum[29],  carry[29],  in_a[29],  in_b[29],  carry[28]);
    full_adder u30 (out_sum[30],  carry[30],  in_a[30],  in_b[30],  carry[29]);
    full_adder u31 (out_sum[31],  carry[31],  in_a[31],  in_b[31],  carry[30]); 
endmodule