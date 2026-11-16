#############################################################
## FreezingRain.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time	0

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
		part	:FreezingRainWaterDrop2.part
		Part	:FreezingSteam.part
		sound	freezerain_loop 80 30 0.6
	End
End

#############################################################

Condition
	On 	Time
	Time	0

	Event
		EName 	sleeticesplatSpinsAndOffsets
		Type	start
		At	Root
		BHVR	Powers\ColdControl\SleetSplatRotate.bhvr
		Geom	CenterDummy
	End

	Event
		EName 	sleeticesplatSpinsAndOffsets2
		Type	start
		At	Root
		BHVR	Powers\ColdControl\SleetSplatRotate.bhvr
		Geom	CenterDummy
	End

	Event
		EName 	sleeticesplatSpinsAndOffsets3
		Type	start
		At	Root
		BHVR	Powers\ColdControl\SleetSplatRotate.bhvr
		Geom	CenterDummy
	End

	Event
		EName 	sleeticesplatSpinsAndOffsets4
		Type	start
		At	Root
		BHVR	Powers\ColdControl\SleetSplatRotate.bhvr
		Geom	CenterDummy
	End
End

Condition
	On 	Time
	Time	14

	Event
		EName 	sleeticesplat
		Type	start
		At	sleeticesplatSpinsAndOffsets
		Altpiv	1
		sound	icebreak3 80 70 0.96
		BHVR	Powers\ColdControl\SleetSplat.bhvr
		BhvrOverride
			StartColor 255 255 255
			PrimaryTint 66
			SecondaryTint 33
		End
		Splat	SleetIceSlick.tga
	End
End

Condition
	On 	Time
	Time	15

	Event
		EName 	sleeticesplat
		Type	start
		At	sleeticesplatSpinsAndOffsets2
		Altpiv	1
		BHVR	Powers\ColdControl\SleetSplat.bhvr
		BhvrOverride
			StartColor 255 255 255
			PrimaryTint 100
			SecondaryTint 0
		End
		Splat	SleetIceSlick.tga
	End
End

Condition
	On 	Time
	Time	16

	Event
		EName 	sleeticesplat
		Type	start
		At	sleeticesplatSpinsAndOffsets4
		Altpiv	1
		BHVR	Powers\ColdControl\SleetSplat.bhvr
		BhvrOverride
			StartColor 255 255 255
			PrimaryTint 66
			SecondaryTint 33
		End
		Splat	SleetIceSlick.tga
	End
End

Condition
	On 	Time
	Time	17

	Event
		EName 	sleeticesplat
		Type	start
		At	sleeticesplatSpinsAndOffsets3
		Altpiv	1
		BHVR	Powers\ColdControl\SleetSplat.bhvr
		BhvrOverride
			StartColor 255 255 255
			PrimaryTint 100
			SecondaryTint 0
		End
		Splat	SleetIceSlick.tga
	End
End

#############################################################

End