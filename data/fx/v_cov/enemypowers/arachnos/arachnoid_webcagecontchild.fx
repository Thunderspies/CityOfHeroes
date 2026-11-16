
FXInfo


#################################################################################
############################  Emitter points  ###################################
#################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	webcage
		Type	posit
		At	root
		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_WebCageSpin.bhvr
#		Sound	webcagehit 88 70 .95			
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
####		Geom	Testing_Target
				
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
####		Geom	Testing_Target
				
	End
End

#################################################################################
############################  Targets point  ####################################
#################################################################################





#################################################################################
###############################    Head    ######################################
#################################################################################


Condition
	On 	Time
	Time 	26

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	1
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	HeadFront
				
	End
End

##

Condition
	On 	Time
	Time 	25

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	6
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	HeadBack
				
	End
End

##

Condition
	On 	Time
	Time 	24
	
	Event
		EName	webcage
		Type	Local
		At	Root
		
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	Head
				
	End

	Event
		EName	webcage
		Type	Local
		At	Root
		
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	Head				
	End
End

#################################################################################
############################       Torso      ###################################
#################################################################################

Condition
	On 	Time
	Time 	23

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	3
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	Neck
				
	End

##

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	10
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	Neck
				
	End
End

##

Condition
	On 	Time
	Time 	20

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	9
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	Chest
				
	End
End

##

Condition
	On 	Time
	Time 	19
	
	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	8
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	Chest
				
	End
End

##

Condition
	On 	Time
	Time 	21
	
	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	7
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	Chest				
	End
End

##

Condition
	On 	Time
	Time 	22

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	6
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	Chest
				
	End
End

##

Condition
	On 	Time
	Time 	20

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	5
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	Chest
				
	End
##

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	4
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	Chest
				
	End
End

#################################################################################
################################   Hips   #######################################
#################################################################################

Condition
	On 	Time
	Time 	17
	
	Event
		EName	webcage							# Spawn WebCage at Hips
		Type	Start
		At	WebCage
		Altpiv	1
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	Hips
				
	End
End

##

Condition
	On 	Time
	Time 	16

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	2
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	Hips
				
	End
End

##

Condition
	On 	Time
	Time 	15

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	3
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	Hips
				
	End
End

##

Condition
	On 	Time
	Time 	15

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	4
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	Hips				
	End
End

##

Condition
	On 	Time
	Time 	18

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	5
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	Hips
				
	End
End

##

Condition
	On 	Time
	Time 	19

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	6
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	Hips
				
	End
End

##

Condition
	On 	Time
	Time 	16

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	7
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	Hips
				
	End
End

##

Condition
	On 	Time
	Time 	14

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	8
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	Hips
				
	End
End
	
#################################################################################
################################   Arms   #######################################
#################################################################################
	
Condition
	On 	Time
	Time 	23

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
End

##

Condition
	On 	Time
	Time 	22

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
End

##

Condition
	On 	Time
	Time 	26

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
End

##

Condition
	On 	Time
	Time 	24

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
End

##

Condition
	On 	Time
	Time 	29

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	5
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
End

##

Condition
	On 	Time
	Time 	27

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
End

#################################################################################
################################   Legs   #######################################
#################################################################################

Condition
	On 	Time
	Time 	14
	
	Event
		EName	webcage							# Spawn WebCage at ULeg
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
End

##

Condition
	On 	Time
	Time 	13
	
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
End

################################################

Condition
	On 	Time
	Time 	11

	Event
		EName	webcage							# Spawn WebCage at Knees
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
End

##

Condition
	On 	Time
	Time 	9

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
End

################################################

Condition
	On 	Time
	Time 	8

	Event
		EName	webcage							# Spawn WebCage at Feet
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
End
##

Condition
	On 	Time
	Time 	7

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
End

################################################

Condition 
	On 	Time
	Time 	5
	
	Event
		EName	webcage							# Spawn WebCage at toes
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
End

#################################################################################
################################   End   #######################################
#################################################################################


End