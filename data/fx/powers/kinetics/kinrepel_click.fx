#########################################################
##	template

FxInfo




Lifespan 80

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	15
	

	Event
		Type	Local
		At 	Origin
		Sound kinetic2 80 80 .85
	End
End

####################### Force ###########################

Condition
	On	Time
	Time	30

	Event
		Ename	GravitationalForce
		Type	Local
		At	Hips
		
		BhvrOverride
			PhysForceType		CCWSwirl
			PhysForceCentripetal	-2
			PhysForceRadius		10
			PhysForcePower		1000
			PhysForcePowerJitter	25
		End

		Lifespan	150

	End

	Event
		Ename	LevitationForce
		Type	Local
		At	Root
		
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		11
			PhysForcePower		150
			PhysForcePowerJitter	5
		End

		Lifespan	120

	End
End

#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition
	On	Cycle 
	Time	60
	
	Event	
		ENAME	ScaleNode
		Type	Local
		At	Hips	
		Bhvr	:KineticScale.bhvr
		Sound Kin2_loop 80 80 .5
		Lifespan 50

	End

	Event	
		ENAME	Swirl1
		Type	Local
		At	ScaleNode
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Bhvr	:KineticSpin1.bhvr
		Lifespan 50
		
	End

	Event	
		ENAME	Swirl2
		Type	Local
		At	ScaleNode
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Bhvr	:KineticSpin2.bhvr
		Lifespan 50
		
	End

	Event	
		ENAME	Swirl3
		Type	Local
		At	ScaleNode
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Bhvr	:KineticSpin3.bhvr
		Lifespan 50
		
	End

	Event	
		ENAME	Glows1
		Type	Local
		At	ScaleNode
		part	:KineticExpandingGlowsRed01.part
		part	:KineticExpandingRings01.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 50
		
	End

	Event	
		ENAME	Flyaways1
		Type	Start
		At	Swirl1
		part	:KineticSwirlFlyAway01.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 50
		
	End
End

Condition
	On	Cycle
	Time	48

	Event	
		ENAME	Pulse1
		Type	Posit
		At	Hips
		part	:KineticOval01.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 45
		
	End


End	


##########################################################
####################     CAST  1   ######################
#########################################################

Condition
	On	Time
	Time	5

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:KineticBodyGlows01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		POther  LarmR
		part	:KineticBodyGlows01.part
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40
		
	End


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
	Time	10

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:KineticBodySwirls01.part
		POther LarmR
		PMagnet Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		POther  LarmR
		PMagnet Chest
		part	:KineticBodySwirls01.part
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:KineticBodySwirls01.part
		POther LarmL
		PMagnet Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:KineticBodySwirls01.part
		POther LarmL
		PMagnet Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40
		
	End

End

#########################################################
####################   Flyaways   #######################
#########################################################


Condition
	On	Time
	Time	15

	Event	
		ENAME	Flyaways1
		Type	Local
		At	Hips
		part	:KineticSwirlFlyAway03.part
		Bhvr	:KineticSpin3.bhvr
		LifeSpan 60
	End

End

Condition
	On	Time
	Time	20

	Event	
		ENAME	Flyaways2
		Type	Local
		At	Hips
		part	:KineticSwirlFlyAway03.part
		Bhvr	:KineticSpin3.bhvr
		LifeSpan 60
	End

End

Condition
	On	Time
	Time	25

	Event	
		ENAME	Flyaways3
		Type	Local
		At	Hips
		part	:KineticSwirlFlyAway03.part
		Bhvr	:KineticSpin3.bhvr
		LifeSpan 60
	End

End

End