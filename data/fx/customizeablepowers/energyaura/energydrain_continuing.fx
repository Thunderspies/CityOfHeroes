#########################################################
##	
##
FxInfo

Input  
	InpName	Root
End

Input  
	InpName	HIPS
End

Input  
	InpName	CHEST
End

Input  
	InpName	HEAD
End

Input  
	InpName	UARMR
End

Input  
	InpName	UARML
End

Input  
	InpName	LARML
End

Input  
	InpName	LARMR
End

Input  
	InpName	ULEGL
End

Input  
	InpName	ULEGR
End

Input  
	InpName	LLEGL
End

Input  
	InpName	LLEGR
End

Input  
	InpName	WepL
End

Input  
	InpName	WepR
End

Input  
	InpName	FootL
End

Input  
	InpName	FootR
End

Input  
	InpName	Origin
End
##########################################################

Condition
	On 	Time
	Time 	35

	Event
		Type	Local 
		At	Head
		Bhvr	CustomizeablePowers\EnergyAura\icearmorTransparrent.bhvr
		geom	FX_IceArmor_HEAD
		part2	CustomizeablePowers\EnergyAura\AbsorptionBits.part
		part3	CustomizeablePowers\EnergyAura\AbsorptionBitsSmall.part
	End
	
End

Condition
	On 	Time
	Time 	30


	Event
		Type	Local 
		At	CHEST
		Bhvr	CustomizeablePowers\EnergyAura\icearmorTransparrent.bhvr
		geom	FX_IceArmor_CHEST
		
	End
End

##############################################

Condition
	On 	Time
	Time 	25


		Event
		Type	Local 
		At	Hips
		Bhvr	CustomizeablePowers\EnergyAura\icearmorTransparrent.bhvr
		geom	FX_IceArmor_HIPS
		
	End


	Event
		Type	Local 
		At	UARML
		Bhvr	CustomizeablePowers\EnergyAura\icearmorTransparrent.bhvr
		geom	FX_IceArmor_UARML
		part2	CustomizeablePowers\EnergyAura\AbsorptionBits.part
		part3	CustomizeablePowers\EnergyAura\AbsorptionBitsSmall.part
	End

	Event
		Type	Local 
		At	UARMR
		Bhvr	CustomizeablePowers\EnergyAura\icearmorTransparrent.bhvr
		geom	FX_IceArmor_UARMR
		part2	CustomizeablePowers\EnergyAura\AbsorptionBits.part
		part3	CustomizeablePowers\EnergyAura\AbsorptionBitsSmall.part
	End
End


####################################################

Condition
	On 	Time
	Time 	20

		Event
		Type	Local 
		At	LARMR
		Bhvr	CustomizeablePowers\EnergyAura\icearmorTransparrent.bhvr
		geom	FX_IceArmor_LARMR
		part2	CustomizeablePowers\EnergyAura\AbsorptionBits.part
		part3	CustomizeablePowers\EnergyAura\AbsorptionBitsSmall.part
	End

		Event
		Type	Local 
		At	LARML
		Bhvr	CustomizeablePowers\EnergyAura\icearmorTransparrent.bhvr
		geom	FX_IceArmor_LARML
		part2	CustomizeablePowers\EnergyAura\AbsorptionBits.part
		part3	CustomizeablePowers\EnergyAura\AbsorptionBitsSmall.part
	End

End
###################################################################
Condition
	On 	Time
	Time 	15

	Event
		Type	Local 
		At	UlegR
		Bhvr	CustomizeablePowers\EnergyAura\icearmorTransparrent.bhvr
		geom	FX_IceArmor_ULEGR
		part2	CustomizeablePowers\EnergyAura\AbsorptionBits.part
		part3	CustomizeablePowers\EnergyAura\AbsorptionBitsSmall.part
	End

	Event
		Type	Local 
		At	ULEGL
		Bhvr	CustomizeablePowers\EnergyAura\icearmorTransparrent.bhvr
		geom	FX_IceArmor_ULEGL
		part2	CustomizeablePowers\EnergyAura\AbsorptionBits.part
		part3	CustomizeablePowers\EnergyAura\AbsorptionBitsSmall.part
	End

	Event
		Type	Local 
		At	WepL
		part2	CustomizeablePowers\EnergyAura\AbsorptionBits.part
		part3	CustomizeablePowers\EnergyAura\AbsorptionBitsSmall.part
	End

		Event
		Type	Local 
		At	WepR
		part2	CustomizeablePowers\EnergyAura\AbsorptionBits.part
		part3	CustomizeablePowers\EnergyAura\AbsorptionBitsSmall.part

	End

End
#################################################################
##Shines
#################################################################

Condition
	On 	Cycle
	Time 	30
	Chance	.2	

	Event
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Type	Local 
		At	LLEGL
		part1	CustomizeablePowers\EnergyAura\FortitudeRaysSmall.part
		part2	CustomizeablePowers\EnergyAura\FortitudeRing.part
		part3	CustomizeablePowers\EnergyAura\FortitudeRing.part
		part4	CustomizeablePowers\EnergyAura\FortitudeDots.part
		LifeSpan	33
	
	End

End

Condition
	On 	Cycle
	Time 	33
	Chance	.2

	Event
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Type	Local 
		At	LLEGR
		part1	CustomizeablePowers\EnergyAura\FortitudeRaysSmall.part
		part2	CustomizeablePowers\EnergyAura\FortitudeRing.part
		part3	CustomizeablePowers\EnergyAura\FortitudeRing.part
		part4	CustomizeablePowers\EnergyAura\FortitudeDots.part
		LifeSpan	33
	
	End
End

Condition
	On 	Cycle
	Time 	35
	Chance	.2

	Event
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Type	Local 
		At	UArmL
		part1	CustomizeablePowers\EnergyAura\FortitudeRaysSmall.part
		part2	CustomizeablePowers\EnergyAura\FortitudeRing.part
		part3	CustomizeablePowers\EnergyAura\FortitudeRing.part
		part4	CustomizeablePowers\EnergyAura\FortitudeDots.part
		LifeSpan	33
	
	End
End

Condition
	On 	Cycle
	Time 	27
	Chance	.2

	Event
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Type	Local 
		At	UArmR
		part1	CustomizeablePowers\EnergyAura\FortitudeRaysSmall.part
		part2	CustomizeablePowers\EnergyAura\FortitudeRing.part
		part3	CustomizeablePowers\EnergyAura\FortitudeRing.part
		part4	CustomizeablePowers\EnergyAura\FortitudeDots.part
		LifeSpan	33
	
	End

End

#################################################################


Condition

	On 	Time
	Time 	15

	Event
		Type	Local 
		At	Origin
		SOund	coldblast2 80 30 .8
	End

End

Condition

	On 	Time
	Time 	9

	Event
		Type	Local 
		At	Origin
		SOund	IcePunch1 80 30 .9
	End

End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local 
		At	LLEGL
		Bhvr	CustomizeablePowers\EnergyAura\icearmorTransparrent.bhvr
		geom	FX_IceArmor_LLEGL
		part2	CustomizeablePowers\EnergyAura\AbsorptionLegBits.part
		part3	CustomizeablePowers\EnergyAura\AbsorptionLegBitsSmall.part
		Sound IceArmor_loop 80 80 .66
	
	End

	Event
		Type	Local 
		At	LLEGR
		Bhvr	CustomizeablePowers\EnergyAura\icearmorTransparrent.bhvr
		geom	FX_IceArmor_LLEGR
		part2	CustomizeablePowers\EnergyAura\AbsorptionLegBits.part
		part3	CustomizeablePowers\EnergyAura\AbsorptionLegBitsSmall.part
	End
	
	Event
		Type	Local 
		At	FootL
		part2	CustomizeablePowers\EnergyAura\AbsorptionLegBits.part
		part3	CustomizeablePowers\EnergyAura\AbsorptionLegBitsSmall.part
	
	End

	Event
		Type	Local 
		At	FootR
		
		part2	CustomizeablePowers\EnergyAura\AbsorptionLegBits.part
		part3	CustomizeablePowers\EnergyAura\AbsorptionLegBitsSmall.part
	End

	Event
		EName   Wind
		Type	start 
		At	Root
		Bhvr	CustomizeablePowers\EnergyAura\IceArmorWind.bhvr
		geom	FX_TmpCtrlWisp01
		part1	CustomizeablePowers\EnergyAura\IceMistUpwards.part
		part2	CustomizeablePowers\EnergyAura\SnowMistUpwards.part
		
	End
End

Condition
	On 	Time
	Time 	15
	Event
		EName   WindFast
		Type	start 
		At	Root
		Bhvr	CustomizeablePowers\EnergyAura\IceArmorWindFast.bhvr
		geom	FX_TmpCtrlWisp01
	End
End

Condition
	On 	Time
	Time 	20
	Event
		EName   WindFirst
		Type	start 
		At	Root
		Bhvr	CustomizeablePowers\EnergyAura\IceArmorWindFirst.bhvr
		geom	FX_TmpCtrlWisp01
		
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