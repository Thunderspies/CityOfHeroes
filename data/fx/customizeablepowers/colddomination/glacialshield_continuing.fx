#############################################################
## Cold Domination - Glacial Shield (continuing)
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0
	
	Event
		Type	Local
		At 	Origin
		Sound Fast2 70 70 .7
	End
End

#############################################################

Condition	
	On	Time
	Time	0

	Event	
		Type	Local
		At	Col_R
		Bhvr	CustomizeablePowers\ColdDomination\icearmor.bhvr
		geom	FX_IceArmor_Icecicle1
		geom	FX_IceArmor_Icecicle2
		geom	FX_IceArmor_Icecicle3
	End

	Event	
		Type	Local
		At	Col_L
		Bhvr	CustomizeablePowers\ColdDomination\icearmor.bhvr
		geom	FX_IceArmor_Icecicle4
		geom	FX_IceArmor_Icecicle5
		geom	FX_IceArmor_Icecicle6
	End

	Event	
		Type	Local
		At	ULegL
		Bhvr	CustomizeablePowers\ColdDomination\GlacialArmor.bhvr
		geom	IceArmorSpikeUlegL
		geom	IceArmorSpikeUlegL2
	End

	Event	
		Type	Local
		At	ULegR
		Bhvr	CustomizeablePowers\ColdDomination\GlacialArmor.bhvr
		geom	IceArmorSpikeUlegR
		geom	IceArmorSpikeUlegR2
	End

	Event	
		Type	Local
		At	LLegL
		Bhvr	CustomizeablePowers\ColdDomination\GlacialArmor.bhvr
		geom	IceArmorSpikeUlegL
		geom	IceArmorSpikeUlegL2
	End

	Event	
		Type	Local
		At	LLegR
		Bhvr	CustomizeablePowers\ColdDomination\GlacialArmor.bhvr
		geom	IceArmorSpikeUlegR
		geom	IceArmorSpikeUlegR2
	End
End

###################################################################

Condition
	On 	Time
	Time 	25
	
	Event
		Type	Local 
		At	Chest
		part3	CustomizeablePowers\ColdDomination\GlacialArmor_Rays1.part
		part1	CustomizeablePowers\ColdDomination\bodychillbig.part
		part2	CustomizeablePowers\ColdDomination\HeadLegschillbig.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	35
	
	Event
		Type	Local 
		At	LARML
		part1	CustomizeablePowers\ColdDomination\bodychillbig.part
	End

	Event
		Type	Local 
		At	LARMR
		Bhvr	CustomizeablePowers\ColdDomination\icearmor.bhvr
		part1	CustomizeablePowers\ColdDomination\bodychillbig.part

	End
End

#############################################################

End		