import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;

public class EthicalOracle {
	private boolean hypothetical;
	private HashMap<String, Integer> benefitList; // Keeps track of benefits
	private HashMap<String, Integer> negativeList; // Keeps track of consequences
	private boolean hasUniversalRule; // Keeps track if there is a universal rule
	private ArrayList<String> ethicalList; // Keeps track what is ethical
	private ArrayList<String> unethicalList; // Keeps track what is not ethical
	private boolean actU; // Keeps track result of actU
	/**
	 * Sets up the Oracle and runs the program.
	 */
	public EthicalOracle() {
		benefitList = new HashMap<String, Integer>();
		negativeList = new HashMap<String, Integer>();
		
		ethicalList = new ArrayList<String>();
		unethicalList = new ArrayList<String>();
		
		hasUniversalRule = false;
		actU = false;
		run();
	}
	/**
	 * Main driver function for program
	 */
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
			actUtil(inputScanner);
			kantianism(inputScanner, sentenceCreation);
			ruleU(inputScanner, sentenceCreation);
			socialContract(inputScanner, sentenceCreation);
			virtue(inputScanner, sentenceCreation);
			printResults();
			
		}
		else { // If the action is not an ethical problem stop program.
			System.out.println("Since this problem is not ethical the oracle can not provide an analysis.");
		}
		inputScanner.close();
		
	}
	/**
	 * Evaluates the total values of all keys in a hashmap.
	 * @param h- hashmap to evaluate
	 * @return the total value of all keys.
	 */
	private int evaluate(HashMap<String, Integer> h) {
		
		int total = 0;
		for(String key : h.keySet() ) {
			total += h.get(key);
		}
		return total;
	}
	/**
	 * Scans user input for rule utilitarianism analysis (Makes sure it enforces format)
	 * @param s - Scanner
	 * @param line - Line to scan
	 * @param h - Hashmap to add to
	 */
	private void scanUtil(Scanner s, String line, HashMap<String, Integer> h) {
		Scanner utilScanner = new Scanner(line);
		utilScanner.useDelimiter(":");
		if(line.matches("[\\w\\s]+:\\s*\\d+")) { // Enforces format of user input.	
			if(utilScanner.hasNext()) {
				String benefitItem = utilScanner.next();
				String value = utilScanner.next();
				value = value.trim(); // Removes whitespace
			
				int valueInt = Integer.parseInt(value);
				h.put(benefitItem, valueInt); // Puts value in hashmap
		
			}
		
		}
		else { // If format incorrect try again
			System.out.println("You did not enter with proper format (format (name: value) exclude parentheses) value must be an integer");
			line = s.nextLine();
			scanUtil(s, line, h);
		}
		
	}
	/**
	 * Act Utilitarian Analysis
	 * @param s - Scanner for user input
	 */
	private void actUtil(Scanner s) {
		
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
		
		int benefit = evaluate(benefitList); // Calculates totals of negative and benefits.
		int cost = evaluate(negativeList);
		
		if(benefit >= cost) {
			//System.out.println("Act Utilitarianism says that the scenario is ethical");
			ethicalList.add("Act Utilitarianism");
			actU = true;
		}
		else {
			//System.out.println("Act Utilitarianism says that the scenario is not ethical");
			unethicalList.add("Act Utilitarianism");
		}
			
	
	}
	/**
	 * Calculates Kantianism Analysis
	 * @param s - Scanner for user input
	 * @param sent - Creates sentence for past/present
	 */
	private void kantianism(Scanner s, SentenceCreator sent) {
		int kantCount = 0; // Keeps track if there was any strikes against Kantianism
		
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
			//System.out.println("According to Kantianism the scenario is unethical");
			unethicalList.add("Kanitanism");
		}
		else {
			//System.out.println("According to Kantianism the scenario is ethical");
			ethicalList.add("Kantianism");
		}
		
		
		
		
		
	}
	/**
	 * Calculates ruleU
	 * @param s - Scanner for user input
	 * @param sent - Creates sentences for past/present
	 */
	private void ruleU(Scanner s, SentenceCreator sent) {
		int ruleUCount = 0;
		System.out.println("\nWe will now calculate the Rule Utilitarian analysis");
		if(hasUniversalRule) {
			String query = sent.createSentence(new String[] {"hypothetically, would", "would"}, "the universal rule given previously", "", 
					"lead to the greatest happiness for the greatest number? y for yes n for no");
			boolean answer = compareAnswer(query, new String[] {"y", "Y", "n", "N"}, s);
			if(!answer) {
				ruleUCount += 1;
			}
			
			if(ruleUCount >=1) {
				unethicalList.add("Rule Utilitarianism");
				//System.out.println("According to Rule U the action is unethical");
				
			}
			else {
				//System.out.println("According to Rule U the action is ethical");
				ethicalList.add("Rule Utilitarianism");
			}
			
		}
		else {
			System.out.println("Since there is no universal rule, the result of act U will be used to determine the result");
			if(ethicalList.contains("Act Utilitarianism")) {
				ethicalList.add("Rule Utilitarianism");
			}
			else {
				unethicalList.add("Rule Utilitarianism");
			}
		}
		
	}
	/**
	 * Calculates the social contract
	 * @param s - Scanner for user input
	 * @param sent - sentence generator
	 */
	
	private void socialContract(Scanner s, SentenceCreator sent) {
		System.out.println("\nWe will now calculate the Social Contract Analysis");
		String query = sent.createSentence(new String[] {"would", "did"}, "the action", "", 
				"break the laws/rules of the society? y for yes n for no");
		boolean answer = compareAnswer(query, new String[]{"y", "Y", "n", "N"}, s);
		
		if(answer) {
			//System.out.println("According to Social Contract Theory the action is unethical");
			unethicalList.add("Social Contract Theory");
		}
		else {
			//System.out.println("According to Social Contract Theory the action is ethical");
			ethicalList.add("Social Contract Theory");
		}
		
	}
	// Prints results of the utilitarian analysis
	private String printUtil(HashMap<String, Integer> h, String type) {
		String s = "";
		s += (type + ":\n");
		for(String key : h.keySet()) {
			s += key + ": " + h.get(key) + "\n";
		}
		
		return s;
	}
	// Prints results of the program
	private void printResults() {
		int ethCount = ethicalList.size();
		int unethCount = unethicalList.size();
		
		String result = "";
		
		if(ethCount >= unethCount) {
			result = "The action you are thinking about is ethical, the majority of ethical approaches ruled in favor of the action\n";
		}
		else {
			result = "The action you are thinking about is not ethical, the majority of ethical approaches ruled against the action\n";
		}
		
		result += "THEORIES FOR THE ACTION: \n";
		
		for(String item : ethicalList) {
			
			result += item + "\n";
			
			if(item.equals("Act Utilitarianism")) {
				result += "-----------Act Util Description-----------\n";
				result += printUtil(benefitList, "\nBENEFITS");
				result += printUtil(negativeList, "\nCONSEQUENCES");
				int benefits = evaluate(benefitList);
				int cons = evaluate(negativeList);
				
				result += "Total (benefits - consequences): " + (benefits - cons) + "\n";
				result += "\n----------------------------\n";
			}
		}
		result += "THEORIES AGAINST THE ACTION: \n";
		for(String item :unethicalList ) {
			
			result += item + "\n";
			
			if(item.equals("Act Utilitarianism")) {
				result += "-----------Act Util Description-----------\n";
				result += printUtil(benefitList, "BENEFITS");
				result += printUtil(negativeList, "CONSEQUENCES");
				
				int benefits = evaluate(benefitList);
				int cons = evaluate(negativeList);
				
				result += "\nTotal (benefits - consequences): " + (benefits - cons) + "\n";
				result += "\n----------------------------------------\n";
			}
		}
		System.out.println(result);
	}
	/**
	 * Calculates virtue
	 * @param s - Scanner for user input
	 * @param sent - Creates sentences
	 */
	private void virtue(Scanner s, SentenceCreator sent) {
		System.out.println("\nWe will now perform the virtue ethics analysis");
		String query = sent.createSentence(new String[] {"would a virtuous person", "if the person was virtuous would they"}, "perform this action?", "", "y "
				+ "for yes n for no");
		boolean answer = compareAnswer(query, new String[]{"y", "Y", "n", "N"}, s);
		if(answer) {
			//System.out.println("According to Virtue ethics the action is ethical");
			ethicalList.add("Virtue Ethics");
		}
		else {
			//System.out.println("According to Virtue Ethics the action is unethical");
			unethicalList.add("Virtue Ethics");
		}
		
	}
	/**
	 * Checks if the problem is ethical or not
	 * @param s - scanner for input
	 * @param sent - sentence generator
	 * @return - True if an ethical issue, false if not
	 */
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
	/**
	 * Makes sure the answer is part of the expected answers.
	 * @param query - Question to print
	 * @param expectedAnswers - List of possible expected answers
	 * @param s - Scanner for s
	 * @return - the given answer
	 */
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
	/**
	 * Prints out true/false if the answer was correct
	 * @param query - Given string
	 * @param expectedAnswers - possible answers
	 * @param s - user input
	 * @return - true if in expected answer otherwise false
	 */
	private boolean compareAnswer(String query, String expectedAnswers[], Scanner s) {
		String answer = checkAnswer(query, expectedAnswers, s);
		
		if(answer.equals(expectedAnswers[0]) || answer.equals(expectedAnswers[1])) {
			return true;
		}
		else {
			return false;
		}
	}
	/**
	 * Used for one function that does not require a query (Same function as rest)
	 * @param expectedAnswers - possible solutions
	 * @param s - scanner
	 * @return - return verifiedString
	 */
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
