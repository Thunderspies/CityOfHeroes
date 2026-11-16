#############################################################
## Sparks_Body.fx
#############################################################

FxInfo

#############################################################

Condition
	On		Cycle
	Time		10
	Chance		0.35
	Random		1


	Event
		Type	PositOnly
		At	ULegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		ChildFX	:CHILD_Explosion.fx

		Lifespan 	3
	End

	Event
		Type	PositOnly
		At	ULegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		ChildFX	:CHILD_Explosion.fx

		Lifespan 	3
	End

	Event
		Type	PositOnly
		At	LLegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		ChildFX	:CHILD_Explosion.fx

		Lifespan 	3
	End

	Event
		Type	PositOnly
		At	LLegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		ChildFX	:CHILD_Explosion.fx

		Lifespan 	3
	End

	Event
		Type	PositOnly
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		ChildFX	:CHILD_Explosion.fx

		Lifespan 	3
	End

	Event
		Type	PositOnly
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		ChildFX	:CHILD_Explosion.fx

		Lifespan 	3
	End
End

#############################################################

End