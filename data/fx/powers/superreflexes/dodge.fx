#########################################################
##	template

FxInfo

Input  
	InpName	Root
End

Input  
	InpName	Head
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

#Lifespan 260



#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition

	On 	Time	
	Time	0

	Event
		Type Local
		At origin
		Sound teletrans4 80 80 .88
	End
End

Condition
	On	Time
	Time	10
	
	
	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:SwirlDistortionOut.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
	
	End

	Event	
		ENAME	ChestNode
		Type	Local
		At	Chest
		part	:DodgeStreaks.part
		part1	:SwirlDistortionOut.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	:SwirlDistortionOut.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
		
	End
	
	Event	
		ENAME	LarmRNode
		Type	Local
		At	LarmR
		
		part1	:SwirlDistortionOut.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
	End

	Event	
		ENAME	LarmLNode
		Type	Local
		At	LarmL
	
		part1	:SwirlDistortionOut.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
	End
	
	Event	
		ENAME	LarmRNode
		Type	Local
		At	UarmR
		
		part1	:SwirlDistortionOut.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
	End

	Event	
		ENAME	LarmLNode
		Type	Local
		At	UarmL
	
		part1	:SwirlDistortionOut.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
	End

	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:SwirlDistortionOut.part
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		
	End

	Event	
		ENAME	rightLarmGlow
		Type	Local
		At	WepR
		part1	:SwirlDistortionOut.part
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
	End

#######################################################################
##Legs
############################################################

	Event	
		ENAME	LarmRNode
		Type	Local
		At	LLegR
		
		part1	:SwirlDistortionOutKnees.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
	End

	Event	
		ENAME	LarmLNode
		Type	Local
		At	LLegL
	
		part1	:SwirlDistortionOutKnees.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
	End
	
	Event	
		ENAME	LarmRNode
		Type	Local
		At	ULegR
		
		part1	:SwirlDistortionOut.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
	End

	Event	
		ENAME	LarmLNode
		Type	Local
		At	ULegL
	
		part1	:SwirlDistortionOut.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
	End

	Event	
		ENAME	LarmRNode
		Type	Local
		At	FootR
		
		part1	:SwirlDistortionOut.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
	End

	Event	
		ENAME	LarmLNode
		Type	Local
		At	FootL
	
		part1	:SwirlDistortionOut.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
	End

End
	
End		