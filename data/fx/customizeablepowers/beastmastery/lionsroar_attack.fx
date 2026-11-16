###########################################################

FxInfo
Lifespan 70

###########################################################


Condition
	On 	Time
	Time	23
	Event
		Type	Local
		At	Root
		Sound 	Cat_RoarLong_01 100 100 .8
	End
End

Condition
	On Time
	Time 23

	Event
		At Root
		Type Local
		BhvrOverride
			PYRRotate	-70 0 0
			PositionOffset	0.0 2.75 3.5
		End
		Part	:Howl_Wind_Streak.part
		Part 	:Howl_Wind_Vertical.part
		LifeSpan 40
	End

	Event
		At Root
		Type Local
		BhvrOverride
			PYRRotate	-70 0 0
			PositionOffset	0.0 2.75 3.5
		End
		Part	:Howl_Rings_Vertical.part
		LifeSpan 20
	End
End

Condition
	On	Time
	Time	25

	Event
		At Root
		Type Local
		BhvrOverride
			PositionOffset	0.0 0.25 0.0
		End
		Part	:Roar_Ring.part
		Part 	:Roar_Wind_Shockwave2.part
		Part 	:Roar_Wind_Shockwave_Core2.part
		LifeSpan 45
	End
End


###########################################################

End