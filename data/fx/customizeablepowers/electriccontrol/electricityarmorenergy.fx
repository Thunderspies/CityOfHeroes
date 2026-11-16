#########################################################
##	Fire_Ball
##
FxInfo
##################################

Condition
	On Cycle
	Time 30
	Event
		At	Origin
		Type	SetLight
		BHVR	GENERIC\LowGlow\lightpulse3.bhvr
		LifeSpan 30
	End

End

#######################################################################################################3
#####Aura
##################################################################


Condition
	On	Time
	Time	0
	
	Event
		EName	2
		Type	Local
		At	FootR
		part	:LeviathanEmber.part
		#Part	:TendrilEnergy.part
		Pmagnet	LlegR
		Sound oreGlow_loop 40 40 .33
	End


#LegL


	Event
		EName	4
		Type	Local
		At	LLEGL
		part	:LeviathanEmber.part
		#Part	:TendrilEnergy.part
		Pmagnet	UlegL
		

	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part	:LeviathanEmber.part
		#Part	:TendrilEnergy.part
		Pmagnet	LlegL
		
	End
	
	Event
		EName	4
		Type	Local
		At	ULEGL
		part	:LeviathanEmber.part
		#Part	:TendrilEnergy.part
		Pmagnet	LlegL
		

	End

	Event
		EName	5		
		Type	Local
		At	LlegL
		part	:LeviathanEmber.part
		#Part	:TendrilEnergy.part
		Pmagnet	FootL
		
	End

	Event
		EName	6
		Type	Local
		At	ULEGR
		part	:LeviathanEmber.part
		#Part	:TendrilEnergy.part
		Pmagnet	LlegR
	End

#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part	:LeviathanEmber.part
		#Part	:TendrilEnergy.part
		Pmagnet	UlegR
		
	End
	

	Event
		EName	7
		Type	Local
		At	LLEGR
		part	:LeviathanEmber.part
		#Part	:TendrilEnergy.part
		Pmagnet	FootR
		
	End
	

	Event
		EName	7
		Type	Local
		At	ULEGR
		part	:LeviathanEmber.part
		#Part	:TendrilEnergy.part
		Pmagnet	LlegR
		
	End

		
###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part	:LeviathanEmber.part
		#Part	:TendrilEnergy.part
		
	End

		
#########################################################################
#Arm
#################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part	:LeviathanEmber.part
		#Part	:TendrilEnergy.part
		
	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part	:LeviathanEmber.part
		#Part	:TendrilEnergy.part
		
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part	:LeviathanEmber.part
		#Part	:TendrilEnergy.part
		

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part	:LeviathanEmber.part
		#Part	:TendrilEnergy.part
	End

End    

##############################################################################################
##Electricity
####################################

Condition
	On	Cycle
	Time	10
	chance	.2

	Event
		EName	2
		Type	Local
		At	FootR
		Part	:electricityzaps.part
		Part	:electricityLight.part
		LifeSpan	5
	End

End

Condition
	On	Cycle
	Time	10
	chance	.2

	Event
		EName	4
		Type	Local
		At	LLEGL
		Part	:electricityzaps.part
		Part	:electricityLight.part
		LifeSpan	5
		

	End

End

Condition
	On	Cycle
	Time	10
	chance	.2

	Event
		EName	5		
		Type	Local
		At	FootL
		Part	:electricityzaps.part
		Part	:electricityLight.part
		LifeSpan	5
		
	End

End

Condition
	On	Cycle
	Time	10
	chance	.2

	
	Event
		EName	4
		Type	Local
		At	ULEGL
		Part	:electricityzaps.part
		Part	:electricityLight.part
		LifeSpan	5
		

	End

End

Condition
	On	Cycle
	Time	10
	chance	.2


	Event
		EName	5		
		Type	Local
		At	LlegL
		Part	:electricityzaps.part
		Part	:electricityLight.part
		LifeSpan	5
		
	End

End

Condition
	On	Cycle
	Time	10
	chance	.2


	Event
		EName	6
		Type	Local
		At	ULEGR
		Part	:electricityzaps.part
		Part	:electricityLight.part
		LifeSpan	5
	End

End

Condition
	On	Cycle
	Time	10
	chance	.2


	Event
		EName	7
		Type	Local
		At	LLEGR
		Part	:electricityzaps.part
		Part	:electricityLight.part
		LifeSpan	5
	End

End

Condition
	On	Cycle
	Time	10
	chance	.2

	

	Event
		EName	7
		Type	Local
		At	LLEGR
		Part	:electricityzaps.part
		Part	:electricityLight.part
		LifeSpan	5
		
	End

End

Condition
	On	Cycle
	Time	10
	chance	.2

	

	Event
		EName	7
		Type	Local
		At	ULEGR
		Part	:electricityzaps.part
		Part	:electricityLight.part
		LifeSpan	5
		
	End

End

Condition
	On	Cycle
	Time	10
	chance	.2

	Event
		EName	9
		Type	Local
		At	Head
		Part	:electricityzaps.part
		Part	:electricityLight.part
		LifeSpan	5
		
	End

End

Condition
	On	Cycle
	Time	10
	chance	.2


	Event
		EName	10
		Type	Local
		At	UArmL
		Part	:electricityzaps.part
		Part	:electricityLight.part
		LifeSpan	5
		
	End

End

Condition
	On	Cycle
	Time	10
	chance	.2


	Event
		EName	11
		Type	Local
		At	LArmL
		Part	:electricityzaps.part
		Part	:electricityLight.part
		LifeSpan	5
		
	End

End

Condition
	On	Cycle
	Time	10
	chance	.2

	Event
		EName	13
		Type	Local
		At	UArmR
		Part	:electricityzaps.part
		Part	:electricityLight.part
		LifeSpan	5
		

	End

End

Condition
	On	Cycle
	Time	10
	chance	.2


	Event
		EName	14
		Type	Local
		At	LArmR
		Part	:electricityzaps.part
		Part	:electricityLight.part
		LifeSpan	5
	End

End    

##########################################################################################################
##Magnet Jolts
################################################################

Condition
	On	Cycle
	Time	5
	chance	.3

	Event

		Type	Local
		At	chest
		
		ChildFx	:electricityBoltArmorStrike.fx
		LifeSpan	15
	End

End    

################################################################################

Condition
	On	Cycle
	Time	4
	chance	 .23

	#Repeat		1
	#RepeatJitter	3
	
	Event
		EName	Sparkfar
		Type 	Start
		At	WepL
		ChildFX	V_COV\PhysicsEnabled\GenericElectricSpark.fx

		Lifespan	190
		
	End

End

Condition
	On	Cycle
	Time	4
	chance	 .23

	#Repeat		1
	#RepeatJitter	3
	
	Event
		EName	Sparkfar
		Type 	Start
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\GenericElectricSpark.fx

		Lifespan	190
		
	End

End

Condition
	On	Cycle
	Time	4
	chance	 .23

	#Repeat		1
	#RepeatJitter	3
	
	Event
		EName	Sparkfar
		Type 	Start
		At	FootL
		ChildFX	V_COV\PhysicsEnabled\GenericElectricSpark.fx

		Lifespan	190
		
	End

End

Condition
	On	Cycle
	Time	4
	chance	 .23

	#Repeat		1
	#RepeatJitter	3
	
	Event
		EName	Sparkfar
		Type 	Start
		At	FootR
		ChildFX	V_COV\PhysicsEnabled\GenericElectricSpark.fx

		Lifespan	190
		
	End

End

Condition
	On	Cycle
	Time	4
	chance	 .23

	#Repeat		1
	#RepeatJitter	3
	
	Event
		EName	Sparkfar
		Type 	Start
		At	UlegL
		ChildFX	V_COV\PhysicsEnabled\GenericElectricSpark.fx

		Lifespan	190
		
	End

End

Condition
	On	Cycle
	Time	4
	chance	 .23

	#Repeat		1
	#RepeatJitter	3
	
	Event
		EName	Sparkfar
		Type 	Start
		At	UlegR
		ChildFX	V_COV\PhysicsEnabled\GenericElectricSpark.fx

		Lifespan	190
		
	End

End

Condition
	On	Cycle
	Time	4
	chance	 .23

	#Repeat		1
	#RepeatJitter	3
	
	Event
		EName	Sparkfar
		Type 	Start
		At	LlegL
		ChildFX	V_COV\PhysicsEnabled\GenericElectricSpark.fx

		Lifespan	190
		
	End

End

Condition
	On	Cycle
	Time	4
	chance	 .23

	#Repeat		1
	#RepeatJitter	3
	
	Event
		EName	Sparkfar
		Type 	Start
		At	LlegL
		ChildFX	V_COV\PhysicsEnabled\GenericElectricSpark.fx

		Lifespan	190
		
	End

End

Condition
	On	Cycle
	Time	4
	chance	 .23

	#Repeat		1
	#RepeatJitter	3
	
	Event
		EName	Sparkfar
		Type 	Start
		At	UarmL
		ChildFX	V_COV\PhysicsEnabled\GenericElectricSpark.fx

		Lifespan	190
		
	End

End

Condition
	On	Cycle
	Time	4
	chance	 .23

	#Repeat		1
	#RepeatJitter	3
	
	Event
		EName	Sparkfar
		Type 	Start
		At	UarmR
		ChildFX	V_COV\PhysicsEnabled\GenericElectricSpark.fx

		Lifespan	190
		
	End

End

Condition
	On	Cycle
	Time	4
	chance	 .23

	#Repeat		1
	#RepeatJitter	3
	
	Event
		EName	Sparkfar
		Type 	Start
		At	LarmL
		ChildFX	V_COV\PhysicsEnabled\GenericElectricSpark.fx

		Lifespan	190
		
	End

End

Condition
	On	Cycle
	Time	4
	chance	 .23

	#Repeat		1
	#RepeatJitter	3
	
	Event
		EName	Sparkfar
		Type 	Start
		At	LarmR
		ChildFX	V_COV\PhysicsEnabled\GenericElectricSpark.fx

		Lifespan	190
		
	End

End

Condition
	On	Cycle
	Time	4
	chance	 .23

	#Repeat		1
	#RepeatJitter	3
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Head
		ChildFX	V_COV\PhysicsEnabled\GenericElectricSpark.fx

		Lifespan	190
		
	End

End

Condition
	On	Cycle
	Time	4
	chance	 .23

	
	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\GenericElectricSpark.fx

		Lifespan	190
		
	End

End

End	

