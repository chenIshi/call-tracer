void a() {}

void b() { a(); a(); }

void c() { b(); a(); }

int main() { a(); c(); return 0; }