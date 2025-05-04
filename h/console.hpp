#ifndef CONSOLE_HPP
#define CONSOLE_HPP

class Console {
public:
    static Console* getInstance();
    char getc();
    void putc(char c);

private:
    static Console* instance;

    Console(){}
    ~Console() = default;

};

#endif //CONSOLE_HPP
