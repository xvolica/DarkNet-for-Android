package x.leon;

import android.content.Context;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

/**
 * Created by Leon on 2017/8/1.
 */

public class UtilFile {
    public static boolean CopyDataToSdcard(Context context, String fileName, String outFileName) {
        try {
            InputStream is = context.getResources().getAssets().open(fileName);
            int size = is.available();
            byte[] buffer = new byte[size];
            is.read(buffer);
            is.close();
            File out = new File(outFileName);
            //if(!out.exists()){
            //    out.createNewFile();
            //}
            FileOutputStream fos = new FileOutputStream(out);
            fos.write(buffer);
            fos.close();
            return true;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }
}
