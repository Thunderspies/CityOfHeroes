#############################################################
## TestTube_Continuing.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	FXOffset
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset		0 5.5 0
			PyrRotate		-90 270 90
		End
		sound SSCalculate_Loop 200 190 .075
	End
End

#############################################################
## LOOP
#############################################################

Condition
	On 	Cycle
	Time 	10

	Event
		Type	Local
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0 0 0
			StartJitter		2.0 0.0 1
			InitialVelocity		0 0 0
			InitialVelocityJitter	.002 0.0 0.01
		End
		At	FXOffset
		Part	:Thoughts_Side_Blur.part
		Part	:Thoughts_Side.part
		Lifespan 150
	End

	Event
		Type	Local
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0 0 0
			StartJitter		2.0 0.0 1
			InitialVelocity		0 0 0
			InitialVelocityJitter	-.002 0.0 0.02
		End
		At	FXOffset
		Part	:Thoughts2_Side_Blur.part
		Part	:Thoughts2_Side.part
		Lifespan 150
	End
End

Condition

	On 	Cycle
	Time 	60

	Event
		Ename	BrainFarts
		Type	Local
		Bhvr	behaviors/GenericParticleFade.bhvr.
		At	FXOffset
		ChildFX	:ThoughtPuff.fx
		Lifespan 50
	End
End

Condition
	On	Cycle
	Time	80

	Event
		Ename	BrainFarts
		Type	StartPositOnly
		Bhvr	behaviors/GenericParticleFade.bhvr
		At	FXOffset
		ChildFX	:ThoughtPuff2.fx
		Lifespan 50
	End
End

#############################################################

End

