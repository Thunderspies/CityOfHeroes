#############################################################
## EnvFX_SubwayDebris.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0
	Chance	0.3

	Event
		EName	CoreAnchor
		Type	Start
		At	Root
		BhvrOverride
			StartJitter	3 2 3
		End
		ChildFX	:EnvFX_SubwayDebris_Heavy_NoSFX_Child.fx
		Lifespan 400
	End
End

Condition
	On	Cycle
	Time	1350 #120 #1350
	Chance	0.6

	Event
		EName	CoreAnchor
		Type	Start
		At	Root
		BhvrOverride
			StartJitter	3 2 3
		End
		ChildFX	:EnvFX_SubwayDebris_Heavy_NoSFX_Child.fx
		Lifespan 400
	End
End

#########################################################

End