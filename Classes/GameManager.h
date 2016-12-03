#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "SpaceShip.h"
#include "cocos2d.h"
#include "MyJNIHelper.h"

class GameManager
{
public:
	static GameManager* getInstance();

    void setMyShipType(ShipType shipType) {this->myShipType = shipType; setMyShipTypeString();}
	void setMyShipTypeString()
	{
		std::string temp = "";

		if(this->myShipType ==  ShipType::Yellow)
			temp = "yellow";
		else if(this->myShipType ==  ShipType::Blue)
			temp = "blue";
		else if(this->myShipType ==  ShipType::Red)
			temp = "red";
		else if(this->myShipType ==  ShipType::Purple)
			temp = "purple";
		else if(this->myShipType ==  ShipType::Green)
			temp = "green";

		this->myShipTypeString = temp;
	};

    void setOpponentShipType(ShipType shipType) {this->opponentShipType = shipType; setOpponentShipTypeString();}
	void setOpponentShipTypeString()
	{
		std::string temp = "";

		if(this->opponentShipType ==  ShipType::Yellow)
			temp = "yellow";
		else if(this->opponentShipType ==  ShipType::Blue)
			temp = "blue";
		else if(this->opponentShipType ==  ShipType::Red)
			temp = "red";
		else if(this->opponentShipType ==  ShipType::Purple)
			temp = "purple";
		else if(this->opponentShipType ==  ShipType::Green)
			temp = "green";

		this->opponentShipTypeString = temp;
	};

	void setOpponentShipTypeWithString(std::string shipTypeStr)
	{
		this->opponentShipTypeString = shipTypeStr;

		ShipType shipType;

		if(shipTypeStr ==  "yellow")
			shipType = ShipType::Yellow;
		else if(shipTypeStr ==  "blue")
			shipType = ShipType::Blue;
		else if(shipTypeStr ==  "red")
			shipType = ShipType::Red;
		else if(shipTypeStr ==  "green")
			shipType = ShipType::Green;
		else if(shipTypeStr ==  "purple")
			shipType = ShipType::Purple;

		this->setOpponentShipType(shipType);
	};

	void setMyShipTypeWithString(std::string shipTypeStr)
	{
		this->myShipTypeString = shipTypeStr;

		ShipType shipType;

		if(shipTypeStr ==  "yellow")
			shipType = ShipType::Yellow;
		else if(shipTypeStr ==  "blue")
			shipType = ShipType::Blue;
		else if(shipTypeStr ==  "red")
			shipType = ShipType::Red;
		else if(shipTypeStr ==  "green")
			shipType = ShipType::Green;
		else if(shipTypeStr ==  "purple")
			shipType = ShipType::Purple;

		this->setMyShipType(shipType);
	};

    ShipType getMyShipType() {return this->myShipType;}
	std::string getMyShipTypeString() {return this->myShipTypeString;}

    ShipType getOpponentShipType() {return this->opponentShipType;}
    std::string getOpponentShipTypeString() {return this->opponentShipTypeString;}

	void networkFunctionOnOpponent(std::string funcArgs)
	{
		NetworkFunctionOnOpponent_CPP(funcArgs);
	}

	void makeToast(std::string message)
	{
		MakeToast_CPP(message.c_str());
	}

	std::string intToString(int number)
	{
		return IntToString_CPP(number);
	}

	std::string floatToString(float number)
	{
		return FloatToString_CPP(number);
	}

	float getDevicePhysicalSize(int resolutionWidth, int resolutionHeight, int dpi) {return roundf(sqrtf(pow((float)resolutionWidth / dpi, 2) + pow((float)resolutionHeight / dpi, 2)) * 100) / 100;}

	void setOpponentScreenDetails(int resolutionWidth, int resolutionHeight, int dpi)
	{
		this->opponentDpi_ = dpi;
		this->opponentResolutionWidth_ = resolutionWidth;
		this->opponentResolutionHeight_ = resolutionHeight;

		this->opponentPhysicalSize_ = getDevicePhysicalSize(resolutionWidth, resolutionHeight, dpi);
		this->opponentPhysicalWidth_ = roundf(((float)resolutionWidth / dpi) * 100) / 100;
		this->opponentPhysicalHeight_ = roundf(((float)resolutionHeight / dpi) * 100) / 100;

		this->opponentResolutionWidthInMyPixels_ = (int)(cocos2d::Director::getInstance()->getVisibleSize().width * opponentPhysicalWidth_ / myPhysicalWidth_);
		this->opponentResolutionHeightInMyPixels_ = (int)(cocos2d::Director::getInstance()->getVisibleSize().height * opponentPhysicalHeight_ / myPhysicalHeight_);

		if(myPhysicalSize_ > opponentPhysicalSize_)
		{
			voidSpace = (int)(cocos2d::Director::getInstance()->getVisibleSize().width - opponentResolutionWidthInMyPixels_);
		}

		if(opponentResolutionWidthInMyPixels_ > cocos2d::Director::getInstance()->getVisibleSize().width)
		{
			opponentResolutionWidthInMyPixels_ = (int)cocos2d::Director::getInstance()->getVisibleSize().width;
		}
	};

    float getMyPhysicalSize() {return this->myPhysicalSize_;}

    float getMyPhysicalWidth() {return this->myPhysicalWidth_;}
    float getMyPhysicalHeight() {return this->myPhysicalHeight_;}

    float getOpponentPhysicalSize() {return this->opponentPhysicalSize_;}
    float getOpponentPhysicalWidth() {return this->opponentPhysicalWidth_;}
    float getOpponentPhysicalHeight() {return this->opponentPhysicalHeight_;}

    float getOpponentWidth(){return this->opponentResolutionWidth_;}
    float getOpponentHeight(){return this->opponentResolutionHeight_;}

	int getOpponentResolutionWidthInMyPixels(){ return this->opponentResolutionWidthInMyPixels_; }
	int getOpponentResolutionHeightInMyPixels(){ return this->opponentResolutionHeightInMyPixels_; }
	void setOpponentResolutionWidthInMyPixels(int newOpResWidthInMPixels) {this->opponentResolutionWidthInMyPixels_ = newOpResWidthInMPixels;}

    int getOpponentDPI(){return this->opponentDpi_;}

	int getHalfVoidSpace() {return this->voidSpace / 2;}

    std::string getOpponentName(){if(this->opponentName == "") return "Unknown"; else return this->opponentName;}
    void setOpponentName(std::string name){this->opponentName = name;}

    std::string getMyName(){return cocos2d::UserDefault::sharedUserDefault()->getStringForKey("name");}

    bool getOpponentIsReady(){return this->opponentIsReady;}
    void setOpponentIsReady(bool isReady){this->opponentIsReady = isReady;}

    void setGlobalImageScaleFactor(float newScaleFactor){this->globalImageScaleFactor_ = newScaleFactor;}
    float getGlobalImageScaleFactor(){return this->globalImageScaleFactor_;}

    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
	{
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
	}

    std::vector<std::string> split(const std::string &s, char delim)
    {
        std::vector<std::string> elems;
        split(s, delim, elems);
        return elems;
    }

    cocos2d::Sprite* createColoredBlankSprite(cocos2d::Size size, cocos2d::Color3B color)
    {
    	cocos2d::Sprite* ret = cocos2d::Sprite::create("rect.png");
    	ret->setTextureRect(cocos2d::Rect(0, 0, size.width, size.height));
    	ret->setColor(color);

    	return ret;
    }

    int getLabelPixels()
    {
    	return labelPixels;
    }

    void setLabelPixels(int newLabelPixels)
    {
    	labelPixels = newLabelPixels;
    }

    int getDPI(){return dpi_;}

    void setDpi(int dpi)
    {
    	this->dpi_ = dpi;
    	cocos2d::UserDefault::sharedUserDefault()->setIntegerForKey("dpi", dpi);

    	myPhysicalSize_ = this->getDevicePhysicalSize(cocos2d::Director::getInstance()->getVisibleSize().width, cocos2d::Director::getInstance()->getVisibleSize().height, this->dpi_);
		myPhysicalWidth_ = roundf((cocos2d::Director::getInstance()->getVisibleSize().width / this->dpi_) * 100) / 100;
		myPhysicalHeight_ = roundf((cocos2d::Director::getInstance()->getVisibleSize().height / this->dpi_) * 100) / 100;
		globalImageScaleFactor_ = this->dpi_ / 304.0f;
    }

private:
	GameManager()
	{
		myShipType = ShipType::None;
		opponentShipType = ShipType::None;

		if(cocos2d::UserDefault::sharedUserDefault()->getIntegerForKey("dpi") == 0)
		{
			dpi_ = GetDPI_CPP();
			cocos2d::UserDefault::sharedUserDefault()->setIntegerForKey("dpi", dpi_);
		}
		else
			dpi_ = cocos2d::UserDefault::sharedUserDefault()->getIntegerForKey("dpi");

		opponentPhysicalSize_ = 0;
		opponentDpi_ = 0;
		opponentResolutionWidth_ = 0;
		opponentResolutionHeight_ = 0;
		opponentPhysicalWidth_ = 0;
		opponentPhysicalHeight_ = 0;
		opponentName = "";
		opponentIsReady = false;
		myPhysicalSize_ = this->getDevicePhysicalSize(cocos2d::Director::getInstance()->getVisibleSize().width, cocos2d::Director::getInstance()->getVisibleSize().height, this->dpi_);
		myPhysicalWidth_ = roundf((cocos2d::Director::getInstance()->getVisibleSize().width / this->dpi_) * 100) / 100;
		myPhysicalHeight_ = roundf((cocos2d::Director::getInstance()->getVisibleSize().height / this->dpi_) * 100) / 100;
		globalImageScaleFactor_ = this->dpi_ / 304.0f;
		opponentResolutionWidthInMyPixels_ = 0;
		opponentResolutionHeightInMyPixels_ = 0;
        voidSpace = 0;
        labelPixels = 0;
    }
	GameManager(GameManager const&)
	{
		myShipType = ShipType::None;
		opponentShipType = ShipType::None;
		if(cocos2d::UserDefault::sharedUserDefault()->getIntegerForKey("dpi") == 0)
		{
			dpi_ = GetDPI_CPP();
			cocos2d::UserDefault::sharedUserDefault()->setIntegerForKey("dpi", dpi_);
		}
		else
			dpi_ = cocos2d::UserDefault::sharedUserDefault()->getIntegerForKey("dpi");
		opponentPhysicalSize_ = 0;
		opponentDpi_ = 0;
		opponentResolutionWidth_ = 0;
		opponentResolutionHeight_ = 0;
		opponentPhysicalWidth_ = 0;
		opponentPhysicalHeight_ = 0;
		opponentName = "";
		opponentIsReady = false;
		myPhysicalSize_ = this->getDevicePhysicalSize(cocos2d::Director::getInstance()->getVisibleSize().width, cocos2d::Director::getInstance()->getVisibleSize().height, this->dpi_);
		myPhysicalWidth_ = roundf((cocos2d::Director::getInstance()->getVisibleSize().width / this->dpi_) * 100) / 100;
		myPhysicalHeight_ = roundf((cocos2d::Director::getInstance()->getVisibleSize().height / this->dpi_) * 100) / 100;
		globalImageScaleFactor_ = this->dpi_ / 304.0f;
		opponentResolutionWidthInMyPixels_ = 0;
		opponentResolutionHeightInMyPixels_ = 0;
        voidSpace = 0;
        labelPixels = 0;
    }

    void operator=(GameManager const&){}  // assignment operator is private

	static GameManager* instance_;

	ShipType myShipType;
	ShipType opponentShipType;

	std::string myShipTypeString;
	std::string opponentShipTypeString;

	int dpi_;

	int opponentDpi_;
	int opponentResolutionWidth_;
	int opponentResolutionHeight_;
	int opponentResolutionWidthInMyPixels_;
	int opponentResolutionHeightInMyPixels_;
	float opponentPhysicalSize_;
	float opponentPhysicalWidth_;
	float opponentPhysicalHeight_;
	std::string opponentName;
	bool opponentIsReady;

	float myPhysicalSize_;
	float myPhysicalWidth_;
	float myPhysicalHeight_;

	float globalImageScaleFactor_;

	int voidSpace;

	int labelPixels;
};

#endif
