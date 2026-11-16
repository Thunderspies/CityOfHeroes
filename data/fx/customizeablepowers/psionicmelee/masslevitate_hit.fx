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
// 		Sound 	PsionicMelee_PsiBlade_Hit 100 100 .8
// 	End
// End

Condition
	On	Time
	Time	0

	Event
		Ename	HitAnchor
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset 0 0.5 0
		End
	End

	Event
		EName	faceringsRigging
		Type	local
		At	HitAnchor
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part 	:PsioHeadGlow.part
		Part	:Hit_Flash_sm.part
		Part	:Hit_Sparks_sm.part
		Part	:Hit_Shockwave_Big.part
		Part	:PunchHit_Streaks_Big.part
		Part	:AOE_Flareup.part
		Part	:AOE_Flareup_Highlight.part
	End

	Event
		EName	facerings2
		Type	local
		At	HitAnchor
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:HitStreaksBig.part
		Lifespan 5
	End
End

End