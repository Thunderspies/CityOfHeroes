#########################################################
##	template

FxInfo

Input  
	InpName	Root
End

Input  
	InpName	Hair
End

Input  
	InpName	Eyes
End

Input  
	InpName	Neck
End

Input  
	InpName	Chest
End

Input  
	InpName	SpadR
End

Input  
	InpName	SpadL
End

Input  
	InpName	UarmR
End

Input  
	InpName	UarmL
End

Input  
	InpName	LarmR
End

Input  
	InpName	LarmL
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

Input  
	InpName	Hips
End

Input  
	InpName	UlegR
End

Input  
	InpName	UlegL
End

Input  
	InpName	LlegR
End

Input  
	InpName	LlegL
End

Input  
	InpName	FootR
End

Input  
	InpName	FootL
End

Input  
	InpName	ToeL
End

Input  
	InpName	ToeR
End

Input
	InpName Origin
End


#LifeSpan 30


#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		#Sound	rumble1 80 30 .8
	End
End


Condition
	On	Time
	Time	10
	

	Event
		Type	Local
		At 	Origin
		#Sound	invisible1 80 30 .5
	End
End

Condition
	On	Time
	Time	10
	

	Event
		Type	Local
		At 	Origin
		Sound glow8_loop 40 40 .5
		bhvr	behaviors/Sound/SoundIn2Out10.bhvr
		lifespan 4
	End
End
#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	10
	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	CustomizeablePowers\EnergyAura\CloakSuitGlow02.part
		#part	:CloakSuitGlowBlue.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	CustomizeablePowers\EnergyAura\CloakSuitGlow02.part
		#part	:CloakSuitGlowBlue.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

End

Condition
	On	Time
	Time	15

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	CustomizeablePowers\EnergyAura\CloakSuitGlow02.part
		#part	:CloakSuitGlowBlue.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	CustomizeablePowers\EnergyAura\CloakSuitGlow02.part
		#part	:CloakSuitGlowBlue.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	CustomizeablePowers\EnergyAura\CloakSuitGlow02.part
		#part	:CloakSuitGlowBlue.part
		POther Neck
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
	
	End

End

Condition
	On	Time
	Time	18

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	CustomizeablePowers\EnergyAura\CloakSuitGlow02.part
		#part	:CloakSuitGlowBlue.part
		POther Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

End
	
Condition
	On	Time
	Time	5

	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	CustomizeablePowers\EnergyAura\CloakSuitGlow02.part
		#part	:CloakSuitGlowBlue.part
		POther Ulegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	CustomizeablePowers\EnergyAura\CloakSuitGlow02.part
		#part	:CloakSuitGlowBlue.part
		POther UlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

End

Condition
	On	Time
	Time	0

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	CustomizeablePowers\EnergyAura\CloakSuitGlow01a.part
		#part	:CloakSuitGlowBlue.part
		POther Llegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 23
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	CustomizeablePowers\EnergyAura\CloakSuitGlow01a.part
		#part	:CloakSuitGlowBlue.part
		POther LlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 23
		
	End


End


End		