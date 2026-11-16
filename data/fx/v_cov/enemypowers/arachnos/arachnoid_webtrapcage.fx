#########################################################
##	forceBubble
########################################################
FxInfo

#lifespan 180

#################################################################################
############################  Emitter points  ###################################
#################################################################################
Condition

	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		#Sound	webgrexplo2 88 70 .95	
	End
End


Condition

	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		#Sound	webspin_loop 60 50 .4
	End
End



Condition
	On 	Time
	Time 	0

	Event
		EName	webcage
		Type	posit
		At	root
		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_WebCageSpin.bhvr
		Geom	WebCage
				
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	HeadFront
		Type	Local
		At	Head
		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_WebCageFace.bhvr	
#		Geom	Testing_Target
				
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	HeadBack
		Type	Local
		At	Head
		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_WebCageBack.bhvr	
#		Geom	Testing_Target
				
	End
End

#################################################################################
###############################    Head    ######################################
#################################################################################

Condition
	On 	Time
	Time 	1

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	1
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	HeadFront
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	6
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	HeadBack
				
	End

	Event
		EName	webcage
		Type	Local
		At	Root
		
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	HeadBack
				
	End

	Event
		EName	webcage
		Type	Local
		At	Root
		
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	HeadFront				
	End

#################################################################################
############################       Torso      ###################################
#################################################################################

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	10
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	Neck
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	9
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	Chest
				
	End

	
	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	8
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	Chest
				
	End

	
	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	7
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	Chest
				
	End

		Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	6
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	Chest
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	5
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	Chest
				
	End

	
	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	4
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	Chest
				
	End

	
	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	3
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	neck
				
	End

#################################################################################
################################   Hips   #######################################
#################################################################################


	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	1
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	Hips
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	2
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	Hips
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	3
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	Hips
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	4
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	Hips
				
	End

		Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	5
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	Hips
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	6
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	Hips
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	7
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	Hips
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	8
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	Hips
				
	End

	
#################################################################################
################################   Arms   #######################################
#################################################################################
	
	
Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	6
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	UarmR
	
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	6
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	UarmR
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	2
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	UarmL
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	2
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	UarmL
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	4
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	LarmL
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	4
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	LarmL
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	9
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	LarmR
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	9
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	LarmR
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	4
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	RingL
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	5
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	RingL
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	10
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	RingR
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	10
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	RingR
				
	End

#################################################################################
################################   Legs   #######################################
#################################################################################

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	1
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	UlegL
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	1
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	UlegL
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	6
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	UlegR
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	6
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	UlegR
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	7
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	LlegL
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	7
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	LlegL
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	2
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	LlegR
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	2
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	LlegR
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	3
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	FootL
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	3
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	FootL
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	8
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	FootR
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	8
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	FootR
				
	End

	
	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	4
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	ToeR
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	9
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	ToeR
				
	End

#################################################################################
################################   End   #######################################
#################################################################################


	Event
		EName	lazer
		Type	Start
		At	root
			
			
		Geom	web1
		
		
	End

	Event
		EName	lazer
		Type	Start
		At	root
			
			
		Geom	web2
		
		
	End

	Event
		EName	lazer
		Type	Start
		At	root
			
			
		Geom	web3
		
		
	End

	Event
		EName	lazer
		Type	Start
		At	root
			
			
		Geom	web4
		
		
	End
	
End


End