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
	Time	20

	Event	
		ENAME	WepLGlow
		Type	Local
		At	WepR
		part	:KineticHandGlowsCyan01.part
		Lifespan 10
		
	End

End

Condition
	On	Time
	Time	15

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:KineticBodyGlowsCyan01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 20
		
	End
	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		POther  LarmR
		part	:KineticBodyGlowsCyan01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 20
		
	End

End

#########################################################
####################     CAST  2   ######################
#########################################################

Condition
	On	Time
	Time	15


	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	Target
		part	:KineticBodySwirlsCyan01.part
		PMagnet LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10
		
	End
	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	LarmR
		part	:KineticBodySwirlsCyan01.part
		PMagnet Target
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10
		
	End

End

#########################################################
####################   Flyaways   #######################
#########################################################


Condition
	On	Time
	Time	5

	Event	
		ENAME	FlyawayNode
		Type	Local
		At	WepR
		Bhvr	:KineticScaleSmall.bhvr
		LifeSpan 50
	End

	Event	
		ENAME	Flyaways1
		Type	Local
		At	FlyawayNode
		part	:KineticSwirlFlyAway03.part
		Bhvr	:KineticSpin3.bhvr
		LifeSpan 5
	End

End

Condition
	On	Time
	Time	10

	Event	
		ENAME	Flyaways2
		Type	Local
		At	FlyawayNode
		part	:KineticSwirlFlyAway03.part
		Bhvr	:KineticSpin3.bhvr
		LifeSpan 5
	End

End

Condition
	On	Time
	Time	15

	Event	
		ENAME	Flyaways3
		Type	Local
		At	FlyawayNode
		part	:KineticSwirlFlyAway03.part
		Bhvr	:KineticSpin3.bhvr
		LifeSpan 5
	End

End

End