//
// Created by stubit on 4/13/20.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#ifndef SPORTBUZZER_HC12_OPTIONS_HPP
#define SPORTBUZZER_HC12_OPTIONS_HPP


enum BaudRate {     // available baud rates. Higher baud rate reduces the range in which the module works
    B1200 = 1200,
    B2400 = 2400,
    B4800 = 4800,
    B9600 = 9600,
    B19200 = 19200,
    B38400 = 38400,
    B57600 = 57600,
    B115200 = 115200
};

enum Channel {  // available channel, each 400KHz higher
    C001 = 1,
    C002 = 2,
    C003 = 3,
    C004 = 4,
    C005 = 5,
    C006 = 6,
    C007 = 7,
    C008 = 8,
    C009 = 9,
    C010 = 10,
    C011 = 11,
    C012 = 12,
    C013 = 13,
    C014 = 14,
    C015 = 15,
    C016 = 16,
    C017 = 17,
    C018 = 18,
    C019 = 19,
    C020 = 20,
    C021 = 21,
    C022 = 22,
    C023 = 23,
    C024 = 24,
    C025 = 25,
    C026 = 26,
    C027 = 27,
    C028 = 28,
    C029 = 29,
    C030 = 30,
    C031 = 31,
    C032 = 32,
    C033 = 33,
    C034 = 34,
    C035 = 35,
    C036 = 36,
    C037 = 37,
    C038 = 38,
    C039 = 39,
    C040 = 40,
    C041 = 41,
    C042 = 42,
    C043 = 43,
    C044 = 44,
    C045 = 45,
    C046 = 46,
    C047 = 47,
    C048 = 48,
    C049 = 49,
    C050 = 50,
    C051 = 51,
    C052 = 52,
    C053 = 53,
    C054 = 54,
    C055 = 55,
    C056 = 56,
    C057 = 57,
    C058 = 58,
    C059 = 59,
    C060 = 60,
    C061 = 61,
    C062 = 62,
    C063 = 63,
    C064 = 64,
    C065 = 65,
    C066 = 66,
    C067 = 67,
    C068 = 68,
    C069 = 69,
    C070 = 70,
    C071 = 71,
    C072 = 72,
    C073 = 73,
    C074 = 74,
    C075 = 75,
    C076 = 76,
    C077 = 77,
    C078 = 78,
    C079 = 79,
    C080 = 80,
    C081 = 81,
    C082 = 82,
    C083 = 83,
    C084 = 84,
    C085 = 85,
    C086 = 86,
    C087 = 87,
    C088 = 88,
    C089 = 89,
    C090 = 90,
    C091 = 91,
    C092 = 92,
    C093 = 93,
    C094 = 94,
    C095 = 95,
    C096 = 96,
    C097 = 97,
    C098 = 98,
    C099 = 99,
    C100 = 100,
};

enum TransmissionPowerMode {
    P1 = 1,     // -1 dBm
    P2 = 2,     //  2 dBm
    P3 = 3,     //  5 dBm
    P4 = 4,     //  8 dBm
    P5 = 5,     // 11 dBm
    P6 = 6,     // 14 dBm
    P7 = 7,     // 17 dBm
    P8 = 8,     // 20 dBm
};

#endif //SPORTBUZZER_HC12_OPTIONS_HPP