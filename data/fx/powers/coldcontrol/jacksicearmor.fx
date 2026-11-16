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
		Part	POWERS\ColdControl\IceMistUpwards.part
		Part	POWERS\ColdControl\SnowMistUpwards.part
		geom	FX_TmpCtrlWisp01
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
		geom	FX_TmpCtrlWisp01
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
		geom	FX_TmpCtrlWisp01
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
		Part	POWERS\ColdControl\Snowfall.part
		Part	POWERS\ColdControl\IceArmorLegBits.part
		Part	POWERS\ColdControl\IceArmorLegBitsSmall.part
		geom	FX_IceArmor_LLEGL
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr 	behaviors\powers\coldcontrol\icearmor.bhvr
		Part	POWERS\ColdControl\Snowfall.part
		Part	POWERS\ColdControl\IceArmorLegBits.part
		Part	POWERS\ColdControl\IceArmorLegBitsSmall.part
		geom	FX_IceArmor_LLEGR
	End

	Event
		Type	Local
		At	FOOTL
		Bhvr 	behaviors\powers\coldcontrol\icearmor.bhvr
		Part	POWERS\ColdControl\IceArmorLegBits.part
		Part	POWERS\ColdControl\IceArmorLegBitsSmall.part
	End

	Event
		Type	Local
		At	FOOTR
		Bhvr 	behaviors\powers\coldcontrol\icearmor.bhvr
		Part	POWERS\ColdControl\IceArmorLegBits.part
		Part	POWERS\ColdControl\IceArmorLegBitsSmall.part
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Type	Local
		At	UlegR
		Bhvr 	behaviors\powers\coldcontrol\icearmor.bhvr
		Part	POWERS\ColdControl\Snowfall.part
		Part	POWERS\ColdControl\IceArmorBits.part
		Part	POWERS\ColdControl\IceArmorBitsSmall.part
		geom	FX_IceArmor_ULEGR
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr 	behaviors\powers\coldcontrol\icearmor.bhvr
		Part	POWERS\ColdControl\Snowfall.part
		Part	POWERS\ColdControl\IceArmorBits.part
		Part	POWERS\ColdControl\IceArmorBitsSmall.part
		geom	FX_IceArmor_ULEGL
	End

	Event
		Type	Local
		At	WepL
		Part	POWERS\ColdControl\Snowfall.part
		Part	POWERS\ColdControl\IceArmorBits.part
		Part	POWERS\ColdControl\IceArmorBitsSmall.part
	End

	Event
		Type	Local
		At	WepR
		Part	POWERS\ColdControl\Snowfall.part
		Part	POWERS\ColdControl\IceArmorBits.part
		Part	POWERS\ColdControl\IceArmorBitsSmall.part
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
		geom	FX_IceArmor_HIPS
	End

	Event
		Type	Local
		At	LARMR
		Bhvr 	behaviors\powers\coldcontrol\icearmor.bhvr
		Part	POWERS\ColdControl\IceArmorBits.part
		Part	POWERS\ColdControl\IceArmorBitsSmall.part
		geom	FX_IceArmor_LARMR
	End

	Event
		Type	Local
		At	LARML
		Bhvr 	behaviors\powers\coldcontrol\icearmor.bhvr
		Part	POWERS\ColdControl\IceArmorBits.part
		Part	POWERS\ColdControl\IceArmorBitsSmall.part
		geom	FX_IceArmor_LARML
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
		Part	POWERS\ColdControl\IceArmorBitsB.part
		Part	POWERS\ColdControl\IceArmorBitsSmallB.part
		geom	FX_IceArmor_UARML
	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	behaviors\powers\coldcontrol\icearmorLowerAlpha.bhvr
		Part	POWERS\ColdControl\IceArmorBitsB.part
		Part	POWERS\ColdControl\IceArmorBitsSmallB.part
		geom	FX_IceArmor_UARMR
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
		geom	FX_IceArmor_CHEST
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
		Part	POWERS\ColdControl\IceArmorBits.part
		Part	POWERS\ColdControl\IceArmorBitsSmall.part
		Part	POWERS\ColdControl\Snowfall.part
		geom	FX_IceArmor_HEAD
	End
End

#############################################################

End