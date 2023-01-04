#ifndef ZEGOVideoCapture_h
#define ZEGOVideoCapture_h
#include "video_format.h"
namespace AVE {
  
    typedef VideoDataFormat VideoCaptureFormat;
    
    class SupportsVideoCapture {
    public:
        /// \brief 设置采集帧率
        /// \param framerate 帧率，一般为10，15，20，30
        /// \note SDK SetVideoFPS异步调用，透传该方法入参
        /// \note 可以不实现
        virtual int SetFrameRate(int framerate) = 0;
        
        /// \brief 设置采集分辨率，采集的分辨率最大不能超过1920*1080
        /// \param width 宽
        /// \param height 高
        /// \note SDK SetVideoResolution异步调用，透传该方法入参
        /// \note 可以不实现
        virtual int SetResolution(int width, int height) = 0;
        
        /// \brief 切换前后摄像头，移动端专用，PC端不需要实现
        /// \param bFront true表示前摄像头，false表示后摄像头
        /// \note SDK SetFrontCam异步调用，透传该方法入参
        /// \note 可以不实现
        virtual int SetFrontCam(int bFront) = 0;
        
        /// \brief 设置采集使用载体
        /// \param view 跨平台预览载体指针
        /// \note SDK SetPreviewView同步调用，透传该方法入参
        /// \note 可以不实现
        virtual int SetView(void *view) = 0;
        
        /// \brief 设置采集预览模式
        /// \param nMode 取值参考ZegoVideoViewMode
        /// \note SDK SetPreviewViewMode异步调用，透传该方法入参
        /// \note 可以不实现
        virtual int SetViewMode(int nMode) = 0;
        
        /// \brief 设置采集预览的逆时针旋转角度
        /// \param nRotation 值为0,90,180,270
        /// \note SDK SetDisplayRotation 异步调用，主要用于修复移动端的横竖屏旋转问题
        /// \note 可以不实现
        virtual int SetViewRotation(int nRotation) = 0;
        
        /// \brief 设置采集buffer的顺时针旋转角度
        /// \param nRotation 值为0,90,180,270
        /// \note SDK SetDisplayRotation 异步调用，主要用于修复移动端的横竖屏旋转问题
        /// \note 可以不实现
        virtual int SetCaptureRotation(int nRotation) = 0;
        
        /// \brief 启动预览
        /// \note SDK StartPreview异步调用
        /// \note 可以不实现
        virtual int StartPreview() = 0;
        
        /// \brief 停止预览
        /// \note SDK StopPreview异步调用
        /// \note 可以不实现
        virtual int StopPreview() = 0;
        
        /// \brief 打开闪光灯
        /// \param bEnable true表示打开，false表示关闭
        /// \note SDK EnableTorch异步调用
        /// \note 可以不实现
        virtual int EnableTorch(bool bEnable) = 0;
        
        /// \brief 对采集预览进行截图，完成后通过client的OnTakeSnapshot方法通知SDK
        /// \note SDK TakeSnapshotPreview异步调用
        /// \note 可以不实现
        virtual int TakeSnapshot() = 0;
        
        /// \brief 设置采集刷新率
        /// \param nFreq 刷新频率
        /// \note 可以不实现
        virtual int SetPowerlineFreq(unsigned int nFreq) { return 0; }
    };
    
    enum VideoFillMode {
        FILL_MODE_BLACKBAR,         // * maintain aspect ratio, adds black bars if needed
        FILL_MODE_CROP,             // * crop video to fit view
        FILL_MODE_STRETCH           // * stretch video to fit view
    };
    
    enum VideoFlipMode {
        FLIP_MODE_NONE = 0,
        FLIP_MODE_X = 1 << 0,       // * 水平翻转，镜像
        FLIP_MODE_Y = 1 << 1,       // * 垂直翻转
    };

    // [top, left](bottom, right)
    struct ROIRect {
        ROIRect() {
            top = left = bottom = right = 0;
            level = 2;
        }
        
        int top;
        int left;
        int bottom;
        int right;
        int level; // [0~6]
    };
    
    class VideoCaptureCallback {
    public:
        /// \brief 通知SDK采集到视频数据，SDK会同步拷贝数据，切换到内部线程进行编码，如果缓冲队列不够，SDK会自动丢帧
        /// \param data 采集buffer指针
        /// \param length 采集buffer的长度
        /// \param frame_format 描述buffer的属性，包括宽高，色彩空间
        /// \param reference_time 采集到该帧的时间戳，用于音画同步，如果采集实现是摄像头，最好使用系统采集回调的原始时间戳，如果不是，最好是生成该帧的UTC时间戳
        /// \param reference_time_scale 采集时间戳单位，毫秒10^3，微妙10^6，纳秒10^9，精度不能低于毫秒
        /// \note 当时图像宽高和编码宽高的宽高比不一致时，默认裁剪
        virtual void OnIncomingCapturedData(const char* data,
                                            int length,
                                            const VideoCaptureFormat& frame_format,
                                            unsigned long long reference_time,
                                            unsigned int reference_time_scale) = 0;

        /// \brief 通知SDK截图成功
        /// \param image 图像数据
        virtual void OnTakeSnapshot(void* image) = 0;
        
        /// \brief 当时图像宽高和编码宽高的宽高比不一致时使用的填充模式，默认裁剪
        /// \param mode 填充模式，参考VideoFillMode
        virtual void SetFillMode(int mode) = 0;

        /// \brief 设置编码器感兴趣的区域，优化图像主观体验
        /// \param rects 区域数组，设置NULL清空配置
        /// \param count 区域个数，最大支持6个，设置0清空配置
        virtual int SetROI(ROIRect* rects, int count) = 0;
    };
    
    class VideoCaptureCVPixelBufferCallback {
    public:
        /// \brief 通知SDK采集到视频数据，SDK会同步拷贝数据，切换到内部线程进行编码，如果缓冲队列不够，SDK会自动丢帧
        /// \param data CVPixelBufferRef转换后的void*
        /// \param reference_time_ms 采集到该帧的时间戳，单位毫秒，不能超过2^52, 用于音画同步，如果采集实现是摄像头，最好使用系统采集回调的原始时间戳，如果不是，最好是生成该帧的UTC时间戳
        virtual void OnIncomingCapturedData(void* buffer,
                                            double reference_time_ms) = 0;
        
        /// \brief 当时图像宽高和编码宽高的宽高比不一致时使用的填充模式，默认裁剪
        /// \param mode 填充模式，参考VideoFillMode
        virtual void SetFillMode(int mode) = 0;
        /// \brief 用于修正图像的角度
        /// \param rotation，顺时针角度
        virtual void SetRotation(int rotation) = 0;

        /// \brief 设置编码器感兴趣的区域，优化图像主观体验
        /// \param rects 区域数组，设置NULL清空配置
        /// \param count 区域个数，最大支持6个，设置0清空配置
        virtual int SetROI(ROIRect* rects, int count) = 0;
    };
    
    class VideoCaptureSurfaceTextureCallback {
    public:
        /// \brief 获取SurfaceTexture对应的jobject
        virtual void* GetSurfaceTexture() = 0;

        /// \brief 设置编码器感兴趣的区域，优化图像主观体验
        /// \param rects 区域数组，设置NULL清空配置
        /// \param count 区域个数，最大支持6个，设置0清空配置
        /// \param timestamp_ns ROI对应帧的时间戳，单位是ns
        virtual int SetROI(ROIRect* rects, int count, unsigned long long timestamp_ns) = 0;
    };
    
    class VideoCaptureTextureCallback {
    public:
        /// \brief 通知SDK采集到视频数据，SDK会同步绘制到FBO，切换到内部线程进行编码，如果缓冲队列不够，SDK会自动丢帧
        /// \param texture_id GL_TEXTURE_2D/GL_TEXTURE_EXTERNAL_OES的纹理
        /// \param width 纹理宽，如果设置transform matrix，必须为执行transform之后的宽
        /// \param height 纹理高，如果设置transform matrix，必须为执行transform之后的高
        /// \param reference_time_ms 采集到该帧的时间戳，单位毫秒，不能超过2^52, 用于音画同步，如果采集实现是摄像头，最好使用系统采集回调的原始时间戳，如果不是，最好是生成该帧的UTC时间戳
        virtual void OnIncomingCapturedData(int texture_id, int width, int height, double reference_time_ms) = 0;
        
        /// \brief 当时图像宽高和编码宽高的宽高比不一致时使用的填充模式，默认裁剪
        /// \param mode 填充模式，参考VideoFillMode
        virtual void SetFillMode(int mode) = 0;
        
        /// \brief SDK默认垂直翻转，如果不需要，设置0，前处理的最后一步
        /// \param mode 翻转模式，参考VideoFlipMode
        virtual void SetFlipMode(int mode) = 0;
        
        /// \brief transform可以包含旋转/翻转/缩放，SDK会先执行transform，再执行其他操作
        /// \param matrix，4x4的变换矩阵
        virtual void SetTransformMatrix(float matrix[16]) = 0;
        
        /// \brief 用于修正图像的角度，rotation在transform后执行，注意不要重复计算rotation
        /// \param rotation，顺时针角度
        virtual void SetRotation(int rotation) = 0;

        /// \brief 设置编码器感兴趣的区域，优化图像主观体验
        /// \param rects 区域数组，设置NULL清空配置
        /// \param count 区域个数，最大支持6个，设置0清空配置
        virtual int SetROI(ROIRect* rects, int count) = 0;
        
        /// \brief 通知SDK纹理相关的上下文发生变化，用来清除缓存的上下文状态，比如EglContext，必须和OnIncomingCapturedData在同一个线程调用，不保证线程安全
        virtual void ResetTextureContext() = 0;
    };
    
    enum VideoCodecType {
        CODEC_TYPE_AVC_AVCC = 0,
        CODEC_TYPE_AVC_ANNEXB = 1,
        CODEC_TYPE_VP8 = 2,
        CODEC_TYPE_HEVC_AVCC = 3,
        CODEC_TYPE_HEVC_ANNEXB = 4
    };
    
    struct VideoCodecConfig {
        VideoCodecConfig() : width(0), height(0), codec_type(CODEC_TYPE_AVC_AVCC), media_side_info(0), media_side_info_size(0), is_external_clock(false), rotation(0) {
        }
        
        int width;
        int height;
        VideoCodecType codec_type;
        const char* media_side_info; // deprecated, use CEngine::SendMediaSideInfo instead
        int media_side_info_size; // deprecated, use CEngine::SendMediaSideInfo instead

        bool is_external_clock;
        int rotation; // counterclockwise degree
    };
    
    class VideoCaptureEncodedFrameCallback {
    public:
        /// \brief 通知SDK采集到编码数据，SDK会切换到内部线程进行发送
        /// \note 推荐每 2S 一个 gop，每个 I 帧必须携带 sps 和 pps，且放在最前面。仅接受 I 帧 和 P 帧，不接受 B 帧
        /// \param data 码流指针，目前只支持 H.264 码流
        /// \param length 码流长度
        /// \param codec_config 码流的信息，参考VideoCodecConfig
        /// \param b_keyframe 是否为关键帧，建议关键帧间隔在 2S 左右
        /// \param reference_time_ms 采集到该帧的时间戳，单位毫秒，不能超过2^52, 用于音画同步，如果采集实现是摄像头，最好使用系统采集回调的原始时间戳，如果不是，最好是生成该帧的UTC时间戳
        virtual void OnEncodedFrame(const char* data, int length,
                                    const VideoCodecConfig& codec_config,
                                    bool b_keyframe, double reference_time_ms) = 0;
    };
    
    enum VideoPixelBufferType {
        PIXEL_BUFFER_TYPE_UNKNOWN = 0,
        PIXEL_BUFFER_TYPE_MEM = 1 << 0,
        PIXEL_BUFFER_TYPE_CV_PIXEL_BUFFER = 1 << 1,
        PIXEL_BUFFER_TYPE_SURFACE_TEXTURE = 1 << 2,
        PIXEL_BUFFER_TYPE_GL_TEXTURE_2D = 1 << 3,
        PIXEL_BUFFER_TYPE_ENCODED_FRAME = 1 << 4,
        PIXEL_BUFFER_TYPE_I422_MEM_FOR_HIGH422 = 1 << 5,
        PIXEL_BUFFER_TYPE_GL_TEXTURE_EXTERNAL_OES = 1 << 6,
    };
    
    class VideoCaptureDeviceBase {
    public:
        class Client {
        public:
            virtual ~Client() {}
            
			/// \brief 通知SDK销毁采集回调
			/// \note 调用此方法后，将client对象置空即可，不要做delete操作
            virtual void Destroy() = 0;
            
            virtual void SetStatusReason(bool is_closed, int reason) = 0;
            
            /// \brief 获取不同采集方式对应的回调接口
            /// \note VideoCaptureDeviceBase::SupportBufferType返回PIXEL_BUFFER_TYPE_MEM时，GetInterface返回VideoCaptureCallback*
            /// \note VideoCaptureDeviceBase::SupportBufferType返回PIXEL_BUFFER_TYPE_CV_PIXEL_BUFFER时，GetInterface返回VideoCaptureCVPixelBufferCallback*
            /// \note VideoCaptureDeviceBase::SupportBufferType返回PIXEL_BUFFER_TYPE_SURFACE_TEXTURE时，GetInterface返回VideoCaptureSurfaceTextureCallback*
            /// \note VideoCaptureDeviceBase::SupportBufferType返回PIXEL_BUFFER_TYPE_GL_TEXTURE_2D时，GetInterface返回VideoCaptureTextureCallback*
            /// \note VideoCaptureDeviceBase::SupportBufferType返回PIXEL_BUFFER_TYPE_ENCODED_FRAME时，GetInterface返回VideoCaptureEncodedFrameCallback*
            /// \note VideoCaptureDeviceBase::SupportBufferType返回PIXEL_BUFFER_TYPE_I422_MEM_FOR_HIGH422时，GetInterface返回VideoCaptureCallback*
            virtual void* GetInterface() = 0;
        };
        
    public:       
		/// \brief 初始化采集使用的资源，例如启动线程，保存SDK传递的回调
		/// \param client SDK实现回调的对象，一定要保存
		/// \note SDK 第一次调用StartPublish或PlayStream时调用，
        /// \note 接口调用顺序:1、VideoCaptureFactory::Create 2、VideoCaptureDevice::AllocateAndStart
		/// \note 一定要实现
        virtual void AllocateAndStart(Client* client) = 0;
        
		/// \brief 停止并且释放采集占用的资源，同时调用client的Destroy方法，这里的client指的是AllocateAndStart传递的client
		/// \note SDK LogoutChannel时调用，如果是异步实现，实现者必须保证AllocateAndStart()和StopAndDeAllocate()在同一个线程执行
        /// \note 接口调用顺序:1、VideoCaptureDevice::StopAndDeAllocate 2、VideoCaptureFactory::Destroy
		/// \note 一定要实现
        virtual void StopAndDeAllocate() = 0;
        
		/// \brief 启动采集，采集的数据通过client对象的OnIncomingCapturedData通知SDK
		/// \note SDK StartPublish异步调用
        /// \note SDK回调StartCapture后开始推数据才有效，否则数据会被丢弃
		/// \note 一定要实现，不要做丢帧逻辑，SDK内部已经包含了丢帧策略
        virtual int StartCapture() = 0;

		/// \brief 停止采集
		/// \note SDK StopPublish异步调用
        /// \note SDK回调StopCapture后，不再接收任何采集数据
		/// \note 一定要实现
        virtual int StopCapture() = 0;
        
        /// \brief 获取采集callback的类型
        /// \note SDK会在AllocateAndStart前先调用此接口实例化对应的client，再调用AllocateAndStart
        /// \note 一定要实现
        virtual VideoPixelBufferType SupportBufferType() = 0;
        
        /// \brief 检查是否支持SupportsVideoCapture接口
        /// \note SDK会在VideoCaptureFactory::Create后调用此方法，如果返回不会空，SDK会按照SupportsVideoCapture
        /// \note 透传SDK的调用
        /// \note 一定要实现
        virtual void* GetInterface() = 0;
    };
    
    class VideoCaptureDevice : public VideoCaptureDeviceBase, public SupportsVideoCapture {
    public:
        class Client : public VideoCaptureDeviceBase::Client, public VideoCaptureCallback {
        public:
            virtual ~Client() {}
            virtual void* GetInterface() {
                return (VideoCaptureCallback*)this;
            }
        };
    
    public :
        virtual void AllocateAndStart(Client* client) = 0;
        
        virtual void AllocateAndStart(VideoCaptureDeviceBase::Client* client) override {
            this->AllocateAndStart((Client*)client);
        }
        
        virtual VideoPixelBufferType SupportBufferType() override {
            return VideoPixelBufferType::PIXEL_BUFFER_TYPE_MEM;
        }
        
        virtual void* GetInterface() override {
            return (SupportsVideoCapture*)this;
        }
    };
    
    class VideoCaptureFactory {
    public:
        virtual ~VideoCaptureFactory() {}
		/// \brief 创建采集设备
		/// \param device_id SDK SetVideoDevice透传的参数
		/// \note SDK第一次StartPublish或者PlayStream时异步调用
		/// \note 一定要实现
        virtual VideoCaptureDeviceBase* Create(const char* device_id) = 0;

		/// \brief 销毁采集设备
		/// \param vc Create方法返回的采集对象
		/// \note SDK LogoutChannel时同步调用
		/// \note 一定要实现
        virtual void Destroy(VideoCaptureDeviceBase *vc) = 0;
    };
    
    enum VideoBufferType {
        BUFFER_TYPE_UNKNOWN = 0,
        BUFFER_TYPE_MEM = 1 << 0,
        BUFFER_TYPE_ASYNC_PIXEL_BUFFER = 1 << 1,
        BUFFER_TYPE_SYNC_PIXEL_BUFFER = 1 << 2,
        BUFFER_TYPE_SURFACE_TEXTURE = 1 << 3,
        BUFFER_TYPE_HYBRID_MEM_GL_TEXTURE_2D = 1 << 4,
        BUFFER_TYPE_SYNC_GL_TEXTURE_2D = 1 << 5,
        BUFFER_TYPE_ASYNC_I420_MEM = 1 << 6,
        BUFFER_TYPE_ASYNC_I420_PIXEL_BUFFER = 1 << 7,
        BUFFER_TYPE_ASYNC_NV12_PIXEL_BUFFER = 1 << 8,
        BUFFER_TYPE_ASYNC_CM_SAMPLE_BUFFER = 1 << 9,
        BUFFER_TYPE_SYNC_CM_SAMPLE_BUFFER = 1 << 10,
        BUFFER_TYPE_SYNC_BGRA32_MEM = 1 << 11
    };

    class VideoBufferPool {
    public:
        virtual int DequeueInputBuffer(int width, int height, int stride) = 0;
        virtual void* GetInputBuffer(int index) = 0;
        virtual void QueueInputBuffer(int index, int width, int height, int stride, unsigned long long timestamp_100n) = 0;
    };
    
    class VideoFilterCallback {
    public:
        virtual void OnProcess(void* buffer, int size, int width, int height, int stride, unsigned long long timestamp_100n) = 0;
    };

    class VideoFilterMemCallback {
    public:
        virtual void OnProcessMem(unsigned char **pData, int* dataLen, int width, int height, int stride[4], unsigned long long timestamp_100n) = 0;
    };
    
    class VideoFilter {
    public:
        class Client {
        public:
            virtual ~Client() {}
            virtual void Destroy() = 0;
            
            /// \brief 获取不同滤镜处理模型对应的接口
            /// \note VideoFilter::SupportBufferType返回BUFFER_TYPE_MEM时，GetInterface返回VideoBufferPool*，
            /// \note VideoBufferPool::GetInputBuffer需要返回uint8_t*, 色彩空间windows、ios、mac为BGRA，android为RGBA，线程模型为异步
            /// \note VideoFilter::SupportBufferType返回BUFFER_TYPE_ASYNC_PIXEL_BUFFER时，GetInterface返回VideoBufferPool*,
            /// \note VideoBufferPool::GetInputBuffer需要返回CVPixelBufferRef, 色彩空间为BGRA，线程模型为异步
            /// \note VideoFilter::SupportBufferType返回BUFFER_TYPE_SYNC_PIXEL_BUFFER时，GetInterface返回VideoFilterCallback*,
            /// \note 色彩空间为BGRA，线程模型为同步
            /// \note VideoFilter::SupportBufferType返回BUFFER_TYPE_SURFACE_TEXTURE时，GetInterface返回VideoBufferPool*
            /// \note VideoBufferPool::GetInputBuffer需要返回SurfaceTexture对应的jobject, 线程模型为异步
            /// \note VideoFilter::SupportBufferType返回BUFFER_TYPE_HYBRID_MEM_GL_TEXTURE_2D时，GetInterface返回VideoFilterCallback*
            /// \note VideoFilterCallback::OnProcess的入参buffer为纹理id转成的int*, 线程模型为异步
            /// \note VideoFilter::SupportBufferType返回BUFFER_TYPE_SYNC_GL_TEXTURE_2D时，GetInterface返回VideoFilterCallback*
            /// \note VideoFilterCallback::OnProcess的入参buffer为纹理id转成的int*, 线程模型为同步
            /// \note VideoFilter::SupportBufferType返回BUFFER_TYPE_ASYNC_I420_MEM时，GetInterface返回VideoBufferPool*,
            /// \note VideoBufferPool::GetInputBuffer需要返回uint8_t*, 色彩空间为I420，线程模型为异步
            /// \note VideoFilter::SupportBufferType返回BUFFER_TYPE_ASYNC_I420_PIXEL_BUFFER时，GetInterface返回VideoBufferPool*
            /// \note VideoBufferPool::GetInputBuffer需要返回CVPixelBufferRef, 色彩空间为I420，线程模型为异步
            /// \note VideoFilter::SupportBufferType返回BUFFER_TYPE_SYNC_BGRA_MEM时，GetInterface返回VideoFilterMemCallback*
            /// \note 色彩空间为BGRA，线程模型为同步
            virtual void* GetInterface() = 0;
        };
        
    public:
        /// \brief 初始化采集使用的资源，例如启动线程，保存SDK传递的回调
        /// \param client SDK实现回调的对象，一定要保存
        /// \note SDK启动摄像头时调用，触发的时机包括StartPreview、SetFrontCam、StartPublish
        /// \note 接口调用顺序:1、VideoFilterFactory::Create 2、VideoFilter::AllocateAndStart
        /// \note 一定要实现
        virtual void AllocateAndStart(Client* client) = 0;
        
        /// \brief 停止并且释放采集占用的资源，同时调用client的Destroy方法，这里的client指的是AllocateAndStart传递的client
        /// \note SDK停止使用摄像头时调用，如果是异步实现，实现者必须保证AllocateAndStart()和StopAndDeAllocate()在同一个线程执行
        /// \note 接口调用顺序:1、VideoFilter::StopAndDeAllocate 2、VideoFilterFactory::Destroy
        /// \note 一定要实现
        virtual void StopAndDeAllocate() = 0;
        
        /// \brief 获取滤镜处理类型，SDK会根据此类型，使用对应的接口传输数据
        /// \note SDK会在AllocateAndStart前先调用此接口实例化对应的client，再调用AllocateAndStart
        /// \note 一定要实现
        virtual VideoBufferType SupportBufferType() = 0;
        
        /// \brief 获取不同滤镜处理模型对应的接口
        /// \note VideoFilter::SupportBufferType返回BUFFER_TYPE_MEM时，GetInterface返回VideoBufferPool*，
        /// \note VideoBufferPool::GetInputBuffer需要返回uint8_t*, 色彩空间windows、ios、mac为BGRA，android为RGBA，线程模型为异步
        /// \note VideoFilter::SupportBufferType返回BUFFER_TYPE_ASYNC_PIXEL_BUFFER时，GetInterface返回VideoBufferPool*,
        /// \note VideoBufferPool::GetInputBuffer需要返回CVPixelBufferRef, 色彩空间为BGRA，线程模型为异步
        /// \note VideoFilter::SupportBufferType返回BUFFER_TYPE_SYNC_PIXEL_BUFFER时，GetInterface返回VideoFilterCallback*,
        /// \note 色彩空间为BGRA，线程模型为同步
        /// \note VideoFilter::SupportBufferType返回BUFFER_TYPE_SURFACE_TEXTURE时，GetInterface返回VideoBufferPool*
        /// \note VideoBufferPool::GetInputBuffer需要返回SurfaceTexture对应的jobject, 线程模型为异步
        /// \note VideoFilter::SupportBufferType返回BUFFER_TYPE_HYBRID_MEM_GL_TEXTURE_2D时，GetInterface返回VideoBufferPool*
        /// \note VideoBufferPool::GetInputBuffer需要返回uint8_t*, 色彩空间android为RGBA，线程模型为异步
        /// \note VideoFilter::SupportBufferType返回BUFFER_TYPE_SYNC_GL_TEXTURE_2D时，GetInterface返回VideoFilterCallback*
        /// \note VideoFilterCallback::OnProcess的入参buffer为纹理id转成的int*, 线程模型为同步
        /// \note VideoFilter::SupportBufferType返回BUFFER_TYPE_ASYNC_I420_MEM时，GetInterface返回VideoBufferPool*,
        /// \note VideoBufferPool::GetInputBuffer需要返回uint8_t*, 色彩空间为I420，线程模型为异步
        /// \note VideoFilter::SupportBufferType返回BUFFER_TYPE_ASYNC_I420_PIXEL_BUFFER时，GetInterface返回VideoBufferPool*
        /// \note VideoBufferPool::GetInputBuffer需要返回CVPixelBufferRef, 色彩空间为I420，线程模型为异步
        virtual void* GetInterface() = 0;
    };
    
    class VideoFilterFactory {
    public:
        virtual ~VideoFilterFactory() {}
        
        /// \brief 创建滤镜
        /// \note SDK第一次StartPublish或者StartPreview时异步调用
        /// \note 一定要实现
        virtual VideoFilter* Create() = 0;
        
        /// \brief 销毁滤镜
        /// \param vf Create方法返回的滤镜对象
        /// \note 一定要实现
        virtual void Destroy(VideoFilter *vf) = 0;
    };
    
    class Camera : public VideoCaptureDeviceBase, public SupportsVideoCapture {
    public:
        static Camera* Create();
        static void Destroy(Camera* camera);
        
        virtual void SetDeviceId(const char* device_id) = 0;
    };
}
#endif /* ZEGOVideoCapture_h */
