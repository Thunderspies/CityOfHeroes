#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0
	Event
		Type	Local
		At	Origin
		ChildFX	:Child_BodyGlow.fx
		Lifespan 40
	End

	Event
		Type	Local
		At	Chest
		Part	:Streaks_Attached.part
		Part	:Streaks_Detached.part

	End
End

Condition
	On	Cycle
	Time	60
	Event
		Type	Local
		At	Origin
		ChildFX	:Child_BodyGlow.fx
		Lifespan 40
	End
End



#############################################################

End