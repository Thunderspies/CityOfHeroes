#########################################################
##	template

FxInfo




Lifespan 300

#########################################################
####################### AUDIO ###########################
#########################################################


Condition  ############ doubled up the soundFX ###########
	On	Time
	Time	15
	


	Event
		Type	Local
		At 	Origin
		Sound CoralAmulet 80 80 .98
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
		At	WepL
		part	POWERS\Kinetics\KineticHandGlowsRed01.part
		Lifespan 35
		
	End

End
Condition
	On	Time
	Time	20

	Event	
		ENAME	WepRGlow
		Type	Local
		At	WepR
		part	POWERS\Kinetics\KineticHandGlowsRed01.part
		Lifespan 35
		
	End

End

Condition
	On	Time
	Time	25
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		POther  LarmL
		part	POWERS\Kinetics\KineticBodyGlowsRed01.part
		POther LarmL
		Lifespan 25
		
	End

End

Condition
	On	Time
	Time	25
	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		POther  LarmR
		part	POWERS\Kinetics\KineticBodyGlowsRed01.part
		POther LarmR
		Lifespan 25
		
	End

End

Condition
	On	Time
	Time	32

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	POWERS\Kinetics\KineticBodyGlowsRed01.part
		POther LarmL
		Lifespan 25
		
	End
	
End

Condition
	On	Time
	Time	32

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	POWERS\Kinetics\KineticBodyGlowsRed01.part
		POther LarmR
		Lifespan 25
		
	End
	
End

#########################################################
####################     CAST  2   ######################
#########################################################

#Condition
#	On	Time
#	Time	20
#
#
#	Event	
#		ENAME	LeftUarmGlow
#		Type	Local
#		At	Target
#		part	POWERS\Kinetics\KineticCastRaysRed01.part
#		PMagnet LarmL
#		Lifespan 40
#		
#	End
#	
#	Event	
#		ENAME	LeftLarmGlow
#		Type	Local
#		At	WepL
#		part	POWERS\Kinetics\KineticIDBodySwirls01.part
#		PMagnet Target
#		Lifespan 40
#		
#	End
#
#End

#########################################################
####################   Flyaways   #######################
#########################################################


Condition
	On	Time
	Time	15

	Event	
		ENAME	FlyawayNode
		Type	Local
		At	WepL
		Bhvr	POWERS\Kinetics\KineticScaleSmall.bhvr
		LifeSpan 50
	End

	Event	
		ENAME	Flyaways1
		Type	Local
		At	FlyawayNode
		part	POWERS\Kinetics\KineticSwirlFlyAway03.part
		Bhvr	POWERS\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End

End
Condition
	On	Time
	Time	15

	Event	
		ENAME	FlyawayNodeR
		Type	Local
		At	WepR
		Bhvr	POWERS\Kinetics\KineticScaleSmall.bhvr
		LifeSpan 50
	End

	Event	
		ENAME	Flyaways1R
		Type	Local
		At	FlyawayNodeR
		part	POWERS\Kinetics\KineticSwirlFlyAway03.part
		Bhvr	POWERS\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End

End

Condition
	On	Time
	Time	20

	Event	
		ENAME	Flyaways2
		Type	Local
		At	FlyawayNode
		part	POWERS\Kinetics\KineticSwirlFlyAway03.part
		Bhvr	POWERS\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End

End
Condition
	On	Time
	Time	20

	Event	
		ENAME	Flyaways2R
		Type	Local
		At	FlyawayNodeR
		part	POWERS\Kinetics\KineticSwirlFlyAway03.part
		Bhvr	POWERS\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End

End

Condition
	On	Time
	Time	25

	Event	
		ENAME	Flyaways3
		Type	Local
		At	FlyawayNode
		part	POWERS\Kinetics\KineticSwirlFlyAway03.part
		Bhvr	POWERS\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End

End
Condition
	On	Time
	Time	25

	Event	
		ENAME	Flyaways3R
		Type	Local
		At	FlyawayNodeR
		part	POWERS\Kinetics\KineticSwirlFlyAway03.part
		Bhvr	POWERS\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End

End


End