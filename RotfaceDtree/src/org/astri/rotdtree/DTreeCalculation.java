package org.astri.rotdtree;

public interface DTreeCalculation {

	public static final String CALCULATION = 
			"r1 = (rs + rrotlut_tree[nodeX.loc1][nodeX.loc2][rotind][s]) >> 8; \n" +
			"c1 = (cs + crotlut_tree[nodeX.loc1][nodeX.loc2][rotind][s]) >> 8; \n" +
			"r2 = (rs + rrotlut_tree[nodeX.loc3][nodeX.loc4][rotind][s]) >> 8; \n" +
			"c2 = (cs + crotlut_tree[nodeX.loc3][nodeX.loc4][rotind][s]) >> 8; \n";
	
	public static final String CALCULATION_TYPE = 
			"int rs = (r << 8); \n" +
			"int cs = (c << 8); \n" +
			"int r1 = (rs + rrotlut_tree[nodeX.loc1][nodeX.loc2][rotind][s]) >> 8; \n" +
			"int c1 = (cs + crotlut_tree[nodeX.loc1][nodeX.loc2][rotind][s]) >> 8; \n" +
			"int r2 = (rs + rrotlut_tree[nodeX.loc3][nodeX.loc4][rotind][s]) >> 8; \n" +
			"int c2 = (cs + crotlut_tree[nodeX.loc3][nodeX.loc4][rotind][s]) >> 8; \n";
}
