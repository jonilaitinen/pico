package org.astri.rotdtree;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayDeque;
import java.util.Deque;
import java.util.Scanner;

class FindfaceArrayParser{
	
	private Deque<Character> bytes = new ArrayDeque<Character>();
	private DTree dtree;
	
	public void initArray(){
		
		File file = new File("/Users/joni/Projects/FaceTracking/face-tracking-android/RotfaceDtree/src/org/astri/rotdtree/facefinder.array");

		try {
			Scanner scanner = new Scanner(file);
			scanner.useDelimiter(",");
			
			while(scanner.hasNext()){
				String hexstring = scanner.next();
				String hex = hexstring.trim().substring(2);
				bytes.add((char)Integer.parseInt(hex, 16));
			}
			scanner.close();
			
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		
		createTree();
	}
	
	public DTree getTree(){
		return dtree;
	}
	
	private void createTree(){
		
			System.out.println("creating tree...");
			dtree = new DTree();
			
			float ratio = Float.intBitsToFloat(popInt());
			
			dtree.ratio = ratio;
			dtree.nstage = popInt();
			
			for(int i = 0; i < dtree.nstage; i++){
				Nstage nstage = new Nstage();
				nstage.numtrees = popInt();
				System.out.println("numtrees: " + nstage.numtrees);
				
				for(int j = 0; j < nstage.numtrees; j++){
					Numtree numtree = new Numtree();
					numtree.toutdim = popInt();
					numtree.tdepth = popInt();
					//numtree.tcodes = popInt();
					
					for(int k = 0; k < 63; k++){
						Node node = new Node();
						node.depth = getDepth(k);
						node.loc1 = bytes.pop();
						node.loc2 = bytes.pop();
						node.loc3 = bytes.pop();
						node.loc4 = bytes.pop();
						numtree.nodes.add(node);
					}
					
					for(int k = 0; k < 63; k++){
						if(numtree.nodes.get(k).depth == numtree.tdepth){
							numtree.nodes.get(k).score1 = Float.intBitsToFloat(popInt());
							numtree.nodes.get(k).score2 = Float.intBitsToFloat(popInt());
						}
					}
					nstage.numtreelist.add(numtree);
				}
				
				nstage.threshold = Float.intBitsToFloat(popInt());
				dtree.nstages.add(nstage);
			}
			
			if(bytes.size() == 1 && bytes.pop() == 0){
				System.out.println("Tree fully parsed");
			}
	}
	
	private int getDepth(int index){
		if(index == 0){
			return 1;
		}
		if(index < 3){
			return 2;
		}
		if(index < 7){
			return 3;
		}
		if(index < 15){
			return 4;
		}
		if(index < 31){
			return 5;
		}
		else {
			return 6;
		}
	}
	
	private int popInt(){
		
		char byte1 = bytes.pop();
		char byte2 = bytes.pop();
		char byte3 = bytes.pop();
		char byte4 = bytes.pop();
		
		return (byte1 & 0xff)
	    | ((byte2 & 0xff) << 8)
	    | ((byte3 & 0xff) << 16)
	    | ((byte4 & 0xff) << 24);
	}
	
	public void printTree(){
		
		StringBuffer sb = new StringBuffer();
		sb.append("ratio: ");
		sb.append(dtree.ratio);
		sb.append("\n");
		sb.append("nstages: ");
		sb.append(dtree.nstage);
		sb.append("\n");
		
		for(int i = 0; i < dtree.nstage; i++){
			Nstage nstage = dtree.nstages.get(i);
			sb.append("nstage ");
			sb.append(i + 1);
			sb.append(":\n");
			sb.append("  threshold: ");
			sb.append(String.format("%.12f", nstage.threshold));
			sb.append("\n");
			
			for(int j = 0; j < nstage.numtrees; j++){
				sb.append("  numtree ");
				sb.append(j + 1);
				sb.append(":\n");
				Numtree numtree = nstage.numtreelist.get(j);
				sb.append("    toutdim: ");
				sb.append(numtree.toutdim);
				sb.append("\n");
				sb.append("    tdepth: ");
				sb.append(numtree.tdepth);
				sb.append("\n");
				sb.append("    tcodes: ");
				sb.append(numtree.tcodes);
				sb.append("\n");
				
				for(int k = 0; k < 63; k++){
					sb.append("      node ");
					sb.append(k + 1);
					sb.append(":");
					Node node = numtree.nodes.get(k);
					sb.append(" loc1: ");
					sb.append(node.loc1);
					sb.append("  \t loc2:");
					sb.append(node.loc2);
					sb.append("  \t loc3:");
					sb.append(node.loc3);
					sb.append("  \t loc4:");
					sb.append(node.loc4);
					if(node.depth == numtree.tdepth){
						sb.append("  \t score1:");
						sb.append(String.format("%.8f", node.score1));
						sb.append("  \t score2:");
						sb.append(String.format("%.8f", node.score2));
					}
					sb.append("\n");
				}
				
			}
			
		}
		
		System.out.print(sb.toString());
	}
	
}