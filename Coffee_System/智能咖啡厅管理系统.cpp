#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <ctime>
#include <iomanip>
#include <time.h>
#include "Order.h"
#include "Coffee.h"
#include <tchar.h>
#include <string.h>
#include <queue>
#include <map>

// 定义全局变量
IMAGE Americano_image;
IMAGE Latte_image;
IMAGE Little_Iron_image;
IMAGE Tea_Coffee_image;
IMAGE Fruit_Coffee_image;
IMAGE Cappuccino_image;
IMAGE BK_image;
// 计时器更新订单状态
bool timer(int ms, int id)
{
    static DWORD t[20];
    if (static_cast<DWORD>(clock() - t[id]) > static_cast<DWORD>(ms))
    {
        t[id] = clock();
        return true;
    }
    return false;
}
// 加载图片
void loadIMAGE()
{
    try
    {
        // 定义图片路径和对应的图片对象
        struct ImageInfo
        {
            const TCHAR* path;
            IMAGE* img;
            const TCHAR* name;
        } imageInfos[] = {
           {_T("D:/images/2.jpg"), &Americano_image, _T("Americano")},
           {_T("D:/images/3.jpg"), &Latte_image, _T("Latte")},
           {_T("D:/images/4.jpg"), &Little_Iron_image, _T("Little Iron")},
           {_T("D:/images/5.jpg"), &Tea_Coffee_image, _T("Tea Coffee")},
           {_T("D:/images/6.jpg"), &Fruit_Coffee_image, _T("Fruit Coffee")},
           {_T("D:/images/7.jpg"), &Cappuccino_image, _T("Cappuccino")} };

        // 逐个加载图片
        for (const auto& info : imageInfos)
        {
            // 尝试加载图片
            try
            {
                loadimage(info.img, info.path, 100, 100);

                // 验证图片是否成功加载
                if (info.img->getwidth() == 0)
                {
                    _tprintf(_T("Warning: Failed to load image for %s at path: %s\n"),
                        info.name, info.path);
                }
                else
                {
                    _tprintf(_T("Successfully loaded image for %s\n"), info.name);
                }
            }
            catch (...)
            {
                _tprintf(_T("Error loading image for %s\n"), info.name);
            }
        }
    }
    catch (const exception& e)
    {
        printf("Error in loadIMAGE: %s\n", e.what());
    }
}

#include <cwchar> // 包含宽字符函数的头文件

void show_name(const string& info, int x, int y)
{
    LOGFONT f;
    gettextstyle(&f);                        // 获取当前字体设置
    f.lfHeight = 24;                         // 设置字体高度
    _tcscpy_s(f.lfFaceName, _T("微软雅黑")); // 设置字体
    f.lfQuality = ANTIALIASED_QUALITY;       // 设置字体抗锯齿
    settextstyle(&f);                        // 设置字体样式
    setbkmode(TRANSPARENT);                  // 设置背景透明
    settextcolor(BLACK);                     // 设置文字颜色

    // 将 string 转换为 wstring
    wstring wstr;
    wstr.assign(info.begin(), info.end());

    // 转换为 TCHAR 字符串并显示
#ifdef UNICODE
    outtextxy(x, y, wstr.c_str());
#endif
}
// 添加辅助函数用于 TCHAR 到 string 的转换
string TcharToString(const TCHAR* str)
{
#ifdef UNICODE
    wstring wstr(str);
    return string(wstr.begin(), wstr.end());

#endif
}
void draw_image()
{
    // 清屏并设置背景
    setbkcolor(WHITE);
    cleardevice();

    // 定义咖啡显示信息
    struct CoffeeDisplay
    {
        int x, y;
        IMAGE* img;
        const TCHAR* name;
        const char* displayName; // 添加一个用于显示的名称
        COLORREF defaultColor;
    } displays[] = {
        {100, 100, &Americano_image, _T("Americano"), "Americano", RGB(200, 150, 100)},
        {350, 100, &Latte_image, _T("Latte"), "Latte", RGB(220, 180, 140)},
        {600, 100, &Little_Iron_image, _T("Little Iron"), "Little Iron", RGB(160, 120, 80)},
        {100, 350, &Tea_Coffee_image, _T("Tea Coffee"), "Tea Coffee", RGB(180, 140, 100)},
        {350, 350, &Fruit_Coffee_image, _T("Fruit Coffee"), "Fruit Coffee", RGB(200, 160, 120)},
        {600, 350, &Cappuccino_image, _T("Cappuccino"), "Cappuccino", RGB(190, 150, 110)} };

    // 绘制每个咖啡选项
    for (const auto& display : displays)
    {
        // 绘制边框
        setlinecolor(BLACK);
        rectangle(display.x, display.y, display.x + 100, display.y + 100);

        // 尝试显示图片，如果失败则显示颜色块
        if (display.img->getwidth() > 0)
        {
            putimage(display.x, display.y, display.img);
        }
        else
        {
           
            setfillcolor(display.defaultColor);
            fillrectangle(display.x, display.y, display.x + 100, display.y + 100);

            // 在颜色块中显示咖啡名称
            LOGFONT f;
            gettextstyle(&f);
            f.lfHeight = 16;
            _tcscpy_s(f.lfFaceName, _T("微软雅黑"));
            f.lfQuality = ANTIALIASED_QUALITY;
            settextstyle(&f);
            setbkmode(TRANSPARENT);
            settextcolor(WHITE);

            // 计算文本位置使其居中
            int textX = display.x + (100 - textwidth(display.name)) / 2;
            int textY = display.y + (100 - textheight(display.name)) / 2;
            outtextxy(textX, textY, display.name);
        }

        // 显示咖啡名称
        show_name(display.displayName, display.x, display.y - 30);
    }

    // 绘制订单区域
    setfillcolor(WHITE);
    solidrectangle(0, 500, 800, 729);

    // 绘制提交按钮
    setfillcolor(LIGHTGRAY);
    solidrectangle(650, 500, 750, 550);
    show_name("Submit", 670, 515);

    FlushBatchDraw();
}
class CafeSystem
{
public:
    // 将这些变量移到 public 区域
    int orderDisplayScroll = 0;
    int queueDisplayScroll = 0;
    static const int ITEMS_PER_PAGE = 4;

    // 将 canSubmitOrder 方法移到 public 区域
    bool canSubmitOrder()
    {
        for (const auto& item : orderCounter)
        {
            if (item.second > 0)
                return true;
        }
        return false;
    }

    CafeSystem() : staffBusy(false), currentOrder(nullptr)
    {
        // 初始化计数器
        orderCounter["Americano"] = 0;
        orderCounter["Latte"] = 0;
        orderCounter["Little_Iron"] = 0;
        orderCounter["Tea_Coffee"] = 0;
        orderCounter["Fruit_Coffee"] = 0;
        orderCounter["Cappuccino "] = 0;
    }

    void addToOrder(string coffeeType)
    {
        orderCounter[coffeeType]++;
        updateOrderDisplay();
    }

    void removeFromOrder(string coffeeType)
    {
        if (orderCounter[coffeeType] > 0)
        {
            orderCounter[coffeeType]--;
            updateOrderDisplay();
        }
    }

    void submitOrder()
    {
        Order newOrder;
        // 将当前计数器中的咖啡添加到订单
        for (const auto& item : orderCounter)
        {
            if (item.second > 0)
            {
                Coffee* coffee = createCoffee(item.first);
                newOrder.addItem(coffee, item.second);
            }
        }

        orderQueue.push(newOrder);
        clearOrderCounter();
        updateOrderDisplay();
    }

    void updateSystem()
    {
        if (!staffBusy && !orderQueue.empty())
        {
            currentOrder = &orderQueue.front();
            currentOrder->startProcessing();
            staffBusy = true;
        }

        if (staffBusy && currentOrder->isCompleted())
        {
            orderQueue.pop();
            staffBusy = false;
            currentOrder = nullptr;
        }
    }

private:
    queue<Order> orderQueue;
    bool staffBusy;
    Order* currentOrder;

    // 咖啡计数器
    map<string, int> orderCounter;

    void updateOrderDisplay()
    {
        // 清除显示区域
        setfillcolor(WHITE);
        solidrectangle(0, 500, 800, 729);

        // 绘制分隔线和区域标题
        setlinecolor(LIGHTGRAY);
        line(0, 500, 800, 500);
        line(400, 500, 400, 729);

        // 左侧：当前订单区域
        show_name("Current Order", 10, 505);
        int y_pos = 535;
        double totalPrice = 0;
        int totalTime = 0;

        // 显示当前订单信息
        for (const auto& item : orderCounter)
        {
            if (item.second > 0)
            {
                Coffee* coffee = createCoffee(item.first);
                if (coffee)
                {
                    string info = item.first + ": " + to_string(item.second) +
                        " x RMB" + to_string(coffee->getPrice());
                    show_name(info, 10, y_pos);
                    totalPrice += coffee->getPrice() * item.second;
                    totalTime += coffee->getPrepTime() * item.second;
                    y_pos += 30;
                    delete coffee;
                }
            }
        }

        // 显示总价和预计时间
        show_name("Total: RMB" + to_string(totalPrice), 10, y_pos);
        show_name("Est. Time: " + to_string(totalTime) + "min", 10, y_pos + 30);

        // 右侧：显示所有待处理订单和店员状态
        show_name("Pending Orders", 410, 505);
        y_pos = 535;

        // 显示店员状态
        string staffStatus = staffBusy ? "Staff: Busy" : "Staff: Available";
        show_name(staffStatus, 410, y_pos);
        y_pos += 30;

        // 显示当前正在处理的订单和进度
        if (currentOrder != nullptr)
        {
            int progress = currentOrder->getProgress();
            int remainingSeconds = (int)((100 - progress) * currentOrder->getTotalTime() * 60 / 100);
            string orderStatus = "Order Progress: " + to_string(progress) + "% (" +
                to_string(remainingSeconds) + " seconds remaining)";
            show_name(orderStatus, 410, y_pos);
            y_pos += 30;

            // 绘制进度条
            setfillcolor(LIGHTGRAY);
            solidrectangle(410, y_pos, 710, y_pos + 20);
            setfillcolor(GREEN);
            solidrectangle(410, y_pos, 410 + (300 * progress / 100), y_pos + 20);
            y_pos += 40;
        }

        // 显示等待中的订单
        queue<Order> tempQueue = orderQueue;
        int queueNum = 1;
        while (!tempQueue.empty())
        {
            Order order = tempQueue.front();
            string orderInfo = "Order #" + to_string(queueNum) + ": " +
                to_string(order.getTotalItems()) + " items";
            show_name(orderInfo, 410, y_pos);
            y_pos += 30;
            tempQueue.pop();
            queueNum++;
        }

        // 修改提交按钮的显示
        if (canSubmitOrder())
        {
            setfillcolor(GREEN);
        }
        else
        {
            setfillcolor(LIGHTGRAY);
        }
        solidrectangle(650, 500, 750, 550);
        show_name("Submit", 670, 515);

        FlushBatchDraw();
    }

    Coffee* createCoffee(string type)
    {
        if (type == "Americano")
            return new Americano();
        if (type == "Latte")
            return new Latte();
        if (type == "Little_Iron")
            return new Little_Iron();
        if (type == "Tea_Coffee")
            return new Tea_Coffee();
        if (type == "Fruit_Coffee")
            return new Fruit_Coffee();
        if (type == "Cappuccino")
            return new Cappuccino();
        return nullptr;
    }

    void clearOrderCounter()
    {
        for (auto& item : orderCounter)
        {
            item.second = 0;
        }
    }
};

bool isInRect(const MOUSEMSG& msg, int x1, int y1, int x2, int y2)
{
    return (msg.x >= x1 && msg.x <= x2 && msg.y >= y1 && msg.y <= y2);
}

int main()
{
    // 初始化图形窗口
    initgraph(800, 729);
    BeginBatchDraw();
    SetWindowText(GetHWnd(), _T("智能咖啡厅管理系统"));

    // 输出当前工作目录
    char buffer[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, buffer);
    printf("Current working directory: %s\n", buffer);

    CafeSystem cafe;

    // 加载并显示图片
    loadIMAGE();
    draw_image();

    // 定义咖啡按钮区域
    struct CoffeeButton
    {
        int x, y;
        string name;
    } buttons[] = {
        {100, 100, "Americano"},
        {350, 100, "Latte"},
        {600, 100, "Little_Iron"},
        {100, 350, "Tea_Coffee"},
        {350, 350, "Fruit_Coffee"},
        {600, 350, "Cappuccino"} };

    bool running = true;
    while (running)
    {
        // 更新系统状态
        if (timer(1000, 1))
        {
            cafe.updateSystem();
            FlushBatchDraw();
        }

        // 处理鼠标消息
        if (MouseHit())
        {
            MOUSEMSG msg = GetMouseMsg();

            // 检查每个咖啡按钮
            for (const auto& btn : buttons)
            {
                if (isInRect(msg, btn.x, btn.y, btn.x + 100, btn.y + 100))
                {
                    if (msg.uMsg == WM_LBUTTONDOWN)
                    {
                        cafe.addToOrder(btn.name);
                        FlushBatchDraw();
                    }
                    else if (msg.uMsg == WM_RBUTTONDOWN)
                    {
                        cafe.removeFromOrder(btn.name);
                        FlushBatchDraw();
                    }
                }
            }

            // 检查提交按钮
            if (isInRect(msg, 650, 500, 750, 550) && msg.uMsg == WM_LBUTTONDOWN)
            {
                if (cafe.canSubmitOrder())
                {
                    cafe.submitOrder();
                    FlushBatchDraw();
                }
            }
        }

        // 检查键盘输入
        if (_kbhit())
        {
            char key = _getch();
            if (key == 27) // ESC键
                running = false;
        }

        Sleep(10);
    }

    EndBatchDraw();
    closegraph();
    return 0;
}