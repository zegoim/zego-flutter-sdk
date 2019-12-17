package com.zego.zegoliveroomplugin.utils;

import android.content.Context;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class ZegoFileHelper {
    /**
     * 将文件从assets目录，拷贝到 /data/data/包名/files/ 目录中。assets 目录中的文件，会不经压缩打包至APK包中，使用时还应从apk包中导出来
     *
     * @param context  上下文对象
     * @param fileName 文件名,如aaa.txt
     */
    public static String copyAssetsFile2Phone(Context context, String fileName) {
        InputStream inputStream = null;
        FileOutputStream fos = null;
        try {
            // getFilesDir() 获得当前APP的安装路径 /data/data/包名/files 目录
            File file = new File(context.getFilesDir().getAbsolutePath() + File.separator + fileName);
            inputStream = context.getAssets().open(fileName);

            // 如果文件不存在（新建），或者文件大小不一致（覆盖）
            if (!file.exists() || file.length() != inputStream.available()) {
                //如果文件不存在，FileOutputStream会自动创建文件
                File folder = file.getParentFile();
                if (!folder.exists()) {
                    folder.mkdirs();
                }

                fos = new FileOutputStream(file);
                int len;
                byte[] buffer = new byte[1024 * 8];
                while ((len = inputStream.read(buffer)) != -1) {
                    fos.write(buffer, 0, len);
                }
                fos.flush();//刷新缓存区
            }
            return file.getPath();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (inputStream != null) {
                try {
                    inputStream.close();
                } catch (IOException ignore) {
                }
            }
            if (fos != null) {
                try {
                    fos.close();
                } catch (IOException ignore) {
                }
            }
        }
        return null;

    }
}
