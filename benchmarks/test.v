module test(
    G0,
    G1,
    G2,
    G3
);

    input G0;
    input G1;
    input G2;
    output G3;

    wire G6;
    wire G7;

    hi1s1 U_G6 (.Q(G6), 
    .DIN(G0));
    and2s1 U_G7 (.Q(G7), 
    .DIN2(G6), 
    .DIN1(G1));
    or2s1 U_G3 (.Q(G3), 
    .DIN2(G7), 
    .DIN1(G2));
endmodule