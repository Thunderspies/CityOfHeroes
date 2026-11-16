#############################################################
## Cold Domination - Glacial Shield (continuing)
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At 	Origin
		Sound Fast2 70 70 0.7
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Col_R
		Bhvr 	Powers\ColdControl\icearmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceArmor_Icecicle1
		geom	Tintable_FX_IceArmor_Icecicle2
		geom	Tintable_FX_IceArmor_Icecicle3
	End

	Event
		Type	Local
		At	Col_L
		Bhvr 	CustomizeablePowers\ColdDomination\GlacialArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceArmor_Icecicle4
		geom	Tintable_FX_IceArmor_Icecicle5
		geom	Tintable_FX_IceArmor_Icecicle6
	End

	Event
		Type	Local
		At	ULegL
		Bhvr 	CustomizeablePowers\ColdDomination\GlacialArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_IceArmorSpikeUlegL
		geom	Tintable_IceArmorSpikeUlegL2
	End

	Event
		Type	Local
		At	ULegR
		Bhvr 	CustomizeablePowers\ColdDomination\GlacialArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_IceArmorSpikeUlegR
		geom	Tintable_IceArmorSpikeUlegR2
	End

	Event
		Type	Local
		At	LLegL
		Bhvr 	CustomizeablePowers\ColdDomination\GlacialArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_IceArmorSpikeUlegL
		geom	Tintable_IceArmorSpikeUlegL2
	End

	Event
		Type	Local
		At	LLegR
		Bhvr 	CustomizeablePowers\ColdDomination\GlacialArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_IceArmorSpikeUlegR
		geom	Tintable_IceArmorSpikeUlegR2
	End
End

###################################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At	Chest
		part3	:GlacialArmor_Rays1.part
		part1	:bodychillbig.part
		part2   :HeadLegschillbig.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	35

	Event
		Type	Local
		At	LARML
		part1	:bodychillbig.part
	End

	Event
		Type	Local
		At	LARMR
		Bhvr 	behaviors\powers\coldcontrol\icearmor.bhvr
		part1	:bodychillbig.part

	End
End

#############################################################

End