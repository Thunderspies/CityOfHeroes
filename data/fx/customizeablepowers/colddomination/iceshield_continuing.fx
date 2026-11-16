#############################################################
## Cold Domination - Ice Shield (continuing)
#############################################################

FxInfo

#############################################################

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
		Bhvr	CustomizeablePowers\ColdDomination\icearmor.bhvr
		geom	FX_IceArmorSpikes_UARML
	End
	
	Event	
		Type	Local
		At	UARMR
		Bhvr	CustomizeablePowers\ColdDomination\icearmor.bhvr
		geom	FX_IceArmorSpikes_UARMR
	End
	
	Event	
		Type	Local
		At	ULegR
		Bhvr	CustomizeablePowers\ColdDomination\icearmor.bhvr
		geom	FX_IceArmorSpikes_ULegR
		geom	FX_IceArmorSpikes2_ULegR
	End

	Event	
		Type	Local
		At	LLegR
		Bhvr	CustomizeablePowers\ColdDomination\icearmor.bhvr
		geom	FX_IceArmorSpikes_LLegR
		geom	FX_IceArmorSpikes2_LLegR
	End
	
	Event	
		Type	Local
		At	ULegL
		Bhvr	CustomizeablePowers\ColdDomination\icearmor.bhvr
		geom	FX_IceArmorSpikes_ULegL
		geom	FX_IceArmorSpikes2_ULegL
	End

	Event	
		Type	Local
		At	LLegL
		Bhvr	CustomizeablePowers\ColdDomination\icearmor.bhvr
		geom	FX_IceArmorSpikes_LLegL
		geom	FX_IceArmorSpikes2_LLegL
	End
End

#############################################################

End