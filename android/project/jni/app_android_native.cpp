#include <boost/shared_array.hpp>
#include <boost/make_shared.hpp>
#include <jni.h>
#include <android/log.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android_native_app_glue.h>
#include "app_android_native.h"
#include "input_manager_android.h"
#include "input_event.h"
#include "log.h"

GlobalsBasePtr g_globals;  

static int s_red_size = 0; 
static int s_green_size = 0; 
static int s_blue_size = 0; 
static int s_alpha_size = 0; 
static int s_depth_size = 24; 
static int s_stencil_size = 0; 
static int s_csaa = 0; 
static boost::shared_array<int> s_val = boost::shared_array<int>(new int[1]); 

AndroidGlobalsPtr cast_android_globals(GlobalsBasePtr globals)
{
	return boost::static_pointer_cast<AndroidGlobals>(get_globals()); 
}

AndroidPlatformInfoPtr cast_android_platform_info(platforminfo_ptr pi)
{
	return boost::static_pointer_cast<AndroidPlatformInfo>(pi); 
}


class JavaVMScope
{
public: 
	
	JavaVMScope()
	{
		AndroidGlobalsPtr globals = cast_android_globals(get_globals());
		
		if (globals->and_app)
		{
			vm = globals->and_app->activity->vm;
			env = globals->and_app->activity->env; 
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
	
	AndroidGlobalsPtr globals = cast_android_globals(get_globals());
	
	jclass activity_class = env->GetObjectClass(globals->and_app->activity->clazz); 
	jclass window_manage_class = env->FindClass("android/view/WindowManager"); 
	jclass display_class = env->FindClass("android/view/Display"); 
	
	jmethodID getWindowManagerID = env->GetMethodID(activity_class, "getWindowManager", "()Landroid/view/WindowManager;"); 
	jmethodID getDefaultDisplayID = env->GetMethodID(window_manage_class, "getDefaultDisplay", "()Landroid/view/Display;");
    jmethodID getWidthID = env->GetMethodID(display_class, "getWidth", "()I");
    jmethodID getHeightID = env->GetMethodID(display_class, "getHeight", "()I");
    jmethodID getRotationID = env->GetMethodID(display_class, "getRotation", "()I");
    
    jobject windowManager = env->CallObjectMethod(globals->and_app->activity->clazz, getWindowManagerID);
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
	App::init();
	
	_create_event_mgr(); 
	_create_input_mgr();
	
	m_event_mgr->add_listener(this); 
	
	create_context();
	
	return RESULT_OK; 
}

int AndroidNativeApp::create_context()
{
	AndroidPlatformInfoPtr platform_info = cast_android_platform_info(m_platform_info);
	if (platform_info->eglContext != EGL_NO_CONTEXT)
		return RESULT_OK;
			
	int result = _create_gles_context(m_settings);
	
	return result; 
}

void AndroidNativeApp::destroy_context()
{
	/*
	AndroidPlatformInfoPtr platform_info = cast_android_platform_info(m_platfom_info);
	if (platform_info->eglContext != EGL_NO_CONTEXT)
	{
		eglMake
	}
	*/
}

void AndroidNativeApp::destroy()
{
	App::destroy();
	
	_destroy_input_mgr(); 
	_destroy_event_mgr(); 
}

void AndroidNativeApp::update()
{
	AndroidGlobalsPtr globals = cast_android_globals(get_globals()); 
	
	App::update();
	
	int ident; 
	int events; 
	struct android_poll_source *source;
	
	// Loop until all events are read, then continue to draw the next frame of animation.
	while ((ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0)
	{
		if (source) 
		{
			source->process(globals->and_app, source);
		}
		
		// If a sensor has data, process it now
		if (ident == LOOPER_ID_USER) 
		{
			
		}
		
		// Check if we are exiting
		if (globals->and_app->destroyRequested != 0) 
		{
			m_is_running = false;
			return; 
		}
	}
	
	if (m_input_mgr)
		m_input_mgr->update_frame();

	if (m_event_mgr)
		m_event_mgr->dispatch_events();
	
	
}

int AndroidNativeApp::run()
{
	Log::info("AndroidNativeApp::run()");

	AndroidGlobalsPtr globals = cast_android_globals(get_globals()); 
	
	m_is_running = true;
	while (m_is_running)
	{
		if (globals->has_focus)
		{
			update();
			present(); 
		}
		else
		{
			update(); 
			::usleep(50*1000);
		}
	}
	
	return RESULT_OK;
}

void AndroidNativeApp::present()
{
	AndroidPlatformInfoPtr platform_info = cast_android_platform_info(m_platform_info); 
	eglSwapBuffers(platform_info->eglDisplay, platform_info->eglWindowSurface);
}

int AndroidNativeApp::_create_gles_context(const CreationSettings& settings)
{
	Log::info("_create_gles_context()");

	AndroidGlobalsPtr globals = cast_android_globals(get_globals());
	
	const EGLint config_attribs_default[] = 
	{
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, 
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8, 
		EGL_BLUE_SIZE, 8, 
		EGL_ALPHA_SIZE, 8,
		EGL_DEPTH_SIZE, 24, 
		EGL_STENCIL_SIZE, 8,
		EGL_NONE
	};
	
	EGLint w, h, dummy, format; 
	EGLint num_configs = 1;
	
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
	
    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, selected_config, EGL_NATIVE_VISUAL_ID, &format);
	
	int rotation; 
	get_window_infos(w, h, rotation); 
	
	Log::info("window: %dx%d rotation=%d", w, h, rotation);

	ANativeWindow_setBuffersGeometry(globals->and_app->window, w, h, format);
	
	surface = eglCreateWindowSurface(display, selected_config, globals->and_app->window, NULL); 
	
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

void AndroidNativeApp::_create_event_mgr()
{
	if (!m_event_mgr)
	{
		m_event_mgr = new EventManager(); 
	}
}

void AndroidNativeApp::_create_input_mgr()
{
	if (!m_input_mgr)
	{
		m_input_mgr = new InputManagerAndroid();
	}
}

void AndroidNativeApp::_destroy_event_mgr()
{
	SAFE_DEL(m_event_mgr); 
}

void AndroidNativeApp::_destroy_input_mgr() 
{
	SAFE_DEL(m_input_mgr);
}

bool AndroidNativeApp::on_event(const CoreEvent& e)
{
	switch (e.type)
	{
	case IET_TOUCHPAD_BEGAN:
		{
			const TouchPadEvent& touch_evt = static_cast<const TouchPadEvent&>(e);
			Log::info("Touch Began! x: %d, y: %d", touch_evt.m_point.x, touch_evt.m_point.y); 
		}
		break; 
	case IET_TOUCHPAD_MOVED:
		{
			const TouchPadEvent& touch_evt = static_cast<const TouchPadEvent&>(e);
			Log::info("Touch Moved! x: %d, y: %d", touch_evt.m_point.x, touch_evt.m_point.y); 
		}
		break; 
	case IET_TOUCHPAD_ENDED:
		{
			const TouchPadEvent& touch_evt = static_cast<const TouchPadEvent&>(e);
			Log::info("Touch Ended! x: %d, y: %d", touch_evt.m_point.x, touch_evt.m_point.y);  
		}
		break; 
	}
	
	return App::on_event(e);
}

//////////////////////////////////////////////////////////////////////////////////////////////


void init_globals()
{
	if (g_globals == GlobalsBasePtr())
	{
		g_globals.reset(new AndroidGlobals());
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

App* get_app() 
{
	assert(g_globals);
	return g_globals->app; 
}

App* CreateApp()
{
	CreationSettings settings;
	settings.wnd_pos_x = 100;
	settings.wnd_pos_y = 100;
	settings.wnd_width = 1024;
	settings.wnd_height = 768;
	settings.pixel_size = 32; 

	AndroidNativeApp *app = new AndroidNativeApp(settings);

	return app;
}

int InitApp()
{
	int result = 0; 
	App *android_app = get_globals()->app; 
	if (android_app)
	{
		result = android_app->init(); 
	}
	return result; 
}

int RunApp()
{	
	int result = 0; 
	App *android_app = get_globals()->app; 
	if (android_app)
	{
		result = android_app->run(); 
	}	
	return result; 
}

void DestroyApp()
{
	int result = 0; 
	App *android_app = get_globals()->app; 
	if (android_app)
	{
		android_app->destroy(); 
		SAFE_DEL(android_app);
		get_globals()->app = NULL;
	} 
}

/*=========================
 Application Command Handler 
 =========================*/
void android_on_app_cmd(struct android_app* app, int32_t cmd) 
{
	AndroidGlobalsPtr globals = cast_android_globals(get_globals()); 
	AndroidNativeApp *and_app = (AndroidNativeApp*)app->userData; 
	if (and_app == NULL)
		return; 
	
	switch (cmd)
	{
		case APP_CMD_SAVE_STATE:
		{
			Log::info("APP_CMD_SAVE_STATE"); 
			break; 
		}
		
		case APP_CMD_INIT_WINDOW:
		{
			Log::info("APP_CMD_INIT_WINDOW"); 
			if (app->window != NULL)
			{
				if (and_app)
				{
					if (!globals->is_app_initialized)
					{
						and_app->init();
						globals->is_app_initialized = true; 
					}
					else 
					{
						and_app->create_context();
					}
				}
			}
			break;
		}
		
		case APP_CMD_TERM_WINDOW:
		{
			Log::info("APP_CMD_TERM_WINDOW"); 
			// and_app->destroy_context();
			break;
		}
		
		case APP_CMD_GAINED_FOCUS:
        case APP_CMD_LOST_FOCUS:
        {
        	globals->has_focus = (cmd == APP_CMD_GAINED_FOCUS);
        	
        	break;
        }
        
        case APP_CMD_INPUT_CHANGED:
        {
        	break; 
        }
	}
}

int32_t android_on_input_event(struct android_app* and_app, AInputEvent* event) 
{	
	AndroidGlobalsPtr globals = cast_android_globals(get_globals()); 
	if (globals->app == NULL)
		return 0; 
	
	if (AInputEvent_getType(event) != AINPUT_EVENT_TYPE_MOTION)
		return 0; 
	
	const int action = AMotionEvent_getAction(event);
	const int mask = (action & AMOTION_EVENT_ACTION_MASK); 
	const int count = AMotionEvent_getPointerCount(event);
	const int pointer_id = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

	if (action == AMOTION_EVENT_ACTION_DOWN)
	{
		Log::info("------ACTION_DOWN---------");
		globals->app->get_input_mgr()->get_gamepad(0).
				raise_touch_event(IET_TOUCHPAD_BEGAN, 0, 
						(int)AMotionEvent_getX(event, 0), 
						(int)AMotionEvent_getY(event, 0));
	}
	
	if (mask == AMOTION_EVENT_ACTION_POINTER_DOWN)
	{
		Log::info("------ACTION_POINTER_DOWN---------");
		globals->app->get_input_mgr()->get_gamepad(0).
				raise_touch_event(IET_TOUCHPAD_BEGAN, pointer_id, 
						(int)AMotionEvent_getX(event, pointer_id), 
						(int)AMotionEvent_getY(event, pointer_id));
	}
	
	if (action == AMOTION_EVENT_ACTION_MOVE)
	{
		Log::info("------ACTION_MOVE---------");
		for (int i = 0; i < count; ++i)
		{
			const int id = AMotionEvent_getPointerId(event, i);
			const int x = (int)AMotionEvent_getX(event, id); 
			const int y = (int)AMotionEvent_getY(event, id); 
		
			Log::info("------ACTION_MOVE for loop id %d: %dx%d ---------", id, x, y);
			globals->app->get_input_mgr()->get_gamepad(0).
					raise_touch_event(IET_TOUCHPAD_MOVED, id, x, y);
		}
	}
	
	if (mask == AMOTION_EVENT_ACTION_POINTER_UP)
	{
		Log::info("------ACTION_POINTER_UP pointerId %d---------", pointer_id);
		globals->app->get_input_mgr()->get_gamepad(0).
				raise_touch_event(IET_TOUCHPAD_ENDED, 
						pointer_id, 
						(int)AMotionEvent_getX(event, pointer_id), 
						(int)AMotionEvent_getY(event, pointer_id));
	}
	
	if (action == AMOTION_EVENT_ACTION_UP)
	{
		Log::info("------ACTION_UP---------");
		for (int i = 0; i < count; ++i)
		{
			const int id = AMotionEvent_getPointerId(event, i); 
			globals->app->get_input_mgr()->get_gamepad(0).
					raise_touch_event(IET_TOUCHPAD_ENDED, 
							id, 
							(int)AMotionEvent_getX(event, id), 
							(int)AMotionEvent_getY(event, id));
		}
	}
	
	return 1; 
}

void android_main(struct android_app *and_app)
{
	app_dummy();

	int result = 0; 
	
	// Initialize the globals
	init_globals();

	AndroidGlobalsPtr globals = cast_android_globals(get_globals());
	
	globals->and_app = and_app; 
	
	Log::info("Create Application");
	
	App *android_app = CreateApp();
	get_globals()->app = android_app; 
	
	and_app->userData = android_app;
	and_app->onAppCmd = android_on_app_cmd;
	and_app->onInputEvent = android_on_input_event; 

	result = RunApp();
	
	DestroyApp(); 
}
