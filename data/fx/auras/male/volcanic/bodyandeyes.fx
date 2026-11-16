#############################################################
## Sparks_Body.fx
#############################################################

FxInfo

#############################################################

Condition
	On		Time
	Time		0

	Event
		Type	PositOnly
		At	Origin
		ChildFX	:Eyes.fx
	End
End

Condition
	On		Cycle
	Time		10
	Chance		0.5
	Random		1

	Event
		Type	PositOnly
		At	Head
		Bhvr	Behaviors\GenericParticleFade.bhvr
		ChildFX	:CHILD_Explosion.fx
		Lifespan 	3
	End

	Event
		Type	PositOnly
		At	UArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		ChildFX	:CHILD_Explosion.fx

		Lifespan 	3
	End


	Event
		Type	PositOnly
		At	UArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		ChildFX	:CHILD_Explosion.fx

		Lifespan 	3
	End


	Event
		Type	PositOnly
		At	LArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		ChildFX	:CHILD_Explosion.fx

		Lifespan 	3
	End

	Event
		Type	PositOnly
		At	LArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Sparks.part
		Part	:Fire_Anim.part
		Part	:Lava_Splash_Anim_Additive.part
		Lifespan 	3
	End

	Event
		Type	PositOnly
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		ChildFX	:CHILD_Explosion.fx
		Lifespan 	3
	End

	Event
		Type	PositOnly
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		ChildFX	:CHILD_Explosion.fx

		Lifespan 	3
	End

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

Condition
	On	Cycle
	Time	10
	Chance	0.25
	Random	1

	Event
		Type	PositOnly
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Sparks.part
		Part	:Fire_Anim.part
		Part	:Lava_Splash_Anim_Additive_Large.part
		Lifespan 	3
	End

	Event
		Type	PositOnly
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Fire_Anim2_Large.part
		Lifespan 	3
	End
End

#############################################################

End