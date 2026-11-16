#############################################################
## TelekineticBlow_Hit.fx
#############################################################

FxInfo
LifeSpan 25

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
// 		Sound 	PsionicMelee_TelekineticBlow_Hit 100 100 .8
// 	End
// End

Condition
	On	Time
	Time	0

	Event
		Ename	Shake
		Type	Local
		At	Chest
		Bhvr	behaviors/CameraShake03.bhvr
	End

	Event
		EName	faceringsRigging
		Type	local
		At	Chest
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part 	:PsioHit_Rings01.part
		Part 	:PsioHit_Rings02.part
		Part 	:PsioHeadGlow.part
		Part	:Hit_Flash_sm.part
		Part	:Hit_Sparks_sm.part
		Part	:Hit_Shockwave_sm.part
		Part	:PunchHit_Streaks_Big.part
	End

	Event
		EName	facerings2
		Type	local
		At	Chest
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:HitStreaksBig.part
		Lifespan 5
	End
End

End