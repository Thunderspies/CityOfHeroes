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
		Type	Local
		At	Hips
		Bhvr	behaviors/WarpOut_Fade.bhvr
		Flags 	AdoptParentEntity
		BhvrOverride
			FadeOutLength	1
			PositionOffset	0.0 3.0 0.0
		End
		Sound Door_Explosion_01 200 200 .9
		Lifespan	8
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	Local
		At	Hips
		Bhvr	behaviors/CameraShake02.bhvr
		BhvrOverride
			PositionOffset	0.0 3.0 0.0
		End
		Part	:Dust_Circle.part
		Part	:Dust_Circle_Lingering.part
		Part	:Dust_Flash.part
		Part	:Sparks_Flash.part
		Lifespan	8
	End

	Event
		Type	Local
		At	Hips
		BhvrOverride
			FadeOutLength	1
			PositionOffset	0.0 3.0 0.0
		End
		Part	:Sparks_Long.part
		Part	:Rocks.part
		Lifespan	8
	End
End
#############################################################

End
