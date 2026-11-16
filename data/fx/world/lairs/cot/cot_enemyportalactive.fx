#########################################################
##	template
FxInfo 

Input  
	InpName	Origin
End
   
#########################################################
#####################  Mist 1  #########################
#########################################################

Condition
	Event
		EName 	FogRing1
		Type	Start 
		At	Origin
		bhvr    behaviors/CoTEnemyGateSpin3.bhvr
		Part	:CoTEnemyGateRingSmall.part
		part1	:CoT_EnemyPortal.part
		Sound hurricane_loop 70 70 .33
		
	End

	Event
		EName 	Hurricane
		Type	Start 
		At	Origin
		bhvr    behaviors/CoTEnemyGateSpin1.bhvr
		Part	:CoTEnemyGate01.part
		Sound COTdoor_loop 60 60 .6
	End

	Event
		EName 	FogRing1
		Type	Start 
		At	Origin
		bhvr    behaviors/CoTEnemyGateSpin1.bhvr
		Part	:CoTEnemyGateRing2.part
	End
End


End



	