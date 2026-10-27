#ifndef __LIBS_H
#define __LIBS_H

 /*------Ascii_to_hex conversion-------*/
unsigned char ascii_hex( unsigned char c_var)
{
   unsigned char c_var1;
   if((0x30 <= c_var) && (c_var <= 0x39))// if numeric subtract 0x30
   c_var1 = c_var - 0x30;
   else c_var1 = c_var - 0x37;   // if not subtract 0x37 ( 55 in decimal )//generate line feed
   return(c_var1);
}/* end of ascii_to_hex fn */

/*------ hex or dec_to_ascii fn -------*/
unsigned char hex_ascii( unsigned char var1)
{
   unsigned char var2;
   if (var1 <= 0x09)
   var2 = var1 + 0X30;
   else 
   var2 = var1 + 0X37;   
   return(var2);
}/* end of dec_to_ascii fn */


/*-------- Hex to decimal conversion --------- */
unsigned int hex_dec(unsigned int c_var) 
{
   unsigned int mul=1,dec_val =0;
   unsigned char rem=0;
  
   while(c_var != 0x00)// eg: hex value is 0x86 or decimal 134
    {			  // first pass gives 4 as remainder and 0d in c_var
      rem = (c_var)%0x0a;  // 4 is positioned in the ls nibble of dec_val by the 
      c_var = (c_var)/0x0a;// multiplication of rem * mul ; in the next pass
      dec_val = dec_val + (rem*mul); // we get 3 remainder and 1 in c_var and the 3
      mul = mul*16;  // is positioned in the higher nibble of dec_val by 2*rem ; this repeats 
    }                // till c_var goes to zero and a packed bcd is in dec_val
   return dec_val;	
}// end of unsigned int hex_2_dec(unsigned int c_var) 

/*--------decimal to hex conversion --------- */
unsigned int dec_hex(unsigned int c_var) 
{
   unsigned int mul=1,hex_val =0;
   unsigned char rem=0;
  
   while(c_var != 0x00)// eg: packed bcd is 45 or hex equt is 0x2d
    {			     // first pass gives 5 as remainder and 4 in C_var
	  rem = (c_var)%0x10;  // 5 is positioned in the lsb location bcos mul is 1 
      c_var = (c_var)/0x10;// mul is multiplied by 10 every pass of the loop
	  hex_val = hex_val + (rem*mul); // in the second pass rem is 4 and c_var is 0
	  mul = mul*10;  // rem * mul is 40 and added to 5 gives 45 in hex ie 2d - so on 
    }                // till c_var goes to zero and a hex value is in hex_val
   return hex_val;	
	
}// end of unsigned int dec_2_hex(unsigned int c_var) 
  
#endif

