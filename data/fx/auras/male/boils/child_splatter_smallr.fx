#############################################################
## Sparks_Body.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Root
		Until	AIR

		ChildFX	:GRANDCHILD_Splatter_SmallR.fx
		Lifespan 1
	End
End


#############################################################

End