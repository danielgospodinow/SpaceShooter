#ifndef __MY_JNI_HELPER_H__
#define __MY_JNI_HELPER_H__

#include <jni.h>
#include "cocos2d.h"

using namespace std;

extern "C"
{
	//Java-> C++
	extern void MakeToast_CPP(const char* message);
	extern void StartServer_CPP(int port);
	extern void ConnectToServer_CPP(const char* IP, int port);
	extern const char* GetIPAdresses_CPP();
	extern std::string GetWantedIPAdress_CPP();
	extern void ToastAllIPAdresses_CPP();
	extern void ToastWantedIPAdress_CPP();
	extern void NetworkFunctionOnOpponent_CPP(std::string parameters);
	extern float GetPhysicalSize_CPP();
	extern int GetDPI_CPP();
	extern std::string IntToString_CPP(int number);
	extern std::string FloatToString_CPP(float number);
	extern void StartHostGamesDiscovery_CPP();
	extern void StartHostGameBroadcasting_CPP();
	extern void StopHostGamesDiscovery_CPP();
	extern void StopHostGameBroadcasting_CPP();
	extern void StopServerThread_CPP();
	extern void StopClientThread_CPP();
	extern void CloseServerClientConnection_CPP();
	extern bool IsIPAvaivable_CPP(std::string ip);
	extern void HideSystemUI_CPP();
	extern bool IsNetworkConnected_CPP();

	//C++-> Java
	extern void Java_org_cocos2dx_cpp_CJNIHelper_GoToGameSceneJAVA(JNIEnv* env, jobject obj);
	extern void Java_org_cocos2dx_cpp_CJNIHelper_GoToMainMenuSceneJAVA(JNIEnv* env, jobject obj);
	extern void Java_org_cocos2dx_cpp_CJNIHelper_LeaveGameSceneJAVA(JNIEnv* env, jobject obj);
	extern void Java_org_cocos2dx_cpp_CJNIHelper_RestartGameJAVA(JNIEnv* env, jobject obj);
	extern void Java_org_cocos2dx_cpp_CJNIHelper_SpawnEnemyShipBlastJAVA(JNIEnv* env, jobject obj, jfloat xCoord, jfloat blastAngle, jint stacker);
	extern jfloat Java_org_cocos2dx_cpp_CJNIHelper_GetScreenWidthJAVA(JNIEnv* env, jobject obj);
	extern jstring Java_org_cocos2dx_cpp_CJNIHelper_GetMyShipNameJAVA(JNIEnv* env, jobject obj);
	extern void Java_org_cocos2dx_cpp_CJNIHelper_SetOpponentShipTypeJAVA(JNIEnv* env, jobject obj, jstring shipTypeString);
	extern void Java_org_cocos2dx_cpp_CJNIHelper_SetOpponentScreenDetailsJAVA(JNIEnv* env, jobject obj, jint resolutionWidth, jint resolutionHeight, jint dpi);
	extern void Java_org_cocos2dx_cpp_CJNIHelper_SetOpponentNameJAVA(JNIEnv* env, jobject obj, jstring name);
	extern void Java_org_cocos2dx_cpp_CJNIHelper_SetOpponentReadyBoolJAVA(JNIEnv* env, jobject obj, jboolean isReady);
	extern void Java_org_cocos2dx_cpp_CJNIHelper_AddHostGameJAVA(JNIEnv* env, jobject obj, jstring args);
}

#endif
