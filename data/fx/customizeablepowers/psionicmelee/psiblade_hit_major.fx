#############################################################
## PsiBlade_Hit_Major.fx
#############################################################

FxInfo
Lifespan 25

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
// 		Sound 	PsionicMelee_PsiBlade_Hit_Major 100 100 .8
// 	End
// End

Condition
	On	Time
	Time	0

		Event
			Ename	TopAnchor
			At	Root
			Type	Local
			Bhvr	behaviors/CameraShake02.bhvr
			BhvrOverride
				TintGeom 	1
				PositionOffset 	0 4 6.5
				PyrRotate	-10 0 10
				Scale		1.25 1.25 1.25
			End
		End

		Event
			Ename 	Slash
			At 	Root
			Type 	Local
			BHVR	Behaviors\GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset	0 6 0
			End
			Part	:Slash_Down.part
			Part	:Slash_Down_Glow.part
		End
	End

Condition
	On Time
	Time 2

		Event
			EName	faceringsRigging
			Type	local
			At	Chest
			BHVR	Behaviors\GenericParticleFade.bhvr
			Part 	:PsioHeadGlow.part
			Part	:Hit_Sparks.part
		End

		Event
			EName	facerings
			Type	local
			At	Chest
			BHVR	Behaviors\GenericParticleFade.bhvr
			Part	CustomizeablePowers\PsionicAssault\InfuseLight2.part
			Part	CustomizeablePowers\PsionicAssault\Infuseglow.part
			Part	CustomizeablePowers\PsionicAssault\Defuseglow.part
			Lifespan 5
		End
End

Condition
	On Time
	Time 5

		Event
			Ename	SlashOutAnchor
			At 	Root
			Type	PositOnly
		End

		Event
			Ename	SlashOut
			At	SlashOutAnchor
			Type	Local
			BhvrOverride
				PositionOffset	0 4 0
				PyrRotate	90 0 0
			End
			Part	:Slash_Out.part
			Part	:Slash_Out_Hilite.part
		End

		Event
			Ename	SlashOut
			At	SlashOutAnchor
			Type	Local
			BhvrOverride
				PositionOffset	0 4 0
				PyrRotate	-90 0 0
			End
			Part	:Slash_Out.part
			Part	:Slash_Out_Hilite.part
		End
	End

Condition
	On Time
	Time 25

		Event
			EName	facerings2
			Type	Destroy
		End
End

Condition
	On Time
	Time 30

		Event
			EName	All
			Type	Destroy
		End
End

End