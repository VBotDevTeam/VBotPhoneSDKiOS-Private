# VBotPhoneSDKiOS-Private
VBot Phone SDK – Cho khách hàng doanh nghiệp, hỗ trợ tùy chỉnh luồng cuộc gọi và giao diện.

## Code demo

[https://github.com/VBotDevTeam/VBot-iOS-SDK-Example](https://github.com/VBotDevTeam/VBot-iOS-SDK-Example)

## Yêu cầu

iOS 12.0 trở lên

## Cài đặt SDK

### Cocoapod

Copy thư mục **Lib** từ project **VBot-iOS-SDK-Example** vào project của bạn

Thêm **VBotPhoneSDK** vào Podfile

```swift
platform :ios, '13.5'

target 'Runner' do
  use_frameworks! :linkage => :static

  pod 'VBotPhoneSDKiOS-Private', '1.0.9'

  target 'RunnerTests' do
    inherit! :search_paths
    # Pods for testing
  end

end

post_install do |installer|
  installer.pods_project.targets.each do |target|
   target.build_configurations.each do |config|
     # Bắt buộc
     config.build_settings['BUILD_LIBRARY_FOR_DISTRIBUTION'] = 'YES'
    end
  end
end
```

Trong đó phần **config.build_settings['BUILD_LIBRARY_FOR_DISTRIBUTION'] = 'YES'** là bắt buộc để VBotPhoneSDK hoạt động với các phiên bản Swift mới hơn


## Cấu hình dự án

### Bật Voip trong dự án Xcode

Chọn **Xcode Project → Capabilities**

Thêm **Background Modes** và **Push Notifications**

Ở **Background Modes,** Bật **Audio, AirPlay, and Picture in Picture |** **Voice over IP | Background Fetch | Remote Notifications**

Mở tệp **info.plist** và thêm key sau

```swift
<key>NSMicrophoneUsageDescription</key>
<string>Microphone access is necessary to be able to make calls.</string>
```

Lưu ý:

Khi khởi chạy dự án mà Xcode trả về lỗi

“Sandbox: rsync.samba (13105) deny(1) file-write-create”

Thực hiện chỉnh sửa sau:

Trong **Build Settings**, tìm **User Script Sandboxing**: Chọn **No**

## Sử dụng SDK

### Khởi tạo

File **AppDelegate.swift**

1. Trong hàm **application didFinishLaunchingWithOptions**, gọi hàm khởi tạo VBotPhone

```swift
let config = VBotConfig(
            supportPopupCall: true,
            iconTemplateImageData: UIImage(named: "callkit-icon")?.pngData())
            
VBotPhone.sharedInstance.setup(token: token, with: config)
```

Trong đó:
- **token** là App Token, đại diện cho ứng dụng của bạn được dùng để xác thực với máy chủ VBot 
- **config** là cấu hình tùy chọn cho SDK

### Gọi đi

Để thực hiện cuộc gọi đi, hãy sử dụng hàm **startOutgoingCall**

```swift
VBotPhone.sharedInstance.startOutgoingCall(
	callerId: <Mã người gọi>, 
  callerName: <Tên người gọi>, 
  callerAvatar: <Ảnh đại diện người gọi>, 
  calleeId: <Mã người nghe>, 
  calleeName: <Tên người nghe>, 
  calleeAvatar: <Ảnh đại diện người nghe>, 
  checkSum: <Mã xác thực cuộc gọi>) 
  {
    [weak self] result in
    guard let self = self else { return }
    DispatchQueue.main.async {
      self.callButton.isEnabled = true
    }

    switch result {
    case .success():
    // Gọi thành công
    case .failure(let error):
    // Lỗi khi thực hiện cuộc gọi
    }
  }
```

### Gọi đến
Để nhận cuộc gọi đến
Trong hàm **pushRegistry didReceiveIncomingPushWith** hãy sử dụng hàm **startIncomingCall**

```swift
func pushRegistry(_ registry: PKPushRegistry, didReceiveIncomingPushWith payload: PKPushPayload, for type: PKPushType, completion: @escaping () -> Void) 
	{
  
  	// Xử lý payload
    	...
    
    // Truyền dữ liệu sau khi xử lý vào hàm khởi tạo của của VBot SDK
		VBotPhone.sharedInstance.startIncomingCall(
			callerId: <Mã người gọi>, 
  		callerName: <Tên người gọi>, 
  		callerAvatar: <Ảnh đại diện người gọi>, 
  		calleeId: <Mã người nghe>, 
  		calleeName: <Tên người nghe>, 
  		calleeAvatar: <Ảnh đại diện người nghe>, 
  		checkSum: <Mã xác thực cuộc gọi>,
      metaData: <MetaData của cuộc gọi>
      completion: completion) 
  }
```

Sau khi gọi hai hàm **startOutgoingCall** và **startIncomingCall** thì luồng cuộc gọi sẽ do SDK xử lý

Ví dụ đầy đủ:

```swift

import VBotPhoneSDK
import PushKit

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {

		private var voipRegistry: PKPushRegistry?

    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {

        // Cấu hình cho SDK
        let config = VBotConfig(
            supportPopupCall: true, // Bật popup call trong cuộc gọi
            iconTemplateImageData: UIImage(named: "callkit-icon")?.pngData()) // Icon cho màn hình CallKit
            
        // Token
        let token = "1scn3fk..3co4vb0"
        
        // Khởi tạo SDK
        VBotPhone.sharedInstance.setup(token: token, with: config)

        // Khởi tạo PKPushRegistry
        voipRegistry = PKPushRegistry(queue: DispatchQueue.main)
        voipRegistry!.desiredPushTypes = [.voIP]
        voipRegistry!.delegate = self

        return true
    }
}

// Lắng nghe các sự kiện cùa Pushkit
extension AppDelegate: PKPushRegistryDelegate {

		// Gọi khi hệ thống cung cấp hoặc cập nhật push token dành cho VoIP.
    func pushRegistry(_ registry: PKPushRegistry, didUpdate pushCredentials: PKPushCredentials, for type: PKPushType) {
    	// Lưu push token mà server cần để gửi thông báo VoIP đến thiết bị
    }
	
  	// Gọi khi ứng dụng nhận được một thông báo VoIP push từ Apple Push Notification Service (APNs).
    func pushRegistry(_ registry: PKPushRegistry, didReceiveIncomingPushWith payload: PKPushPayload, for type: PKPushType, completion: @escaping () -> Void) {
        
    	// Xử lý payload
    	...
    
    	// Truyền dữ liệu sau khi xử lý vào hàm khởi tạo của của VBot SDK
      if type == .voIP {
        VBotPhone.sharedInstance.startIncomingCall(
          callerId: <Mã người gọi>, 
          callerName: <Tên người gọi>, 
          callerAvatar: <Ảnh đại diện người gọi>, 
          calleeId: <Mã người nghe>, 
          calleeName: <Tên người nghe>, 
          calleeAvatar: <Ảnh đại diện người nghe>, 
          checkSum: <Mã xác thực cuộc gọi>,
          metaData: <MetaData của cuộc gọi>
          completion: completion) 
      } else {
          completion()
      }
      
    }
		
    // Gọi khi hệ thống thu hồi (invalidate) push token của ứng dụng.
    func pushRegistry(_ registry: PKPushRegistry, didInvalidatePushTokenFor type: PKPushType) {}
}
```

---

### Đa ngôn ngữ

Các màn hình cuộc gọi VBot SDK mặc định chỉ hỗ trợ Tiếng Việt

Để thay đổi ngôn ngữ truyền dữ liệu qua metadata khi gọi hàm **startIncomingCall** và **startOutgoingCall**

### Thao tác trong cuộc gọi

Từ ứng dụng của bạn, có thể gọi vào các hàm khác nhau của VBot SDK để thực hiện các hành động trong cuộc gọi

Ví dụ:
```swift
// Kiểm tra xem có call hay không
VBotPhone.sharedInstance.hasActiveCall()
```

```swift
// Quay lại màn hình cuộc gọi
func returnToCallVCIfNeeded()

// Ẩn màn hình cuộc gọi
func hideCallVCIfNeeded()

// Kết thúc cuộc gọi
func endCall(completion: @escaping (Error?) -> Void)

// Tắt microphone
func muteCall(completion: @escaping (Error?) -> Void)

// Lấy trạng thái microphone
func isCallMute() -> Bool

// Bật tắt loa ngoài
func onOffSpeaker()

// Lấy trạng thái loa ngoài
func isSpeakerOn() -> Bool

Và các hàm khác

```

---

### Lắng nghe các sự kiện

Có 2 cách để lắng nghe các sự kiện trong VBot SDK

1. **Sử dụng Protocol delegate**

Đăng ký nhận sự kiện VBot:

```swift
  // Đăng ký nhận sự kiện cuộc gọi
  VBotPhone.sharedInstance.addDelegate(self)

  // Hủy đăng ký
   deinit {
       VBotPhone.sharedInstance.removeDelegate(self)
   }
```

Các delegate method bao gồm

```swift
protocol VBotPhoneDelegate {

    // Trạng thái cuộc gọi thay đổi
    func callStateChanged(state: VBotCallState)

    // Cuộc gọi đi đã bắt đầu
    func callStarted()

    // Cuộc gọi đến được chấp nhận (Khi user chọn chấp nhận cuộc gọi)
    func callAccepted()

    // Cuộc gọi kết thúc, cùng nguyên nhân
    func callEnded(reason: VBotEndCallReason)
    
    // Lấy quyền microphone
    func microphonePermission(status: AVAudioSession.RecordPermission)

    // Trạng thái Microphone thay đổi
    func callMuteStateDidChange(muted: Bool)

    // Nhấn vào nút nhắn tin
    func messageButtonTapped()

}
```

---

### Xem thêm

#### VBotEndCallReason và VBotError


```
    // Timeout
    case timeOut = -1001
    
    // Khởi tạo không thành công
    case initiationFailed = 1001
    
    case initiationFailed_1 = 1002
    
    // Chưa cấp truyền mic
    case microphonePermissionDenied = 1003
    
    case invalidPhoneNumber = 1004
    
    // Không có dữ liệu từ máy chủ
    case noDataFromServer = 1005
    
    case initiationFailed_2 = 1006
    
    case initiationFailed_3 = 1007
    
    // Dữ liệu không hợp lệ
    case dataInvalid = 1008
    
    case initiationFailed_4 = 1009
    
    // Xác thực thất bại
    case authenticatedFailed = 1010
    
    // Đang có cuộc gọi khác
    case anotherCallInProgress = 1011
    
    // Cuộc gọi kết thúc
    case normal = 1012
    
    // Từ chối cuộc gọi
    case decline = 1013
    
    // Không liên lạc được
    case temporarilyUnavailable = 1014
    
    // Máy bận
    case busy = 1015
    
    // reportNewIncomingCall lỗi
    case reportNewIncomingCallFailed = 1016
    
    // Lỗi chưa xác định
    case unknownError = 1999
```




