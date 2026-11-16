#############################################################
## HEADER
#############################################################

FxInfo

#############################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		EName	Sound1
#		Type	start
#		At	T_Root
#		Sound	WarShadeAnimateFoe 90 80 .83
#	End
#End
#
#Condition
#	On	Time
#	Time	10
#
#	Event
#		EName	Sound2
#		Type	local
#		At	T_Root
#		Sound	flameburst_loop 60 22 .74
#		Sound	fireballfly 60 20 .3
#	End
#End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	start
		At	Origin
		part	POWERS\CosmicPowers\Nictus\AnimateFoe\FlatTendrils1.part
		part	POWERS\CosmicPowers\Nictus\AnimateFoe\FlatTendrils1Purple.part
		part	POWERS\CosmicPowers\Nictus\AnimateFoe\FlatTendrils1Blue.part
		part	POWERS\CosmicPowers\Nictus\AnimateFoe\FlatTendrilsPurple.part
		part	POWERS\CosmicPowers\Nictus\AnimateFoe\FlatTendrilsBlue.part
		#Lifespan 180
	End

	Event
		Type	start
		At	Origin
		BHVR	POWERS\CosmicPowers\Nictus\AnimateFoe\Offset.bhvr
		part	POWERS\CosmicPowers\Nictus\AnimateFoe\BlackTendrilsRing.part
		part	POWERS\CosmicPowers\Nictus\AnimateFoe\PurpleTendrilsRing.part
		part	POWERS\CosmicPowers\Nictus\AnimateFoe\BlueTendrilsRing.part
	End

	Event
		Type	Local
		At	Origin
		Part	POWERS\CosmicPowers\Nictus\AnimateFoe\Gloom.part
		Part	POWERS\CosmicPowers\Nictus\AnimateFoe\TendrilPurpleGloom.part
		Part	POWERS\CosmicPowers\Nictus\AnimateFoe\TendrilBlueGloom.part
		Part	POWERS\CosmicPowers\Nictus\AnimateFoe\TendrilGloom.part
		Pmagnet	Chest
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Origin
		Part	POWERS\CosmicPowers\Nictus\AnimateFoe\MainBaseFlatTendrils.part
		Part	POWERS\CosmicPowers\Nictus\AnimateFoe\MainBaseFlatTendrilsPurple.part
		Part	POWERS\CosmicPowers\Nictus\AnimateFoe\MainBaseFlatTendrilsBlue.part
		#Lifespan 140
	End
End

#############################################################

Condition
	On	Time
	Time	10

	Event
		Type	start
		At	Origin
		part1	:Ring_Tendrils_Black1.part
		part1	:Ring_Tendrils_Blue1.part
		part1	:Ring_Tendrils_Violet1.part
	End

	Event
		Type	start
		At	Origin
		part2	:Ring_Tendrils_Black2.part
		part2	:Ring_Tendrils_Blue2.part
		part2	:Ring_Tendrils_Violet2.part
	End
End

#############################################################

End