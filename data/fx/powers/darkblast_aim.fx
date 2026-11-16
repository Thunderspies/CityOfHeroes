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
		Bhvr	Behaviors/GenericParticleFade.bhvr

		Part	:DarkBlastHitTendril2_Subtractive.part
		Part1	:DarkBlastHitTendril2.part
		Part	:DarkBlastHitPuddle2_Subtractive.Part
		Part2	:DarkBlastHitPuddle2.part


		LifeSpan	35

	End


End

######################################################3

Condition
	On Time
	Time 0

	Event
		EName	Pow2
		Type 	local
		At	Chest
		CHILDFX	:Child_Aim.fx
	End

End

######################################################3


End

