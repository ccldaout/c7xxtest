#include <c7app.hpp>
#include <c7factory.hpp>


using c7::p_;


// Task の抽象基底クラス
struct TaskInterface {
    virtual ~TaskInterface() {}
    virtual void run() = 0;
};


// オーバーロード解決に必要な具象コンフィグクラス
struct LoadConfig {
    std::string name;
};
struct UnloadConfig {
    std::string name;
};
struct MoveConfig {
    std::string name;
};


// 具象タスククラス (1)
class LoadTask: public TaskInterface {
public:
    using config_type = LoadConfig;
    explicit LoadTask(const config_type& c) {
	p_("LoadTask: %{}", c.name);
    }
    void run() override {
	p_("LoadTask::run()");
    }
};
class UnloadTask: public TaskInterface {
public:
    using config_type = UnloadConfig;
    explicit UnloadTask(const config_type& c) {
	p_("UnloadTask: %{}", c.name);
    }
    void run() override {
	p_("UnloadTask::run()");
    }
};
class MoveTask: public TaskInterface {
public:
    using config_type = MoveConfig;
    explicit MoveTask(const config_type& c) {
	p_("MoveTask: %{}", c.name);
    }
    void run() override {
	p_("MoveTask::run()");
    }
};


// 具象タスククラス (2)
class DebugLoadTask: public TaskInterface {
public:
    using config_type = LoadConfig;
    explicit DebugLoadTask(const config_type& c) {
	p_("DebugLoadTask: %{}", c.name);
    }
    void run() override {
	p_("DebugLoadTask::run()");
    }
};
class DebugUnloadTask: public TaskInterface {
public:
    using config_type = UnloadConfig;
    explicit DebugUnloadTask(const config_type& c) {
	p_("DebugUnloadTask: %{}", c.name);
    }
    void run() override {
	p_("DebugUnloadTask::run()");
    }
};
class DebugMoveTask: public TaskInterface {
public:
    using config_type = MoveConfig;
    explicit DebugMoveTask(const config_type& c) {
	p_("DebugMoveTask: %{}", c.name);
    }
    void run() override {
	p_("DebugMoveTask::run()");
    }
};


// ファクトリー (generic_factory を利用)

using FactoryInterface =
    c7::generic_factory::interface<TaskInterface,
				   LoadConfig,
				   UnloadConfig,
				   MoveConfig>;

static
c7::generic_factory::factory<TaskInterface,
			     LoadTask,
			     UnloadTask,
			     MoveTask> std_factory;

static
c7::generic_factory::factory<TaskInterface,
			     DebugLoadTask,
			     DebugUnloadTask,
			     DebugMoveTask> debug_factory;


// テストコード

static void test(FactoryInterface *factory)
{
    std::shared_ptr<TaskInterface> tp1, tp2, tp3;

    {
	LoadConfig c{"load"};
	tp1 = factory->make(c);
    }
    {
	UnloadConfig c{"unload"};
	tp2 = factory->make(c);
    }
    {
	MoveConfig c{"move"};
	tp3 = factory->make(c);
    }

    tp1->run();
    tp2->run();
    tp3->run();
}

int main()
{
    p_("factory");
    test(&std_factory);

    p_("\ndebug_factory");
    test(&debug_factory);
}
