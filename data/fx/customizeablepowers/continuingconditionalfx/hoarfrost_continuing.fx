#############################################################
## Ice Armor - Continuing FX
#############################################################

FxInfo
Flags InheritAlpha

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	Origin
		bhvr	behaviors\soundFade3.bhvr
		Sound IceArmor_loop 80 80 0.55
		Flags	PowerLoopingSound
		Lifespan 300
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
		Bhvr 	powers\coldcontrol\IceArmorWind.bhvr
		BhvrOverride
			Alpha		30
			TintGeom	1
		End
		geom	Tintable_FX_TmpCtrlWisp01
		part1	:IceMistUpwards.part
		part2	:SnowMistUpwards.part
		Sound IcePunch1 80 80 0.9
		Lifespan 26
	End
End

Condition
	On 	Time
	Time 	15
	Event
		EName   WindFast
		Type	start
		At	Root
		Bhvr 	powers\coldcontrol\IceArmorWindFast.bhvr
		BhvrOverride
			Alpha		30
			TintGeom	1
		End
		geom	Tintable_FX_TmpCtrlWisp01
		Sound coldblast2 80 80 0.8
		Lifespan 24
	End
End

Condition
	On 	Time
	Time 	20
	Event
		EName   WindFirst
		Type	start
		At	Root
		Bhvr 	powers\coldcontrol\IceArmorWindFirst.bhvr
		BhvrOverride
			Alpha		30
			TintGeom	1
		End
		geom	Tintable_FX_TmpCtrlWisp01
		Lifespan 24
	End
End

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	LLEGL
		part2	:Hoarfrost_Medium.part
		pOther	FootL
	End

	Event
		Type	Local
		At	LLEGR
		part2	:Hoarfrost_Medium.part
		pOther	FootR
	End

	Event
		Type	Local
		At	FootL
		part2	:Hoarfrost_Small.part
	End

	Event
		Type	Local
		At	FootR
		part2	:Hoarfrost_Small.part
	End
End

#################################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	ULegR
		part2	:Hoarfrost_Medium.part
		pOther	LLegR
	End

	Event
		Type	Local
		At	ULegL
		part2	:Hoarfrost_Medium.part
		pOther	LLegL
	End

	Event
		Type	Local
		At	WepL
		part2	:Hoarfrost_Small.part
	End

	Event
		Type	Local
		At	WepR
		part2	:Hoarfrost_Small.part
	End
End

#################################################################

Condition
	On 	Time
	Time 	20

	Event
		Type	Local
		At	LArmR
		part2	:Hoarfrost_Small.part
		pOther	HandR
	End

	Event
		Type	Local
		At	LArmL
		part2	:Hoarfrost_Small.part
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
		part2	:Hoarfrost_Small.part
		pOther	LArmL
	End

	Event
		Type	Local
		At	UArmR
		part2	:Hoarfrost_Small.part
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
		part2	:Hoarfrost_Large.part
		pOther	Waist
	End
End

#################################################################

End