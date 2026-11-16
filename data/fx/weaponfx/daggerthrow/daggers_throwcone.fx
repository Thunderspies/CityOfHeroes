#########################################################
##	
##

FxInfo

Flags     InheritAnimScale

LifeSpan  190



######################################################  NOTES   ######################################################
##
## Overview: this fx consists of 8 thrown knives in a cone.  A child FX file establishes targets and knives for 
##		that miss but travel in the cone.  While the 2 fx knives below hit a targeted entity.
## 
## Setup of targets in child fx file
## 1. an FX node is generated at the local position of BendMystic called "TargetParent" to establish relative  
##	orientation to character.  Standard behavior offsets are global so this is necessary
## 2. Target nodes spawned at start.  First two spawn at extremes with small jitter to establish cone.  
##	All others spawn randomly within this cone.
## 
## Tossing of knives
## 3. Knives spawned in hand, then alternate knives are released.  
## 4. Some target player nodes, while others target "dummy Targets" to create illusion of cone
##
##
##
#######################################################################################################################


## child fx call for knives that miss entity ##

Condition
	On 	Time
	Time 	0
	Event
		EName 	KnifeMisses
		Type	Start 
		
		At	BendMystic
		ChildFx :Daggers_ThrowCone_Child.fx
	End
End



##################################################   Toss Knives   ##################################################
##################################################   Toss Knives   ##################################################
##################################################   Toss Knives   ##################################################

## Knives that hit entity, Knife 1 ##

Condition
	On 	Time
	Time 	9

	Event
		EName 	knifeHand_1
		Type	Local 
		At	WepL
		Bhvr	Behaviors\FX_Daggers_Multi.bhvr
		#Geom	GEO_WepR_Thorn_1
		lifespan 9
	End

End

Condition
	On 	Time
	Time 	18

	Event
		EName	Prime
		Type	start
		At	WepL
		Geom	CenterDummy
		
		Bhvr	:dagger01.bhvr	
		Magnet	Target
		LookAt	Target
		Sound throw3 100.0 100.0 .8

	End

	Event
		EName 	knifeToss_1
		Type	local
		At	Prime
		
		Geom	GEO_WepR_Thorn_1
		
	End

	Event
		EName 	knifeStreaksToss_1
		Type	local
		At	Prime
		part	:daggerstreaksCone.part
		
	End

End

Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End
End


##  Knife 2 ##
#
#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName 	knifeHand_2
#		Type	Local 
#		At	WepL
#		Bhvr	Behaviors\FX_Daggers_Multi01.bhvr
#		Geom	GEO_WepR_Thorn_1
#		lifespan 19
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	19
#
#	Event
#		EName	Prime2
#		Type	start
#		At	WepL
#		Geom	CenterDummy
#		
#		Bhvr	:dagger01.bhvr	
#		Magnet	Target
#		LookAt	Target
#		Sound	throw3 100.0 30.0 .8
#
#	End
#
#	Event
#		EName 	knifeToss_2
#		Type	local
#		At	Prime2
#		
#		Geom	GEO_WepR_Thorn_1
#		
#	End
#
#	Event
#		EName 	knifeStreaksToss_2
#		Type	local
#		At	Prime2
#		part	:daggerstreaksCone.part
#		
#	End
#
#End
#
#
#Condition
#	On 	Prime2Hit
#
#	Event
#		Ename 	Prime2
#		Type	Destroy
#	End
#End



End				