`include "fulladder.v"
module rca_36b (S, Cout, A, B, Cin);
	input [35:0] 		A, B;
	input 				Cin;
	output [35:0] 		S;
	output 				Cout;
	
	wire [35:0]			Carry;
	assign Cout = Carry[35];
	
	fulladder FA0 (S[0], Carry[0], A[0], B[0], Cin);
	fulladder FA1 (S[1], Carry[1], A[1], B[1], Carry[0]);
   fulladder FA2 (S[2], Carry[2], A[2], B[2], Carry[1]);
   fulladder FA3 (S[3], Carry[3], A[3], B[3], Carry[2]);
   fulladder FA4 (S[4], Carry[4], A[4], B[4], Carry[3]);
   fulladder FA5 (S[5], Carry[5], A[5], B[5], Carry[4]);
   fulladder FA6 (S[6], Carry[6], A[6], B[6], Carry[5]);
   fulladder FA7 (S[7], Carry[7], A[7], B[7], Carry[6]);
   fulladder FA8 (S[8], Carry[8], A[8], B[8], Carry[7]);
   fulladder FA9 (S[9], Carry[9], A[9], B[9], Carry[8]);
   fulladder FA10 (S[10], Carry[10], A[10], B[10], Carry[9]);
   fulladder FA11 (S[11], Carry[11], A[11], B[11], Carry[10]);
   fulladder FA12 (S[12], Carry[12], A[12], B[12], Carry[11]);
   fulladder FA13 (S[13], Carry[13], A[13], B[13], Carry[12]);
   fulladder FA14 (S[14], Carry[14], A[14], B[14], Carry[13]);
   fulladder FA15 (S[15], Carry[15], A[15], B[15], Carry[14]);
   fulladder FA16 (S[16], Carry[16], A[16], B[16], Carry[15]);
   fulladder FA17 (S[17], Carry[17], A[17], B[17], Carry[16]);
   fulladder FA18 (S[18], Carry[18], A[18], B[18], Carry[17]);
   fulladder FA19 (S[19], Carry[19], A[19], B[19], Carry[18]);
   fulladder FA20 (S[20], Carry[20], A[20], B[20], Carry[19]);
   fulladder FA21 (S[21], Carry[21], A[21], B[21], Carry[20]);
   fulladder FA22 (S[22], Carry[22], A[22], B[22], Carry[21]);
   fulladder FA23 (S[23], Carry[23], A[23], B[23], Carry[22]);
   fulladder FA24 (S[24], Carry[24], A[24], B[24], Carry[23]);
   fulladder FA25 (S[25], Carry[25], A[25], B[25], Carry[24]);
   fulladder FA26 (S[26], Carry[26], A[26], B[26], Carry[25]);
   fulladder FA27 (S[27], Carry[27], A[27], B[27], Carry[26]);
   fulladder FA28 (S[28], Carry[28], A[28], B[28], Carry[27]);
   fulladder FA29 (S[29], Carry[29], A[29], B[29], Carry[28]);
   fulladder FA30 (S[30], Carry[30], A[30], B[30], Carry[29]);
   fulladder FA31 (S[31], Carry[31], A[31], B[31], Carry[30]);
	fulladder FA32 (S[32], Carry[32], A[32], B[32], Carry[31]); 
	fulladder FA33 (S[33], Carry[33], A[33], B[33], Carry[32]); 
	fulladder FA34 (S[34], Carry[34], A[34], B[34], Carry[33]); 
	fulladder FA35 (S[35], Carry[35], A[35], B[35], Carry[34]); 
endmodule
