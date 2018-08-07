package x.leon;

import android.content.Context;
import android.os.Environment;
import android.util.Log;

import java.io.File;

/**
 * Created by Leon on 2018/7/26.
 */

public class DarknetDao {

    private static String PATH = Environment.getExternalStorageDirectory()+ File.separator + "XDarknet" + File.separator;
    static {
        System.loadLibrary("native-lib");
    }
    private static DarknetDao api;

    public static DarknetDao getInstance(Context context) {
        synchronized (DarknetDao.class) {

            if (api == null) {
                copyFiles(context);
                api = new DarknetDao();
                boolean rst = api.load(PATH+"coco.names", PATH+"yolov3.cfg",
                        PATH+"yolov3.weights");
                Log.e("Darknet","load "+rst);
            }
            return api;
        }
    }

    //darknet.exe detector test data/coco.data yolov3.cfg yolov3.weights -i 0 -thresh 0.25 dog.jpg -ext_output
    private static void copyFiles(Context context){
        /*File file = new File(PATH+"yolov3.cfg");
        if(file.exists()) {
            return;
        }
        new File(PATH).mkdirs();
        UtilFile.CopyDataToSdcard(context,"coco.names",PATH+"coco.names");
        UtilFile.CopyDataToSdcard(context,"yolov3.weights",PATH+"yolov3.weights");
        UtilFile.CopyDataToSdcard(context,"yolov3.cfg",PATH+"yolov3.cfg");*/
        File file = new File(PATH+"yolov3tiny.cfg");
        if(file.exists()) {
            return;
        }
        new File(PATH).mkdirs();
        UtilFile.CopyDataToSdcard(context,"coco.names",PATH+"coco.names");
        UtilFile.CopyDataToSdcard(context,"yolov3.weights",PATH+"yolov3-tiny.weights");
        UtilFile.CopyDataToSdcard(context,"yolov3.cfg",PATH+"yolov3tiny.cfg");
    }

    public native Result[] inference(String pathToRawFile);

    private native boolean load(String name_list_file, String cfgfile, String weightfile);

    public native boolean unload();
}
