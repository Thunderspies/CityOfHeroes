#############################################################
## TestTube_Continuing.fx
#############################################################

FxInfo

#############################################################
## LOOP
#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	FXOffset
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset		0 5.5 0
			StartJitter		2.5 1.5 0
			PyrRotate		-90 270 90
			InitialVelocity		0 0 0
			InitialVelocityJitter	.002 0.0 0.02
		End
	End

	Event
		Ename	BrainFarts
		Type	Local
		Bhvr	behaviors/GenericParticleFade.bhvr
		At	FXOffset
		Part	:Thoughts2_Stationary_Blur.part
		Lifespan 100
	End
End

Condition
	On	Time
	Time	5

	Event
		Ename	BrainFarts
		Type	Local
		Bhvr	behaviors/GenericParticleFade.bhvr
		At	FXOffset
		Part	:Thoughts2_Stationary.part
		Lifespan 80
	End
End

#############################################################

End

