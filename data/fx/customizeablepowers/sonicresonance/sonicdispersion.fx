#############################################################
## Sonic Dispersion
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound herosonicblast1 80 80 .30
		LifeSpan 90
	End
End

Condition
	On 	Time
	Time 	18

	Event
		Type	Local
		At	Root
		Sound ForcefieldOn 100 100 .90
		LifeSpan 90
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Type	Local
		At	Root
		Sound SonicBubble4_loop 100 100 .60
		bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 180

	End
End

#############################################################

Condition
	On 	Time
	Time 	1

	Event
		EName 	HeadOffset
		Type	local
		At	Head
		Geom	PsionicRigging
		LifeSpan 90
	End
End

Condition
	On 	Time
	Time 	17

	Event
		Type	Local
		At	HeadOffset
		AltPiv	3
		Bhvr	Behaviors\GenericParticleFadeQuick.bhvr
		Part	:SonicRings1.part
		LifeSpan 45
	End
End

Condition
	On 	Time
	Time 	17

	Event
		Type	Local
		At	HandL
		Bhvr	Behaviors\GenericParticleFadeQuick.bhvr
		BhvrOverride
			PyrRotate	0 0 -90
		End
		Part	:SonicRings2.part
		LifeSpan 45
	End
End

Condition
	On 	Time
	Time 	17

	Event
		Type	Local
		At	HandR
		Bhvr	Behaviors\GenericParticleFadeQuick.bhvr
		BhvrOverride
			PyrRotate	0 0 90
		End
		Part	:SonicRings2.part
		LifeSpan 45
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
			TintGeom		1
		End
		Geom	Tintable_SonicBuff_Bubble2
	End
End

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
			Spin		0 -0.0125 0
			TintGeom		1
		End
		Geom	Tintable_SonicBuff_WaveformBubble1
	End
End

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
			Spin		0 0.0125 0
			TintGeom		1
		End
		Geom	Tintable_SonicBuff_WaveformBubble2
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
		part1	:DispersionBubble_Motes1.part
		part1	:DispersionBubble_Motes2.part
	End
End

#############################################################

End