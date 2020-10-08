
public class SentenceCreator {
	private boolean hypothetical;
	
	public SentenceCreator(boolean hyp) {
		this.hypothetical = hyp;
	}
	/**
	 * Creates sentence past/present
	 * @param tenseWords - array of two words, [0] = hypothetical, [1] = not hypothetical
	 * @param frag1 - Fragment of the string
	 * @param helpVerb - optional helping verb
	 * @param fragEnd - End of sentence
	 * @return - A full sentence past/present depending on if hypothetical or not
	 */
	public String createSentence(String[] tenseWords, String frag1, String helpVerb, String fragEnd) {
		if(hypothetical) {
			return tenseWords[0] + " " + frag1 + " " + helpVerb + ""+ fragEnd;
		}
		return tenseWords[1] + " " + frag1 + " " + fragEnd;
	}
}
