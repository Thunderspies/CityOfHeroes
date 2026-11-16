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
	Time 	23

	Event
		EName	core
		Type	Local
		At	Belt

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_TotDominateEmitter.bhvr
#		Geom	Testing_Target
		Sound mental6b 60 60 .71
	End

#################################################################################
################################  Spiral Cones  #################################
#################################################################################

	Event
		EName	InsideCircle
		Type	Posit
		At	core
		
		Geom	InnerCircle
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_MentalSpin.bhvr
		Lifespan 30
	End

	Event
		EName	OutsideCircle
		Type	Posit
		At	core
		
		Geom	OutsideCircle
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_MentalSpinOpposite.bhvr
		Lifespan 30
	End

#################################################################################
##############################  Emitting Rings  #################################
#################################################################################

	Event
		EName	alt
		Type	Posit
		At	core
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_TotDomRotate.bhvr
##		part1	POWERS\MentalPowers\LargeMindControlRing.part
		part3	POWERS\MentalPowers\LargeExpandingMindControlRing.part
		Lifespan 40
	End

#################################################################################
################################  Emitting Gas  #################################
#################################################################################

	Event
		EName	lightning
		Type	Posit
		At	Core

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_TotDomSmoke.bhvr
		part2	POWERS\MentalPowers\FreezeSnow.part	
		part4	POWERS\MentalPowers\FreezeMist.part
		part5	POWERS\MentalPowers\Freezesteam.part
		Lifespan 45
	End

End

#################################################################################
###############  Prime points (invisible, only for hit timing)  #################
#################################################################################

Condition
	On 	Time
	Time 	23

	Event
		EName	Prime
		Type	Start
		At	Core
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_TeleTracker.bhvr
#		Geom	Testing_Target
		Magnet	Target
		LookAt	Target
	End
End

Condition
	On 	PrimeHit
	
	Event	
		EName	Prime
		Type	Destroy
	End
End

################################################################################
######################## Additional Emitting Rings  #############################
#################################################################################

Condition
	On 	Time
	Time 	28

	Event
		EName	alt2
		Type	Local
		At	alt
		
#		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_TotDomRotate.bhvr
		part1	POWERS\MentalPowers\MindControlRingsBurst2.part
	End
End

Condition
	On 	Time
	Time 	53

	Event
		EName	alt3
		Type	Local
		At	alt
		
#		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_TotDomRotate.bhvr
		part1	POWERS\MentalPowers\MindControlRingsBurst2.part
	End
End


End