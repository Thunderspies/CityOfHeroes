###########################################################

FxInfo

###########################################################
Condition
	On	Time
	Time	0
	Event
		Type	Local
		At	Origin
		ChildFX	IncarnatePowers\Incarnate_Gather.fx
		Lifespan	30
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	StartPositOnly
		At	Hips
	#	Part	:Burst_Flash.part
		Part	:Burst_Star.part

	End
End

Condition
	On	Time
	Time	24

	Event
		Type	StartPositOnly
		At	Hips
		BhvrOverride
			PyrRotateJitter	0 0 90
		End
		Part	:Rings_Orbiting.part
		Part	:Rings_Orbiting_Large.part
		Part	:Rings_Orbiting_Large_Flat.part
		Part	:Rings_Large_Lingering.part
		Part	:Rings_Large_Spinning.part
		Lifespan	6
	End

End

Condition
	On	Time
	Time	28

	Event
		Type	StartPositOnly
		At	Hips
		Part	:Burst_Sparks.part
	End
End


###########################################################

End
