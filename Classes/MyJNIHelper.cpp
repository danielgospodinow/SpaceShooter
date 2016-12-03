#include "MyJNIHelper.h"
#include "platform/android/jni/JniHelper.h"
#include "Definitions.h"
#include "GamePrepareScene.h"
#include "EnemyShipBlast.h"
#include "GameScene.h"
#include "ClientWaitingScene.h"
#include "MainMenuScene.h"

#define  CLASS_NAME_CJNIHELPER "org/cocos2dx/cpp/CJNIHelper"
#define  CLASS_NAME_APP_ACTIVITY "org/cocos2dx/cpp/AppActivity"

typedef struct JniMethodInfo_
{
    JNIEnv *    env;
    jclass      classID;
    jmethodID   methodID;
} JniMethodInfo;


extern "C"
{
	// get env and cache it
	static JNIEnv* getJNIEnv(void)
	{
		JavaVM* jvm = cocos2d::JniHelper::getJavaVM();
		if (NULL == jvm) {
			// -- LOGD("Failed to get JNIEnv. JniHelper::getJavaVM() is NULL");
			return NULL;
		}

		JNIEnv *env = NULL;
		// get jni environment
		jint ret = jvm->GetEnv((void**)&env, JNI_VERSION_1_4);

		switch (ret) {
			case JNI_OK :
				// Success!
				return env;

			case JNI_EDETACHED :
				// Thread not attached

				// TODO : If calling AttachCurrentThread() on a native thread
				// must call DetachCurrentThread() in future.
				// see: http://developer.android.com/guide/practices/design/jni.html

				if (jvm->AttachCurrentThread(&env, NULL) < 0)
				{
					// -- LOGD("Failed to get the environment using AttachCurrentThread()");
					return NULL;
				} else {
					// Success : Attached and obtained JNIEnv!
					return env;
				}

			case JNI_EVERSION :
				// Cannot recover from this error
				// -- LOGD("JNI interface version 1.4 not supported");
				return NULL;
			default :
				// -- LOGD("Failed to get the environment using GetEnv()");
				return NULL;
		}
	}

	// get class and make it a global reference, release it at endJni().
	static jclass getClassID(JNIEnv *pEnv, const char *class_name)
	{
		jclass ret = pEnv->FindClass(class_name);
		if (! ret)
		{
			LOGD("Failed to find class of %s", class_name);
		}

		return ret;
	}

	static bool getStaticMethodInfo(JniMethodInfo &methodinfo, const char *methodName, const char *paramCode, const char *class_name)
	{
		jmethodID methodID = 0;
		JNIEnv *pEnv = 0;
		bool bRet = false;

		do
		{
			pEnv = getJNIEnv();
			if (! pEnv)
			{
				break;
			}

			jclass classID = getClassID(pEnv, class_name);

			methodID = pEnv->GetStaticMethodID(classID, methodName, paramCode);
			if (! methodID)
			{
				LOGD("Failed to find static method id of %s", methodName);
				break;
			}

			methodinfo.classID = classID;
			methodinfo.env = pEnv;
			methodinfo.methodID = methodID;

			bRet = true;
		} while (0);

		return bRet;
	}

	static bool getMethodInfo(JniMethodInfo &methodinfo, const char *methodName, const char *paramCode, const char *class_name)
	{
		jmethodID methodID = 0;
		JNIEnv *pEnv = 0;
		bool bRet = false;

		do
		{
			pEnv = getJNIEnv();
			if (! pEnv)
			{
				break;
			}

			jclass classID = getClassID(pEnv, class_name);

			methodID = pEnv->GetMethodID(classID, methodName, paramCode);
			if (! methodID)
			{
				LOGD("Failed to find method id of %s", methodName);
				break;
			}

			methodinfo.classID = classID;
			methodinfo.env = pEnv;
			methodinfo.methodID = methodID;

			bRet = true;
		} while (0);

		return bRet;
	}

	void MakeToast_CPP(const char* message)
	{
		JniMethodInfo methodInfo;

		if(!getStaticMethodInfo(methodInfo, "MakeToast_JAVA", "(Ljava/lang/String;)V", CLASS_NAME_APP_ACTIVITY))
		{
			LOGD("Failed to MakeToast");
			return;
		}

		jstring stringArg = methodInfo.env->NewStringUTF(message);

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void StartServer_CPP(int port)
	{
		JniMethodInfo methodInfo;

		if(!getStaticMethodInfo(methodInfo, "StartServer_JAVA", "(I)V", CLASS_NAME_CJNIHELPER))
		{
			LOGD("Failed to start server");
			return;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, port);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void ConnectToServer_CPP(const char* IP, int port)
	{
		JniMethodInfo methodInfo;

		if(!getStaticMethodInfo(methodInfo, "ConnectToServer_JAVA", "(Ljava/lang/String;I)V", CLASS_NAME_CJNIHELPER))
		{
			LOGD("Failed to connect to server");
			return;
		}

		jstring jIP = methodInfo.env->NewStringUTF(IP);

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jIP, port);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
		methodInfo.env->DeleteLocalRef(jIP);
	}

	const char* GetIPAdresses_CPP()
	{
		const char* ipAdresses = "";

		//Implementation

		return ipAdresses;
	}

	std::string GetWantedIPAdress_CPP()
	{
		JniMethodInfo methodInfo;

		if(!getStaticMethodInfo(methodInfo, "GetWantedIPAdress_JAVA", "()Ljava/lang/String;", CLASS_NAME_CJNIHELPER))
		{
			LOGD("Failed to get call GetIPAdresses_JAVA");
			return "";
		}

		jstring jTempResult = (jstring) methodInfo.env->CallStaticObjectMethod(methodInfo.classID,methodInfo.methodID);
		const char* cTempResult = methodInfo.env->GetStringUTFChars(jTempResult, NULL);
		std::string wanterIPAdress(cTempResult);

		methodInfo.env->ReleaseStringUTFChars(jTempResult, cTempResult);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);

		return wanterIPAdress;
	}

	void ToastAllIPAdresses_CPP()
	{
		JniMethodInfo methodInfo;

		if(!getStaticMethodInfo(methodInfo, "ToastAllIPAdresses_JAVA", "()V", CLASS_NAME_CJNIHELPER))
		{
			LOGD("Failed to ToastAllIPAdresses");
			return;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void ToastWantedIPAdress_CPP()
	{
		JniMethodInfo methodInfo;

		if(!getStaticMethodInfo(methodInfo, "ToastWantedAdress_JAVA", "()V", CLASS_NAME_CJNIHELPER))
		{
			LOGD("Failed to ToastWantedIPAdress");
			return;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void NetworkFunctionOnOpponent_CPP(std::string parameters)
	{
		JniMethodInfo methodInfo;

		if(!getStaticMethodInfo(methodInfo, "NetworkFunctionOnOpponent_JAVA", "(Ljava/lang/String;)V", CLASS_NAME_CJNIHELPER))
		{
			LOGD("Failed to call a network function");
			return;
		}

		jstring jParameters = methodInfo.env->NewStringUTF(parameters.c_str());

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jParameters);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
		methodInfo.env->DeleteLocalRef(jParameters);
	}

	float GetPhysicalSize_CPP()
	{
		JniMethodInfo methodInfo;
		jfloat result = 0;

		if(!getStaticMethodInfo(methodInfo, "GetPhysicalSize", "()F", CLASS_NAME_APP_ACTIVITY))
		{
			LOGD("Failed to get physical size");
			return -1;
		}

		result = methodInfo.env->CallStaticFloatMethod(methodInfo.classID,methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);

		return result;
	}

	std::string IntToString_CPP(int number)
	{
		JniMethodInfo methodInfo;

		if(!getStaticMethodInfo(methodInfo, "IntToString_JAVA", "(I)Ljava/lang/String;", CLASS_NAME_CJNIHELPER))
		{
			LOGD("Failed to get call IntToString");
			return "";
		}

		jstring jTempResult = (jstring) methodInfo.env->CallStaticObjectMethod(methodInfo.classID,methodInfo.methodID, number);
		const char* cTempResult = methodInfo.env->GetStringUTFChars(jTempResult, NULL);
		std::string ret(cTempResult);

		methodInfo.env->ReleaseStringUTFChars(jTempResult, cTempResult);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);

		return ret;
	}

	std::string FloatToString_CPP(float number)
	{
		JniMethodInfo methodInfo;

		if(!getStaticMethodInfo(methodInfo, "FloatToString_JAVA", "(F)Ljava/lang/String;", CLASS_NAME_CJNIHELPER))
		{
			LOGD("Failed to get call FloatToString");
			return "";
		}

		jstring jTempResult = (jstring) methodInfo.env->CallStaticObjectMethod(methodInfo.classID,methodInfo.methodID, number);
		const char* cTempResult = methodInfo.env->GetStringUTFChars(jTempResult, NULL);
		std::string ret(cTempResult);

		methodInfo.env->ReleaseStringUTFChars(jTempResult, cTempResult);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);

		return ret;
	}

	int GetDPI_CPP()
	{
		JniMethodInfo methodInfo;
		jint result = 0;

		if(!getStaticMethodInfo(methodInfo, "GetDPI", "()I", CLASS_NAME_APP_ACTIVITY))
		{
			LOGD("Failed to get DPI");
			return -1;
		}

		result = methodInfo.env->CallStaticIntMethod(methodInfo.classID,methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);

		return result;
	}

	void StartHostGamesDiscovery_CPP()
	{
		JniMethodInfo methodInfo;

		if(!getStaticMethodInfo(methodInfo, "StartHostGamesDiscoveryJAVA", "()V", CLASS_NAME_CJNIHELPER))
		{
			LOGD("Failed to start host game discovery");
			return;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void StartHostGameBroadcasting_CPP()
	{
		JniMethodInfo methodInfo;

		if(!getStaticMethodInfo(methodInfo, "StartHostGameBroadcastingJAVA", "()V", CLASS_NAME_CJNIHELPER))
		{
			LOGD("Failed to start hosting the game");
			return;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void StopHostGamesDiscovery_CPP()
	{
		JniMethodInfo methodInfo;

		if(!getStaticMethodInfo(methodInfo, "StopHostGamesDiscoveryJAVA", "()V", CLASS_NAME_CJNIHELPER))
		{
			LOGD("Failed to stop host game discovery");
			return;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void StopHostGameBroadcasting_CPP()
	{
		JniMethodInfo methodInfo;

		if(!getStaticMethodInfo(methodInfo, "StopHostGameBroadcastingJAVA", "()V", CLASS_NAME_CJNIHELPER))
		{
			LOGD("Failed to stop hosting the game");
			return;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void StopServerThread_CPP()
	{
		JniMethodInfo methodInfo;

		if(!getStaticMethodInfo(methodInfo, "StopServerThreadJAVA", "()V", CLASS_NAME_CJNIHELPER))
		{
			LOGD("Failed to stop server thread");
			return;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void StopClientThread_CPP()
	{
		JniMethodInfo methodInfo;

		if(!getStaticMethodInfo(methodInfo, "StopClientThreadJAVA", "()V", CLASS_NAME_CJNIHELPER))
		{
			LOGD("Failed to stop client thread");
			return;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void CloseServerClientConnection_CPP()
	{
		JniMethodInfo methodInfo;

		if(!getStaticMethodInfo(methodInfo, "CloseServerClientConnectionJAVA", "()V", CLASS_NAME_CJNIHELPER))
		{
			LOGD("Failed to close server client connection");
			return;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	bool IsIPAvaivable_CPP(std::string ip)
	{
		JniMethodInfo methodInfo;

		if(!getStaticMethodInfo(methodInfo, "PingIP_JAVA", "(Ljava/lang/String;)Z", CLASS_NAME_CJNIHELPER))
		{
			LOGD("Failed to call a PingIP_JAVA function");
			return false;
		}

		jstring jParameters = methodInfo.env->NewStringUTF(ip.c_str());
		bool isAvaivable = (bool)methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID, jParameters);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
		methodInfo.env->DeleteLocalRef(jParameters);

		return isAvaivable;
	}

	bool IsNetworkConnected_CPP()
	{
		JniMethodInfo methodInfo;

		if(!getStaticMethodInfo(methodInfo, "isNetworkConnected_JAVA", "()Z", CLASS_NAME_APP_ACTIVITY))
		{
			LOGD("Failed to call a isNetworkConnected_JAVA function");
			return false;
		}

		bool isNetworkConnected = (bool)methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);

		return isNetworkConnected;
	}

	void HideSystemUI_CPP()
	{
		JniMethodInfo methodInfo;

		if(!getStaticMethodInfo(methodInfo, "hideUI_JAVA", "()V", CLASS_NAME_APP_ACTIVITY))
		{
			LOGD("Failed to hide system UI");
			return;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void Java_org_cocos2dx_cpp_CJNIHelper_GoToGameSceneJAVA(JNIEnv* env, jobject obj)
	{
		cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(1, GamePrepareScene::createScene()));
	}

	void Java_org_cocos2dx_cpp_CJNIHelper_GoToMainMenuSceneJAVA(JNIEnv* env, jobject obj)
	{
		cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(1, MainMenuScene::createScene()));
	}

	void Java_org_cocos2dx_cpp_CJNIHelper_LeaveGameSceneJAVA(JNIEnv* env, jobject obj)
	{
		static_cast<GameScene*>(cocos2d::Director::getInstance()->getRunningScene()->getChildByName("layer"))->leaveGame();
	}

	void Java_org_cocos2dx_cpp_CJNIHelper_RestartGameJAVA(JNIEnv* env, jobject obj)
	{
		static_cast<GameScene*>(cocos2d::Director::getInstance()->getRunningScene()->getChildByName("layer"))->gameOver(true);
	}

	void Java_org_cocos2dx_cpp_CJNIHelper_SpawnEnemyShipBlastJAVA(JNIEnv* env, jobject obj, jfloat xCoord, jfloat blastAngle, jint stacker)
	{
		xCoord = (GameManager::getInstance()->getOpponentResolutionWidthInMyPixels() * xCoord / 100) + GameManager::getInstance()->getHalfVoidSpace();
		EnemyShipBlast *enemyShipBlast = new EnemyShipBlast(xCoord, blastAngle, stacker);
	}

	jfloat Java_org_cocos2dx_cpp_CJNIHelper_GetScreenWidthJAVA(JNIEnv* env, jobject obj)
	{
		return cocos2d::Director::getInstance()->getVisibleSize().width;
	}

	jstring Java_org_cocos2dx_cpp_CJNIHelper_GetMyShipNameJAVA(JNIEnv* env, jobject obj)
	{
		jstring myName = env->NewStringUTF(GameManager::getInstance()->getMyName().c_str());
		return myName;
	}

	void Java_org_cocos2dx_cpp_CJNIHelper_SetOpponentShipTypeJAVA(JNIEnv* env, jobject obj, jstring shipTypeString)
	{
		const char* shipTypeStr = env->GetStringUTFChars(shipTypeString, NULL);
		GameManager::getInstance()->setOpponentShipTypeWithString(shipTypeStr);

		env->ReleaseStringUTFChars(shipTypeString, shipTypeStr);
	}

	void Java_org_cocos2dx_cpp_CJNIHelper_SetOpponentScreenDetailsJAVA(JNIEnv* env, jobject obj, jint resolutionWidth, jint resolutionHeight, jint dpi)
	{
		GameManager::getInstance()->setOpponentScreenDetails(resolutionWidth, resolutionHeight, dpi);
	}

	void Java_org_cocos2dx_cpp_CJNIHelper_SetOpponentNameJAVA(JNIEnv* env, jobject obj, jstring name)
	{
		const char* opponentName = env->GetStringUTFChars(name, NULL);
		GameManager::getInstance()->setOpponentName(opponentName);

		env->ReleaseStringUTFChars(name, opponentName);
	}

	void Java_org_cocos2dx_cpp_CJNIHelper_SetOpponentReadyBoolJAVA(JNIEnv* env, jobject obj, jboolean isReady)
	{
		GameManager::getInstance()->setOpponentIsReady(isReady);
	}

	void Java_org_cocos2dx_cpp_CJNIHelper_AddHostGameJAVA(JNIEnv* env, jobject obj, jstring args)
	{
		const char* cargs = env->GetStringUTFChars(args, NULL);
		cocos2d::Node* runningScene = cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(69);

		if(runningScene == NULL || runningScene->getName() != "cwsLayer")
		{
			env->ReleaseStringUTFChars(args, cargs);
			return;
		}

		ClientWaitingScene* cws = static_cast<ClientWaitingScene*>(runningScene);
		if(cws != NULL && cws->getName() == "cwsLayer")
			cws->addHostGame(std::string(cargs));

		env->ReleaseStringUTFChars(args, cargs);
	}
}
