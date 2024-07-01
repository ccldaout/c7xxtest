#include <c7app.hpp>
#include <c7factory.hpp>


using c7::p_;


struct LoadTaskInterface {
    struct config_type {
	std::string name;
    };
    virtual ~LoadTaskInterface() {}
    virtual void run() = 0;
};

struct UnloadTaskInterface {
    struct config_type {
	std::string name;
    };
    virtual ~UnloadTaskInterface() {}
    virtual void run() = 0;
};

struct MoveTaskInterface {
    struct config_type {
	std::string name;
    };
    virtual ~MoveTaskInterface() {}
    virtual void run() = 0;
};


// 具象タスククラス (1)
class StdLoadTask: public LoadTaskInterface {
public:
    StdLoadTask(const config_type& c) {}
    void run() override {
	p_("LoadTask::run()");
    }
};
class StdUnloadTask: public UnloadTaskInterface {
public:
    StdUnloadTask(const config_type& c) {}
    void run() override {
	p_("UnloadTask::run()");
    }
};
class StdMoveTask: public MoveTaskInterface {
public:
    StdMoveTask(const config_type& c) {}
    void run() override {
	p_("MoveTask::run()");
    }
};


// 具象タスククラス (2)
class DebugLoadTask: public LoadTaskInterface {
public:
    DebugLoadTask(const config_type& c) {}
    void run() override {
	p_("DebugLoadTask::run()");
    }
};
class DebugUnloadTask: public UnloadTaskInterface {
public:
    DebugUnloadTask(const config_type& c) {}
    void run() override {
	count_++;
	p_("DebugUnloadTask::run()");
    }
private:
    int count_ = 0;
};
class DebugMoveTask: public MoveTaskInterface {
public:
    DebugMoveTask(const config_type& c) {}
    void run() override {
	p_("DebugMoveTask::run()");
    }
};


// ファクトリー (factory_type1 を利用)

using FactoryInterface =
    c7::factory_type3::interface<LoadTaskInterface,
				 UnloadTaskInterface,
				 MoveTaskInterface>;

using StdFactory =
    c7::factory_type3::factory<FactoryInterface,
			       StdLoadTask,
			       StdUnloadTask,
			       StdMoveTask>;

using DebugFactory =
    c7::factory_type3::factory<FactoryInterface,
			       DebugLoadTask,
			       DebugUnloadTask,
			       DebugMoveTask>;

// テストコード

static void test(const FactoryInterface *factory)
{
    std::shared_ptr<LoadTaskInterface>   tp1;
    std::shared_ptr<UnloadTaskInterface> tp2;
    std::shared_ptr<MoveTaskInterface>   tp3;
    {
	LoadTaskInterface::config_type c{"load"};
	tp1 = factory->make(c);
    }
    {
	UnloadTaskInterface::config_type c{"unload"};
	tp2 = factory->make(c);
    }
    {
	MoveTaskInterface::config_type c{"move"};
	tp3 = factory->make(c);
    }

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
