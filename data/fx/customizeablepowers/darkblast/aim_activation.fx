#########################################################
##	template

FxInfo
Lifespan 75

#######################################################################################

Condition
	On Time
	Time 0

	Event

		Type	local
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr

		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril2_Subtractive.part
		Part1	CustomizeablePowers\DarkMelee\DarkBlastHitTendril2.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitPuddle2_Subtractive.Part
		Part2	CustomizeablePowers\DarkMelee\DarkBlastHitPuddle2.part


		LifeSpan	35

	End

######################################################3

	Event
		EName	Pow2
		Type 	local
		At	Chest
		CHILDFX	POWERS\Child_Aim.fx
	End

End

######################################################3

End

