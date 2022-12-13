#import <Foundation/Foundation.h>
#import "brave/ios/browser/web/web_state/brave_web_state.h"
#include "base/memory/weak_ptr.h"

class Browser;

namespace web {
class WebState;
}  // namespace web

@interface BraveWebState(Private)
- (instancetype)initWithBrowser:(Browser*)browser isOffTheRecord:(bool)isOffTheRecord;
- (void)setTitle:(NSString*)title;
- (void)setURL:(NSURL*)url;
- (base::WeakPtr<web::WebState>)internalWebState;
@end
