
#include <memory>
#include "base/memory/weak_ptr.h"
#include "base/strings/string_piece.h"
#include "components/sessions/core/session_id.h"
#include "ios/web/public/web_state_observer.h"

class Browser;
class GURL;
class NavigationDelegate;

class BraveNativeWebState final {
  public:
    BraveNativeWebState(Browser* browser, bool off_the_record);
    ~BraveNativeWebState();

    void SetTitle(const std::u16string& title);
    void SetURL(const GURL& url);
    base::WeakPtr<web::WebState> GetWeakWebState();

  private:
    class Observer : public web::WebStateObserver {
      public:
        explicit Observer(BraveNativeWebState* tab);
        Observer(const Observer&) = delete;
        Observer& operator=(const Observer&) = delete;
        ~Observer() override;

      private:
        // WebStateObserver:
        void WebStateDestroyed(web::WebState* web_state) override;
        BraveNativeWebState* native_state_;  // NOT OWNED
    };

    Browser* browser_;
    SessionID session_id_;
    std::unique_ptr<NavigationDelegate> navigation_delegate_;
    web::WebState* web_state_;
    std::unique_ptr<Observer> web_state_observer_;
};
