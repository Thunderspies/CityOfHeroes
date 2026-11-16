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
		At	Origin
		BhvrOverride
		#	Scale		0.15 0.15 0.15
			PositionOffset	0 5 1
		End
		Part	:RadiantTrail_FlySpecks.part
	End

	Event	Ename	GlowSpecks
		Type	Local
		At	Origin
		BhvrOverride
		#	Scale		0.15 0.15 0.15
			PositionOffset	0 6 1
		End
		Part	:RadiantTrail_FlySpecksUp.part
	End

	Event
		EName 	Hookup
		Type	Local
		At	Origin
		geom	FX_RuneStreak
		bhvr	CustomizeablePowers\Invulnerability\Invulnerability_Spin.bhvr
		BhvrOverride
			Scale		0.15 0.15 0.15
			PositionOffset	0 4 0
			PyrRotate	90 0 0
			Spin		0.0 -0.15 0.0
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
		Part	:RadiantStreaks_Fly.part
		Part	:RadiantStreaks_FlyHighlight.part
	End

	Event
		Type	Local
		At	Hookup
		altpiv  2
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			#Scale	5 5 5
		End
		Part	:RadiantStreaks_Fly.part
		Part	:RadiantStreaks_FlyHighlight.part
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