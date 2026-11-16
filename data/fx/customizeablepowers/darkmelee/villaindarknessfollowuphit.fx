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


End

######################################################3

Condition
	On Time
	Time 0

	Event
		EName	Pow2
		Type 	local
		At	Chest
		Part1	CustomizeablePowers\DarkMelee\FollowUpGlow.part
		Part2	CustomizeablePowers\DarkMelee\FollowUpSpecks2.part
		Sound clawfollow3 100 100 .8
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 13
	End

End

Condition
	On Time
	Time 10

	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:FollowUpSpecks.part
		Part2	CustomizeablePowers\DarkMelee\FollowUpRing01.part
		Part3	CustomizeablePowers\DarkMelee\FollowUpRing01a.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

End

Condition
	On Time
	Time 20

	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:FollowUpSpecks.part
		Part3	CustomizeablePowers\DarkMelee\FollowUpRing02.part
		Part4	CustomizeablePowers\DarkMelee\FollowUpRing02a.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

End

Condition
	On Time
	Time 30

	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:FollowUpSpecks2.part
		Part2	CustomizeablePowers\DarkMelee\FollowUpRingSlow.part
		Part3	CustomizeablePowers\DarkMelee\FollowUpRing03.part
		Part4	CustomizeablePowers\DarkMelee\FollowUpRing03a.part
		Bhvr	Behaviors\GenericParticleFade.bhvr

	End

End


#Condition
#	On Time
#	Time 13
#
#	Event
#		EName	pow2
#		Type 	Destroy
#
#	End
#
#End

#Condition
#	On Time
#	Time 75
#
#	Event
#		EName	all
#		Type 	Destroy
#
#	End
#
#End



End

