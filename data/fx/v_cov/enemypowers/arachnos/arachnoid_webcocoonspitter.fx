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
	Time 	0

	Event
		EName	Emitter
		Type	Local
		At	Head
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_SpitEmit.bhvr
#		Geom	Testing_Target
#		Geom	LocalPivotz
		
	End
End

Condition

	On	Time
	Time	19

	Event	
		Type	Local
		At	Head
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
		Geom	Testing_Target
		Magnet	T_Root
		LookAt	T_Root
	End

	Event
		EName	WebTrail
		Type	Local
		At	Prime

		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_WebScale.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebTrail.part
	End

	Event
		EName	WebTrail01
		Type	Local
		At	Prime
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_SpitTrailOffset.bhvr
		Part1	V_COV\EnemyPowers\Arachnos\Arachnoid_P_WebTrail01.part
	End

	
End

###############################################################################
#################################   Impact   ##################################
###############################################################################

Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy
	End


	Event
		EName 	Trails
		Type	Destroy
	End

	Event
		EName	Splash
		Type	Start
		At	T_Root
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_Splash.bhvr
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_Splash.part
#		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_Splash01.part
#		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_Splash02.part

		LifeSpan	1		
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	T_Root
		Bhvr	powers/firecontrol/Inferno.bhvr
	End


	Event
		EName 	Prime1
		Type	start 
		At	RadiusFxScale
		bhvr    WEAPONs/Bow/TrickArrow/stickyBubble.bhvr
		Part	WEAPONs/Bow/TrickArrow/StickyYellow2.part
		Part	WEAPONs/Bow/TrickArrow/StickyGlows.part
		LifeSpan	30
		
	End		

End

End				