#############################################################
## DimensionShift_Emote.fx
#############################################################

FxInfo
LifeSpan 85

#############################################################

Condition
	On 	Time
	Time	0

	Event
		EName	MotionBlur_Male
		Type	Local
		At	Root
		ChildFX	:DimensionShift_Dummies_Female.fx
	End

	Event
		Type	Local
		At	Root
		Bhvr	behaviors\fadeOut.bhvr
		BhvrOverride
			FadeOutLength		100
		End

		Flags	AdoptParentEntity
		Lifespan 5
	End
End

Condition
	On 	Time
	Time	7

	Event
		Type Local
		At Origin
		Sound Mutant_DimensionShift_01 200 200 .8
	End	
End

Condition
	On	Time
	Time	43

	Event
		Type	StartPositOnly
		At	Hips
		Part	:Burst_Flash.part
		Part	:Burst_Star.part

	End
End

Condition
	On	Time
	Time	45

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

	Event
		Type	StartPositOnly
		At	Hips
		BhvrOverride
			PositionOffset	0.0 -1.5 0.0
		End
		Part	:Shockwave_Flat.part
		Lifespan	5
	End
End

Condition
	On	Time
	Time	46

	Event
		Type	StartPositOnly
		At	Hips
		Part	:Burst_Speckles.part
		Part	:Burst_Sparks.part
	End
End
#############################################################

End