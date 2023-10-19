#pragma once

#include "Modules/ModuleManager.h"

class FWeaponModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<class FWeaponContextMenu> ContextMenu;
	TSharedPtr<class FWeaponCommand> Command;
};
