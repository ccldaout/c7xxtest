#include <c7app.hpp>
#include <c7factory.hpp>


using c7::p_;


// タスク毎のインターフェイスクラス
struct LoadTaskInterface {
    virtual ~LoadTaskInterface() {}
    virtual void run() = 0;
};
struct UnloadTaskInterface {
    virtual ~UnloadTaskInterface() {}
    virtual void run() = 0;
};
struct MoveTaskInterface {
    virtual ~MoveTaskInterface() {}
    virtual void run() = 0;
};


// 具象タスククラス (1)
class StdLoadTask: public LoadTaskInterface {
public:
    void run() override {
	p_("LoadTask::run()");
    }
};
class StdUnloadTask: public UnloadTaskInterface {
public:
    void run() override {
	p_("UnloadTask::run()");
    }
};
class StdMoveTask: public MoveTaskInterface {
public:
    void run() override {
	p_("MoveTask::run()");
    }
};


// 具象タスククラス (2)
class DebugLoadTask: public LoadTaskInterface {
public:
    void run() override {
	p_("DebugLoadTask::run()");
    }
};
class DebugUnloadTask: public UnloadTaskInterface {
public:
    void run() override {
	count_++;
	p_("DebugUnloadTask::run()");
    }
private:
    int count_ = 0;
};
class DebugMoveTask: public MoveTaskInterface {
public:
    void run() override {
	p_("DebugMoveTask::run()");
    }
};


// ファクトリー

using FactoryInterface =
    c7::factory_type1::interface<LoadTaskInterface,
				 UnloadTaskInterface,
				 MoveTaskInterface>;

using StdFactory =
    c7::factory_type1::factory<FactoryInterface,
			       StdLoadTask,
			       StdUnloadTask,
			       StdMoveTask>;

using DebugFactory =
    c7::factory_type1::factory<FactoryInterface,
			       DebugLoadTask,
			       DebugUnloadTask,
			       DebugMoveTask>;

// テストコード

static void test(const FactoryInterface *factory)
{
    auto tp1 = factory->make<LoadTaskInterface>();
    auto tp2 = factory->make<UnloadTaskInterface>();
    auto tp3 = factory->make<MoveTaskInterface>();

    tp1->run();
    tp2->run();
    tp3->run();
}

int main()
{
    StdFactory sf;
    test(&sf);
    p_("");
    DebugFactory df;
    test(&df);
}
