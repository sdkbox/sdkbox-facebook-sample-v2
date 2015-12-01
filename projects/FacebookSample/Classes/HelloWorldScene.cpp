#include "HelloWorldScene.h"
#include "PluginFacebook/PluginFacebook.h"

USING_NS_CC;
using namespace sdkbox;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    const char* defaultFont = "arial.ttf";
    int defaultFontSize = 32;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    
    CCMenuItemFont* loginItem = CCMenuItemFont::create("login", this, menu_selector(HelloWorld::onLogin));
    
    CCMenuItemFont* logoutItem = CCMenuItemFont::create("logout", this, menu_selector(HelloWorld::onLogout));
    
    CCMenuItemFont* checkStatusItem = CCMenuItemFont::create("check status", this, menu_selector(HelloWorld::onCheckStatus));
    
    CCMenuItemFont* readPermItem = CCMenuItemFont::create("Read Permission", this, menu_selector(HelloWorld::onRequestReadPermission));
    
    CCMenuItemFont* publishPermItem = CCMenuItemFont::create("Publish Permission", this, menu_selector(HelloWorld::onRequestPublishPermission));
    
    CCMenuItemFont* shareLinkItem = CCMenuItemFont::create("Share Link", this, menu_selector(HelloWorld::onShareLink));
    
    CCMenuItemFont* sharePhotoItem = CCMenuItemFont::create("Share Photo", this, menu_selector(HelloWorld::onSharePhoto));

    CCMenuItemFont* dialogLinkItem = CCMenuItemFont::create("Share and comment", this, menu_selector(HelloWorld::onDialogLink));

    CCMenuItemFont* dialogPhotoItem = CCMenuItemFont::create("Share photo and comment", this, menu_selector(HelloWorld::onDialogPhoto));
    
    CCMenu *menu = CCMenu::create(loginItem, logoutItem, checkStatusItem, 
                                    readPermItem, publishPermItem,
                                    shareLinkItem, sharePhotoItem, dialogLinkItem, dialogPhotoItem, NULL);
    menu->alignItemsVerticallyWithPadding(5);
    menu->setAnchorPoint(cocos2d::CCPoint(0, 0));
    menu->setPosition(cocos2d::CCPoint(visibleSize.width/2, visibleSize.height/2));
    addChild(menu);
    
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
    "CloseNormal.png",
    "CloseSelected.png",
    this,
    menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
        origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
    
    CCLabelTTF *userLabel = CCLabelTTF::create("user: sdkbox.test@gmail.com", defaultFont, defaultFontSize);
    userLabel->setPosition(CCPoint(visibleSize.width/2, visibleSize.height - defaultFontSize));
    addChild(userLabel);

    CCLabelTTF *pwLabel = CCLabelTTF::create("password: r43GAfkY", defaultFont, defaultFontSize);
    pwLabel->setPosition(CCPoint(visibleSize.width/2, visibleSize.height - defaultFontSize*2));
    addChild(pwLabel);
    
    sdkbox::PluginFacebook::setListener(this);
    sdkbox::PluginFacebook::init();
    
    return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

//Menu callbacks
void HelloWorld::onLogin(CCObject* sender)
{
    if (sdkbox::PluginFacebook::isLoggedIn())
    {
        CCLOG("Already logged in");
    }
    else
    {
        sdkbox::PluginFacebook::login();
    }
}
void HelloWorld::onLogout(CCObject* sender)
{
    if (sdkbox::PluginFacebook::isLoggedIn())
    {
        sdkbox::PluginFacebook::logout();
    }
    else
    {
        CCLOG("Already logged out");
    }
}
void HelloWorld::onCheckStatus(CCObject* sender)
{
    CCLOG("##FB> permission list: ");
    std::vector<std::string> perms = PluginFacebook::getPermissionList();
    for (int i=0; i < perms.size(); ++i)
    {
        CCLOG("##FB>> permission %s", perms[i].c_str());
    }
    
    CCLOG("##FB> access token: %s", PluginFacebook::getAccessToken().c_str());
    CCLOG("##FB> user id: %s", PluginFacebook::getUserID().c_str());
    CCLOG("##FB> FBSDK version: %s", PluginFacebook::getSDKVersion().c_str());
}
void HelloWorld::onGetMyInfo(CCObject* sender)
{
    sdkbox::FBAPIParam params;
    PluginFacebook::api("me", "GET", params, "me");
}

void HelloWorld::onGetMyFriends(CCObject* sender)
{
    PluginFacebook::fetchFriends();
}

void HelloWorld::onShareLink(CCObject* sender)
{
    CCLOG("##FB %s", __FUNCTION__);
    
    FBShareInfo info;
    info.type  = FB_LINK;
    info.link  = "http://www.cocos2d-x.org";
    info.title = "cocos2d-x";
    info.text  = "Best Game Engine";
    info.image = "http://cocos2d-x.org/images/logo.png";
    PluginFacebook::share(info);
}
void HelloWorld::onSharePhoto(CCObject* sender)
{
    CCLOG("##FB %s", __FUNCTION__);
    
    std::string file = CCFileUtils::sharedFileUtils()->fullPathForFilename("HelloWorld.png");
    
    CCLOG("##FB dialog photo: %s", file.c_str());
    
    FBShareInfo info;
    info.type  = FB_PHOTO;
    info.title = "capture screen";
    info.image = file;
    PluginFacebook::share(info);
}

void HelloWorld::onDialogLink(CCObject* sender)
{
    CCLOG("##FB %s", __FUNCTION__);
    
    FBShareInfo info;
    info.type  = FB_LINK;
    info.link  = "http://www.cocos2d-x.org";
    info.title = "cocos2d-x";
    info.text  = "Best Game Engine";
    info.image = "http://cocos2d-x.org/images/logo.png";
    PluginFacebook::dialog(info);
}

void HelloWorld::onDialogPhoto(CCObject* sender)
{
    CCLOG("##FB %s", __FUNCTION__);
    
    std::string file = CCFileUtils::sharedFileUtils()->fullPathForFilename("HelloWorld.png");
    
    CCLOG("##FB dialog photo: %s", file.c_str());
    
    FBShareInfo info;
    info.type  = FB_PHOTO;
    info.title = "capture screen";
    info.image = file;
    PluginFacebook::dialog(info);
}

void HelloWorld::onRequestReadPermission(CCObject* sender)
{
    CCLOG("##FB %s", __FUNCTION__);
    
    std::vector<std::string> permissions;
    permissions.push_back(FB_PERM_READ_USER_FRIENDS);
    
    PluginFacebook::requestReadPermissions(permissions);
}

void HelloWorld::onRequestPublishPermission(CCObject* sender)
{
    CCLOG("##FB %s", __FUNCTION__);
    
    std::vector<std::string> permissions;
    permissions.push_back(FB_PERM_PUBLISH_POST);
    
    PluginFacebook::requestPublishPermissions(permissions);
}

//Facebook callbacks
void HelloWorld::onLogin(bool isLogin, const std::string& msg)
{
    if (isLogin) {
        CCLog("Login successful");
    }
    else {
        CCLog("Login failed %s", msg.c_str());
    }
}
void HelloWorld::onSharedSuccess(const std::string& message)
{
    CCLOG("##FB onSharedSuccess:%s", message.c_str());
}
void HelloWorld::onSharedFailed(const std::string& message)
{
    CCLOG("##FB onSharedFailed:%s", message.c_str());
}
void HelloWorld::onSharedCancel()
{
    CCLOG("##FB onSharedCancel");
}
void HelloWorld::onAPI(const std::string& tag, const std::string& jsonData)
{
    CCLOG("##FB onAPI: tag -> %s, json -> %s", tag.c_str(), jsonData.c_str());
}
void HelloWorld::onPermission(bool isLogin, const std::string& msg)
{
    CCLOG("##FB onPermission: %d, error: %s", isLogin, msg.c_str());
}
void HelloWorld::onFetchFriends(bool ok, const std::string& msg)
{
CCLOG("##FB %s: %d = %s", __FUNCTION__, ok, msg.data());
    
    const std::vector<sdkbox::FBGraphUser>& friends = sdkbox::PluginFacebook::getFriends();
    for (int i = 0; i < friends.size(); i++)
    {
        const sdkbox::FBGraphUser& user = friends.at(i);
        CCLOG("##FB> -------------------------------");
        CCLOG("##FB>> %s", user.uid.data());
        CCLOG("##FB>> %s", user.firstName.data());
        CCLOG("##FB>> %s", user.lastName.data());
        CCLOG("##FB>> %s", user.name.data());
        CCLOG("##FB>> %s", user.isInstalled ? "app is installed" : "app is not installed");
        CCLOG("##FB");
    }
}
void HelloWorld::onRequestInvitableFriends( const sdkbox::FBInvitableFriendsInfo& friends )
{
    CCLOG("##FB> %s", __FUNCTION__);
}
void HelloWorld::onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg )
{
    CCLOG("##FB> %s", __FUNCTION__);
}
void HelloWorld::onInviteFriendsResult( bool result, const std::string& msg )
{
    CCLOG("##FB> %s", __FUNCTION__);
}
void HelloWorld::onGetUserInfo( const sdkbox::FBGraphUser& userInfo )
{
    CCLOG("##FB> %s", __FUNCTION__);
}
















