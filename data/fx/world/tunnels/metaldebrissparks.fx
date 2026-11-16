#############################################################
## ScrapeSparks.fx
#############################################################

FxInfo

Lifespan 5

#############################################################

Condition
	Time 	0

	Event
		EName	VaultHookup
		Type	local
		At	origin
		Part	V_COV\PhysicsEnabled\ScrapeSparks.part
		ChildFX	:DebrisMetal_SFX.fx
	End

End

#############################################################

End