public class SEMain {

	public native void SEJavaMain( String[] Args );

/*	public static void main( String [] Args )
	{
        Main CallFunc = new Main();
        CallFunc.SEJavaMain( Args );

	}*/

    static { System.loadLibrary("Survivor_Engine"); }
}
