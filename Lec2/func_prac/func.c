#include <stdlib.h>
#include <assert.h>

// 定义函数类型
typedef enum { F_FUNC, G_FUNC } FuncType;

// 栈帧结构
typedef struct {
    FuncType func;  // 函数类型 (f 或 g)
    int n;          // 参数 n
    int pc;         // 程序计数器
    int temp;       // 临时存储中间结果
} Frame;

// 模拟栈结构
typedef struct {
    Frame* items;
    int top;
    int capacity;
} Stack;

void push(Stack* s, Frame frame) {
    if (s->top >= s->capacity) {
        s->capacity *= 2;
        s->items = realloc(s->items, s->capacity * sizeof(Frame));
    }
    s->items[s->top++] = frame;
}

Frame pop(Stack* s) {
    return s->items[--s->top];
}

int f_nonrec(int n);
int g_nonrec(int n);

int main() {
    // 测试示例
    int result = f_nonrec(5);
    return 0;
}

int f_nonrec(int n) {
    Stack stack = {malloc(10 * sizeof(Frame)), 0, 10};
    push(&stack, (Frame){F_FUNC, n, 0, 0});
    int retval = 0;

    while (stack.top > 0) {
        Frame* f = &stack.items[stack.top - 1];
        
        switch (f->func) {
            case F_FUNC:
                switch (f->pc) {
                    case 0:
                        if (f->n <= 1) {
                            retval = 1;
                            pop(&stack);
                        } else {
                            f->pc = 1;
                            push(&stack, (Frame){F_FUNC, f->n - 1, 0, 0});
                        }
                        break;
                    case 1:
                        f->temp = retval;  // 保存 f(n-1) 结果
                        f->pc = 2;
                        push(&stack, (Frame){G_FUNC, f->n - 2, 0, 0});
                        break;
                    case 2:
                        retval = f->temp + retval;  // f(n-1) + g(n-2)
                        pop(&stack);
                        break;
                }
                break;
                
            case G_FUNC:
                switch (f->pc) {
                    case 0:
                        if (f->n <= 1) {
                            retval = 1;
                            pop(&stack);
                        } else {
                            f->pc = 1;
                            push(&stack, (Frame){F_FUNC, f->n + 1, 0, 0});  // 注意: n+1
                        }
                        break;
                    case 1:
                        f->temp = retval;  // 保存 f(n+1) 结果
                        f->pc = 2;
                        push(&stack, (Frame){G_FUNC, f->n - 1, 0, 0});
                        break;
                    case 2:
                        retval = f->temp + retval;  // f(n+1) + g(n-1)
                        pop(&stack);
                        break;
                }
                break;
        }
    }
    
    free(stack.items);
    return retval;
}

// g 函数可以直接调用 f_nonrec 或实现类似结构
int g_nonrec(int n) {
    // 实现类似 f_nonrec 的结构
    // ...
}