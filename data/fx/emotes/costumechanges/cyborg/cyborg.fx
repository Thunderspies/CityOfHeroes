#########################################################
##
##

FxInfo

Condition
	On	Time
	Time	0

	Event
		EName 	SpinOffset
		Type	StartPositOnly
		At	Root
		BhvrOverride
			Spin 0.0 .05 0.0
		End
	#	Lifespan 10
	End

	Event
		EName 	LightBeams
		Type	Local
		At	SpinOffset
		BhvrOverride
			PositionOffset 0.0 -5.0 0.0
		End
		part	:LightBeam.part
		part	:LightBeamCenter.part
		Lifespan 90
	End
End

Condition
	On 	Time
	Time 	15
	Event
		EName 	GroundRing
		Type	StartPositOnly
		At	Root
		part	:GroundRing.part
		part	:GroundRingStationary.part
		Lifespan 40
	End

End




Condition
	On	Time
	Time	35
	Event
		EName 	Flash3
		Type	Local
		At	Chest
	#	part	:LargeBlast2.part
	#	part	:LargeBlast3.part
	Lifespan 10
	End

End



Condition
	On 	Time
	Time 	35

	Event
		EName 	Flash3
		Type	Local
		At	Root
		ChildFX :BodyGlow.fx
	Lifespan 20
	End

End