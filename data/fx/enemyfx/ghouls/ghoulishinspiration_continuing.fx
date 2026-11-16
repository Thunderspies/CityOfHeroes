#############################################################
## GhoulishInspiration_Continuing.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Prime
		Type	Local
		At	Eyes
		Geom	CircleOfThorns

	End

	Event
		Type	Local
		At	Prime
		AltPiv 1
		BHVR	V_COV\Powers\Necromacy\Litcheye.bhvr
		Part	:Eye_Glow.part
		Part	:Eye_Glow_Bright.part

	End

	Event
		Type	Local
		At	Prime
		AltPiv 2
		Part	:Eye_Glow.part
		Part	:Eye_Glow_Bright.part

	End
End

Condition
	On 	Cycle
	Time 	40

	Event
		EName	left
		Type	Local
		At	Origin
		ChildFX	:GhoulishInspiration_Continuing_Child.fx
		Lifespan	15
	End
End

#############################################################

End