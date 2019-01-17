import java.util.*;

class WordCount {
	public WordCount() {
		List<String> words = new ArrayList<String>();
		Scanner console = new Scanner(System.in);

		while (console.hasNextLine()) {
			String line = console.nextLine();
			Scanner tokenizer = new Scanner(line);
			while (tokenizer.hasNext()) {
				String word = tokenizer.next();
				words.add(word);
			}
			tokenizer.close();
		}
		console.close();
		countWordFrequencies(words);
	}

	void countWordFrequencies(List<String> words) {
		// YOUR CODE GOES HERE
		/* Code you may need
		   Set<Map.Entry<String, Integer>> wordSet = frequencyCount.entrySet();
		   List<Map.Entry<String, Integer>> wordList = new ArrayList<Map.Entry<String, Integer>>(wordSet);
		 */
	}

	public static void main(String args[]) {
		new WordCount();
	}
}
