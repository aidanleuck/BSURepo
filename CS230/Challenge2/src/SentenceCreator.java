
public class SentenceCreator {
	private boolean hypothetical;
	
	public SentenceCreator(boolean hyp) {
		this.hypothetical = hyp;
	}
	
	public String createSentence(String[] tenseWords, String frag1, String helpVerb, String fragEnd) {
		if(hypothetical) {
			return tenseWords[0] + " " + frag1 + " " + helpVerb + ""+ fragEnd;
		}
		return tenseWords[1] + " " + frag1 + " " + fragEnd;
	}
}
