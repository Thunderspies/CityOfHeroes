#########################################################
##	pump_smoke
##
FxInfo

Input  
	InpName	Origin
End

LifeSpan 800

Condition
	On	Time
	Time	0

	Event
		EName	ChildTele
		Type	Local
		At	Root

		ChildFX	V_COV\EnemyPowers\Guards\F_Teleportation.fx
#		ChildFX POWERS/Teleportation/TeleportSelf.fx
	End
End

Condition
	On Time
	Time 30
	Event
		EName 	Prime
		Type	Local
		At	Origin 
		
		Bhvr	:B_FadeIn_Entity.bhvr
		Flags	AdoptParentEntity
	End
End



End			