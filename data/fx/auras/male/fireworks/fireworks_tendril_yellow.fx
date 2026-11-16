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
		Part	:Fireworks_Flash_Yellow.part
		Part	:Fireworks_Glow_Yellow.part
		Part	:Fireworks_Smoke_Yellow.part
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
			InitialVelocityJitter 	0.2 0.2 0.2
			Gravity			.001
			Drag			0.05
		End
		Part	:Fireworks_Tendril_Yellow.part
		Lifespan 8
	End

	Event
		Type	Start
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			InitialVelocityJitter 	0.2 0.2 0.2
			Gravity			.0015
			Drag			0.02

		End
		Part	:Fireworks_Tendril_Yellow.part
		Part	:Fireworks_Head_Yellow.part
		Lifespan 12
		LifespanJitter	1
	End


End

#############################################################

End