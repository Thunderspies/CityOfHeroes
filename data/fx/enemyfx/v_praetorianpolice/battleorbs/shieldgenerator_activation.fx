#############################################################
## Sonic Dispersion
#############################################################

FxInfo
Lifespan 90

#############################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		Type	Local
#		At	Root
#		Sound herosonicblast1 80 80 .30
#		LifeSpan 90
#	End
#End
#
#Condition
#	On 	Time
#	Time 	18
#
#	Event
#		Type	Local
#		At	Root
#		Sound ForcefieldOn 100 100 .90
#		LifeSpan 90
#	End
#End

#Condition
#	On 	Time
#	Time 	20
#
#	Event
#		Type	Local
#		At	Root
#		Sound SonicBubble4_loop 100 100 .60
#		bhvr	Behaviors\soundFade3.bhvr
#		Flags	PowerLoopingSound
#		Lifespan 180
#
#	End
#End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	MkIOffsets
		Type	Local
		At	Chest
		BhvrOverride
			Alpha	1
		End
		Geom	FX_BattleOrb_MkI_Offsets
		Sound IDF_BattleOrb_Shield_Activate_01 50 50 .75
	End


	Event
		EName	TargetAnchor
		Type	PositOnly
		At	Root
		LookAt	MkIOffsets_8
	End

	Event
		Ename	MkIOffsets_8
		Type	Local
		At	MkIOffsets
		AltPiv	8
	End


End

Condition
	On	Cycle
	Time	5

	Event
		Ename	Flash
		Type	Local
		At	MkIOffsets_8
		Part	:Muzzle_Glow_Shield_Activation.part
		Part	:Muzzle_Glow2_Shield_Activation.part

		Lifespan 60
	End


End

Condition
	On	Time
	Time	10

	Event
		Ename	Flash
		Type	Local
		At	TargetAnchor
		Part	:Beam_Glow_Small_Shield.part
		POther	MkIOffsets_8
		Lifespan 60
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	Root
		Part	:Rings_Static_Ground_Shield_Small.part

	End
End


## OFFSETS #######################################################

Condition
	On 	Time
	Time	1

	Event
		EName	Hookup
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 3 0
		End
		Geom	RootToChestAdjustment
	End
End


#############################################################

End