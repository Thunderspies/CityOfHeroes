#############################################################
## Cold Domination - Ice Shield (continuing)
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

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
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	250
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	UARML
		Bhvr 	V_COV\Powers\ColdDomination\icearmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceArmorSpikes_UARML
	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	V_COV\Powers\ColdDomination\icearmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceArmorSpikes_UARMR
	End

	Event
		Type	Local
		At	ULegR
		Bhvr 	V_COV\Powers\ColdDomination\icearmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceArmorSpikes_ULegR
		geom	Tintable_FX_IceArmorSpikes2_ULegR
	End

	Event
		Type	Local
		At	LLegR
		Bhvr 	V_COV\Powers\ColdDomination\icearmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceArmorSpikes_LLegR
		geom	Tintable_FX_IceArmorSpikes2_LLegR
	End

	Event
		Type	Local
		At	ULegL
		Bhvr 	V_COV\Powers\ColdDomination\icearmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceArmorSpikes_ULegL
		geom	Tintable_FX_IceArmorSpikes2_ULegL
	End

	Event
		Type	Local
		At	LLegL
		Bhvr 	V_COV\Powers\ColdDomination\icearmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceArmorSpikes_LLegL
		geom	Tintable_FX_IceArmorSpikes2_LLegL
	End
End

#############################################################

End