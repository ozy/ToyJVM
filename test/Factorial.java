public class Factorial
{
	public static void main(String[] args)
	{
		final int NUM_FACTS = 12;
		long test=0;
		for (long a=0; a<100; a++){
			for(int i = 0; i < NUM_FACTS; i++){
				test += a/factorial(i);
			}
		}
		System.out.println("a"+test);
	}
	
	public static int factorial(int n)
	{	int result = 1;
		for(int i = 2; i <= n; i++)
			result *= i;
		return result;
	}
}