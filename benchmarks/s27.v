module s27 (CK, G0, G1, G2, G3, G17);

input G0;
input G1;
input G2;
input G3;
input CK;

output G17;

// Internal wires
wire G14;
wire G11;
wire G6;
wire G8;
wire G12;
wire G15;
wire G16;
wire G9;
wire G10;
wire G5;
wire G7;
wire G13;

hi1s1 U_G14( .DIN(G0), .Q(G14) );
hi1s1 U_G17( .DIN(G11), .Q(G17) );
and2s1 U_G8( .DIN1(G14), .DIN2(G6), .Q(G8) );
or2s1 U_G15( .DIN1(G12), .DIN2(G8), .Q(G15) );
or2s1 U_G16( .DIN1(G3), .DIN2(G8), .Q(G16) );
nnd2s1 U_G9( .DIN1(G16), .DIN2(G15), .Q(G9) );
nor2s1 U_G10( .DIN1(G14), .DIN2(G11), .Q(G10) );
nor2s1 U_G11( .DIN1(G5), .DIN2(G9), .Q(G11) );
nor2s1 U_G12( .DIN1(G1), .DIN2(G7), .Q(G12) );
nor2s1 U_G13( .DIN1(G2), .DIN2(G12), .Q(G13) );
dffs1 U_G5 (.DIN(G10), .CLK(CK), .Q(G5) );
dffs1 U_G6 (.DIN(G11), .CLK(CK), .Q(G6) );
dffs1 U_G7 (.DIN(G13), .CLK(CK), .Q(G7) );

endmodule
