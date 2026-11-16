#############################################################
## Sonic Dispersion
#############################################################

FxInfo

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
	Time 	1

	Event
		EName 	HeadOffset
		Type	local
		At	Hips
		Geom	PsionicRigging
		LifeSpan 90
	End
End

Condition
	On 	Time
	Time 	17

	Event
		Type	Local
		At	Root
	#	Bhvr	Behaviors\GenericParticleFadeQuick.bhvr
		Part	:Rings_Static_Ground_Shield_Small.part
		Part	:Rings_Static_Ground_Shield.part

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
		Sound IDF_BattleOrb_Shield_Continuing_Loop 50 50 .15
	End
End

## LARGE BUBBLE #######################################################

Condition
	On 	Time
	Time	27

	Event
		Type	Local
		At	Hookup
		Altpiv	1
		Bhvr	:SonicBubble_Fade1.bhvr
		Type 	Create
		Inherit	Position
		Update	Position
		BhvrOverride
			StartColor	255 150 0
		End
		Geom	Tintable_SonicBuff_Bubble2
	End
End

Condition
	On 	Time
	Time	60

	Event
		Type	Local
		At	Hookup
		Altpiv	1
		Type 	Create
		Inherit	Position
		Update	Position
		Bhvr	:SonicBubble_Fade1.bhvr
		Part	:Sparkles_Shield.part
		Part	:Sparkles2_Shield.part
	End
End

#############################################################

End