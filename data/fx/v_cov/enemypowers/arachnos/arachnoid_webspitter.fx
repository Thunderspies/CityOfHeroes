#########################################################
##	
##

FxInfo

LifeSpan  200
Flags	InheritAnimScale

###############################################################################
##################################   Spit   ###################################
###############################################################################

Condition
	On 	Time
	Time 	17

	Event
		EName	Emitter
		Type	Local
		At	Head
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_SpitEmit.bhvr
#		Geom	Testing_Target
#		Geom	LocalPivotz
		Sound arachnoidwebspit 80 80 .88
	End
End

Condition
	On 	Time
	Time 	23

	Event
		EName	Prime
		Type	Start
		At	Emitter

		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_SpitTravel.bhvr
		Part2	V_COV\EnemyPowers\Arachnos\Arachnoid_P_SpitBallSM.part
		Part2	V_COV\EnemyPowers\Arachnos\Arachnoid_P_SpitBall.part
		Magnet	T_Root
		LookAt	T_Root
	End

	Event
		EName	WebTrail
		Type	Local
		At	Prime

		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_WebScale.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebTrail.part
		PMagnet	Prime
	End

	Event
		EName	WebTrail01
		Type	Local
		At	Prime
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_SpitTrailOffset.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebTrail01.part
		PMagnet	Prime
	End

	
End

###############################################################################
#################################   Impact   ##################################
###############################################################################

Condition
	On 	PrimeHit
	
	Event
		EName 	Trails
		Type	Destroy
	End

	Event
		Ename	HitSpot
		Type	Start
		At	Prime

#		Geom	Testing_Target
	End

	Event
		EName	Splash
		Type	Local
		At	HitSpot

		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebSpitImpact.part
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebSpitImpactWeb.part
		PMagnet	HitSpot
	End

	Event
		EName 	Prime
		Type	Destroy
	End

End

End				