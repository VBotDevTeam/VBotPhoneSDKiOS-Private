# VBotPhoneSDKiOS-Private

SDK tích hợp cuộc gọi VoIP cho ứng dụng iOS, hỗ trợ CallKit, PushKit và quản lý cuộc gọi đến/đi.

## Code demo

[https://github.com/VBotDevTeam/VBot-iOS-SDK-Example](https://github.com/VBotDevTeam/VBot-iOS-SDK-Example)

## Yêu cầu

- iOS 13.0+
- Swift 5.0+
- Xcode 14+

## Cài đặt SDK

### CocoaPods

Thêm **VBotPhoneSDKiOS-Private** vào `Podfile`:

```ruby
platform :ios, '13.0'

target 'YourApp' do
  use_frameworks! :linkage => :static

  pod 'VBotPhoneSDKiOS-Private'

end

post_install do |installer|
  installer.pods_project.targets.each do |target|
    target.build_configurations.each do |config|
      # Bắt buộc để VBotPhoneSDK hoạt động với các phiên bản Swift mới hơn
      config.build_settings['BUILD_LIBRARY_FOR_DISTRIBUTION'] = 'YES'
    end
  end
end
```

Sau đó chạy:

```bash
pod install
```

> **Lưu ý:** `BUILD_LIBRARY_FOR_DISTRIBUTION = YES` là **bắt buộc**.

## Cấu hình dự án

### 1. Bật Capabilities

Trong Xcode → target app → **Signing & Capabilities**, thêm:

- **Background Modes**: bật `Voice over IP` và `Remote notifications`
- **Push Notifications**

### 2. Info.plist

Thêm các key sau vào `Info.plist`:

```xml
<!-- Quyền sử dụng microphone -->
<key>NSMicrophoneUsageDescription</key>
<string>Ứng dụng cần truy cập microphone để thực hiện cuộc gọi</string>
```

### 3. Xử lý lỗi Sandbox

Nếu Xcode trả về lỗi: `Sandbox: rsync.samba deny(1) file-write-create`

→ Trong **Build Settings**, tìm **User Script Sandboxing** → chọn **No**

## Tích hợp SDK

### 1. Khởi tạo SDK

```swift
// AppDelegate.swift - didFinishLaunchingWithOptions
let config = VBotConfig(
    supportPopupCall: true,              // Hiển thị popup call in-app
    includesCallsInRecents: true,        // Hiển thị trong lịch sử cuộc gọi iOS
    iconTemplateImageData: UIImage(named: "callkit-icon")?.pngData()
)

let success = VBotPhone.sharedInstance.setup(token: "your_encrypted_token", with: config)
```

Trong đó:

- **token** là App Token, đại diện cho ứng dụng của bạn, dùng để xác thực với máy chủ VBot
- **config** là cấu hình tùy chọn cho SDK

> **Lưu ý:** `setup()` trả về `Bool` — `false` nếu đang có cuộc gọi active, không thể re-setup.

### 2. Đăng ký PushKit (nhận cuộc gọi đến)

```swift
// AppDelegate.swift - didFinishLaunchingWithOptions
voipRegistry = PKPushRegistry(queue: .main)
voipRegistry!.desiredPushTypes = [.voIP]
voipRegistry!.delegate = self
```

### 3. Xử lý cuộc gọi đến

```swift
// PKPushRegistryDelegate
func pushRegistry(_ registry: PKPushRegistry,
                  didReceiveIncomingPushWith payload: PKPushPayload,
                  for type: PKPushType,
                  completion: @escaping () -> Void) {
    guard type == .voIP else { completion(); return }

    let data = payload.dictionaryPayload as? [String: Any] ?? [:]
    let metaData = data["metaData"] as? [String: Any] ?? [:]

    VBotPhone.sharedInstance.startIncomingCall(
        callerId: data["callerId"] as? String ?? "",
        callerName: data["callerName"] as? String ?? "",
        callerAvatar: data["callerAvatar"] as? String ?? "",
        calleeId: data["calleeId"] as? String ?? "",
        calleeName: data["calleeName"] as? String ?? "",
        calleeAvatar: data["calleeAvatar"] as? String ?? "",
        checkSum: metaData["check_sum"] as? String ?? "",
        metaData: metaData,
        completion: completion
    )
}
```

### 4. Gọi đi

```swift
VBotPhone.sharedInstance.startOutgoingCall(
    callerId: "caller_id",
    callerName: "Caller Name",
    calleeId: "callee_id",
    calleeName: "Callee Name",
    checkSum: "unique_call_id",
    metaData: ["key": "value"]
) { result in
    switch result {
    case .success(): break
    case .failure(let error): print(error)
    }
}
```

### 5. Gọi lại từ lịch sử cuộc gọi iPhone

Nếu bật `includesCallsInRecents: true`, lịch sử cuộc gọi sẽ được lưu vào ứng dụng Điện thoại. Người dùng có thể chọn vào lịch sử này để gọi lại.

Trong file `AppDelegate.swift`, thêm:

```swift
import Intents

// Đón sự kiện người dùng bấm vào lịch sử để gọi lại
func application(_ application: UIApplication,
                 continue userActivity: NSUserActivity,
                 restorationHandler: @escaping ([UIUserActivityRestoring]?) -> Void) -> Bool {
    switch userActivity.activityType {
    case "INStartAudioCallIntent":
        return handleStartCallIntent(
            INStartAudioCallIntent.self,
            userActivity: userActivity,
            contacts: \.contacts
        )
    case "INStartCallIntent":
        return handleStartCallIntent(
            INStartCallIntent.self,
            userActivity: userActivity,
            contacts: \.contacts
        )
    default:
        return false
    }
}

// Lấy ra calleeId để gọi lại
private func handleStartCallIntent<T: INIntent>(
    _ intentType: T.Type,
    userActivity: NSUserActivity,
    contacts: KeyPath<T, [INPerson]?>
) -> Bool {
    guard let intent = userActivity.interaction?.intent as? T,
          let person = intent[keyPath: contacts]?.first else {
        return false
    }

    let (id, name) = VBotPhone.sharedInstance.getCallDataFromUserActivity(person.displayName)
    // Gọi hàm startOutgoingCall và truyền data để gọi lại
    // VBotPhone.sharedInstance.startOutgoingCall(...)
    return true
}
```

### 6. Implement VBotPhoneDelegate

Đăng ký nhận sự kiện cuộc gọi:

```swift
// Đăng ký
VBotPhone.sharedInstance.addDelegate(self)

// Hủy đăng ký
deinit {
    VBotPhone.sharedInstance.removeDelegate(self)
}
```

Các delegate method:

```swift
extension YourClass: VBotPhoneDelegate {
    // Trạng thái cuộc gọi thay đổi
    func callStateChanged(state: VBotCallState) { }

    // Cuộc gọi đi đã bắt đầu
    func callStarted() { }

    // Cuộc gọi đến được chấp nhận
    func callAccepted() { }

    // Cuộc gọi kết thúc
    func callEnded() { }

    // Trạng thái Microphone thay đổi
    func callMuteStateDidChange(muted: Bool) { }

    // Thông tin cuộc gọi cập nhật
    func callInfoUpdated() { }

    // Nhấn vào nút nhắn tin
    func messageButtonTapped() { }
}
```

---

## Thao tác trong cuộc gọi

Từ ứng dụng của bạn, có thể gọi các hàm sau để thao tác trong cuộc gọi:

```swift
// Kiểm tra xem có cuộc gọi active hay không
VBotPhone.sharedInstance.hasActiveCall()

// Quay lại màn hình cuộc gọi
VBotPhone.sharedInstance.returnToCallVCIfNeeded()

// Ẩn màn hình cuộc gọi
VBotPhone.sharedInstance.hideCallVCIfNeeded()

// Kết thúc cuộc gọi
VBotPhone.sharedInstance.endCall { error in }

// Tắt/bật microphone
VBotPhone.sharedInstance.muteCall { error in }

// Lấy trạng thái microphone
VBotPhone.sharedInstance.isCallMute()

// Bật/tắt loa ngoài
VBotPhone.sharedInstance.onOffSpeaker()

// Lấy trạng thái loa ngoài
VBotPhone.sharedInstance.isSpeakerOn()
```

---

## Đa ngôn ngữ

Các màn hình cuộc gọi VBot SDK mặc định hỗ trợ Tiếng Việt.

Để thay đổi ngôn ngữ, truyền dữ liệu qua `metaData` khi gọi hàm `startIncomingCall` và `startOutgoingCall`.

---

## Sử dụng với nhiều token (Multi-Tenant)

SDK hỗ trợ multi-tenant — mỗi tenant/tổ chức sử dụng một token riêng. Khi user thuộc nhiều tổ chức, app cần quản lý việc chuyển đổi token.

### Luồng hoạt động

```
User đăng nhập → Lấy danh sách tenant → Chọn tenant → setup(token) → Sẵn sàng gọi
                                              ↓
                                     Chuyển tenant → setup(token mới) → Đăng ký lại PushKit
```

### Triển khai

```swift
// MARK: - Lưu trữ token theo tenant

struct TenantManager {
    private static let currentTenantKey = "vbot_current_tenant_id"

    /// Lưu token cho một tenant (Khuyến cáo: Nên sử dụng Keychain để bảo mật thay vì UserDefaults)
    static func saveToken(_ token: String, forTenant tenantId: String) {
        // Ví dụ sử dụng Keychain giả định (như KeychainSwift)
        // KeychainSwift().set(token, forKey: "vbot_token_\(tenantId)")

        // Demo
        SecureStorage.save(token, forKey: "vbot_token_\(tenantId)")
    }

    /// Lấy token của tenant
    static func getToken(forTenant tenantId: String) -> String? {
        // return KeychainSwift().get("vbot_token_\(tenantId)")
        return SecureStorage.get(forKey: "vbot_token_\(tenantId)")
    }

    /// Lưu tenant đang active (có thể lưu UserDefaults vì không phải dữ liệu nhạy cảm)
    static func setCurrentTenant(_ tenantId: String) {
        UserDefaults.standard.set(tenantId, forKey: currentTenantKey)
    }

    /// Lấy tenant đang active
    static func getCurrentTenantId() -> String? {
        return UserDefaults.standard.string(forKey: currentTenantKey)
    }
}
```

```swift
// MARK: - Khi user đăng nhập và chọn tenant

func onTenantSelected(tenant: Tenant) {
    // 1. Lưu token và tenant hiện tại
    TenantManager.saveToken(tenant.token, forTenant: tenant.id)
    TenantManager.setCurrentTenant(tenant.id)

    // 2. Setup SDK với token mới
    let config = VBotConfig(
        supportPopupCall: true,
        includesCallsInRecents: true,
        iconTemplateImageData: UIImage(named: "callkit-icon")?.pngData()
    )
    VBotPhone.sharedInstance.setup(token: tenant.token, with: config)

    // 3. Gửi PushKit token lên server của tenant mới
    if let pushToken = currentPushToken {
        sendPushToken(pushToken, toTenant: tenant.id)
    }
}
```

```swift
// MARK: - Chuyển đổi giữa các tenant

func switchTenant(to newTenantId: String) {
    guard let token = TenantManager.getToken(forTenant: newTenantId) else {
        print("Không tìm thấy token cho tenant: \(newTenantId)")
        return
    }

    let config = VBotConfig(
        supportPopupCall: true,
        includesCallsInRecents: true,
        iconTemplateImageData: UIImage(named: "callkit-icon")?.pngData()
    )

    let success = VBotPhone.sharedInstance.setup(token: token, with: config)

    if success {
        let oldTenantId = TenantManager.getCurrentTenantId()
        TenantManager.setCurrentTenant(newTenantId)

        // Chuyển PushKit token sang server tenant mới
        if let pushToken = currentPushToken {
            if let oldId = oldTenantId {
                removePushToken(fromTenant: oldId)
            }
            sendPushToken(pushToken, toTenant: newTenantId)
        }
    } else {
        print("Không thể chuyển tenant — đang có cuộc gọi active")
    }
}
```

```swift
// MARK: - Khôi phục khi mở app

// AppDelegate.swift - didFinishLaunchingWithOptions
func restoreLastTenant() {
    guard let tenantId = TenantManager.getCurrentTenantId(),
          let token = TenantManager.getToken(forTenant: tenantId) else {
        return
    }

    let config = VBotConfig(
        supportPopupCall: true,
        includesCallsInRecents: true,
        iconTemplateImageData: UIImage(named: "callkit-icon")?.pngData()
    )
    VBotPhone.sharedInstance.setup(token: token, with: config)
}
```

> **Lưu ý quan trọng:**
>
> - `setup()` trả về `false` nếu đang có cuộc gọi active → không thể chuyển tenant khi đang gọi.
> - Khi chuyển tenant, **phải** gửi PushKit token lên server tenant mới và xóa ở server tenant cũ, nếu không cuộc gọi đến sẽ không hoạt động đúng.
> - Mỗi thời điểm chỉ có **một tenant active** — SDK không hỗ trợ nhận cuộc gọi từ nhiều tenant cùng lúc.

---

## SceneDelegate

SDK tương thích với cả app dùng `AppDelegate` lẫn `SceneDelegate`. Call UI window tự động gắn `UIWindowScene` khi cần — không cần cấu hình thêm từ phía app.

---

## Ví dụ đầy đủ

```swift
import VBotPhoneSDK
import PushKit

@main
class AppDelegate: UIResponder, UIApplicationDelegate {

    var voipRegistry: PKPushRegistry!

    func application(_ application: UIApplication,
                     didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {

        // Cấu hình SDK
        let config = VBotConfig(
            supportPopupCall: true,
            iconTemplateImageData: UIImage(named: "callkit-icon")?.pngData()
        )

        let token = "your_encrypted_token"
        VBotPhone.sharedInstance.setup(token: token, with: config)

        // Khởi tạo PKPushRegistry
        voipRegistry = PKPushRegistry(queue: .main)
        voipRegistry!.desiredPushTypes = [.voIP]
        voipRegistry!.delegate = self

        return true
    }
}

// Lắng nghe các sự kiện của PushKit
extension AppDelegate: PKPushRegistryDelegate {

    func pushRegistry(_ registry: PKPushRegistry,
                      didUpdate pushCredentials: PKPushCredentials,
                      for type: PKPushType) {
        // Lưu push token để server gửi thông báo VoIP đến thiết bị
    }

    func pushRegistry(_ registry: PKPushRegistry,
                      didReceiveIncomingPushWith payload: PKPushPayload,
                      for type: PKPushType,
                      completion: @escaping () -> Void) {
        guard type == .voIP else { completion(); return }

        let data = payload.dictionaryPayload as? [String: Any] ?? [:]
        let metaData = data["metaData"] as? [String: Any] ?? [:]

        VBotPhone.sharedInstance.startIncomingCall(
            callerId: data["callerId"] as? String ?? "",
            callerName: data["callerName"] as? String ?? "",
            callerAvatar: data["callerAvatar"] as? String ?? "",
            calleeId: data["calleeId"] as? String ?? "",
            calleeName: data["calleeName"] as? String ?? "",
            calleeAvatar: data["calleeAvatar"] as? String ?? "",
            checkSum: metaData["check_sum"] as? String ?? "",
            metaData: metaData,
            completion: completion
        )
    }

    func pushRegistry(_ registry: PKPushRegistry,
                      didInvalidatePushTokenFor type: PKPushType) { }
}

// Lắng nghe sự kiện cuộc gọi
extension AppDelegate: VBotPhoneDelegate {
    func callStarted() { }
    func callAccepted() { }
    func callEnded() { }
    func callStateChanged(state: VBotCallState) { }
    func callMuteStateDidChange(muted: Bool) { }
    func callInfoUpdated() { }
    func messageButtonTapped() { }
}
```

---

## Tham khảo

### VBotEndCallReason

```swift
@objc public enum VBotEndCallReason: Int {
    case timeOut = -1001                    // Timeout
    case initiationFailed = 1001            // Khởi tạo không thành công
    case initiationFailed_1 = 1002          // Không sẵn sàng để gọi đi
    case microphonePermissionDenied = 1003  // Chưa cấp quyền mic
    case invalidPhoneNumber = 1004          // Số điện thoại không hợp lệ
    case noDataFromServer = 1005            // Không có dữ liệu từ máy chủ
    case endCallBeforeServerStartCall = 1006 // Kết thúc trước khi server bắt đầu
    case initiationFailed_3 = 1007          // Lỗi khi khởi tạo cuộc gọi
    case dataInvalid = 1008                 // Dữ liệu không hợp lệ
    case initiationFailed_4 = 1009          // Tài khoản VBot không tồn tại
    case authenticatedFailed = 1010         // Xác thực thất bại
    case anotherCallInProgress = 1011       // Đang có cuộc gọi khác
    case normal = 1012                      // Cuộc gọi kết thúc bình thường
    case decline = 1013                     // Từ chối cuộc gọi
    case temporarilyUnavailable = 1014      // Không liên lạc được
    case busy = 1015                        // Máy bận
    case reportNewIncomingCallFailed = 1016  // reportNewIncomingCall lỗi
    case alertDataNotFound = 1017           // Không có alert_data
    case unknownError = 1999                // Lỗi chưa xác định
}
```
