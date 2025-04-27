# **KeySave — менеджер паролей**  

🔒 **Безопасное хранение паролей**
📱 **Кроссплатформенность**: Linux (C++), Android (Kotlin).  
---

## **🚀 Установка и настройка**  

### **1. Установка зависимостей (Linux)**  

#### **OpenSSL (для шифрования)**  
```bash
sudo apt update
sudo apt install openssl libssl-dev
```  
Проверьте версию:  
```bash
openssl version
```  

#### **wxWidgets (для GUI на C++)**  
```bash
sudo apt install libwxgtk3.2-dev
```  
Проверьте установку:  
```bash
wx-config --version
```  
---

### **2. Сборка и запуск (десктопная версия)**  
1. Клонируйте репозиторий:  
```bash
git clone https://github.com/BaikovaAlina/KeySave.git
cd KeySave/desktop
```  
2. Соберите проект:  
```bash
g++ main.cpp -o KeySave `wx-config --cxxflags --libs` -lssl -lcrypto
```  
3. Запустите:  
```bash
./KeySave
```  

---

## **📱 Инструкция для Android-приложения**  

*Находится в доработке*

---

## **🔐 Безопасность**  
- Пароли **шифруются на устройстве** перед отправкой в облако.  
- Файл хранится в формате:  
  ```xml
  <block title="example@mail.com" login="user" password="зашифрованный_пароль">
    <site title="google.com" login="user123" password="зашифрованный_пароль"/>
  </block>
  ```  
