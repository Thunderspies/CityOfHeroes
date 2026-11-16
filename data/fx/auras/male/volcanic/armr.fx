#############################################################
## Sparks_Body.fx
#############################################################

FxInfo

#############################################################

Condition
	On		Cycle
	Time		10
	Chance		0.25
	Random		1

	Event
		Type	PositOnly
		At	LArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		ChildFX	:CHILD_Explosion.fx

		Lifespan 	3
	End

	Event
		Type	PositOnly
		At	WepR
	#	Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Sparks.part
		Part	:Fire_Anim.part
		Part	:Lava_Splash_Anim_Additive.part
		Lifespan 	3
	End
End

#############################################################

End