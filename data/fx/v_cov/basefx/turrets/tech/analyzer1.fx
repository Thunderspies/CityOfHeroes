#########################################################
##	template

FxInfo

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

#Condition
#	On	Time
#	Time	25
#
#	Event	
#		ENAME	WepLGlow
#		Type	Local
#		At	WepL
#		part	POWERS\Kinetics\KineticHandGlows01.part
#		Lifespan 35
#		
#	End
#	
#	Event	
#		ENAME	WepRGlow
#		Type	Localq
#		At	WepR
#		part	POWERS\Kinetics\KineticHandGlows01.part
#		Lifespan 35
#		
#	End
#End


#########################################################
####################     CAST  2   ######################
#########################################################

Condition
	On	Time
	Time	0

	Event	
		ENAME	aimrig
		Type	positonly
		At	T_Chest	
		lookat	waist
	End

end

Condition
	On	Time
	Time	20


	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	aimrig
		part	:AnalyzerRaysBlue01.part
		PMagnet WepL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40
		
	End
	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepL
		part	:AnalyzerBodySwirls01.part
		PMagnet aimrig
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40		
		bhvroverride
			PositionOffset		0 0 -1
		end
		lookat	aimrig
		
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	aimrig
		part	:AnalyzerRaysBlue01.part
		PMagnet WepR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepR
		part	:AnalyzerBodySwirls01.part
		PMagnet aimrig
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40		
		bhvroverride
			PositionOffset		0 0 -1
		end
		lookat	aimrig
		
	End

End

#########################################################
####################   Flyaways   #######################
#########################################################


Condition
	On	Time
	Time	20

	Event	
		ENAME	FlyawayNodeL
		Type	positonly
		At	LeftLarmGlow
#		Bhvr	POWERS\Kinetics\KineticScaleSmall.bhvr
		LifeSpan 50
		lookat aimrig
		bhvroverride
			PositionOffset		0 0 -1.5
		end

	End

	Event	
		ENAME	FlyawaysL1
		Type	Local
		At	FlyawayNodeL
		Bhvr	:rot2.bhvr
		part	:AnalyzerSwirlFlyAway03.part	
		#Bhvr	POWERS\Kinetics\KineticSpin3.bhvr
		LifeSpan 41
		pmagnet target
	End

	Event	
		ENAME	FlyawayNodeR
		Type	positonly
		At	RightLarmGlow
#		Bhvr	POWERS\Kinetics\KineticScaleSmall.bhvr
		LifeSpan 50
		lookat aimrig
		bhvroverride
			PositionOffset		0 0 -1.5
		end
	End

	Event	
		ENAME	FlyawaysR1
		Type	Local
		At	FlyawayNodeR
		Bhvr	:rot2.bhvr	
		part	:AnalyzerSwirlFlyAway03.part
		#Bhvr	POWERS\Kinetics\KineticSpin3.bhvr
		LifeSpan 41
		pmagnet target
	End

End


Condition
	On	Time
	Time	70

	Event	
		ENAME	All
		Type	Destroy	
	End

End

End
