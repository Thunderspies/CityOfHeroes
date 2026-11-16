#########################################################
##	forceBubble
########################################################
FxInfo

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
		Sound webcagehit 88 88 .95
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
		EName	WebTie01
		Type	Start
		At	WebCage
		AltPiv	1

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttach.bhvr
###		Geom	Testing_Target
		Magnet	HeadFront
		LookAt	HeadFront
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	1
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie01
				
	End
End

##

Condition
	On 	Time
	Time 	25
	
	Event
		EName	WebTie06
		Type	Start
		At	WebCage
		AltPiv	6

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttach.bhvr
###		Geom	Testing_Target
		Magnet	HeadBack
		LookAt	HeadBack
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	6
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie06
				
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
		POther	HeadTieB
				
	End

	Event
		EName	webcage
		Type	Local
		At	Root
		
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	HeadTie				
	End
End

#################################################################################
############################       Torso      ###################################
#################################################################################

Condition
	On 	Time
	Time 	23
	
	Event
		EName	WebTie03
		Type	Start
		At	WebCage
		AltPiv	3

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttach.bhvr
###		Geom	Testing_Target
		Magnet	Neck
		LookAt	Neck
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	3
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	Webtie03
				
	End
##

	Event
		EName	WebTie10
		Type	Start
		At	WebCage
		AltPiv	10

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttach.bhvr
###		Geom	Testing_Target
		Magnet	Neck
		LookAt	Neck
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	10
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie10
				
	End
End

##

Condition
	On 	Time
	Time 	20

	Event
		EName	WebTie09
		Type	Start
		At	WebCage
		AltPiv	9

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttach.bhvr
###		Geom	Testing_Target
		Magnet	Chest
		LookAt	Chest
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	9
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	WebTie09
				
	End
End

##

Condition
	On 	Time
	Time 	19
	
	Event
		EName	WebTie08
		Type	Start
		At	WebCage
		AltPiv	8

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttach.bhvr
###		Geom	Testing_Target
		Magnet	Chest
		LookAt	Chest
	End
	
	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	8
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie08
				
	End
End

##

Condition
	On 	Time
	Time 	21

	Event
		EName	WebTie07
		Type	Start
		At	WebCage
		AltPiv	7

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttach.bhvr
###		Geom	Testing_Target
		Magnet	Chest
		LookAt	Chest
	End
	
	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	7
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	WebTie07				
	End
End

##

Condition
	On 	Time
	Time 	22

	Event
		EName	WebTie06a
		Type	Start
		At	WebCage
		AltPiv	6

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttachAlt.bhvr
###		Geom	Testing_Target
		Magnet	Chest
		LookAt	Chest
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	6
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	Webtie06a
				
	End
End

##

Condition
	On 	Time
	Time 	20

	Event
		EName	WebTie05
		Type	Start
		At	WebCage
		AltPiv	5

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttach.bhvr
###		Geom	Testing_Target
		Magnet	Chest
		LookAt	Chest
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	5
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	WebTie05
				
	End
##
	Event
		EName	WebTie04
		Type	Start
		At	WebCage
		AltPiv	4

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttachAlt.bhvr
###		Geom	Testing_Target
		Magnet	Chest
		LookAt	Chest
	End
	
	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	4
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie04
				
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	Webbing
		Type	Start
		At	Root
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapSpin.bhvr
		Part3	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapCageSwarm.part
		Part4	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapCageSwarm01.part
		Part5	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapCloudBurst01.part
		Magnet	Root
		LookAt	Root
		Lifespan 30
	End
End


#################################################################################
################################   Hips   #######################################
#################################################################################

Condition
	On 	Time
	Time 	17
	
	Event
		EName	WebTie01a
		Type	Start
		At	WebCage
		AltPiv	1

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttach.bhvr
###		Geom	Testing_Target
		Magnet	Hips
		LookAt	Hips
	End
	
	Event
		EName	webcage							# Spawn WebCage at Hips
		Type	Start
		At	WebCage
		Altpiv	1
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	WebTie01a
				
	End
End

##

Condition
	On 	Time
	Time 	16
	
	Event
		EName	WebTie02
		Type	Start
		At	WebCage
		AltPiv	2

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttachAlt.bhvr
###		Geom	Testing_Target
		Magnet	Hips
		LookAt	Hips
	End
	
	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	2
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie02
				
	End
End

##

Condition
	On 	Time
	Time 	15

	Event
		EName	WebTie03a
		Type	Start
		At	WebCage
		AltPiv	3

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttach.bhvr
###		Geom	Testing_Target
		Magnet	Hips
		LookAt	Hips
	End
	
	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	3
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	WebTie03a
				
	End
End

##

Condition
	On 	Time
	Time 	15
	
	Event
		EName	WebTie04a
		Type	Start
		At	WebCage
		AltPiv	4

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttachAlt.bhvr
###		Geom	Testing_Target
		Magnet	Hips
		LookAt	Hips
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	4
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie04a				
	End
End

##

Condition
	On 	Time
	Time 	18

	Event
		EName	WebTie05a
		Type	Start
		At	WebCage
		AltPiv	5

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttach.bhvr
###		Geom	Testing_Target
		Magnet	Hips
		LookAt	Hips
	End
	
	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	5
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	Webtie05a
				
	End
End

##

Condition
	On 	Time
	Time 	19

	Event
		EName	WebTie06b
		Type	Start
		At	WebCage
		AltPiv	6

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttachAlt.bhvr
###		Geom	Testing_Target
		Magnet	Hips
		LookAt	Hips
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	6
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie06b
				
	End
End

##

Condition
	On 	Time
	Time 	16
	
	Event
		EName	WebTie07a
		Type	Start
		At	WebCage
		AltPiv	7

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttach.bhvr
###		Geom	Testing_Target
		Magnet	Hips
		LookAt	Hips
	End
	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	7
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	WebTie07a
				
	End
End

##

Condition
	On 	Time
	Time 	14
	
	Event
		EName	WebTie08a
		Type	Start
		At	WebCage
		AltPiv	8

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttachAlt.bhvr
###		Geom	Testing_Target
		Magnet	Hips
		LookAt	Hips
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	8
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie08a
				
	End
End
	
#################################################################################
################################   Arms   #######################################
#################################################################################
	
Condition
	On 	Time
	Time 	23

	Event
		EName	WebTie06c
		Type	Start
		At	WebCage
		AltPiv	6

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttach.bhvr
###		Geom	Testing_Target
		Magnet	UarmR
		LookAt	UarmR
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	6
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie06c
	
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	6
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie06c
				
	End
End

##

Condition
	On 	Time
	Time 	22

	Event
		EName	WebTie02a
		Type	Start
		At	WebCage
		AltPiv	2

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttachAlt.bhvr
###		Geom	Testing_Target
		Magnet	UarmL
		LookAt	UarmL
	End
	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	2
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie02a
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	2
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie02a
				
	End
End

##

Condition
	On 	Time
	Time 	26

	Event
		EName	WebTie04b
		Type	Start
		At	WebCage
		AltPiv	4

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttach.bhvr
###		Geom	Testing_Target
		Magnet	LarmL
		LookAt	LarmL
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	4
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	WebTie04b
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	4
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie04b
				
	End
End

##

Condition
	On 	Time
	Time 	24

	Event
		EName	WebTie09a
		Type	Start
		At	WebCage
		AltPiv	9

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttachAlt.bhvr
###		Geom	Testing_Target
		Magnet	LarmR
		LookAt	LarmR
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	9
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	WebTie09a
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	9
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie09a		
	End
End

##

Condition
	On 	Time
	Time 	29

	Event
		EName	WebTie05b
		Type	Start
		At	WebCage
		AltPiv	5

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttach.bhvr
###		Geom	Testing_Target
		Magnet	RingL
		LookAt	RingL
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	5
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	WebTie05b
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	5
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie05b
				
	End
End

##

Condition
	On 	Time
	Time 	27

	Event
		EName	WebTie10a
		Type	Start
		At	WebCage
		AltPiv	10

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttachAlt.bhvr
#		Geom	Testing_Target
		Magnet	RingR
		LookAt	RingR
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	10
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	WebTie10a
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	10
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie10a
				
	End
End

#################################################################################
################################   Legs   #######################################
#################################################################################

Condition
	On 	Time
	Time 	14

	Event
		EName	WebTie01b
		Type	Start
		At	WebCage
		AltPiv	1

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttach.bhvr
#		Geom	Testing_Target
		Magnet	UlegL
		LookAt	UlegL
	End
	
	Event
		EName	webcage							# Spawn WebCage at ULeg
		Type	Start
		At	WebCage
		Altpiv	1
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	WebTie01b
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	1
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie01b
				
	End
End

##

Condition
	On 	Time
	Time 	13

	Event
		EName	WebTie06c
		Type	Start
		At	WebCage
		AltPiv	6

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttachAlt.bhvr
#		Geom	Testing_Target
		Magnet	UlegR
		LookAt	UlegR
	End
	
	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	6
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	WebTie06c
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	6
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie06c
				
	End
End

################################################

Condition
	On 	Time
	Time 	11

	Event
		EName	WebTie07b
		Type	Start
		At	WebCage
		AltPiv	7

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttach.bhvr
#		Geom	Testing_Target
		Magnet	LlegL
		LookAt	LlegL
	End

	Event
		EName	webcage							# Spawn WebCage at Knees
		Type	Start
		At	WebCage
		Altpiv	7
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	WebTie07b
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	7
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie07b
				
	End
End

##

Condition
	On 	Time
	Time 	9

	Event
		EName	WebTie02b
		Type	Start
		At	WebCage
		AltPiv	2

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttachAlt.bhvr
#		Geom	Testing_Target
		Magnet	LlegR
		LookAt	LlegR
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	2
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	WebTie02b
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	2
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie02b
				
	End
End

################################################

Condition
	On 	Time
	Time 	8

	Event
		EName	WebTie03b
		Type	Start
		At	WebCage
		AltPiv	3

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttach.bhvr
#		Geom	Testing_Target
		Magnet	FootL
		LookAt	FootL
	End

	Event
		EName	webcage							# Spawn WebCage at Feet
		Type	Start
		At	WebCage
		Altpiv	3
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	WebTie03b
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	3
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie03b
				
	End
End
##

Condition
	On 	Time
	Time 	7

	Event
		EName	WebTie08b
		Type	Start
		At	WebCage
		AltPiv	8

		bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_TrapAttachAlt.bhvr
#		Geom	Testing_Target
		Magnet	FootR
		LookAt	FootR
	End

	Event
		EName	webcage							
		Type	Start
		At	WebCage
		Altpiv	8
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage01.part
		POther	WebTie08b
				
	End

	Event
		EName	webcage
		Type	Start
		At	WebCage
		Altpiv	8
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebCage.part
		POther	WebTie08b
				
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


Condition
	On 	Time
	Time 	1

	Event
		EName	Webbing
		Type	Local
		At	Hips

#		Part3	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapCageSwarm.part
#		Part4	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapCageSwarm01.part
#		Part5	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapCageSwarm02.part
#		Part5	V_COV\EnemyPowers\Arachnos\Arachnoid_P_TrapCageSwarm03.part
		Magnet	Hips
		Lifespan 60
	End
End


Condition
	On 	Time
	Time 	1

#	Event
#		EName	lazer
#		Type	Start
#		At	root
#			
#			
#		Geom	web1
#		
#		
#	End
#
#	Event
#		EName	lazer
#		Type	Start
#		At	root
#			
#			
#		Geom	web2
#		
#		
#	End
#
#	Event
#		EName	lazer
#		Type	Start
#		At	root
#			
#			
#		Geom	web3
#		
#		
#	End
#
#	Event
#		EName	lazer
#		Type	Start
#		At	root
#			
#			
#		Geom	web4
#		
#		
#	End
#	
End


End