#############################################################
## BuildUp_Attack.fx
#############################################################

FxInfo

Input
	Inpname	Hips
End

Lifespan 75

#############################################################

Condition
	On	Time
	Time	10

	Event
		Type	start
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\EnergyMelee\Subtractive\PowerPunchHit.part
		Part2	CustomizeablePowers\EnergyMelee\Subtractive\PowerPunchHit1.part
		Part3	CustomizeablePowers\EnergyMelee\Subtractive\PowerPunchHit2.part
		Part4	CustomizeablePowers\EnergyMelee\Subtractive\PowerPunchHitRaysSmall.part
		Part1	CustomizeablePowers\EnergyMelee\Subtractive\HitRing.part
		Part3	CustomizeablePowers\EnergyMelee\Subtractive\HitRing2.part
		Part4	CustomizeablePowers\EnergyMelee\Subtractive\PowerPunchRingz.part
		LifeSpan 20
	End
End

#############################################################

Condition
	On	Time
	Time	15

	Event
		Type 	local
		At	Root
		ChildFX	POWERS\Child_BuildUp.fx
	End
End

#############################################################

End