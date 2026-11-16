#############################################################
## CostumeChange_Tech.fx
#############################################################

FxInfo
LifeSpan 100

## SIGIL #######################################################


Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		ChildFX	IncarnatePowers\Incarnate_Gather.fx
		Lifespan	30
	End
End

Condition
	On	Time
	Time	35
	Event
		Type	Local
		At	T_Root
		ChildFX	:Lore_Summon.fx
	End
End

#############################################################




End