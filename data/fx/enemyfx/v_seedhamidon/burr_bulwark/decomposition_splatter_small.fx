#############################################################
## TestTube_Continuing.fx
#############################################################

FxInfo
Lifespan 80

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	ChestSplatter
		Type	Local
		At	Chest
		ChildFX	:CHILD_splatter_Small.fx
		POther  Hips
	End
End

#############################################################

End