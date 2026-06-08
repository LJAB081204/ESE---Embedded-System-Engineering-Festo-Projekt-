#ifndef ESTOPBASE_H
#define ESTOPBASE_H

class EStopContext;

class EStopBase {
public:
    virtual ~EStopBase() = default;
    virtual void entry(EStopContext& ctx) {}
    virtual void exit(EStopContext& ctx)  {}
    virtual void eventButtonEstopPressed(EStopContext& ctx)       {}
    virtual void eventButtonEstopReleased(EStopContext& ctx)      {}
    virtual void eventButtonEstopPressedRemote(EStopContext& ctx) {}
    virtual void eventButtonEstopReleasedRemote(EStopContext& ctx){}
    virtual void eventButtonResetPressed(EStopContext& ctx)       {}
    virtual void eventButtonResetPressedRemote(EStopContext& ctx) {}
};

#endif
