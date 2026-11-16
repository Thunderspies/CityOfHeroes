#########################################################
##	geometry for Arachnoid Spin Cocoon PL
########################################################
FxInfo

####  start by spawning the cocoon geometry ####

Condition
	On	Time
	Time	0

	Event
		EName	Cocoon
		Type	Local
		At	Root
		bhvr	V_COV\EnemyPowers\Arachnos\B_ArachnoidCocoon.bhvr

#		Geom	Arachnoid_Cocoon
#		Lifespan 1
	End

	Event
		EName	CocoonPos
		Type	Start
		At	Cocoon
	End

	Event
		EName	CocoonFill
		Type	Local
		At	Cocoon

		bhvr	V_COV\EnemyPowers\Arachnos\B_ArachnoidCocoonFill.bhvr
		Part	V_COV\EnemyPowers\Arachnos\P_Web_ArachnoidFill.part
		PMagnet	Cocoon
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	CocoonChild
		Type	Local
		At	Cocoon

		ChildFX V_COV\EnemyPowers\Arachnos\F_ArachnoidCocoonChild.fx
	End
End


############################################################################################
######    spawn targets based off of geometry, and spit web from arm Right top       #######
############################################################################################

Condition
	On	Time	
	Time	1

	Event
		EName	TP_R01
		Type	Local
		At	Cocoon

		bhvr	V_COV\EnemyPowers\Arachnos\B_Arachnoid_WebTargetParent.bhvr
#		Geom	Testing_Target
	End
End

Condition
	On	Time	
	Time	2

	Event
		EName	T_R01
		Type	Local
		At	TP_R01
		bhvrOverride
			PositionOffset	.5 0 0
			scale		1 1 1
		End
#		Geom	Testing_Target
	End

	Event
		Ename	T_R_Impact
		Type	Local
		At	T_R01

		Part	V_COV\EnemyPowers\Arachnos\P_Web_ArachnoidImpact.part
	End
End
	
		

Condition
	On	Time
	Time	2
	

	Event
		EName	Emitter
		Type	Local
		At	C_ToeR

##		Geom	Testing_TargetB
		Part	V_COV\EnemyPowers\Arachnos\P_Web_ArachnoidStrand.part
		PMagnet	T_R01
	End
End


############################################################################################
######    spawn targets based off of geometry, and spit web from arm Left  top       #######
############################################################################################

Condition
	On	Time	
	Time	1

	Event
		EName	TP_L01
		Type	Local
		At	Cocoon
		bhvrOverride
			PositionOffset		0 -1.5 0
			Spin			-0.1 0.0 0.1
		End

		bhvr	V_COV\EnemyPowers\Arachnos\B_Arachnoid_WebTargetParent.bhvr
#		Geom	Testing_Target
	End
End

Condition
	On	Time	
	Time	2

	Event
		EName	T_L01
		Type	Local
		At	TP_L01
		bhvrOverride
			PositionOffset	1 0 .0
			scale		1 1 1
		End
#		Geom	Testing_Target
	End

	Event
		Ename	T_L_Impact
		Type	Local
		At	T_L01

		Part	V_COV\EnemyPowers\Arachnos\P_Web_ArachnoidImpact.part
	End
End
	
		

Condition
	On	Time
	Time	2
	

	Event
		EName	Emitter
		Type	Local
		At	C_ToeL

##		Geom	Testing_TargetB
		Part	V_COV\EnemyPowers\Arachnos\P_Web_ArachnoidStrand.part
		PMagnet	T_L01
	End
End


###########################################################################################
######    spawn targets based off of geometry, and spit web from arm Left bottom     #######
############################################################################################

Condition
	On	Time	
	Time	1

	Event
		EName	BP_L01
		Type	Local
		At	Cocoon
		bhvrOverride
			PositionOffset		0 -3 0
			Spin			0.05 0.1 0.05
		End

		bhvr	V_COV\EnemyPowers\Arachnos\B_Arachnoid_WebTargetParent.bhvr
#		Geom	Testing_Target
	End
End

Condition
	On	Time	
	Time	2

	Event
		EName	B_L01
		Type	Local
		At	BP_L01
		bhvrOverride
			PositionOffset	.75 0 0
			scale		1 1 1
		End
#		Geom	Testing_Target
	End

	Event
		Ename	B_L_Impact
		Type	Local
		At	B_L01

		Part	V_COV\EnemyPowers\Arachnos\P_Web_ArachnoidImpact.part
	End
End
	
		

Condition
	On	Time
	Time	2
	

	Event
		EName	Emitter
		Type	Local
		At	C_Hind_ToeL

##		Geom	Testing_TargetB
		Part	V_COV\EnemyPowers\Arachnos\P_Web_ArachnoidStrand.part
		PMagnet	B_L01
	End
End


############################################################################################
######    spawn targets based off of geometry, and spit web from arm rIGHT bottom     #######
############################################################################################

Condition
	On	Time	
	Time	1

	Event
		EName	BP_r01
		Type	Local
		At	Cocoon
		bhvrOverride
			PositionOffset		0 2.75 0
			Spin			0.05 -0.1 0.05
		End

		bhvr	V_COV\EnemyPowers\Arachnos\B_Arachnoid_WebTargetParent.bhvr
#		Geom	Testing_Target
#		ChildFX V_COV\EnemyPowers\Arachnos\F_ArachnoidCocoon_ChildRLow.fx
#		Lifespan 100
	End
End

Condition
	On	Time	
	Time	2

	Event
		EName	B_R01
		Type	Local
		At	BP_R01
		bhvrOverride
			PositionOffset	.5 0 0
			scale		1 1 1
		End
#		Geom	Testing_Target
	End

	Event
		Ename	B_R_Impact
		Type	Local
		At	B_R01

		Part	V_COV\EnemyPowers\Arachnos\P_Web_ArachnoidImpact.part
	End
End
	
		

Condition
	On	Time
	Time	2
	

	Event
		EName	Emitter
		Type	Local
		At	C_Hind_ToeR

##		Geom	Testing_TargetB
		Part	V_COV\EnemyPowers\Arachnos\P_Web_ArachnoidStrand.part
		PMagnet	B_R01
	End
End



End