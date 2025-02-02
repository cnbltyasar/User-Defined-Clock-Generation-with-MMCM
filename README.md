# User-Defined-Clock-Generation-with-MMCM

Bu projede, UART aracılığıyla kullanıcıdan istenen frekans değeri (MHz cinsinden) alınmakta ve bu değere ulaşmak için gerekli MMCM parametreleri (divider ve multiplier gibi) hesaplanmaktadır. Hesaplanan bu parametreler, Clock Wizard tarafından oluşturulan MMCM yapılandırma kayıtlarına yazılmaktadır.
Ek olarak, ILA kullanılarak FPGA içindeki sinyal akışları ve MMCM yapılandırmasının güncel durumu sürekli izlenmekte, böylece UART üzerinden gönderilen frekans değerine ulaşılıp ulaşılmadığı gözlemlenebilmektedir.

# Özellikler
# UART Tabanlı Kontrol:
Menü tabanlı arayüz ile kullanıcı frekans değeri ayarlayabilir.
# Dinamik Frekans Ayarı: 
Kullanıcı tarafından girilen yeni frekans değerine göre MMCM parametreleri (divider, multiplier, vs.) hesaplanır ve uygulanır.
# Clock Wizard Entegrasyonu:
Projede bulunan Clock Wizard, MMCM yapılandırmasını gerçekleştirmek için kullanılır.
# ILA ile Gerçek Zamanlı İzleme: 
ILA (Integrated Logic Analyzer) kullanılarak FPGA içindeki sinyaller ve MMCM yapılandırması sürekli olarak izlenir, böylece ayarlanan frekansa ulaşılıp ulaşılmadığı gözlemlenir.

# Kaynak Dosya Açıklamaları
# main.c:

main() fonksiyonu ve kullanıcı arayüzünü içerir.
Menü sistemini yönetir; kullanıcının frekans girişi yapması, ayarlanan frekans değerinin hesaplanması ve MMCM yapılandırma işlemleri burada gerçekleşir.
ILA kullanılarak MMCM durumunun izlenebilmesi için gerekli debug/izleme sinyalleri sağlanır.
clock_generation.c (Opsiyonel):

calculateClockParameters() fonksiyonu ile girilen frekans değeri üzerinden gerekli parametreleri (divider, multiplier, fractional kısımlar) hesaplar.
custom_round() fonksiyonu ile yuvarlama işlemleri gerçekleştirilir.
Hesaplanan parametreler, Clock Wizard tarafından üretilen MMCM yapılandırma kayıtlarına yazılır.

Ek Açıklamalar
# Clock Wizard Kullanımı:
Projede yer alan Clock Wizard, MMCM yapılandırmasını gerçekleştirmek üzere kullanılır. UART üzerinden girilen frekans değeri doğrultusunda MMCM ayarları hesaplanır ve uygulanır. Bu sayede, FPGA içerisindeki saat frekansı dinamik olarak değiştirilir.

# ILA ile İzleme:
ILA (Integrated Logic Analyzer) kullanılarak, MMCM’nin ve ilgili sinyal akışının gerçek zamanlı olarak izlenmesi sağlanır. Böylece, UART üzerinden ayarlanan frekans değerine ulaşılıp ulaşılmadığı doğrudan gözlemlenebilir. ILA, Vivado içinde konfigüre edilir ve izlemek istediğiniz sinyalleri seçerek değişimleri takip edebilirsiniz.

#  Bellek Erişimli Giriş/Çıkış: MMCM kayıtları, Xilinx sürücüleri (Xil_In32 ve Xil_Out32) kullanılarak doğrudan erişilir.
![image](https://github.com/user-attachments/assets/ab5fbf9f-e5be-4130-ae8c-5e9c6a235253)

![WhatsApp Görsel 2024-08-30 saat 12 17 34_c1b9eb58](https://github.com/user-attachments/assets/85a541f7-bcd2-4e16-94f8-8e3a096ad0ac)

![WhatsApp Görsel 2024-08-30 saat 12 16 00_0b45835d](https://github.com/user-attachments/assets/e1aad15c-f255-4151-9e0c-40b9f66e5b0d)

# UART Addresses: 

![WhatsApp Görsel 2024-08-30 saat 12 20 21_c1502ef6](https://github.com/user-attachments/assets/f82f48cd-0815-43aa-80a5-f5f1142a357a)

# UART Frequecny Control Menu:
![WhatsApp Görsel 2024-10-08 saat 23 25 35_7cc2dddc](https://github.com/user-attachments/assets/ea5cb6e0-e6ee-4e41-82cf-058d10388599)

# Waveform Result With İLA: 

![WhatsApp Görsel 2024-10-08 saat 20 14 20_455acace](https://github.com/user-attachments/assets/77d80e85-e56b-4a26-81f5-9fc1da07160a)

