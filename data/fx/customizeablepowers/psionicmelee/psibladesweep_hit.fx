#############################################################
## PsiBlade_Hit_Major.fx
#############################################################

FxInfo
Lifespan 25
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
// 		Sound 	PsionicMelee_PsiBladeSweep_Hit 100 100 .8
// 	End
// End

Condition
	On	Time
	Time	0

		Event
			Ename	TopAnchor
			At	Root
			Type	Local
			BhvrOverride
				TintGeom 	1
				PositionOffset 	0 4 6.5
				PyrRotate	-10 0 10
				Scale		1.25 1.25 1.25
			End
			#Geom FX_Slash
			Lifespan 4
		End

		Event
			Ename Slash
			At Hips
			Type Local
			BhvrOverride
				PositionOffset	0 0.5 0
				PyrRotate	45 0 0
			End
			Part	:Slash_Side.part
		End
	End



Condition
	On	Time
	Time	1

	Event
		EName	faceringsRigging
		Type	local
		At	Chest
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part 	:PsioHit_Rings01.part
		Part 	:PsioHit_Rings02.part
		Part 	:PsioHeadGlow.part
		Part	:Hit_Flash_sm.part
		Part	:SliceHit_Sparks.part
	#	Part	:Hit_Shockwave_sm.part
	End

	Event
		EName	facerings2
		Type	PositOnly
		At	Chest
		BHVR	Behaviors/GenericParticleFade.bhvr
			Part	:SliceStreaks.part
		Lifespan 5
	End

End

End