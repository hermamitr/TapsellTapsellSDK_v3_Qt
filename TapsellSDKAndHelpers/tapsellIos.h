#ifndef TAPSELLIOS_H
#define TAPSELLIOS_H

#include <functional>
#include <map>
#include <QString>

#include "TSTapsell.h"

#define ON_AD_SHOW_FINISHED std::function<void(QString, QString, bool, bool)>
#define ON_AD_SHOW_FINISHED_WRAPPER std::function<void(char *, char *, bool, bool)>
#define ON_AD_AVAILABLE_CB std::function<void(QString)>
#define ON_AD_AVAILABLE_CB_WRAPPER std::function<void(char *)>
#define ON_ERROR_CB std::function<void(QString)>
#define ON_ERROR_CB_WRAPPER std::function<void(char *)>
#define ON_NO_AD_AVAILABLE_CB std::function<void()>
#define ON_NO_NETWORK_CB std::function<void()>
#define ON_EXPIRING_CB std::function<void(QString)>
#define ON_EXPIRING_CB_WRAPPER std::function<void(char *)>
#define ON_OPENED_CB std::function<void(QString)>
#define ON_OPENED_CB_WRAPPER std::function<void(char *)>
#define ON_CLOSED_CB std::function<void(QString)>
#define ON_CLOSED_CB_WRAPPER std::function<void(char *)>

#define ON_NATIVE_BANNER_AD_AVAILABLE_CB std::function<void(QString, QString, QString, QString, QString, QString, QString)>
#define ON_NATIVE_BANNER_AD_AVAILABLE_CB_WRAPPER std::function<void(char *, char *, char *, char *, char *, char *, char *)>
#define ON_NATIVE_BANNER_ERROR_CB std::function<void(QString)>
#define ON_NATIVE_BANNER_ERROR_CB_WRAPPER std::function<void(char *)>
#define ON_NATIVE_BANNER_NO_AD_AVAILABLE_CB std::function<void()>
#define ON_NATIVE_BANNER_NO_NETWORK_CB std::function<void()>

#define ON_NATIVE_VIDEO_AD_AVAILABLE_CB std::function<void(QString, QString, QString, QString, QString, QString)>
#define ON_NATIVE_VIDEO_AD_AVAILABLE_CB_WRAPPER std::function<void(char *, char *, char *, char *, char *, char *)>
#define ON_NATIVE_VIDEO_ERROR_CB std::function<void(QString)>
#define ON_NATIVE_VIDEO_ERROR_CB_WRAPPER std::function<void(char *)>
#define ON_NATIVE_VIDEO_NO_AD_AVAILABLE_CB std::function<void()>
#define ON_NATIVE_VIDEO_NO_NETWORK_CB std::function<void()>

#define ROTATION_LOCKED_PORTRAIT 1
#define ROTATION_LOCKED_LANDSCAPE 2
#define ROTATION_UNLOCKED 3
#define ROTATION_LOCKED_REVERSED_LANDSCAPE 4
#define ROTATION_LOCKED_REVERSED_PORTRAIT 5

#define PERMISSION_HANDLER_DISABLED 0
#define PERMISSION_HANDLER_AUTO 1
#define PERMISSION_HANDLER_AUTO_INSIST 2

#define BANNER_320x50 1
#define BANNER_320x100 2
#define BANNER_250x250 3
#define BANNER_300x250 4

#define TOP 1
#define BOTTOM 2
#define LEFT 3
#define RIGHT 4
#define CENTER 5


class Tapsell{
public:
    static void initialize(QString appKey) {
        QByteArray appKeyByteArray = appKey.toLatin1();
        char *finalAppKey = appKeyByteArray.data();
        TSTapsell::initialize(finalAppKey);
    }

    static void setRewardListener(ON_AD_SHOW_FINISHED onAdShowFinished) {
        onAdShowFinishedCb = onAdShowFinished;

        ON_AD_SHOW_FINISHED_WRAPPER onAdShowFinishedWrapper = [onAdShowFinished](char *arg_0, char *arg_1, bool arg_2, bool arg_3){
          onAdShowFinished(QString(arg_0), QString(arg_1), arg_2, arg_3);
        };

        TSTapsell::setRewardListener(onAdShowFinishedWrapper);
    }

    static void requestAd(QString zoneId, bool isCached, ON_AD_AVAILABLE_CB onAdAvailable,
                          ON_NO_AD_AVAILABLE_CB onNoAdAvailable, ON_NO_NETWORK_CB onNoNetwork,
                          ON_ERROR_CB onError, ON_EXPIRING_CB onExpiring) {
        onAdAvailableCbs[zoneId.toStdString()] = onAdAvailable;
        onErrorCbs[zoneId.toStdString()] = onError;
        onNoAdAvailableCbs[zoneId.toStdString()] = onNoAdAvailable;
        onNoNetworkCbs[zoneId.toStdString()] = onNoNetwork;
        onExpiringCbs[zoneId.toStdString()] = onExpiring;

        ON_AD_AVAILABLE_CB_WRAPPER onAdAvailableWrapper = [onAdAvailable](char *arg_0){
            onAdAvailable(QString(arg_0));
        };

        ON_ERROR_CB_WRAPPER onErrorWrapper = [onError](char *arg_0){
            onError(QString(arg_0));
        };

        ON_ERROR_CB_WRAPPER onExpiringWrapper = [onExpiring](char *arg_0){
          onExpiring(QString(arg_0));
        };

        QByteArray zoneIdInByteArrayFormat = zoneId.toLatin1();
        char *zoneIdWithProperFormat = zoneIdInByteArrayFormat.data();

        TSTapsell::requestAd(zoneIdWithProperFormat, isCached, onAdAvailableWrapper,
                             onNoAdAvailable, onNoNetwork,
                             onErrorWrapper, onExpiringWrapper);
    }

    static void showAd(QString zoneId, QString adId, bool back_disabled,
                       bool immersive_mode, int rotation_mode, bool showExitDialog){
        ON_OPENED_CB_WRAPPER onOpenedWrapper = [](char *arg_0){};
        
        ON_CLOSED_CB_WRAPPER onClosedWrapper = [](char *arg_0){};
        
        QByteArray zoneIdInByteArrayFormat = zoneId.toLatin1();
        char *zoneIdWithProperFormat = zoneIdInByteArrayFormat.data();
        
        QByteArray adIdInByteArrayFormat = adId.toLatin1();
        char *adIdWithProperFormat = adIdInByteArrayFormat.data();
        
        TSTapsell::showAd(zoneIdWithProperFormat, adIdWithProperFormat, back_disabled,
                          immersive_mode, rotation_mode, showExitDialog,
                          onOpenedWrapper, onClosedWrapper);
    }

    static void showAd(QString zoneId, QString adId, bool back_disabled,
                       bool immersive_mode, int rotation_mode, bool showExitDialog,
                       ON_OPENED_CB onOpened, ON_CLOSED_CB onClosed){
        onOpenedCbs[zoneId.toStdString()] = onOpened;
        onClosedCbs[zoneId.toStdString()] = onClosed;
        
        ON_OPENED_CB_WRAPPER onOpenedWrapper = [onOpened](char *arg_0){
            onOpened(QString(arg_0));
        };
        
        ON_CLOSED_CB_WRAPPER onClosedWrapper = [onClosed](char *arg_0){
            onClosed(QString(arg_0));
        };
        
        QByteArray zoneIdInByteArrayFormat = zoneId.toLatin1();
        char *zoneIdWithProperFormat = zoneIdInByteArrayFormat.data();
        
        QByteArray adIdInByteArrayFormat = adId.toLatin1();
        char *adIdWithProperFormat = adIdInByteArrayFormat.data();
        
        TSTapsell::showAd(zoneIdWithProperFormat, adIdWithProperFormat, back_disabled,
                          immersive_mode, rotation_mode, showExitDialog,
                          onOpenedWrapper, onClosedWrapper);
    }

    static void setDebugMode(bool debug) {
        TSTapsell::setDebugMode(debug);
    }

    static bool isDebugMode() {
        return TSTapsell::isDebugMode();
    }

    static void setAppUserId(QString appUserId) {
        QByteArray appUserIdInByteArrayFormat = appUserId.toLatin1();
        char *appUserIdWithProperFormat = appUserIdInByteArrayFormat.data();
        
        TSTapsell::setAppUserId(appUserIdWithProperFormat);
    }

    static QString getAppUserId() {
        return QString(TSTapsell::getAppUserId());
    }

    static QString getVersion() {
        return QString(TSTapsell::getVersion());
    }

    static void requestNativeBannerAd(QString zoneId, ON_NATIVE_BANNER_AD_AVAILABLE_CB onAdAvailable,
                          ON_NATIVE_BANNER_NO_AD_AVAILABLE_CB onNoAdAvailable, ON_NATIVE_BANNER_NO_NETWORK_CB onNoNetwork,
                          ON_NATIVE_BANNER_ERROR_CB onError) {
        QByteArray zoneIdInByteArrayFormat = zoneId.toLatin1();
        char *zoneIdWithProperFormat = zoneIdInByteArrayFormat.data();
        
        ON_NATIVE_BANNER_AD_AVAILABLE_CB_WRAPPER onAdAvailableWrapper = [onAdAvailable](char *arg_0, char *arg_1,
                                                                                        char *arg_2, char *arg_3,
                                                                                        char *arg_4, char *arg_5, char *arg_6){
            onAdAvailable(QString(arg_0), QString(arg_1), QString(arg_2),
                          QString(arg_3), QString(arg_4), QString(arg_5), QString(arg_6));
        };
        
        ON_NATIVE_BANNER_ERROR_CB_WRAPPER onErrorWrapper = [onError](char *arg_0){
            onError(QString(arg_0));
        };
        
        TSTapsell::requestNativeBannerAd(zoneIdWithProperFormat, onAdAvailableWrapper,
                                         onNoAdAvailable, onNoNetwork, onErrorWrapper);
    }

    static void onNativeBannerAdShown(QString adId) {
        QByteArray adIdInByteArrayFormat = adId.toLatin1();
        char *adIdWithProperFormat = adIdInByteArrayFormat.data();
        
        TSTapsell::onNativeBannerAdShown(adIdWithProperFormat);
    }

    static void onNativeBannerAdClicked(QString adId) {
        QByteArray adIdInByteArrayFormat = adId.toLatin1();
        char *adIdWithProperFormat = adIdInByteArrayFormat.data();
        
        TSTapsell::onNativeBannerAdClicked(adIdWithProperFormat);
    }

    static void requestNativeVideoAd(QString zoneId, ON_NATIVE_VIDEO_AD_AVAILABLE_CB onAdAvailable,
                          ON_NATIVE_VIDEO_NO_AD_AVAILABLE_CB onNoAdAvailable, ON_NATIVE_VIDEO_NO_NETWORK_CB onNoNetwork,
                          ON_NATIVE_VIDEO_ERROR_CB onError) {
        QByteArray zoneIdInByteArrayFormat = zoneId.toLatin1();
        char *zoneIdWithProperFormat = zoneIdInByteArrayFormat.data();

        ON_NATIVE_VIDEO_AD_AVAILABLE_CB_WRAPPER onAdAvailableWrapper = [onAdAvailable](char *arg_0, char *arg_1,
                                                                                        char *arg_2, char *arg_3,
                                                                                        char *arg_4, char *arg_5){
            onAdAvailable(QString(arg_0), QString(arg_1), QString(arg_2),
                          QString(arg_3), QString(arg_4), QString(arg_5));
        };

        ON_NATIVE_VIDEO_ERROR_CB_WRAPPER onErrorWrapper = [onError](char *arg_0){
            onError(QString(arg_0));
        };
        
        TSTapsell::requestNativeVideoAd(zoneIdWithProperFormat, onAdAvailableWrapper,
                                         onNoAdAvailable, onNoNetwork, onErrorWrapper);
    }

    static void onNativeVideoAdShown(QString adId) {
        QByteArray adIdInByteArrayFormat = adId.toLatin1();
        char *adIdWithProperFormat = adIdInByteArrayFormat.data();
        
        TSTapsell::onNativeVideoAdShown(adIdWithProperFormat);
    }

    static void onNativeVideoAdClicked(QString adId) {
        QByteArray adIdInByteArrayFormat = adId.toLatin1();
        char *adIdWithProperFormat = adIdInByteArrayFormat.data();
        
        TSTapsell::onNativeVideoAdClicked(adIdWithProperFormat);
    }

    static void requestStandardBannerAd(QString zoneId, int bannerType,
                                        int horizontalGravity, int verticalGravity) {
        QByteArray zoneIdInByteArrayFormat = zoneId.toLatin1();
        char *zoneIdWithProperFormat = zoneIdInByteArrayFormat.data();
        
        TSTapsell::requestStandardBannerAd(zoneIdWithProperFormat, bannerType, horizontalGravity, verticalGravity);
    }

    static ON_AD_SHOW_FINISHED onAdShowFinishedCb;
    static std::map<std::string, ON_AD_AVAILABLE_CB> onAdAvailableCbs;
    static std::map<std::string, ON_ERROR_CB> onErrorCbs;
    static std::map<std::string, ON_NO_AD_AVAILABLE_CB> onNoAdAvailableCbs;
    static std::map<std::string, ON_NO_NETWORK_CB> onNoNetworkCbs;
    static std::map<std::string, ON_EXPIRING_CB> onExpiringCbs;
    static std::map<std::string, ON_OPENED_CB> onOpenedCbs;
    static std::map<std::string, ON_CLOSED_CB> onClosedCbs;

    static std::map<std::string, ON_NATIVE_BANNER_AD_AVAILABLE_CB> onNativeBannerAdAvailableCbs;
    static std::map<std::string, ON_NATIVE_BANNER_ERROR_CB> onNativeBannerErrorCbs;
    static std::map<std::string, ON_NATIVE_BANNER_NO_AD_AVAILABLE_CB> onNativeBannerNoAdAvailableCbs;
    static std::map<std::string, ON_NATIVE_BANNER_NO_NETWORK_CB> onNativeBannerNoNetworkCbs;

    static std::map<std::string, ON_NATIVE_VIDEO_AD_AVAILABLE_CB> onNativeVideoAdAvailableCbs;
    static std::map<std::string, ON_NATIVE_VIDEO_ERROR_CB> onNativeVideoErrorCbs;
    static std::map<std::string, ON_NATIVE_VIDEO_NO_AD_AVAILABLE_CB> onNativeVideoNoAdAvailableCbs;
    static std::map<std::string, ON_NATIVE_VIDEO_NO_NETWORK_CB> onNativeVideoNoNetworkCbs;
};

#endif // TAPSELLIOS_H
