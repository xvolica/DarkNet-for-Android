package x.leon;

public class Result {

	private int left;
	private int top;
	private int right;
	private int bot;
	private float confidence;
	private String label;

	public Result(int left, int top, int right, int bot, float confidence, String label) {
		this.left = left;
		this.top = top;
		this.right = right;
		this.bot = bot;
		this.confidence = confidence;
		this.label = label;
	}

	public int getLeft() {
		return left;
	}

	public int getTop() {
		return top;
	}

	public int getRight() {
		return right;
	}

	public int getBot() {
		return bot;
	}

	public float getConfidence() {
		return confidence;
	}

	public String getLabel() {
		return label;
	}

}
