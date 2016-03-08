unsigned fib(unsigned n) {
    if (n < 2)
        return n;

    unsigned f = fib(n - 1) + fib(n - 2);
    return f;
}
