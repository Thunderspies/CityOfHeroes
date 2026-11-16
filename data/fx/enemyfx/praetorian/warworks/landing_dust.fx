#############################################################
## Smash_Activate.fx
#############################################################

FxInfo
LifeSpan 80

#############################################################


Condition
	On	Time
	Time	0

	Event
		EName	DustAnchor
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 30.0 0.0
		End
	End

	Event
		EName	FootRAnchor
		Type	Local
		At	FootR
		BhvrOverride
			PositionOffset	0.0 -0.5 0.0
		End
	End

	Event
		EName	FootLAnchor
		Type	Local
		At	FootL
		BhvrOverride
			PositionOffset	0.0 -0.5 0.0
		End
	End

	Event
		Type	StartPositOnly
		At	Root
		Bhvr	behaviors/CameraShake01.bhvr
		BhvrOverride
			PositionOffset	0.0 -2.5 0.0
		End
		Part	:Dust_Circle.part
		Part	:Dust_Circle_Lingering.part
		Part	:Dust_Flash.part
		Lifespan	8
	End

	Event
		Type	StartPositOnly
		At	Root
		Part	:Dust_Shockwave.part
		Part	:Dust_Shockwave_Fill.part
		Lifespan	8
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	PositOnly
		At	FootLAnchor
		Part	:Foot_Sparks.part
		Part	:Foot_Rocks.part
		Lifespan	8
	End
End

Condition
	On	Time
	Time	7
	Event
		Type	PositOnly
		At	FootRAnchor
		Part	:Foot_Sparks.part
		Part	:Foot_Rocks.part
		Lifespan	8
	End
End

#############################################################

End
