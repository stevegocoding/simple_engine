#include <boost/shared_array.hpp>
#include <boost/make_shared.hpp>
#include <jni.h>
#include <android/log.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android_native_app_glue.h>
#include "app_android_native.h"
#include "app.h"
#include "log.h"


GlobalsBasePtr g_globals;
struct android_app *g_android_app = NULL;  

static int s_red_size = 0; 
static int s_green_size = 0; 
static int s_blue_size = 0; 
static int s_alpha_size = 0; 
static int s_depth_size = 24; 
static int s_stencil_size = 0; 
static int s_csaa = 0; 
static boost::shared_array<int> s_val = boost::shared_array<int>(new int[1]); 


class JavaVMScope
{
public: 
	
	JavaVMScope()
	{
		if (g_android_app)
		{
			vm = g_android_app->activity->vm;
			env = g_android_app->activity->env; 
			vm->AttachCurrentThread(&env, NULL); 
		}
	}
	
	~JavaVMScope()
	{
		vm->DetachCurrentThread(); 
	}
	
	JavaVM *vm;
	JNIEnv *env; 
};


void get_window_infos(int& width, int& height, int& rotation)
{
	JavaVMScope scope;
	JNIEnv *env = scope.env; 
	
	jclass activity_class = env->GetObjectClass(g_android_app->activity->clazz); 
	jclass window_manage_class = env->FindClass("android/view/WindowManager"); 
	jclass display_class = env->FindClass("android/view/Display"); 
	
	jmethodID getWindowManagerID = env->GetMethodID(activity_class, "getWindowManager", "()Landroid/view/WindowManager;"); 
	jmethodID getDefaultDisplayID = env->GetMethodID(window_manage_class, "getDefaultDisplay", "()Landroid/view/Display;");
    jmethodID getWidthID = env->GetMethodID(display_class, "getWidth", "()I");
    jmethodID getHeightID = env->GetMethodID(display_class, "getHeight", "()I");
    jmethodID getRotationID = env->GetMethodID(display_class, "getRotation", "()I");
    
    jobject windowManager = env->CallObjectMethod(g_android_app->activity->clazz, getWindowManagerID);
    jobject display = env->CallObjectMethod(windowManager, getDefaultDisplayID);
    width = env->CallIntMethod(display, getWidthID);
    height = env->CallIntMethod(display, getHeightID);
    rotation = env->CallIntMethod(display, getRotationID);
} 

int find_config_attrib(EGLDisplay dpl, EGLConfig cfg, int attrib, int default_val)
{
	if (eglGetConfigAttrib(dpl, cfg, attrib, s_val.get()))
	{
		return s_val[0];
	}
	return default_val; 
}

//////////////////////////////////////////////////////////////////////////////////////////////

AndroidNativeApp::AndroidNativeApp(const CreationSettings& settings)
	: App(settings)
{
	m_platform_info = _create_platform_info(); 
}

AndroidNativeApp::~AndroidNativeApp()
{
	
}

platforminfo_ptr AndroidNativeApp::_create_platform_info()
{
	platforminfo_ptr platform_info = boost::make_shared<AndroidPlatformInfo>();
	return platform_info; 
}

int AndroidNativeApp::init()
{
	int ret = App::init();
	return ret; 
}

int AndroidNativeApp::_create_gles_context(const CreationSettings& settings)
{
	const EGLint config_attribs_default[] = 
	{
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, 
		EGL_RED_SIZE, 4, 
		EGL_GREEN_SIZE, 4, 
		EGL_BLUE_SIZE, 4, 
		EGL_NONE
	};
	
	EGLint w, h, dummy, format; 
	EGLint num_configs = 100;
	
	EGLSurface surface; 
	EGLContext context; 
	
	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	
	if (display == EGL_NO_DISPLAY)
	{
		Log::info("Unable to eglGetDisplay"); 
		return 1; 
	}
	
	eglInitialize(display, 0, 0);
		
    Log::info("EGL Vendor %s", eglQueryString(display, EGL_VENDOR));
    Log::info("EGL Version: %s", eglQueryString(display, EGL_VERSION));
	
    switch(settings.pixel_size)
    {
    case 32: 
    	s_red_size = s_green_size = s_blue_size = s_alpha_size = 8;
    	break; 
    
    case 24:
    	s_red_size = s_green_size = s_blue_size = 8; 
    	s_alpha_size = 0; 
    	break; 
    	
    default: 
    	s_red_size = 5; 
    	s_green_size = 6; 
    	s_blue_size = 5; 
    	s_alpha_size = 0; 
    }
    
    s_depth_size = settings.depthbuf_size;
    s_stencil_size = 8; 
    s_csaa = 4; 
    
	EGLConfig *configs = new EGLConfig[num_configs];
	
	// Choose the default config for now 
	eglChooseConfig(display, config_attribs_default, configs, num_configs, &num_configs); 
	
	EGLConfig selected_config = _choose_egl_cfg(display, configs, num_configs);
	
	int rotation; 
	get_window_infos(w, h, rotation); 
	
	Log::info("window: %dx%d rotation=%d", w, h, rotation);

	ANativeWindow_setBuffersGeometry(g_android_app->window, w, h, format);
	
	surface = eglCreateWindowSurface(display, selected_config, g_android_app->window, NULL); 
	
	const EGLint context_attribs[] = 
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	
	context = eglCreateContext(display, selected_config, NULL, context_attribs); 
	if (context == EGL_NO_CONTEXT)
	{
		Log::info("eglCreateContext() FAILED!");
		return 1; 
	}
	
	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
	{
		Log::info("eglMakeCurrent() FAILED!");
		return 1; 
	}
	
	eglQuerySurface(display, surface, EGL_WIDTH, &w); 
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);
	
	
	AndroidPlatformInfo& pi = static_cast<AndroidPlatformInfo&>(*(get_platform_info())); 
	pi.eglDisplay = (void*)display;
	pi.eglContext = (void*)context; 
	pi.eglWindowSurface = (void*)surface; 
		
	return 0;
} 

EGLConfig AndroidNativeApp::_choose_egl_cfg(EGLDisplay display, EGLConfig *configs, int num_configs)
{
	
	Log::info("Choosing EGL Config");
	EGLConfig *valid_cfgs = new EGLConfig[num_configs];
	int valid_cfg_count = 0;
	
	while (valid_cfg_count == 0)
	{
		int max_available_depth = 0; 
		
		// 
		for (int i = 0; i < num_configs; ++i)
		{
			EGLConfig cfg = configs[i];
			
			int d = find_config_attrib(display, cfg, EGL_DEPTH_SIZE, 0); 
			int s = find_config_attrib(display, cfg, EGL_STENCIL_SIZE, 0);
			
			// We want an *exact* match for red/green/blue/alpha
			int r = find_config_attrib(display, cfg, EGL_RED_SIZE, 0); 
			int g = find_config_attrib(display, cfg, EGL_GREEN_SIZE, 0); 
			int b = find_config_attrib(display, cfg, EGL_BLUE_SIZE, 0); 
			int a = find_config_attrib(display, cfg, EGL_ALPHA_SIZE, 0); 
			
			if (r == s_red_size && g == s_green_size && b == s_blue_size && a == s_alpha_size)
			{
				if (s < s_stencil_size)
					continue; 
				
				if (d < s_depth_size)
				{
					max_available_depth = std::min(d, max_available_depth); 
					continue;
				}
				
				valid_cfgs[valid_cfg_count++] = cfg; 
			}
		}
		
		// fall back
        if(valid_cfg_count == 0)
        {
            Log::info("Selected Depth=%d  not available!\n", s_depth_size);
            s_depth_size = max_available_depth;
            Log::info("Available Depth=%d !\n", s_depth_size);
        }
	}
	
	if (valid_cfg_count == 0)
		return NULL;
	
    //choose the best config according to CSAA requirement
    EGLConfig best_cfg = valid_cfgs[0];
    /*
    int minDistanceCSAA = 0xFFFFFFFF;
    for (int i = 0; i < validConfigCount; ++i) 
    {
        EGLConfig config = validConfigs[i];
        int csaa = (hasCoverageSampleNV ? (findConfigAttrib(display, config, EGL_COVERAGE_BUFFERS_NV, 0) * findConfigAttrib(display, config, EGL_COVERAGE_SAMPLES_NV, 0)) : 0);
        if (mCSAA == csaa) //best config
        { 
            return config;
        } 
        else if (csaa > 0 && abs(csaa - mCSAA) < minDistanceCSAA)  //choose the nearest value 
        { 
            bestConfig = config;
            minDistanceCSAA = abs(csaa - mCSAA);
        }
    }
    */ 
    return best_cfg; 
}


void AndroidNativeApp::destroy()
{
	App::destroy();
}

void AndroidNativeApp::update()
{
	App::update();
}



//////////////////////////////////////////////////////////////////////////////////////////////


void init_globals()
{
	if (g_globals == GlobalsBasePtr())
	{
		g_globals.reset(new GlobalsBase());
	}
}

GlobalsBasePtr get_globals()
{
	return g_globals;
}

void destroy_globals()
{
	g_globals.reset();
} 

App* CreateApp()
{
	CreationSettings settings;
	settings.wnd_pos_x = 100; 
	settings.wnd_pos_y = 100; 
	settings.wnd_width = 1024; 
	settings.wnd_height = 768; 

	AndroidNativeApp *app = new AndroidNativeApp(settings); 
	
	return app;
}

App* get_app() 
{
	assert(g_globals);
	return g_globals->app; 
}


/*=========================
 Application Command Handler 
 =========================*/
void android_on_app_cmd(struct android_app* app, int32_t cmd) 
{
	
}

int32_t android_on_input_event(struct android_app* and_app, AInputEvent* event) 
{
	AndroidNativeApp *app = static_cast<AndroidNativeApp*>(and_app->userData);
	if (!app)
		return 0; 
	
	if (AInputEvent_getType(event) != AINPUT_EVENT_TYPE_MOTION)
		return 0; 
	
	
	
	return 1; 
}

void android_main(struct android_app *and_app)
{
	app_dummy();

	// Initialize the globals
	init_globals();
	
	App *app = CreateApp();
	get_globals()->app = app;
	
	g_android_app = and_app; 

	and_app->userData = app;
	and_app->onAppCmd = android_on_app_cmd;
	and_app->onInputEvent = android_on_input_event; 
	
	int result = 0; 
	if (app->init() == RESULT_OK)
	{
		result = app->run(); 
	}
	app->destroy(); 
	
	SAFE_DEL(app); 
}
