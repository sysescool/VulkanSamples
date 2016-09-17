//#ifdef ANDROID
//#define VK_USE_PLATFORM_ANDROID_KHR
//#endif

//==========================ANDROID=============================
#ifdef VK_USE_PLATFORM_ANDROID_KHR

#include "native.h"     //for Android_App
#include "WindowImpl.h"

#ifndef WINDOW_ANDROID
#define WINDOW_ANDROID

//======================MULTI-TOUCH=======================  TODO: Move MTouch to WindowImpl, and expose events.
struct CPointer{bool active; float x; float y;};
struct MTouchEvent{eMouseAction act; float x; float y; char id;};

class CMTouch{
    static const int MAX_POINTERS=10;  //Max 10 fingers
public:
    int count;
    CPointer Pointers[MAX_POINTERS];
    MTouchEvent LastEvent;

    void Clear(){ memset(this,0,sizeof(this)); }

    void Set(char id,eMouseAction act,float x,float y) {
        if (id >= MAX_POINTERS)return;  // Exit if too many fingers
        CPointer& P=Pointers[id];
        if(act) P.active=(act==mDOWN);
        P.x=x;  P.y=y;
        LastEvent={act,x,y,id};
    }
};
//========================================================

// Convert native Android key-code to cross-platform USB HID code.
const unsigned char ANDROID_TO_HID[256] = {
  0,227,231,  0,  0,  0,  0, 39, 30, 31, 32, 33, 34, 35, 36, 37,
 38,  0,  0, 82, 81, 80, 79,  0,  0,  0,  0,  0,  0,  4,  5,  6,
  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
 23, 24, 25, 26, 27, 28, 29, 54, 55,226,230,225,229, 43, 44,  0,
  0,  0, 40,  0, 53, 45, 46, 47, 48, 49, 51, 52, 56,  0,  0,  0,
  0,  0,118,  0,  0,  0,  0,  0,  0,  0,  0,  0, 75, 78,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0, 57, 71,  0,  0,  0,  0, 72, 74, 77, 73,  0,  0,  0,
 24, 25,  0, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 83,
 98, 89, 90, 91, 92, 93, 94, 95, 96, 97, 84, 85, 86, 87, 99,  0,
 88,103,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

//==========================Android=============================
class Window_android : public WindowImpl{
    android_app* app=0;
    CMTouch MTouch;

    void SetTitle(const char* title){};  //TODO : Set window title?

    void CreateSurface(VkInstance instance){
        VkAndroidSurfaceCreateInfoKHR android_createInfo;
        android_createInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
        android_createInfo.pNext = NULL;
        android_createInfo.flags = 0;
        android_createInfo.window = app->window;
        VkResult err=vkCreateAndroidSurfaceKHR(instance, &android_createInfo, NULL, &surface);
        VKERRCHECK(err);
        printf("Surface created\n");
    }

public:
    Window_android(CInstance& inst, const char* title, uint width, uint height) {
        instance = &inst;
        shape.width  = 0;//width;
        shape.height = 0;//height;
        running = true;
        //printf("Creating Android-Window...\n");
        app = Android_App;
//return;
        //---Wait for window to be created AND gain focus---
        //bool hasFocus = false;
        while (!has_focus) {
            int events = 0;
            struct android_poll_source *source;
            int id = ALooper_pollOnce(100, NULL, &events, (void **) &source);
            if (id == LOOPER_ID_MAIN) {
                int8_t cmd = android_app_read_cmd(app);
                android_app_pre_exec_cmd(app, cmd);
                if (app->onAppCmd != NULL) app->onAppCmd(app, cmd);
                if (cmd == APP_CMD_INIT_WINDOW ) {
                    shape.width = ANativeWindow_getWidth(app->window);
                    shape.height = ANativeWindow_getHeight(app->window);
                    eventFIFO.push(ShapeEvent(0,0,shape.width,shape.height));      //post shape-event
                }
                if (cmd == APP_CMD_GAINED_FOCUS) eventFIFO.push(FocusEvent(true)); //post focus-event
                android_app_post_exec_cmd(app, cmd);
            }
        }
        ALooper_pollAll(10, NULL, NULL, NULL);  //for keyboard
        //--------------------------------------------------

        CreateSurface(inst);
    };

    virtual ~Window_android(){};

    EventType GetEvent(){
        EventType event={};
//return {EventType::NONE};

        if(!eventFIFO.isEmpty()) return *eventFIFO.pop();  //pop message from message queue buffer

        //--Char event--
        static char buf[4]={};
        static bool charEvent=false;
        if(charEvent){ charEvent=false;  return TextEvent(buf); }
        //--------------


        int events=0;
        struct android_poll_source* source;
        int id=ALooper_pollOnce(0, NULL,&events,(void**)&source);
        //ALooper_pollAll(0, NULL,&events,(void**)&source);

        //if(id>=0) printf("id=%d events=%d, source=%d",id,(int)events, source[0]);
        //if(source) source->process(app, source);


        if(id==LOOPER_ID_MAIN){
            int8_t cmd = android_app_read_cmd(app);
            android_app_pre_exec_cmd(app, cmd);
            if (app->onAppCmd != NULL) app->onAppCmd(app, cmd);
            switch(cmd){
                case APP_CMD_GAINED_FOCUS: event=FocusEvent(true);  break;
                case APP_CMD_LOST_FOCUS  : event=FocusEvent(false); break;
            }
            android_app_post_exec_cmd(app, cmd);
        }else if(id==LOOPER_ID_INPUT) {
            AInputEvent* a_event = NULL;
            while (AInputQueue_getEvent(app->inputQueue, &a_event) >= 0) {
                //LOGV("New input event: type=%d\n", AInputEvent_getType(event));
                if (AInputQueue_preDispatchEvent(app->inputQueue, a_event)) {
                    continue;
                }
                int32_t handled = 0;
                if (app->onInputEvent != NULL) handled = app->onInputEvent(app, a_event);


                int32_t type=AInputEvent_getType(a_event);
                if (type == AINPUT_EVENT_TYPE_KEY){  //KEYBOARD
                    int32_t a_action = AKeyEvent_getAction (a_event);
                    int32_t keycode  = AKeyEvent_getKeyCode(a_event);
                    uint8_t hidcode  = ANDROID_TO_HID[keycode];
                    //printf("key action:%d keycode=%d",a_action,keycode);
                    switch(a_action) {
                        case AKEY_EVENT_ACTION_DOWN:{
                            //int32_t keycode  = AKeyEvent_getKeyCode(a_event);
                            int metaState = AKeyEvent_getMetaState(a_event);
                            int unicode = GetUnicodeChar(AKEY_EVENT_ACTION_DOWN, keycode, metaState);
                            (int&)buf=unicode;
                            event=KeyEvent(keyDOWN,hidcode);                                     //key pressed event
                            charEvent=!!buf[0];                                                  //text typed event
                            break;
                        }
                        case AKEY_EVENT_ACTION_UP:{
                            event=KeyEvent(keyUP  ,hidcode);                                     //key released event
                        }
                    }

                }else
                if (type == AINPUT_EVENT_TYPE_MOTION) {  //TOUCH-SCREEN
                    int32_t a_action = AMotionEvent_getAction(a_event);
                    int  action=(a_action&255);   //get action-code from bottom 8 bits
                    MTouch.count=AMotionEvent_getPointerCount(a_event);
                    if(action==AMOTION_EVENT_ACTION_MOVE) {
                        forCount(MTouch.count) {
                            char id = AMotionEvent_getPointerId(a_event, i);
                            float x = AMotionEvent_getX(a_event, i);
                            float y = AMotionEvent_getY(a_event, i);
                            MTouch.Set(id, mMOVE, x, y);
                        }
                    }else{
                        int  inx =(a_action>>8); //get index from top 24 bits
                        int  id  =AMotionEvent_getPointerId(a_event,inx);
                        float x = AMotionEvent_getX(a_event, inx);
                        float y = AMotionEvent_getY(a_event, inx);
                        switch (action) {
                            case AMOTION_EVENT_ACTION_POINTER_DOWN:
                            case AMOTION_EVENT_ACTION_DOWN      :  MTouch.Set(id,mDOWN,x,y);  break;
                            case AMOTION_EVENT_ACTION_POINTER_UP:
                            case AMOTION_EVENT_ACTION_UP        :  MTouch.Set(id,mUP  ,x,y);  break;
                            case AMOTION_EVENT_ACTION_CANCEL    :  MTouch.Clear();            break;
                        }
                    }
/*
                    //--Test: check first 3 pointers--
                    CPointer& P0=MTouch.Pointers[0];
                    CPointer& P1=MTouch.Pointers[1];
                    CPointer& P2=MTouch.Pointers[2];
                    printf("0:%s %4.0fx%4.0f   1:%s %4.0fx%4.0f  2:%s %4.0fx%4.0f ...\n"
                        ,P0.active?"*":" ",P0.x,P0.y
                        ,P1.active?"*":" ",P1.x,P1.y
                        ,P2.active?"*":" ",P2.x,P2.y);
                    //--------------------------------
*/
                    //---Emulate mouse from MTouch---
                    MTouchEvent te=MTouch.LastEvent;
                    if(te.id==0){  //only use first pointer
                        event=MouseEvent(te.act,te.x,te.y,1);
                    }
                    //-------------------------------

                    //printf("%f x %f\n",x,y);
                    //event=MouseEvent(mMOVE,x,y,1);
                    handled=0;
                }
                AInputQueue_finishEvent(app->inputQueue, a_event, handled);
                return event;
            }

        }else if(id==LOOPER_ID_USER) {

        }

        // Check if we are exiting.
        if (app->destroyRequested){
            printf("destroyRequested");
            running=false;
            return {EventType::NONE};
        }

        return {EventType::NONE};
    };

    //--Show / Hide keyboard--
    void TextInput(bool enabled){
        textinput=enabled;
        ShowKeyboard(enabled);
        printf("%s keyboard",enabled ? "Show" : "Hide");
    }
};


#endif

#endif //VK_USE_PLATFORM_ANDROID_KHR
//==============================================================