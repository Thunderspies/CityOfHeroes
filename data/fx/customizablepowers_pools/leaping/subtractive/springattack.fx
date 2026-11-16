#############################################################
## Ashes_Heavy.fx
#############################################################

FxInfo
Lifespan 500
#############################################################


###########################################

Condition
	On	Time
	Time	5

	Event
		Ename	Flash
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 0.5 0
		End
		Part	:SpringAttack_Flash.part
		Part	:SpringAttack_Flash_LocalFacing.part
		Part	:SpringAttack_Glow.part
		Lifespan 3
	End
		Event
		Type	Local
		At	Root
		Sound 	SpringAttack_Jump_01 100 100 .8
	End

End

Condition
	On	Time
	Time	17.5

	Event
		Type	Local
		At	Head
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part3	:SpringAttack_BodyStreak.part
		POther	StartOffset_Head
		Lifespan 1
	End

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part3	:SpringAttack_BodyStreak.part
		POther	StartOffset_Chest
		Lifespan 1
	End

	Event
		Type	Local
		At	UArmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part3	:SpringAttack_BodyStreak.part
		POther	StartOffset_UArmL
		Lifespan 1
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part3	:SpringAttack_BodyStreak.part
		POther	StartOffset_UArmR
		Lifespan 1
	End

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part3	:SpringAttack_BodyStreak.part
		POther	StartOffset_Hips
		Lifespan 1
	End

	Event
		Type	Local
		At	LLegL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part3	:SpringAttack_BodyStreak.part
		POther	StartOffset_LLegL
		Lifespan 1
	End

	Event
		Type	Local
		At	LLegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part3	:SpringAttack_BodyStreak.part
		POther	StartOffset_LLegR
		Lifespan 1
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Head
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part3	:SpringAttack_StreakIn.part
		POther	StartOffset_Head
		Lifespan 1
	End

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part3	:SpringAttack_StreakIn.part
		POther	StartOffset_Chest
		Lifespan 1
	End

	Event
		Type	Local
		At	UArmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part3	:SpringAttack_StreakIn.part
		POther	StartOffset_UArmL
		Lifespan 1
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part3	:SpringAttack_StreakIn.part
		POther	StartOffset_UArmR
		Lifespan 1
	End

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part3	:SpringAttack_StreakIn.part
		POther	StartOffset_Hips
		Lifespan 1
	End

	Event
		Type	Local
		At	LLegL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part3	:SpringAttack_StreakIn.part
		POther	StartOffset_LLegL
		Lifespan 1
	End

	Event
		Type	Local
		At	LLegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part3	:SpringAttack_StreakIn.part
		POther	StartOffset_LLegR
		Lifespan 1
	End
End
Condition
	On 	Time
	Time	25
	Event
		Type	Local
		At	Root
		Sound 	SpringAttack_Land_01 100 100 .8
	End
End

Condition
	On	Time
	Time	40

	Event
		Ename	TeleportIn
		Type	Start
		At	Root
		bhvr    behaviors/Braz/ExpandRingFast.bhvr
		BhvrOverride
			PositionOffset	0 1 0
		End
		Part	:SpringAttack_DustShockwave.part
		Lifespan 2
	End
End

Condition
	On	Time
	Time	40

	Event
		Ename	TeleportIn
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 0.5 0
		End
		Part	:SpringAttack_ImpactFlash.part
		Part	:SpringAttack_ImpactFlash_Localfacing.part
		Part	:SpringAttack_Glow2.part
		Part	:SpringAttack_Glow2_localFacing.part
		Lifespan 1
	End

	Event
		Ename	DustUp
		Type	Start
		At	Root
		Part	:SpringAttack_DustPillar.part
		Part	:SpringAttack_Rocks.part
		Lifespan 5
	End
End


Condition
	On 	Time
	Time 	40

	Event
		EName	GroundCrack
		Type	start
		At	Root
		Bhvr	POWERS\SuperStrength\Splat_FootStomp.bhvr
		BhvrOverride
			PositionOffset	1.5 0 1
			StartColor 	55 50 40
		End
		Splat	Crater_Cracks2.tga
		Lifespan 300
	End
End

#Condition
#	On 	Time
#	Time 	50
#
#	Event
#		Type	start
#		At	StartOffset
#		BhvrOverride
#			Shake          1.5
#			ShakeFallOff   0.075
#			ShakeRadius    128
#		End
#		Lifespan 1
#	End
#End

#############################################################

End
