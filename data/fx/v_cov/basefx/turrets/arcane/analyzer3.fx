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
####################     CAST  2   ######################
#########################################################

Condition
	On	Time
	Time	0


	Event
		EName 	crystal
		Type	Local 
		At	root
		bhvroverride
			PositionOffset		0 8.5 0
		end
	End

	Event	
		ENAME	aimrig
		Type	positonly
		At	T_Chest	
		lookat	Crystal
	End

end

Condition
	On	Time
	Time	20



	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	aimrig
		part	V_COV\BaseFx\turrets\tech\AnalyzerRaysBlue01.part
		PMagnet crystal
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40
		
	End
	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	crystal
		part	V_COV\BaseFx\turrets\tech\AnalyzerBodySwirls01.part
		PMagnet aimrig
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40		
		bhvroverride
			PositionOffset		0 0 -.5
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
		At	RightLarmGlow
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
		Bhvr	V_COV\BaseFx\turrets\tech\rot2.bhvr
		part	V_COV\BaseFx\turrets\tech\AnalyzerSwirlFlyAway03.part	
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
