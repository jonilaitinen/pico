package org.astri.rotdtree;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;

public class DTreeGenerator {

	private String template;

	public DTreeGenerator() {
		init();
	}

	public void init() {
		File file = new File("D:/Projects/FaceTracking/face_android/RotfaceDtree/src/org/astri/rotdtree/Bintest.txt");
		try {
			BufferedReader reader = new BufferedReader(new FileReader(file));
			String line = null;
			StringBuilder stringBuilder = new StringBuilder();
			String ls = System.getProperty("line.separator");

			while ((line = reader.readLine()) != null) {
				stringBuilder.append(line);
				stringBuilder.append(ls);
			}
			
			reader.close();
			template = stringBuilder.toString();
			
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void generateDTree(DTree tree) {
		
		StringBuffer stageTrees = new StringBuffer();
		
		int stage = 0;
		for(Nstage nstage : tree.nstages){
			int treenum = 0;
			for(Numtree numtree : nstage.numtreelist){
				String numtreeCode = generateNumtree(numtree);
				numtreeCode = numtreeCode.replace("calculateScore", "stage" + stage + "tree" + treenum);
				stageTrees.append("\n");
				stageTrees.append(numtreeCode);
				treenum++;
			}
			stage++;
		}
		
		System.out.println(stageTrees.toString());
	}
	
	private String generateNumtree(Numtree numtree){
		
		String numtreeCode = template;
		
		int index = 0;
		for(Node node : numtree.nodes){
			String calc;
			if(node.depth == 1){
				calc = DTreeCalculation.CALCULATION_TYPE.replace("nodeX.loc1", Integer.toString(node.loc1));
			} else {
				calc = DTreeCalculation.CALCULATION.replace("nodeX.loc1", Integer.toString(node.loc1));
			}
			calc = calc.replace("nodeX.loc2", Integer.toString(node.loc2));
			calc = calc.replace("nodeX.loc3", Integer.toString(node.loc3));
			calc = calc.replace("nodeX.loc4", Integer.toString(node.loc4));
			numtreeCode = numtreeCode.replace("// node " + index + ".", calc);
			
			if(node.depth == numtree.tdepth){
				numtreeCode = numtreeCode.replace("node" + index + ".score1", String.format("%.12f", node.score1));
				numtreeCode = numtreeCode.replace("node" + index + ".score2", String.format("%.12f", node.score2));
			}
			
			index++;
		}
		
		return numtreeCode;
	}

}
