package org.astri.rotdtree;

public class Main {
	
	public static void main(String[] argv){
		System.out.println("start decode");
		
		FindfaceArrayParser parser = new FindfaceArrayParser();
		parser.initArray();
		parser.printTree();
		
		//DTreeGenerator generator = new DTreeGenerator();
		//generator.generateDTree(parser.getTree());
		
		
	}
	
}
