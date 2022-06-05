module fulladder(S, Cout, A, B, Cin);
  input A, B, Cin;
  output S, Cout;
  wire w1, w2, w3;
  
  xor G1(w1, A, B);
  xor G2(S, w1, Cin);
  and G3(w2, w1, Cin);
  and G4(w3, A, B);
  or G5(Cout, w2, w3);
endmodule
