#########################################################
##	template

FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	Root
End

Input  
	InpName	Chest
End

Input  
	InpName	WepL
End

Input  
	InpName	WepR
End

Input  
	InpName	LlegL
End

Input  
	InpName	LlegR
End

Input  
	InpName	FootL
End

Input  
	InpName	FootR
End

Lifespan 200



#########################################################
#####################  Mist 1  #########################
#########################################################

Condition
	On 	Time
	Time	0
	
	Event	
		Type	Local
		At 	Origin
		Sound SSthunder 120 120 1.0

	End
End

Condition

	On 	Time	
	Time	17

	Event	
		ENAME	HandGlow
		Type	Posit
		At	WepL
		part	:SSBodyMistFast01.part
		LifeSpan 40	
	End

	Event	
		ENAME	HandGlow
		Type	Posit
		At	WepR
		part	:SSBodyMistFast01.part
		LifeSpan 40	
	End

End


Condition

	On 	Time	
	Time	40

	Event	
		ENAME	HandGlow
		Type	Local
		At	WepL
		part	:SSTClapHandGlow.part
		#Bhvr	Behaviors/SSThunderClapShake.bhvr
		LifeSpan 30	
	End

	Event	
		ENAME	HandGlow
		Type	Local
		At	WepR
		part	:SSTClapHandGlow.part
		#Bhvr	Behaviors/SSThunderClapShake.bhvr
		LifeSpan 30	
	End

End


Condition

	On 	Time	
	Time	54

	Event	
		ENAME	ThunderShake1
		Type	Local
		At	WepL
		part	:SSTClapHandLightning.part
		part	:SSTClapHandFlashGlow.part
		#Bhvr	Behaviors/SSThunderClapShake.bhvr
		LifeSpan 10	
	End

End

Condition

	On 	Time	
	Time	64

	Event	
		ENAME	Overhead
		Type	Local
		At	Root
		Geom	OverHeadDummy
		LifeSpan 15	
	End

	Event	
		ENAME	Flash
		Type	Local
		At	Overhead
		AltPiv	1
		part	:SSTClapFlash.part
		LifeSpan 12	
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Overhead
		AltPiv	1
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		450
			PhysForcePowerJitter	150
		End

		Lifespan	1

	End
End

Condition

	On 	Time	
	Time	67

	Event	
		ENAME	ThunderShake1
		Type	Local
		At	Chest
		Bhvr	Behaviors/SSThunderClapShake.bhvr
		#LifeSpan 70
		
		
	End

End


End



	