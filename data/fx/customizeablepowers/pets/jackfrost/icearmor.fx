#############################################################
## JacksIceArmor.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time	5

	Event
		Type	Local
		At 	Origin
		Sound cold_loop 80 80 0.5
	End
End

Condition
	On 	Time
	Time 	10

	Event
		EName   Wind
		Type	start
		At	Root
		Bhvr 	powers\coldcontrol\IceArmorWind.bhvr
		Part	CustomizeablePowers\IceArmor\IceMistUpwards.part
		Part	CustomizeablePowers\IceArmor\SnowMistUpwards.part
		BhvrOverride
			Alpha		20
			TintGeom 	1
		End
		geom	Tintable_FX_TmpCtrlWisp01
		Sound blizzard 80 80 0.65
		Lifespan 28
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	start
		At	Root
		Bhvr 	powers\coldcontrol\IceArmorWindFast.bhvr
		BhvrOverride
			Alpha		20
			TintGeom 	1
		End
		geom	Tintable_FX_TmpCtrlWisp01
		Lifespan 25
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Type	start
		At	Root
		Bhvr 	powers\coldcontrol\IceArmorWindFirst.bhvr
		BhvrOverride
			Alpha		20
			TintGeom 	1
		End
		geom	Tintable_FX_TmpCtrlWisp01
		Lifespan 25
	End
End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	LLEGL
		Bhvr 	behaviors\powers\coldcontrol\icearmor.bhvr
		Part	CustomizeablePowers\IceArmor\Snowfall.part
		Part	:IceArmorLegBits.part
		Part	:IceArmorLegBitsSmall.part
		BhvrOverride
			TintGeom 	1
		End
		geom	Tintable_FX_IceArmor_LLEGL
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr 	behaviors\powers\coldcontrol\icearmor.bhvr
		Part	CustomizeablePowers\IceArmor\Snowfall.part
		Part	:IceArmorLegBits.part
		Part	:IceArmorLegBitsSmall.part
		BhvrOverride
			TintGeom 	1
		End
		geom	Tintable_FX_IceArmor_LLEGR
	End

	Event
		Type	Local
		At	FOOTL
		Bhvr 	behaviors\powers\coldcontrol\icearmor.bhvr
		Part	:IceArmorLegBits.part
		Part	:IceArmorLegBitsSmall.part
	End

	Event
		Type	Local
		At	FOOTR
		Bhvr 	behaviors\powers\coldcontrol\icearmor.bhvr
		Part	:IceArmorLegBits.part
		Part	:IceArmorLegBitsSmall.part
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Type	Local
		At	UlegR
		Bhvr 	behaviors\powers\coldcontrol\icearmor.bhvr
		Part	CustomizeablePowers\IceArmor\Snowfall.part
		Part	:IceArmorBits.part
		Part	:IceArmorBitsSmall.part
		BhvrOverride
			TintGeom 	1
		End
		geom	Tintable_FX_IceArmor_ULEGR
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr 	behaviors\powers\coldcontrol\icearmor.bhvr
		Part	CustomizeablePowers\IceArmor\Snowfall.part
		Part	:IceArmorBits.part
		Part	:IceArmorBitsSmall.part
		BhvrOverride
			TintGeom 	1
		End
		geom	Tintable_FX_IceArmor_ULEGL
	End

	Event
		Type	Local
		At	WepL
		Part	CustomizeablePowers\IceArmor\Snowfall.part
		Part	:IceArmorBits.part
		Part	:IceArmorBitsSmall.part
	End

	Event
		Type	Local
		At	WepR
		Part	CustomizeablePowers\IceArmor\Snowfall.part
		Part	:IceArmorBits.part
		Part	:IceArmorBitsSmall.part
	End
End

####################################################

Condition
	On 	Time
	Time 	20

	Event
		Type	Local
		At	Hips
		Bhvr 	behaviors\powers\coldcontrol\icearmorLowerAlpha.bhvr
		BhvrOverride
			TintGeom 	1
		End
		geom	Tintable_FX_IceArmor_HIPS
	End

	Event
		Type	Local
		At	LARMR
		Bhvr 	behaviors\powers\coldcontrol\icearmor.bhvr
		Part	:IceArmorBits.part
		Part	:IceArmorBitsSmall.part
		BhvrOverride
			TintGeom 	1
		End
		geom	Tintable_FX_IceArmor_LARMR
	End

	Event
		Type	Local
		At	LARML
		Bhvr 	behaviors\powers\coldcontrol\icearmor.bhvr
		Part	:IceArmorBits.part
		Part	:IceArmorBitsSmall.part
		BhvrOverride
			TintGeom 	1
		End
		geom	Tintable_FX_IceArmor_LARML
	End
End

#############################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At	UARML
		Bhvr 	behaviors\powers\coldcontrol\icearmorLowerAlpha.bhvr
		Part	:IceArmorBitsB.part
		Part	:IceArmorBitsSmallB.part
		BhvrOverride
			TintGeom 	1
		End
		geom	Tintable_FX_IceArmor_UARML
	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	behaviors\powers\coldcontrol\icearmorLowerAlpha.bhvr
		Part	:IceArmorBitsB.part
		Part	:IceArmorBitsSmallB.part
		BhvrOverride
			TintGeom 	1
		End
		geom	Tintable_FX_IceArmor_UARMR
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	CHEST
		Bhvr 	behaviors\powers\coldcontrol\icearmorLowerAlpha.bhvr
		BhvrOverride
			TintGeom 	1
		End
		geom	Tintable_FX_IceArmor_CHEST
	End
End

#############################################################

Condition
	On 	Time
	Time 	35

	Event
		Type	Local
		At	Head
		Bhvr 	behaviors\powers\coldcontrol\icearmor.bhvr
		Part	:IceArmorBits.part
		Part	:IceArmorBitsSmall.part
		Part	CustomizeablePowers\IceArmor\Snowfall.part
		BhvrOverride
			TintGeom 	1
		End
		geom	Tintable_FX_IceArmor_HEAD
	End
End

#############################################################

End