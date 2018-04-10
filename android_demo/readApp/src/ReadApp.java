import java.io.FileInputStream;
import java.io.InputStream;


public class ReadApp {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		InputStream is = new FileInputStream("A210Aplication_V2100210_1.app"); 
		byte[] buff = new byte[40500];
		int len; 
		StringBuffer sb = new StringBuffer(); 
		while((len = is.read(buff)) != -1){ 
			for(int i = 0 ; i < len ; i++){ 
				byte b = buff[i];
				sb.append(b); 
			} 

		} 
	}

}
