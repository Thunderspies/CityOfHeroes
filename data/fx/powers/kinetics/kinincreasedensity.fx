#########################################################
##	template

FxInfo

Input
	InpName Origin
End

Input  
	InpName	Target
End

Input  
	InpName	Hair
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




Lifespan 300

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	15
	

	Event
		Type	Local
		At 	Origin
		Sound kinetic2 80 80 .8
	End
End


#########################################################
####################     CAST  1   ######################
#########################################################

Condition
	On	Time
	Time	15

	Event	
		ENAME	WepLGlow
		Type	Local
		At	WepL
		part	:KineticHandGlows01.part
		Lifespan 35
		
	End

End

Condition
	On	Time
	Time	10

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:KineticBodyGlows01.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		POther  LarmL
		part	:KineticBodyGlows01.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40
		
	End

End

#########################################################
####################     CAST  2   ######################
#########################################################

Condition
	On	Time
	Time	15


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	Target
		part	:KineticIDBodySwirls01.part
		PMagnet LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	LarmL
		part	:KineticIDBodySwirls01.part
		PMagnet Target
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40
		
	End

End

#########################################################
####################   Flyaways   #######################
#########################################################


Condition
	On	Time
	Time	10

	Event	
		ENAME	FlyawayNode
		Type	Local
		At	WepL
		Bhvr	:KineticScaleSmall.bhvr
		LifeSpan 50
	End

	Event	
		ENAME	Flyaways1
		Type	Local
		At	FlyawayNode
		part	:KineticSwirlFlyAway03.part
		Bhvr	:KineticSpin3.bhvr
		LifeSpan 15
	End

End

Condition
	On	Time
	Time	15

	Event	
		ENAME	Flyaways2
		Type	Local
		At	FlyawayNode
		part	:KineticSwirlFlyAway03.part
		Bhvr	:KineticSpin3.bhvr
		LifeSpan 15
	End

End

Condition
	On	Time
	Time	20

	Event	
		ENAME	Flyaways3
		Type	Local
		At	FlyawayNode
		part	:KineticSwirlFlyAway03.part
		Bhvr	:KineticSpin3.bhvr
		LifeSpan 15
	End

End

End