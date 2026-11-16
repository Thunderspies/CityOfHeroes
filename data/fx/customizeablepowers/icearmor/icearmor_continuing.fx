#########################################################
##
##
FxInfo


Flags    InheritAlpha


##########################################################


Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At 	origin
		Sound IceEmerge3 80 80 .95
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At 	origin
		Sound IceArmor_loop 80 80 .5
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 250
	End
End







Condition
	On 	Time
	Time 	35

	Event
		Type	Local
		At	Head
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceArmor_HEAD
		part2	CustomizeablePowers\IceArmor\IceArmorBits.part
		part3	CustomizeablePowers\IceArmor\IceArmorBitsSmall.part
		part4	CustomizeablePowers\IceArmor\Snowfall.part

	End
End

Condition
	On 	Time
	Time 	30


	Event
		Type	Local
		At	CHEST
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceArmor_CHEST
	End
End

##############################################

Condition
	On 	Time
	Time 	25


		Event
		Type	Local
		At	Hips
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceArmor_HIPS
	End


	Event
		Type	Local
		At	UARML
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceArmor_UARML
	End

	Event
		Type	Local
		At	UARMR
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceArmor_UARMR
	End
End


####################################################

Condition
	On 	Time
	Time 	20

		Event
		Type	Local
		At	LARMR
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceArmor_LARMR
		part2	CustomizeablePowers\IceArmor\IceArmorBits.part
		part3	CustomizeablePowers\IceArmor\IceArmorBitsSmall.part

	End

		Event
		Type	Local
		At	LARML
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceArmor_LARML
		part2	CustomizeablePowers\IceArmor\IceArmorBits.part
		part3	CustomizeablePowers\IceArmor\IceArmorBitsSmall.part

	End

		Event
		Type	Local
		At	WepL
		part1	CustomizeablePowers\IceArmor\Snowfall.part
		part2	CustomizeablePowers\IceArmor\IceArmorBits.part
		part3	CustomizeablePowers\IceArmor\IceArmorBitsSmall.part
	End

		Event
		Type	Local
		At	WepR
		part1	CustomizeablePowers\IceArmor\Snowfall.part
		part2	CustomizeablePowers\IceArmor\IceArmorBits.part
		part3	CustomizeablePowers\IceArmor\IceArmorBitsSmall.part

	End



End
###################################################################
Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	UlegR
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceArmor_ULEGR
		part1	CustomizeablePowers\IceArmor\Snowfall.part
		part2	CustomizeablePowers\IceArmor\IceArmorBits.part
		part3	CustomizeablePowers\IceArmor\IceArmorBitsSmall.part

	End

	Event
		Type	Local
		At	ULEGL
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceArmor_ULEGL
		part1	CustomizeablePowers\IceArmor\Snowfall.part
		part2	CustomizeablePowers\IceArmor\IceArmorBits.part
		part3	CustomizeablePowers\IceArmor\IceArmorBitsSmall.part

	End
End
#################################################################


Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	LLEGL
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceArmor_LLEGL
		part1	CustomizeablePowers\IceArmor\Snowfall.part
		part2	CustomizeablePowers\IceArmor\IceArmorLegBits.part
		part3	CustomizeablePowers\IceArmor\IceArmorLegBitsSmall.part
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceArmor_LLEGR
		part1	CustomizeablePowers\IceArmor\Snowfall.part
		part2	CustomizeablePowers\IceArmor\IceArmorLegBits.part
		part3	CustomizeablePowers\IceArmor\IceArmorLegBitsSmall.part
	End

	Event
		Type	Local
		At	FOOTL
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		part1	CustomizeablePowers\IceArmor\IceArmorLegBits.part
		part2	CustomizeablePowers\IceArmor\IceArmorLegBitsSmall.part

	End

	Event
		Type	Local
		At	FOOTR
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		part1	CustomizeablePowers\IceArmor\IceArmorLegBits.part
		part2	CustomizeablePowers\IceArmor\IceArmorLegBitsSmall.part

	End
End

###################################################################


Condition
	On 	Time
	Time 	10

	Event
		EName   Wind
		Type	start
		At	Root
		Bhvr	CustomizeablePowers\IceArmor\IceArmorWind.bhvr
		BhvrOverride
			Alpha		40
			TintGeom	1
		End
		geom	Tintable_FX_TmpCtrlWisp01
		part1	CustomizeablePowers\IceArmor\IceMistUpwards.part
		part2	CustomizeablePowers\IceArmor\SnowMistUpwards.part
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
		BhvrOverride
			Alpha		40
			TintGeom	1
		End
		geom	Tintable_FX_TmpCtrlWisp01
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
		BhvrOverride
			Alpha		40
			TintGeom	1
		End
		geom	Tintable_FX_TmpCtrlWisp01

	End
End

Condition
	On	Time
	Time    36
	Event
		EName 	Wind
		Type	Destroy
	End
End

Condition
	On	Time
	Time    39
	Event
		EName 	WindFast
		Type	Destroy
	End
End

Condition
	On	Time
	Time   44
	Event
		EName 	WindFirst
		Type	Destroy
	End

End

End