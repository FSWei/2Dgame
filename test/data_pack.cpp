#include <stdio.h>

int pack(int a, int b, int c) {
    // 使用位运算将三个整数打包成一个整数
    return (a << 20) | (b << 10) | c;
}

void unpack(int packed, int *a, int *b, int *c) {
    // 使用位运算将一个整数解包成三个整数
    *a = (packed >> 20) & 1023; // 获取前 10 位
    *b = (packed >> 10) & 1023; // 获取中间 10 位
    *c = packed & 1023; // 获取后 10 位
}

int main() {
    int x = 123, y = 456, z = 789;
    int packed = pack(x, y, z);
    int a, b, c;
    unpack(packed, &a, &b, &c);
    
    printf("Original values: x = %d, y = %d, z = %d\n", x, y, z);
    printf("Packed value: %d\n", packed);
    printf("Unpacked values: a = %d, b = %d, c = %d\n", a, b, c);
    
    return 0;
}
