// #include <stdio.h>
// int main()
// {
//     struct S
//     {
//         char a[7];
//         const char *const p;
//         union U
//         {
//             int i;
//             double d;
//             char c;
//         } u;
//     } s = {"小端", "大端", 1};
//     printf("%s\n", s.u.c == 1 ? s.a : s.p);
//     printf("%f\n", s.u.d);
//     printf("%u\n", (size_t) & (((S *)0)->u));
//     printf("%u\n", sizeof s);
//     return 0;
// }

#include <stdio.h>
int main()
{
    // s是一个S类型的结构体，s类型包括三个成员变量：a,p和u，其内存分布将在后面阐述
    struct S
    {
        // a是一个存储7个char类型元素的数组，在内存中将每个元素由低地址到高地址连续存储
        char a[7];
        // p是一个指向char类型的指针变量，两个const分别表明其指向的内容不可更改，其指向不可更改
        // 指针变量存储的是操作系统所提供的逻辑地址，其逻辑地址编码采用的地址位数决定指针变量的大小
        const char *const p;
        // u是一个U类型的联合体，u类型包括三个成员变量：i、d和c，其内存分布将在后面阐述
        union U
        {
            // i是一个int类型的整型变量，在内存中以补码的形式存储
            int i;
            // d是一个double类型的浮点型变量，在内存中以国际标准IEEE 754所定义的格式存储
            double d;
            // c是一个字符型变量，在内存中先按照某种编码规则（ASCII，UTF-8...）转化成整型，再以补码的形式存储
            char c;
        } u;
    } s = {"小端", "大端", 1}; // 将字符串"小端"，字符串"大端"的地址和整形常量1通过值传递初始化给a,p和s.u.i

    // 为了解决多字节数据各字节如何在内存中分配的问题，产生了小端和大端的分配方式
    // 小端模式即低地址放低位权数据，大端则相反，而我们知道数据的地址都是起始地址即开头的低地址
    // 又由于联合体各成员地址相同，查看s.u.c即可知当前硬件的字节内存分配方式
    // 值得一提的是条件表达式运算的结果在c和c++中略有不同，一个是值，一个是引用
    printf("%s\n", s.u.c == 1 ? s.a : s.p); // 打印结果不确定，由当前设备的CPU决定

    // 因为联合体成员共用同一块内存，而当前内存存储的是整形变量s.u.i
    // 所以将打印以浮点型读取的整形形式存储的二进制序列，结果自然有所差异
    printf("%f\n", s.u.d); // 打印结果为：0.000000\n

    // ((S *)0) 取零处地址并将其转换为指向S的指针
    // & (((S *)0)->u) 找到结构成员u并计算u的地址，显然得到的就是u在整个结构中的位置
    // 所以(size_t) & (((S *)0)->u)) 能计算s.u相较于0位置也就是s的起始地址的偏移量
    // 数组s.a的每个元素占1字节，所以s.a将占7个字节，当我们知道我们要访问的s.p占4或8个字节时
    // 从首地址开始读取,为了能一次便读取p中的全部数据，将空出1字节，让s.p能处在4或8的整数倍偏移处
    // 同样的原因为了保证s.u中成员变量s.u.d数据的读取，u将处在8的整数倍偏移处
    // 当操作系统为32位时，s.u的偏移量为7+1+4+4字节，64位系统则是7+1+8字节
    printf("%u\n", (size_t) & (((S *)0)->u)); // 打印结果为：16\n

    // 同时为了防止越权访问，结构体和联合体的总大小为最大读取量的整数倍
    // 联合体s.u的大小为8字节，结构体s的大小为16+8字节
    printf("%u\n", sizeof s); // 打印结果为：24\n
    return 0;
}

// 总结：
// 类型决定分配的内存空间大小及对数据的操作权限
// 字符型-ASCII码，UTF-8编码等等 整型-补码 浮点型-IEEE浮点数编码 指针类型-逻辑地址编码
// 数组-低地址到高地址连续存储 结构体-依次内存对齐存储 联合体-成员共用总内存大小和地址