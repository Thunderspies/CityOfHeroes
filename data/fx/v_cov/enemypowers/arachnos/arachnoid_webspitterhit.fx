#########################################################
##	
##

FxInfo

LifeSpan  200
Flags	InheritAnimScale


###############################################################################
#################################   Impact   ##################################
###############################################################################

Condition
	On 	Time
	Time	0
	
	Event
		EName	Splash
		Type	Start
		At	Root
		
		Bhvr	V_COV\EnemyPowers\Arachnos\Arachnoid_B_Splash.bhvr
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_Splash.part
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_Splash01.part
		Part	V_COV\EnemyPowers\Arachnos\Arachnoid_P_Splash02.part
		PMagnet	Chest
		POther	Head
		LifeSpan	1		
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	Root
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