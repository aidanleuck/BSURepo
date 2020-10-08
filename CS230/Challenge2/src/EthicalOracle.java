import java.util.HashMap;
import java.util.Scanner;

public class EthicalOracle {
	private boolean hypothetical;
	private HashMap<String, Integer> benefitList;
	private HashMap<String, Integer> negativeList;
	
	public EthicalOracle() {
		benefitList = new HashMap<String, Integer>();
		negativeList = new HashMap<String, Integer>();
		run();
	}
	
	private void run() {
		Scanner inputScanner = new Scanner(System.in);
		
		String query = "Is this a hypothetical scenario?: \ny for yes \nn for no";

		String hyp = checkAnswer(query, new String[] {"y", "n"}, inputScanner);
			
		if(hyp.equals("y")) {
			hypothetical = true;
		}
		else if(hyp.equals("n")) {
			hypothetical = false;
		}	
		
		SentenceCreator sentenceCreation = new SentenceCreator(hypothetical);
		boolean ethical = isEthical(inputScanner, sentenceCreation);
		
		if(ethical) {
			boolean actAnalysis = actUtil(inputScanner);
		}
		else {
			System.out.println("Since this problem is not ethical the oracle can not provide an analysis.");
		}
		
	}
	
	private int evaluate(HashMap<String, Integer> h) {
		
		int total = 0;
		for(String key : h.keySet() ) {
			total += h.get(key);
		}
		return total;
	}
	private int scanUtil(Scanner s, String line, HashMap<String, Integer> h) {
		Scanner utilScanner = new Scanner(line);
		utilScanner.useDelimiter(",");
		while(utilScanner.hasNext()) {
			String token = utilScanner.next();
			Scanner tokenScanner = new Scanner(token);
			tokenScanner.useDelimiter(":");
			String benefitItem = tokenScanner.next();
			String value = tokenScanner.next();
			value = value.trim();
			if(isInteger(value)) {
				int valueInt = Integer.parseInt(value);
				h.put(benefitItem, valueInt);
				tokenScanner.close();
				utilScanner.close();
				
				int total = evaluate(h);
				return total;
			
			}
			else {
				System.out.println("You did not enter valid input");
				scanUtil(s, line, h);
			}
		
		
		}
		System.out.println("You did not enter valid input, please try again");
		scanUtil(s, line, h);
		
		
	
	
	}
	private boolean actUtil(Scanner s) {
		
		System.out.println("We will now determine the act utilitarian analysis");
		System.out.println("Please enter a list of benefits separated by , (value must be numeric) "
				+ "\n(Ex: Saves snowy owls: 45, Helps family: 3 etc... ");
		String line = s.nextLine();
		
		int benefit = scanUtil(s, line, benefitList);
		System.out.println("Please enter a list of consequences separated by , (value must be numeric) "
				+ "\n(Ex: Saves snowy owls: 45, Helps family: 3 etc...) ");
		String line2 = s.nextLine();
		int cost = scanUtil(s, line2, negativeList);
		
		if(benefit > cost) {
			return true;
		}
		
			return false;
	
	}
	private static boolean isInteger(String s) {
		s = s.trim();
		return s.matches("-?\\d+");
		}
	
	private boolean isEthical(Scanner s, SentenceCreator sent) {
		String query = sent.createSentence(new String[] {"would", "was"}, "the action", 
				"be ", "voluntary? enter y for yes n for no");
		boolean comparedAns1 = compareAnswer(query, new String[] {"y", "n"}, s);
		
	    query = sent.createSentence(new String[] {"would", "was"}, "the action", 
				"be ", "on a moral agent? enter y for yes n for no");
		boolean comparedAns2 = compareAnswer(query, new String[] {"y", "n"}, s);
		
		query = sent.createSentence(new String[] {"would", "did"}, "the action", 
				"", "have an effect on society? enter y for yes n for no");
		boolean comparedAns3 = compareAnswer(query, new String[] {"y", "n"}, s);
		
		if(comparedAns1 && comparedAns2 && comparedAns3) {
			return true;
		}
		return false;	
		
	}
	private String checkAnswer(String query, String expectedAnswers[], Scanner s) {
		boolean checkAnswer = false;
		String answer = "";
		while(!checkAnswer) {
			System.out.println(query);
			answer = s.nextLine();
			for(int i = 0; i < expectedAnswers.length; i++) {
				if(answer.equals(expectedAnswers[i])) {
					checkAnswer = true;
					break;
				}
			}
			if(!checkAnswer) {
				System.out.println("You did not enter a valid answer please try again.");
			}
			
		}
		return answer;
		
	}
	
	private boolean compareAnswer(String query, String expectedAnswers[], Scanner s) {
		String answer = checkAnswer(query, expectedAnswers, s);
		
		if(answer.equals(expectedAnswers[0])) {
			return true;
		}
		else {
			return false;
		}
	}
	
}
