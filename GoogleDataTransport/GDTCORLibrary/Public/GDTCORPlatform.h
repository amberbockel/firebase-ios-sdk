/*
 * Copyright 2019 Google
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#import <Foundation/Foundation.h>

#if !TARGET_OS_WATCH
#import <SystemConfiguration/SystemConfiguration.h>
#endif

#if TARGET_OS_IOS || TARGET_OS_TV
#import <UIKit/UIKit.h>
#elif TARGET_OS_OSX
#import <AppKit/AppKit.h>
#elif TARGET_OS_WATCH
#import <WatchKit/WatchKit.h>
#endif  // TARGET_OS_IOS || TARGET_OS_TV

#if TARGET_OS_IOS
#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#endif

NS_ASSUME_NONNULL_BEGIN

/** The GoogleDataTransport library version. */
FOUNDATION_EXPORT NSString *const kGDTCORVersion;

/** A notification sent out if the app is backgrounding. */
FOUNDATION_EXPORT NSString *const kGDTCORApplicationDidEnterBackgroundNotification;

/** A notification sent out if the app is foregrounding. */
FOUNDATION_EXPORT NSString *const kGDTCORApplicationWillEnterForegroundNotification;

/** A notification sent out if the app is terminating. */
FOUNDATION_EXPORT NSString *const kGDTCORApplicationWillTerminateNotification;

/** The different possible network connection type. */
typedef NS_ENUM(NSInteger, GDTCORNetworkType) {
  GDTCORNetworkTypeUNKNOWN = 0,
  GDTCORNetworkTypeWIFI = 1,
  GDTCORNetworkTypeMobile = 2,
};

/** The different possible network connection mobile subtype. */
typedef NS_ENUM(NSInteger, GDTCORNetworkMobileSubtype) {
  GDTCORNetworkMobileSubtypeUNKNOWN = 0,
  GDTCORNetworkMobileSubtypeGPRS = 1,
  GDTCORNetworkMobileSubtypeEdge = 2,
  GDTCORNetworkMobileSubtypeWCDMA = 3,
  GDTCORNetworkMobileSubtypeHSDPA = 4,
  GDTCORNetworkMobileSubtypeHSUPA = 5,
  GDTCORNetworkMobileSubtypeCDMA1x = 6,
  GDTCORNetworkMobileSubtypeCDMAEVDORev0 = 7,
  GDTCORNetworkMobileSubtypeCDMAEVDORevA = 8,
  GDTCORNetworkMobileSubtypeCDMAEVDORevB = 9,
  GDTCORNetworkMobileSubtypeHRPD = 10,
  GDTCORNetworkMobileSubtypeLTE = 11,
};

#if !TARGET_OS_WATCH
/** Compares flags with the WWAN reachability flag, if available, and returns YES if present.
 *
 * @param flags The set of reachability flags.
 * @return YES if the WWAN flag is set, NO otherwise.
 */
BOOL GDTCORReachabilityFlagsContainWWAN(SCNetworkReachabilityFlags flags);
#endif

/** Generates an enum message GDTCORNetworkType representing network connection type.
 *
 * @return A GDTCORNetworkType representing network connection type.
 */
GDTCORNetworkType GDTCORNetworkTypeMessage(void);

/** Generates an enum message GDTCORNetworkMobileSubtype representing network connection mobile
 * subtype.
 *
 * @return A GDTCORNetworkMobileSubtype representing network connection mobile subtype.
 */
GDTCORNetworkMobileSubtype GDTCORNetworkMobileSubTypeMessage(void);

/** A typedef identify background identifiers. */
typedef volatile NSUInteger GDTCORBackgroundIdentifier;

/** A background task's invalid sentinel value. */
FOUNDATION_EXPORT const GDTCORBackgroundIdentifier GDTCORBackgroundIdentifierInvalid;

#if TARGET_OS_IOS || TARGET_OS_TV
/** A protocol that wraps UIApplicationDelegate, WKExtensionDelegate or NSObject protocol, depending
 * on the platform.
 */
@protocol GDTCORApplicationDelegate <UIApplicationDelegate>
#elif TARGET_OS_OSX
@protocol GDTCORApplicationDelegate <NSApplicationDelegate>
#elif TARGET_OS_WATCH
@protocol GDTCORApplicationDelegate <WKExtensionDelegate>
#else
@protocol GDTCORApplicationDelegate <NSObject>
#endif  // TARGET_OS_IOS || TARGET_OS_TV

@end

/** A cross-platform application class. */
@interface GDTCORApplication : NSObject <GDTCORApplicationDelegate>

/** Flag to determine if the application is running in the background. */
@property(atomic, readonly) BOOL isRunningInBackground;

/** Creates and/or returns the shared application instance.
 *
 * @return The shared application instance.
 */
+ (nullable GDTCORApplication *)sharedApplication;

#if !TARGET_OS_WATCH
/** Creates a background task with the returned identifier if on a suitable platform.
 *
 * @name name The name of the task, useful for debugging which background tasks are running.
 * @param handler The handler block that is called if the background task expires.
 * @return An identifier for the background task, or GDTCORBackgroundIdentifierInvalid if one
 * couldn't be created.
 */
- (GDTCORBackgroundIdentifier)beginBackgroundTaskWithName:(NSString *)name
                                        expirationHandler:(void (^__nullable)(void))handler;

/** Ends the background task if the identifier is valid.
 *
 * @param bgID The background task to end.
 */
- (void)endBackgroundTask:(GDTCORBackgroundIdentifier)bgID;

#elif TARGET_OS_WATCH
/** Begin an activity using the given options and reason.
 *
 * @param options NSActivityOptions for activity.
 * @param reason A string used in debugging to indicate the reason the activity began.
 * @return An object token representing the activity.
 */
- (id<NSObject>)beginActivityWithOptions:(NSActivityOptions)options reason:(NSString *)reason;

/** Ends the given activity.
 *
 * @param activity The activity to end.
 */
- (void)endActivity:(id<NSObject>)activity;

#endif

/** Creates and executes a background task on a suitable platform.
 *
 * @name name The name of the task, useful for debugging which background tasks are running.
 * @param object The NSObject that calls this Api, such as GDTCORTransformer, GDTCORStorage.
 * @param block The block that should be execute.
 */
- (void)beginBackgroundTaskWithNameBlock:(NSString *)name initiator:(id<NSObject> _Nonnull) object usingblock:(void (^)(void))block;

@end

NS_ASSUME_NONNULL_END
