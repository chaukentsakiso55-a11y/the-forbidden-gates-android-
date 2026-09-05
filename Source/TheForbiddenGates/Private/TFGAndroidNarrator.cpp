#include "TFGAndroidNarrator.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#endif

void UTFGAndroidNarrator::Speak(const FString& Text, float Pitch, float Rate)
{
#if PLATFORM_ANDROID
    if (Text.IsEmpty()) return;

    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
    {
        static jmethodID SpeakMethod = FJavaWrapper::FindMethod(
            Env,
            FJavaWrapper::GameActivityClassID,
            "AndroidThunkJava_TFGNarratorSpeak",
            "(Ljava/lang/String;FF)V",
            false);

        if (SpeakMethod)
        {
            jstring JavaText = Env->NewStringUTF(TCHAR_TO_UTF8(*Text));
            FJavaWrapper::CallVoidMethod(
                Env,
                FJavaWrapper::GameActivityThis,
                SpeakMethod,
                JavaText,
                FMath::Clamp(Pitch, 0.1f, 2.0f),
                FMath::Clamp(Rate, 0.1f, 2.0f));
            Env->DeleteLocalRef(JavaText);
        }
    }
#endif
}

void UTFGAndroidNarrator::Stop()
{
#if PLATFORM_ANDROID
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
    {
        static jmethodID StopMethod = FJavaWrapper::FindMethod(
            Env,
            FJavaWrapper::GameActivityClassID,
            "AndroidThunkJava_TFGNarratorStop",
            "()V",
            false);

        if (StopMethod)
        {
            FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, StopMethod);
        }
    }
#endif
}
