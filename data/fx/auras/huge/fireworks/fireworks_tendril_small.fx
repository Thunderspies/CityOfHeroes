#############################################################
## Fireflies_Body.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	1

	Event
		Type	Start
		At	Origin
		Part	:Fireworks_Flash_Small.part
		Part	:Fireworks_Glow_Small.part
		Part	:Fireworks_Smoke_Small.part
		Lifespan 2
	End
End

Condition
	On	Time
	Repeat	20
	Random	1

	Event
		Type	Start
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			InitialVelocityJitter 	0.15 0.15 0.15
			Gravity			.001
			Drag			0.05
		End
		Part	:Fireworks_Tendril_Small.part
		Lifespan 3
	End

	Event
		Type	Start
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			InitialVelocityJitter 	0.15 0.15 0.15
			Gravity			.0015
			Drag			0.02

		End
		Part	:Fireworks_Tendril_Small.part
		Part	:Fireworks_Head_Small.part
		Lifespan 	6
		LifespanJitter 1
	End


End

#############################################################

End