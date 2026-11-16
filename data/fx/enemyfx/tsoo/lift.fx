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
 

Flags    InheritAlpha


Lifespan 100



#########################################################
#####################  CLOUD 1  #########################
#########################################################

Condition
	On 	Time
	Time	0

	Event	
		Type	Local
		At 	Origin
		Sound SS04 120 120 .9
	End
End





Condition

	On 	Time	
	Time	0
	
	Event	
		ENAME	Lift
		Type	Start
		At	Root
		part	:LiftMist01.part
		part	:LiftStreaks01.part
		part	:LiftChunks01.part
		#Bhvr	Behaviors/Spin2.bhvr
		LifeSpan 60
	End

End



End		