#ifdef __cplusplus
#import "react-native-fast-base64.h"
#endif

#ifdef RCT_NEW_ARCH_ENABLED
#import "RNFastBase64Spec.h"

@interface FastBase64 : NSObject <NativeFastBase64Spec>
#else
#import <React/RCTBridgeModule.h>

@interface FastBase64 : NSObject <RCTBridgeModule>
#endif

@property (nonatomic, assign) BOOL setBridgeOnMainQueue;

@end
