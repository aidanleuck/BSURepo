import java.util.HashMap;
import java.util.Scanner;

public class EthicalOracle {
	private boolean hypothetical;
	private HashMap<String, Integer> benefitList;
	private HashMap<String, Integer> negativeList;
	private boolean hasUniversalRule;
	public EthicalOracle() {
		benefitList = new HashMap<String, Integer>();
		negativeList = new HashMap<String, Integer>();
		hasUniversalRule = false;
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
			int actAnalysis = actUtil(inputScanner);
			int kantianism = kantianism(inputScanner, sentenceCreation);
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
	private void scanUtil(Scanner s, String line, HashMap<String, Integer> h) {
		Scanner utilScanner = new Scanner(line);
		utilScanner.useDelimiter(":");
		if(line.matches("[\\w\\s]+:\\s*\\d+")) {	
			if(utilScanner.hasNext()) {
				String benefitItem = utilScanner.next();
				String value = utilScanner.next();
				value = value.trim();
			
				int valueInt = Integer.parseInt(value);
				h.put(benefitItem, valueInt);
		
			}
		
		}
		else {
			System.out.println("You did not enter with proper format (format (name: value) exclude parentheses) value must be an integer");
			line = s.nextLine();
			scanUtil(s, line, h);
		}
		
	}
	private int actUtil(Scanner s) {
		
		System.out.println("We will now determine the act utilitarian analysis");
		
		boolean benefitBool = false;
		while(!benefitBool) {
			System.out.println("Please enter a benefit (format (name: value) exclude parentheses) value must be an integer");
			String line = s.nextLine();
			scanUtil(s, line, benefitList);
			
			System.out.println("Would you like to add another benefit? y for yes n for no");
			String ans = verifyAnswer(new String[] {"y", "Y", "n", "N"}, s);
			
			if(ans.equals("N") || ans.equals("n")) {
				benefitBool = true;
			}
		}
		boolean negative = false;
		
		while(!negative) {
			System.out.println("Please enter a consequence (format (name: value) exclude parentheses) value must be an integer");
			String line2 = s.nextLine();
			scanUtil(s, line2, negativeList);
			
			System.out.println("Would you like to add another consequence?, y for yes n for no");
			String ans = verifyAnswer(new String[] {"y", "Y", "n", "N"}, s);
			
			if(ans.equals("N") || ans.equals("n")) {
				negative = true;
			}
			
			
		}
		
		int benefit = evaluate(benefitList);
		int cost = evaluate(negativeList);
		
		if(benefit >= cost) {
			System.out.println("Act Utilitarianism says that the scenario is ethical");
			return 1;
		}
			System.out.println("Act Utilitarianism says that the scenario is not ethical");
			return 0;
	
	}
	private int kantianism(Scanner s, SentenceCreator sent) {
		int kantCount = 0;
		
		System.out.println("\nWe will now perform a Kantian analysis");
		
		String query = sent.createSentence(new String[] {"hypothetically, is there", "was there"}, "a universal rule" ,"", 
				"that could have been applied? y for yes n for no");
		
		boolean answer = compareAnswer(query, new String[] {"y", "Y", "n", "N"}, s);
		hasUniversalRule = answer;
		
		if(answer) {
			
			query = "Given this universal rule, would it be okay if everyone was "
					+ "allowed to break this rule? y for yes n for no";
			answer = compareAnswer(query, new String[] {"y", "Y", "n", "N"}, s);
			if(!answer) {
				kantCount +=1;
			}
		}
		
		
		query  = "Are you treating yourself or others as means to an end, "
				+ "rather than ends in themselves? y for yes n for no ";
		answer = compareAnswer(query, new String[] {"y", "Y", "n", "N"}, s);
		if(answer) {
			kantCount +=1;
		}
		
		if(kantCount >= 1) {
			System.out.println("According to Kantianism the scenario is unethical");
			return 0;
		}
		System.out.println("According to Kantianism the scenario is ethical");
		return 1;
		
		
		
		
	}
	private int ruleU(Scanner s, SentenceCreator sent) {
		int ruleUCount = 0;
		System.out.println("We will now calculate the Rule Utilitarian analysis");
		if(hasUniversalRule) {
			String query = sent.createSentence(new String[] {"hypothetically, would, would"}, "the universal rule given last time", "", 
					"lead to the greatest happiness for the greatest number? y for yes n for no");
			boolean answer = compareAnswer(query, new String[] {"y", "Y", "n", "N"}, s);
		}
	}
	
	private boolean isEthical(Scanner s, SentenceCreator sent) {
		String query = sent.createSentence(new String[] {"would", "was"}, "the action", 
				"be ", "voluntary? enter y for yes n for no");
		boolean comparedAns1 = compareAnswer(query, new String[] {"y", "Y", "n", "N"}, s);
		
	    query = sent.createSentence(new String[] {"would", "was"}, "the action", 
				"be ", "on a moral agent? enter y for yes n for no");
		boolean comparedAns2 = compareAnswer(query, new String[] {"y", "Y", "n", "N"}, s);
		
		query = sent.createSentence(new String[] {"would", "did"}, "the action", 
				"", "have an effect on society? enter y for yes n for no");
		boolean comparedAns3 = compareAnswer(query, new String[] {"y", "Y", "n", "N"}, s);
		
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
		
		if(answer.equals(expectedAnswers[0]) || answer.equals(expectedAnswers[1])) {
			return true;
		}
		else {
			return false;
		}
	}
	
	private String verifyAnswer(String expectedAnswers[], Scanner s) {
		boolean verifyAnswer = false;
		while(!verifyAnswer) {
			String answer = s.nextLine();
			for(int i = 0; i < expectedAnswers.length; i++) {
				if(answer.equals(expectedAnswers[i])) {
					verifyAnswer = true;
					return answer;
					
				}
			}
			if(!verifyAnswer) {
				System.out.println("You did not eneter a valid answer please try again");
			}
		}
		return null;
	}
	
}
