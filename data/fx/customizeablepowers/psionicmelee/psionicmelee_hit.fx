#############################################################
## InfuseThoughtsHit.fx
#############################################################

FxInfo
LifeSpan	25

###########################################################

Input
	Inpname	Hips
End

###########################################################
# SOUND

#Condition
#	On	Time
#	Time	0
#
#	Event
#		Type	local
#		At	Chest
#		Sound Infuse4 80 80 .6
#	End
#End

########################################################
// Condition
// 	On 	Time
// 	Time	0
// 	Event
// 		Type	Local
// 		At	Root
// 		Sound 	PsionicMelee_Hit 100 100 .8
// 	End
// End

Condition
	On	Time
	Time	0

	Event
		EName	faceringsRigging
		Type	local
		At	Chest
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part 	:PsioHit_Rings01.part
		Part 	:PsioHit_Rings02.part
		Part 	:PsioHeadGlow.part
		Part	:Hit_Flash_sm.part
		Part	:Hit_Shockwave_sm.part
		Part	:PunchHit_Streaks.part
	End

End


End