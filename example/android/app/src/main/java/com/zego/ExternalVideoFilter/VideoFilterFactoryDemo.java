package com.zego.ExternalVideoFilter;

/**
 * Created by Patrick Fu on 2020-03-05.
 */

import com.zego.zegoavkit2.videofilter.ZegoVideoFilterFactory;
import com.zego.zegoavkit2.videofilter.ZegoVideoFilter;

import com.zego.ExternalVideoFilter.VideoFilter.*;

public class VideoFilterFactoryDemo extends ZegoVideoFilterFactory {

    public FilterType filterType = FilterType.FilterTypeSurfaceTexture;

    private ZegoVideoFilter mFilter = null;

    // 外部滤镜类型枚举
    public enum FilterType {
        FilterTypeMem,
        FilterTypeHybridMem,
        FilterTypeSyncGLTexture2D,
        FilterTypeSurfaceTexture,
        FilterTypeAsyncI420Mem
    }

    // 创建外部滤镜实例
    public ZegoVideoFilter create() {
        switch (filterType) {
            case FilterTypeMem:
                mFilter = new VideoFilterMemDemo();
                break;
            case FilterTypeHybridMem:
                mFilter = new VideoFilterHybridDemo();
                break;
            case FilterTypeSyncGLTexture2D:
                mFilter = new VideoFilterSyncGlTexture2dDemo();
                break;
            case FilterTypeSurfaceTexture:
                mFilter = new VideoFilterSurfaceTextureDemo();
                break;
            case FilterTypeAsyncI420Mem:
                mFilter = new VideoFilterAsyncI420MemDemo();
                break;
        }

        return mFilter;
    }

    // 销毁外部滤镜实例
    public void destroy(ZegoVideoFilter filter) {
        mFilter = null;
    }
}
