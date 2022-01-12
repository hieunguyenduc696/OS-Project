# OS-Project
Xây dựng hệ thống chương trình cho phép liên lạc giữa các tiến trình trong 1 máy và /hoặc trên các máy 
(với các hệ điều hành có thể khác nhau); sao cho chúng có khả năng phòng Deadlock hoặc tránh cùng 
rơi vào Critical section.

Hệ thống giám sát /can thiệp thời gian sử dụng máy tính của trẻ em bao gồm 2 chương trình chính:
1/ Chương trình C (for Children): chạy trên máy laptop /desktop của trẻ và được đặt ở chế độ Autorun (tự động 
chạy khi bật máy), thực hiện các việc:
*C0: Lấy mật khẩu (từ bàn phím)
*C1: Nếu chuỗi nhập là mật khẩu của phụ huynh: chương trình đợi đến 60 phút sau thì mới quay lại hỏi lại mật 
khẩu (thực hiện lại bước C0) // lúc này là phụ huynh sử dụng máy chứ không phải trẻ!
*C2: Nếu không phải là mật khẩu của phụ huynh:
 +C2.1: Kiểm tra xem thời điểm hiện tại có nằm trong khung thời gian trẻ chưa được dùng máy hay không:
 -C2.1.1: Nếu đang trong khoảng thời gian trẻ chưa được dùng máy: Thông báo tới khi nào mới được dùng 
(hiển thị ra màn hình và /hoặc nói ra loa), sau đó thực hiện song song 2 việc: (1): kiểm tra xem đã đủ 15 giây
chưa kể từ lúc thông báo xong & nếu đã đủ thì chương trình tự tắt máy (shutdown hệ điều hành -không cho 
người dùng can thiệp) – (2) thực hiện lại từ đầu việc C0 & C1 (tức nếu người dùng kịp nhập đúng mật khẩu phụ
huynh thì không tắt máy mà thực hiện C1 - đợi đến 60 phút sau…)
 -C2.1.2: Ngược lại (đang trong khoảng thời gian trẻ được dùng máy): 
.C2.1.2.1: Nếu mật khẩu không phải mật khẩu của trẻ: thực hiện lại việc hỏi và kiểm tra mật khẩu (thực 
hiện lại C0) cho đến lần nhập sai mật khẩu thứ 3 thì đặt thời gian không được dùng máy là 10 phút kể từ thời 
điểm hiện tại rồi tắt máy.
.C2.1.2.2: Ngược lại (đúng mật khẩu của trẻ): Đọc thông tin về khung giờ được dùng (a) và Thông báo 
còn bao nhiêu phút nữa máy sẽ tắt & đến mấy giờ thì trẻ có thể bật lên lại (b), sau đó chạy ở chế độ giám sát thực 
hiện cùng lúc các việc: (1) Sau mỗi phút lại lưu lại màn hình và /hoặc các phím đã gõ, (2) thực hiện (a) và thấy 
Đồ án CK - Lớp 19CTT
 - GV Thái Hùng Văn
có thay đổi (do cha /mẹ chạy tiến trình P và điều chỉnh) thì cập nhật thông tin và thực hiện (b), (3) kiểm tra thấy 
còn 1 phút đến thời điểm tắt máy thì thực hiện (b) và còn 0 phút thì tắt máy. 
Thông tin về <khung giờ được dùng> được lưu trong 01 Text File có đặt synchronize (đồng bộ hóa)
qua Cloud để tiến trình P (for Parent) từ các máy khác (có thể chạy trên Windows hoặc Android, MacOS, IOS, 
Linux,…) có thể tham khảo và điều chỉnh. File có quy ước định dạng mỗi dòng như sau:
F<h1:m1> T<h2:m2> [ D<mD> I<mI> ] [ S<mS> ]
Trong đó F=from , T=to, D=duration, I=interrupt_time, S=sum cho biết khung giờ được dùng là từ <h1:m1> 
đến <h2:m2>; và trong khung giờ này chỉ được dùng mS phút chia làm các quãng mD phút rồi nghỉ mI phút.
Ví dụ cụ thể, với nội dung file gồm 3 dòng như sau:
F06:00 T06:45
F07:30 T11:30 D60 I20 S150 
F19:00 T21:30 S90 
Thì các khung giờ được dùng là:
1) Từ 06:00 đến 06:45
2) Trong khoảng thời gian từ 07:30 đến 11:30 có thể sử dụng máy, nhưng mỗi lần bật máy thì chỉ được dùng tối đa 
60 phút – sau đó máy sẽ không hoạt động cho đến khi đã ngắt đủ 20 phút, đồng thời khi đã dùng đủ 150 phút thì 
máy cũng sẽ không chịu chạy nữa. 
3) Từ 19:00 đến 21:30 có thể bật /tắt máy bất cứ lúc nào nhưng thời gian được dùng tổng cộng bị giới hạn là 90 phút 
(máy sẽ tắt lúc 21:30 hoặc khi đã dùng đủ 90 phút (và sau đó không thể bật lên dùng tiếp dù chưa đến 21:30)) 
2/ Chương trình P (for Parent): nếu có thể thì viết để chạy trên điện thoại (Android và /hoặc IOS), không thì 
viết để chạy trên laptop /desktop PC (Windows /MacOS /…) (chưa làm được thì chỉ cần viết trên Windows), thực 
hiện việc giám sát – cho phép xem và điều chỉnh các khung giờ được dùng trong text file nêu trên, đồng thời cũng 
xem được lịch sử sử dụng máy của trẻ và các màn hình /phím mà chương trình C đã lưu ở mức Online (làm cơ 
bản thì chỉ xét trong ngày, cao cấp hơn thì có thể truy cập vào các ngày khác – cả trong quá khứ lẫn tương lai).
Lưu ý: Chương trình P có thể được chạy cùng lúc từ cả 2 phụ huynh của trẻ (trên 2 máy khác nhau) nên có thể
xảy ra đụng độ tài nguyên, ví dụ như khi cả 2 mà cùng lúc sửa text file trên thì có thể dẫn đến data corruption và 
chương trình cần được thiết kế để các process không cùng lúc đi vào critical section.
Để giải quyết bài toán miền găng (critical section - đoạn chương trình có khả năng xảy ra mâu thuẫn truy xuất 
trên tài nguyên chung) có thể dùng cờ hiệu, kiểm tra luân phiên, semaphore, monitors,.. tùy ý. Cơ chế liên lạc 
giữa cá tiến trình cũng có thể thiết lập tùy ý.

