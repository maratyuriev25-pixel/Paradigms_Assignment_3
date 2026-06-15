#include <iostream>
#include <string>
#include <cstring>
#include <windows.h>

typedef void* cipher_t;
typedef cipher_t* (*CreateCaesarFunc)(int key);
typedef cipher_t* (*CreateVigenereFunc)(const char* key);
typedef char* (*EncryptFunc)(cipher_t* cipher, const char* text);
typedef char* (*DecryptFunc)(cipher_t* cipher, const char* text);
typedef void (*DestroyFunc)(cipher_t* cipher);
typedef void (*FreeFunc)(char* str);

int main() {
    std::cout << "===== Encryption Cipher Library =====" << std::endl;

    HMODULE hLib = LoadLibraryA("DLL.dll");

    if (!hLib) {
        std::cerr << "Failed to load cipher.dll" << std::endl;
        return 1;
    }

    CreateCaesarFunc  create_caesar = (CreateCaesarFunc)GetProcAddress(hLib, "cipher_create_caesar");
    CreateVigenereFunc create_vigenere = (CreateVigenereFunc)GetProcAddress(hLib, "cipher_create_vigenere");
    EncryptFunc encrypt = (EncryptFunc)GetProcAddress(hLib, "cipher_encrypt");
    DecryptFunc decrypt = (DecryptFunc)GetProcAddress(hLib, "cipher_decrypt");
    DestroyFunc destroy = (DestroyFunc)GetProcAddress(hLib, "cipher_destroy");
    FreeFunc free_str = (FreeFunc)GetProcAddress(hLib, "cipher_free");

    int choice = 0;
    bool running = true;

    while (running) {
        std::cout << "\n===== Menu =====" << std::endl;
        std::cout << "1. Caesar Cipher" << std::endl;
        std::cout << "2. Vigenere Cipher" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Choose an option (1-3): ";
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 3) {
            running = false;
            std::cout << "Goodbye!" << std::endl;
            break;
        }

        if (choice == 1) {
            std::cout << "\n----- Caesar Cipher -----" << std::endl;

            int key;
            std::cout << "Enter shift key (1-25): ";
            std::cin >> key;
            std::cin.ignore();

            cipher_t* cipher = create_caesar(key);
            if (!cipher) {
                std::cerr << "Error creating Caesar cipher!" << std::endl;
                continue;
            }

            int subChoice = 0;
            while (true) {
                std::cout << "\n1. Encrypt" << std::endl;
                std::cout << "2. Decrypt" << std::endl;
                std::cout << "3. Back to main menu" << std::endl;
                std::cout << "Choose an option (1-3): ";
                std::cin >> subChoice;
                std::cin.ignore();

                if (subChoice == 3) break;

                std::string text;
                std::cout << "Enter text: ";
                std::getline(std::cin, text);

                if (subChoice == 1) {
                    char* result = encrypt(cipher, text.c_str());
                    std::cout << "Encrypted: " << result << std::endl;
                    free_str(result); 
                }
                else if (subChoice == 2) {
                    char* result = decrypt(cipher, text.c_str());
                    std::cout << "Decrypted: " << result << std::endl;
                    free_str(result);
                }
            }

            destroy(cipher);
        }
        else if (choice == 2) {
            std::cout << "\n----- Vigenere Cipher -----" << std::endl;

            std::string key;
            std::cout << "Enter keyword (letters only): ";
            std::getline(std::cin, key);

            cipher_t* cipher = create_vigenere(key.c_str());
            if (!cipher) {
                std::cerr << "Error creating Vigenere cipher!" << std::endl;
                continue;
            }

            int subChoice = 0;
            while (true) {
                std::cout << "\n1. Encrypt" << std::endl;
                std::cout << "2. Decrypt" << std::endl;
                std::cout << "3. Back to main menu" << std::endl;
                std::cout << "Choose an option (1-3): ";
                std::cin >> subChoice;
                std::cin.ignore();

                if (subChoice == 3) break;

                std::string text;
                std::cout << "Enter text: ";
                std::getline(std::cin, text);

                if (subChoice == 1) {
                    char* result = encrypt(cipher, text.c_str());
                    std::cout << "Encrypted: " << result << std::endl;
                    free_str(result);
                }
                else if (subChoice == 2) {
                    char* result = decrypt(cipher, text.c_str());
                    std::cout << "Decrypted: " << result << std::endl;
                    free_str(result);
                }
            }

            destroy(cipher);
        }
        else {
            std::cout << "Invalid choice! Please enter 1, 2, or 3." << std::endl;
        }
    }

    FreeLibrary(hLib);
    std::cout << "Library unloaded." << std::endl;

    return 0;
}
