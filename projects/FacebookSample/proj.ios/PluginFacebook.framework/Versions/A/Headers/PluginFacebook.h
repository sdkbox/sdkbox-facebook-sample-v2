/****************************************************************************

 Copyright (c) 2014-2015 Chukong Technologies

 ****************************************************************************/
#ifndef _PLUGIN_FACKBOOK_H_
#define _PLUGIN_FACKBOOK_H_

#include <vector>
#include <map>
#include <string>

namespace sdkbox
{
    const std::string FB_PERM_READ_PUBLIC_PROFILE("public_profile");
    const std::string FB_PERM_READ_EMAIL("email");
    const std::string FB_PERM_READ_USER_FRIENDS("user_friends");
    const std::string FB_PERM_PUBLISH_POST("publish_actions");
    const std::string FB_API_ME_FRIENDS_TAG("__fb_me_friends__");
    const std::string FB_API_ME_FRIENDS_PATH("me/friends");

    enum FBShareType
    {
        FB_NONE  = 0,
        FB_LINK  = 1,
        FB_PHOTO = 2
    };

    struct FBShareInfo
    {
        FBShareInfo()
        : type(FB_NONE), link(""), title("")
        , text(""), image("")
        {}

        FBShareType type;
        std::string link;
        std::string title;
        std::string text;
        std::string image;
    };

    struct FBGraphUser
    {
        FBGraphUser();

        std::string uid;
        std::string name;
        std::string firstName;
        std::string lastName;
        bool        isInstalled;
    };

    class FacebookListener
    {
    public:
        virtual void onLogin(bool isLogin, const std::string& msg) = 0;
        virtual void onSharedSuccess(const std::string& message) = 0;
        virtual void onSharedFailed(const std::string& message) = 0;
        virtual void onSharedCancel() = 0;
        virtual void onAPI(const std::string& key, const std::string& jsonData) = 0;
        virtual void onPermission(bool isLogin, const std::string& msg) = 0;
        virtual void onFetchFriends(bool ok, const std::string& msg) = 0;
    };

    class PluginFacebook
    {
    public:
        typedef std::map<std::string, std::string> FBAPIParam;

        /*!
         * initialize the plugin instance.
         */
        static void init();

        /**
         * Set listener to listen for facebook events
         */
        static void setListener(FacebookListener* listener);

        /**
         * Get the listener
         */
        static FacebookListener* getListener();

        /**
         * Remove the listener, and can't listen to events anymore
         */
        static void removeListener();

        /**
         * @brief log in
         */
        static void login();

        /**
         * @brief log in with specific read permissions, conflict with publish permissions
         * https://developers.facebook.com/docs/facebook-login/permissions
         *
         * @param read permissions
         */
        static void requestReadPermissions(const std::vector<std::string>& permissions);

        /**
         * @brief log in with specific public permissions
         * https://developers.facebook.com/docs/facebook-login/permissions
         *
         * @param publish permissions
         */
        static void requestPublishPermissions(const std::vector<std::string>& permissions);

        /**
         * @brief log out
         */
        static void logout();

        /**
         * @brief Check whether the user logined or not
         */
        static bool isLoggedIn();

        /**
         * @brief get UserID
         */
        static std::string getUserID();

        /**
         * @brief get AccessToken
         */
        static std::string getAccessToken();

        /**
         * @brief get permissoin list
         */
        static std::vector<std::string> getPermissionList();

        /**
         * @brief share
         *
         * @param info share information
         */
        static void share(const FBShareInfo& info);

        /**
         * @brief open a dialog of Facebook app or WebDialog (dialog with photo only avaible with native Facebook app)
         *
         * @param info share information
         */
        static void dialog(const FBShareInfo& info);

        /**
         * @brief return the version of Facebook SDK
         */
        static std::string getSDKVersion();

        /**
         * @brief use Facebook Open Graph api
         * https://developers.facebook.com/docs/ios/graph
         *
         * @param path path of Open Graph api
         * @param method HttpMethod ["GET","POST","DELETE"]
         * @param params request parameters
         * @param cb callback of request
         */
        static void api(const std::string& path, const std::string& method, const FBAPIParam& params, const std::string& tag);

        /**
         * @brief fetch friends data from Facebook
         */
        static void fetchFriends();

        /**
         * @brief get friends info
         */
        static std::vector<FBGraphUser> getFriends();

        /**
         * @brief check whether can present Facebook App
         */
        static bool canPresentWithFBApp(const FBShareInfo& info);
    };
}

#endif
