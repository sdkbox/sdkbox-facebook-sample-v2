#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "PluginFacebook/PluginFacebook.h"

class HelloWorld : public cocos2d::CCLayer, sdkbox::FacebookListener
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    
private:
    
    //Menu callbacks
    void onLogin(CCObject* sender);
    void onLogout(CCObject* sender);
    void onCheckStatus(CCObject* sender);
    void onGetMyInfo(CCObject* sender);
    void onGetMyFriends(CCObject* sender);
    void onShareLink(CCObject* sender);
    void onSharePhoto(CCObject* sender);
    void onDialogLink(CCObject* sender);
    void onDialogPhoto(CCObject* sender);
    void onRequestReadPermission(CCObject* sender);
    void onRequestPublishPermission(CCObject* sender);
    
    //Facebook callbacks
    void onLogin(bool isLogin, const std::string& msg);
    void onSharedSuccess(const std::string& message);
    void onSharedFailed(const std::string& message);
    void onSharedCancel();
    void onAPI(const std::string& key, const std::string& jsonData);
    void onPermission(bool isLogin, const std::string& msg);
    void onFetchFriends(bool ok, const std::string& msg);

    void onRequestInvitableFriends( const sdkbox::FBInvitableFriendsInfo& friends );
    void onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg );
    void onInviteFriendsResult( bool result, const std::string& msg );

    void onGetUserInfo( const sdkbox::FBGraphUser& userInfo );
};

#endif // __HELLOWORLD_SCENE_H__
