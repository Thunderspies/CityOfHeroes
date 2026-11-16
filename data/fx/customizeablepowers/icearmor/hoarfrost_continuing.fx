#############################################################
## Ice Armor - Continuing FX
#############################################################

FxInfo

Flags    InheritAlpha

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	Origin
		bhvr	behaviors\soundFade3.bhvr
		Sound IceArmor_loop 80 80 .55
		Flags	PowerLoopingSound
		Lifespan	300
	End
End

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName   Wind
		Type	start
		At	Root
		Bhvr	CustomizeablePowers\IceArmor\IceArmorWind.bhvr
		geom	FX_TmpCtrlWisp01
		part1	CustomizeablePowers\ContinuingConditionalFX\IceMistUpwards.part
		part2	CustomizeablePowers\ContinuingConditionalFX\SnowMistUpwards.part
		Sound IcePunch1 80 80 .9
		Lifespan	26
	End
End

Condition
	On 	Time
	Time 	15
	Event
		EName   WindFast
		Type	start
		At	Root
		Bhvr	CustomizeablePowers\IceArmor\IceArmorWindFast.bhvr
		geom	FX_TmpCtrlWisp01
		Sound coldblast2 80 80 .8
		Lifespan	24
	End
End

Condition
	On 	Time
	Time 	20
	Event
		EName   WindFirst
		Type	start
		At	Root
		Bhvr	CustomizeablePowers\IceArmor\IceArmorWindFirst.bhvr
		geom	FX_TmpCtrlWisp01
		Lifespan	24
	End
End

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	LLEGL
		part2	CustomizeablePowers\ContinuingConditionalFX\Hoarfrost_Medium.part
		pOther	FootL
	End

	Event
		Type	Local
		At	LLEGR
		part2	CustomizeablePowers\ContinuingConditionalFX\Hoarfrost_Medium.part
		pOther	FootR
	End

	Event
		Type	Local
		At	FootL
		part2	CustomizeablePowers\ContinuingConditionalFX\Hoarfrost_Small.part
	End

	Event
		Type	Local
		At	FootR
		part2	CustomizeablePowers\ContinuingConditionalFX\Hoarfrost_Small.part
	End
End

#################################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	ULegR
		part2	CustomizeablePowers\ContinuingConditionalFX\Hoarfrost_Medium.part
		pOther	LLegR
	End

	Event
		Type	Local
		At	ULegL
		part2	CustomizeablePowers\ContinuingConditionalFX\Hoarfrost_Medium.part
		pOther	LLegL
	End

	Event
		Type	Local
		At	WepL
		part2	CustomizeablePowers\ContinuingConditionalFX\Hoarfrost_Small.part
	End

	Event
		Type	Local
		At	WepR
		part2	CustomizeablePowers\ContinuingConditionalFX\Hoarfrost_Small.part
	End
End

#################################################################

Condition
	On 	Time
	Time 	20

	Event
		Type	Local
		At	LArmR
		part2	CustomizeablePowers\ContinuingConditionalFX\Hoarfrost_Small.part
		pOther	HandR
	End

	Event
		Type	Local
		At	LArmL
		part2	CustomizeablePowers\ContinuingConditionalFX\Hoarfrost_Small.part
		pOther	HandL
	End

End

##############################################

Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At	UArmL
		part2	CustomizeablePowers\ContinuingConditionalFX\Hoarfrost_Small.part
		pOther	LArmL
	End

	Event
		Type	Local
		At	UArmR
		part2	CustomizeablePowers\ContinuingConditionalFX\Hoarfrost_Small.part
		pOther	LArmR
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	Neck
		part2	CustomizeablePowers\ContinuingConditionalFX\Hoarfrost_Large.part
		pOther	Waist
	End
End

#################################################################

End