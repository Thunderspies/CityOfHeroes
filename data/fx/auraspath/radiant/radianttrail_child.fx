#############################################################
## RadiantTrail_CHILD.fx
#############################################################

FxInfo
Flags	DontSuppress
#########################################################

Condition
	On	Time
	Time	0

	Event	Ename	GlowSpecks
		Type	Local
		At	Root
		BhvrOverride
			Scale		0.15 0.15 0.15
			PositionOffset	0 1 1
		End
		Part	:RadiantTrail_Specks.part
		Part	:RadiantTrail_SpecksUp.part
	End

	Event
		EName 	Hookup
		Type	Local
		At	root
		geom	FX_RuneStreak
		bhvr	CustomizeablePowers\Invulnerability\Invulnerability_Spin.bhvr
		BhvrOverride
			Scale		0.15 0.15 0.15
			PositionOffset	0 1 0
			PyrRotate	0 0 0
			Spin		0.0 -0.15 0.0
		End
	End

	Event
		EName 	Hookup2
		Type	Local
		At	root
		geom	FX_RuneStreak
		bhvr	CustomizeablePowers\Invulnerability\Invulnerability_Spin.bhvr
		BhvrOverride
			Scale		0.2 0.2 0.2
			PositionOffset	0 0.8 0
			PyrRotate	0 0 0
			#Spin		0.0 0.2 0.0
		End
	End

	Event
		Type	Local
		At	Hookup
		altpiv  1
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 1 0
			StartJitter	0 1 0
			#Scale	5 5 5
		End
		Part	:RadiantStreaks.part
		Part	:RadiantStreaks_Highlight.part
	End

	Event
		Type	Local
		At	Hookup
		altpiv  2
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			#Scale	5 5 5
		End
		Part	:RadiantStreaks.part
		Part	:RadiantStreaks_Highlight.part
	End

	Event
		Type	Local
		At	Hookup2
		altpiv  1
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 1 0
			StartJitter	0 1 0
			#Scale	5 5 5
		End
	#	Part	:RadiantStreaks2.part
	End

	Event
		Type	Local
		At	Hookup2
		altpiv  2
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			#Scale	5 5 5
		End
	#	Part	:RadiantStreaks2.part
	End
End

#Condition
#	On	Time
#	Time	0
#
#	Event
#		Type	Local
#		At	Root
#		BhvrOverride
#			PositionOffset	0 0 -0.65
#		End
#		Part	:RadiantStreaks.part
#	End
#End
#########################################################
End