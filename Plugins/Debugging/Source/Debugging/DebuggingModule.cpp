#include "DebuggingModule.h"
#include "DebuggerCategory.h"

#include "GameplayDebugger.h"

// 국가별로 언어를 지원해주는 템플릿이라고 생각하면 된다.
#define LOCTEXT_NAMESPACE "FDebuggingModule"

// 모듈의 시작점
IMPLEMENT_MODULE(FDebuggingModule, Debugging)

void FDebuggingModule::StartupModule()
{
	IGameplayDebugger::FOnGetCategory category;
	category.BindStatic(&FDebuggerCategory::MakeInstance);

	IGameplayDebugger::Get().RegisterCategory("Debugger", category, EGameplayDebuggerCategoryState::EnabledInGameAndSimulate, 5);
	IGameplayDebugger::Get().NotifyCategoriesChanged();
}

void FDebuggingModule::ShutdownModule()
{
	if (IGameplayDebugger::IsAvailable())
		IGameplayDebugger::Get().UnregisterCategory("Debugger");
}

#undef LOCTEXT_NAMESPACE
	
