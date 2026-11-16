#############################################################
## Afterburner_Activation.fx
#############################################################

FxInfo
#Lifespan 90

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		ChildFX	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\ActivationEyes.fx
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/CameraShake_Subtle60.bhvr
		Part	:Implosion_Shockwave.part
		PMagnet	Chest
		Lifespan	10
	End
		Event
		Type	Local
		At	Root
		Sound 	Afterburner_01 100 100 .8
	End

End

Condition
	On	Time
	Time	33

	Event
		Type	Start
		At	Chest
		Part	:Explosion_Flash.part
		Lifespan	4
	End
End

Condition
	On	Time
	Time	35

	Event
		Type	Start
		At	Chest
		Part	:Explosion_Shockwave.part
		Part	:Explosion_Streaks.part
		Part	:Explosion_Streaks_Thick.part
		Lifespan	8
	End
End

Condition
	On	Time
	Time	40

	Event
		Type	StartPositOnly
		At	Root
		Bhvr	behaviors/Dust_Expanding.bhvr
		Part	:Dust_Expanding.part
		Part	:Dust_Shockwave.part
	End
End

Condition
	On	Time
	Time	45

	Event
		Type	Local
		At	Origin
		ChildFX	:Afterburner_Continuing.fx
	End
End

#############################################################

End