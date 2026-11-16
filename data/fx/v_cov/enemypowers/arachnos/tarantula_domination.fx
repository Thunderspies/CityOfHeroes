#########################################################
##	Tarantula Total Domination
########################################################

FxInfo

Lifespan 80

#################################################################################
############################  Emitter points  ###################################
#################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Emitter
		Type	Local
		At	Belt

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_TotDominateEmitter.bhvr
#		Geom	Testing_Target
		Sound mental6b 60 60 .71
	End
	
	Event
		EName	Target_Spin
		Type	Local
		At	T_Chest
		bhvrOverride
			behavior
			spin		0 5 0
			Scale		3 3 3
		End
#		Geom	Testing_TargetB
	End

End

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
#		part	V_COV\BaseFx\turrets\Tech\AnalyzerRaysBlue01.part
		PMagnet Emitter
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40
		
	End
	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	Emitter
#		part	V_COV\BaseFx\turrets\Tech\AnalyzerBodySwirls01.part
		PMagnet aimrig
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40		
#		bhvroverride
#			PositionOffset		0 0 -1
#		end
		lookat	aimrig
		
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	aimrig
#		part	V_COV\BaseFx\turrets\Tech\AnalyzerRaysBlue01.part
		PMagnet Emitter
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	Emitter
#		part	V_COV\BaseFx\turrets\Tech\AnalyzerBodySwirls01.part
		PMagnet aimrig
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40		
#		bhvroverride
#			PositionOffset		0 0 -1
#		end
		lookat	aimrig
		
	End

End

#########################################################
####################   Rings      #######################
#########################################################


Condition
	On	Time
	Time	15

	Event	
		ENAME	Prime
		Type	Start
		At	Emitter
		Bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_Prime.bhvr
		
#		part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_TDRings01.part
#		Geom	Testing_TargetB
		Magnet	Target
#		LookAt	Target
		PMagnet	Prime
		LifeSpan 50
	End

	Event	
		ENAME	Rings01
		Type	Posit
		At	Emitter
		bhvrOverride
			behavior
			FadeOutLength		150
		End

		part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_TDRings.part
		part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_TDRings01.part
		LifeSpan 15
		pmagnet Target_Spin
	End

	Event	
		ENAME	Rings02
		Type	Posit
		At	Emitter
		bhvrOverride
			behavior
			FadeOutLength		150
		End

		part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_TDRings02.part
		LifeSpan 18
		pmagnet Target_Spin
	End

	Event
		EName	Specks
		Type	Posit
		At	Emitter

		part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_TDSpecks.part
		LifeSpan 10
		pmagnet T_Head
	End


#################################################################################
################################  Emitting Gas  #################################
#################################################################################

	Event
		EName	lightning
		Type	Posit
		At	Emitter

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_TotDomSmoke.bhvr
		part2	POWERS\MentalPowers\FreezeSnow.part	
		part4	POWERS\MentalPowers\FreezeMist.part
		part5	POWERS\MentalPowers\Freezesteam.part
		Lifespan 10
	End

End



Condition
	On	Time
	Time	150

	Event	
		ENAME	All
		Type	Destroy	
	End

End

End



































#################################################################################
#################################  Spiral Cones  #################################
##################################################################################
#
#	Event
#		EName	InsideCircle
#		Type	Posit
#		At	core
#		
#		Geom	InnerCircle
#		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_TotDomSpin.bhvr
#		Lifespan 30
#	End
#
#	Event
#		EName	OutsideCircle
#		Type	Posit
#		At	core
#		
#		Geom	OutsideCircle
#		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_TotDomSpinOpposite.bhvr
#		Lifespan 30
#	End
#
##################################################################################
###############################  Emitting Rings  #################################
##################################################################################
#
#	Event
#		EName	alt
#		Type	Posit
#		At	core
#		
#		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_TotDomRotate.bhvr
###		part1	POWERS\MentalPowers\LargeMindControlRing.part
#		part3	POWERS\MentalPowers\LargeExpandingMindControlRing.part
#		Lifespan 40
#	End
#
##################################################################################
#################################  Emitting Gas  #################################
##################################################################################
#
#	Event
#		EName	lightning
#		Type	Posit
#		At	Core
#
#		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_TotDomSmoke.bhvr
#		part2	POWERS\MentalPowers\FreezeSnow.part	
#		part4	POWERS\MentalPowers\FreezeMist.part
#		part5	POWERS\MentalPowers\Freezesteam.part
#		Lifespan 45
#	End
#
#End
#
##################################################################################
################  Prime points (invisible, only for hit timing)  #################
##################################################################################
#
#Condition
#	On 	Time
#	Time 	23
#
#	Event
#		EName	Prime
#		Type	Start
#		At	Core
#		
#		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_TeleTracker.bhvr
##		Geom	Testing_Target
#		Magnet	Target
#		LookAt	Target
#	End
#End
#
#Condition
#	On 	PrimeHit
#	
#	Event	
#		EName	Prime
#		Type	Destroy
#	End
#End
#
#################################################################################
######################### Additional Emitting Rings  #############################
##################################################################################
#
#Condition
#	On 	Time
#	Time 	28
#
#	Event
#		EName	alt2
#		Type	Local
#		At	alt
#		
##		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_TotDomRotate.bhvr
#		part1	POWERS\MentalPowers\MindControlRingsBurst2.part
#	End
#End
#
#Condition
#	On 	Time
#	Time 	53
#
#	Event
#		EName	alt3
#		Type	Local
#		At	alt
#		
##		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_TotDomRotate.bhvr
#		part1	POWERS\MentalPowers\MindControlRingsBurst2.part
#	End
#End
#
#
#End