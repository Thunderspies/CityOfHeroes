#############################################################
## Cold Domination - Sleet
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Root
		sound	icepatch_loop 80 30 0.76
	End

	Event
		EName	Hookup
		Type	local
		At	Root
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Geom	OverHeadDummy01
		Sound icerain 100 100 0.86
	End

	Event
		Type	local
		At	Hookup
		altpiv	1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:FreezingRainWaterDrop.part
		Part	:FreezingSteam.part
	End

	Event
		Ename	LastRainFalling
		Type	local
		At	Hookup
		altpiv	1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:FreezingRainWaterDrop.part
		Part	:FreezingSteam.part
	End
End

#############################################################

Condition
	On 	Time
	Time	10

	Event
		Type	Start
		At	Root
		BHVR	V_COV\Powers\ColdDomination\SleetSplat.bhvr
		BhvrOverride
			StartColor		255 255 255
			PrimaryTint		100
			SecondaryTint		0
		End
		Splat	SleetIceSlick.tga
		sound	icebreak3 80 70 0.96
	End
End

Condition
	On 	Time
	Time	13

	Event
		Type	Start
		At	Root
		BHVR	V_COV\Powers\ColdDomination\SleetSplat.bhvr
		BhvrOverride
			StartColor		255 255 255
			PrimaryTint		75
			SecondaryTint		25
		End
		Splat	SleetIceSlick.tga
	End
End

Condition
	On 	Time
	Time	15

	Event
		Type	Start
		At	Root
		BHVR	V_COV\Powers\ColdDomination\SleetSplat.bhvr
		BhvrOverride
			StartColor		255 255 255
			PrimaryTint		66
			SecondaryTint		33
		End
		Splat	SleetIceSlick.tga
	End
End

#############################################################

End