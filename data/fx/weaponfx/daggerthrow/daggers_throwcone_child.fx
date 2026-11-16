#########################################################
##	
##

FxInfo

Flags     InheritAnimScale

LifeSpan  190


######################################################  NOTES   ######################################################
##
## Setup of targets 
## 1. an FX node is generated at the local position of BendMystic called "TargetParent" to establish relative  
##	orientation to character.  Standard behavior offsets are global so this is necessary
## 2. Target nodes spawned at start.  First two spawn at extremes with small jitter to establish cone.  
##	All others spawn randomly within this cone.
## 
## Tossing of knives
## 3. Knives spawned in hand, then alternate knives are released.  
## 4. Some target player nodes, while others target "dummy Targets" to create illusion of cone
##
#######################################################################################################################

Condition
	On	Time
	Time	0					## Parent spawned to establish local orientation for targets

	Event
		Ename	TargetParent01			
		Type	Local
		At	Root
		Bhvr	Behaviors\FX_SnakeMen_Targets.bhvr
		 
		
		#Geom	TESTING_Target			## reference object used to see target position, leaving in for debugging help if needed
		Lifespan 100
	End
End
	
Condition
	On	Time
	Time	8					## Parent spawned to establish local orientation for targets

	Event
		Ename	TargetParent			
		Type	Start
		At	TargetParent01
		#Bhvr	Behaviors\FX_SnakeMen_Targets.bhvr
		 
		
		#Geom	TESTING_Target			## reference object used to see target position, leaving in for debugging help if needed
		Lifespan 100
	End
End

################### End Target Parent start spawning of dummy Targets ###################

Condition
	On	Time
	Time	8					## Parent spawned to establish local orientation for targets


	Event
		Ename	Target01			## Target node spawned relative to "TargetParent"
		Type	Local
		At	TargetParent
		Bhvr	Behaviors\FX_SnakeMen_Targets_Right.bhvr
		 
		
		#Geom	TESTING_Target			
		Lifespan 100
	End

	Event
		Ename	Target02			## Target node spawned relative to "TargetParent"
		Type	Local
		At	TargetParent
		Bhvr	Behaviors\FX_SnakeMen_Targets_Left.bhvr
		 
		
		#Geom	TESTING_Target			
		Lifespan 100
	End

	Event
		Ename	Target03			## Target node spawned relative to "TargetParent"
		Type	Start
		At	TargetParent
		Bhvr	Behaviors\FX_SnakeMen_Targets_Random.bhvr
		 
		
##		Geom	TESTING_Target			
		Lifespan 100
	End

	Event
		Ename	Target04			## Target node spawned relative to "TargetParent"
		Type	Start
		At	TargetParent
		Bhvr	Behaviors\FX_SnakeMen_Targets_Random.bhvr
		 
		
##		Geom	TESTING_Target			
		Lifespan 100
	End

	Event
		Ename	Target05			## Target node spawned relative to "TargetParent"
		Type	Start
		At	TargetParent
		Bhvr	Behaviors\FX_SnakeMen_Targets_Random.bhvr
		 
		
##		Geom	TESTING_Target			
		Lifespan 100
	End

	Event
		Ename	Target06			## Target node spawned relative to "TargetParent"
		Type	Start
		At	TargetParent
		Bhvr	Behaviors\FX_SnakeMen_Targets_Right.bhvr
		 
		
##		Geom	TESTING_Target			
		Lifespan 100
	End

End

##################################################   Toss Knives   ##################################################
##################################################   Toss Knives   ##################################################
##################################################   Toss Knives   ##################################################


##  Knife 1 ##

Condition
	On 	Time
	Time 	9

	Event
		EName 	knife01
		Type	Local 
		At	WepL
		Bhvr	Behaviors\FX_Daggers_Multi02.bhvr
		Geom	GEO_WepR_Thorn_1
		lifespan 8
	End

End

Condition
	On 	Time
	Time 	17

	Event
		EName	Prime1
		Type	start
		At	WepL
		Geom	CenterDummy
		
		Bhvr	:dagger01.bhvr	
		Magnet	Target01
		#LookAt	Target
		Sound throw3 100.0 100.0 .8

	End

	Event
		EName 	knife01
		Type	local
		At	Prime1
		Altpiv	1
		Geom	GEO_WepR_Thorn_1
		
	End

	Event
		EName 	knifestreaks01
		Type	local
		At	Prime1
		part	:daggerstreaksCone.part
		
	End

End

Condition
	On 	Prime1Hit

	Event
		Ename 	Prime1
		Type	Destroy
	End
End


##  Knife 2 ##

Condition
	On 	Time
	Time 	8

	Event
		EName 	knife02
		Type	Local 
		At	WepL
		Bhvr	Behaviors\FX_Daggers_Multi03.bhvr
		Geom	GEO_WepR_Thorn_1
		lifespan 10
	End

End

Condition
	On 	Time
	Time 	18

	Event
		EName	Prime2
		Type	start
		At	WepL
		Geom	CenterDummy
		
		Bhvr	:dagger01.bhvr	
		Magnet	Target02
		#LookAt	Target
#		Sound	throw3 100.0 30.0 .8

	End

	Event
		EName 	knife02
		Type	local
		At	Prime2
		Altpiv	1
		Geom	GEO_WepR_Thorn_1
		
	End

	Event
		EName 	knifestreaks02
		Type	local
		At	Prime2
		part	:daggerstreaksCone.part
		
	End

End


Condition
	On 	Prime2Hit

	Event
		Ename 	Prime2
		Type	Destroy
	End
End


##  Knife 3 ##

Condition
	On 	Time
	Time 	9

	Event
		EName 	knife03
		Type	Local 
		At	WepL
		Geom	GEO_WepR_Thorn_1
		lifespan 9
	End

End

Condition
	On 	Time
	Time 	18

	Event
		EName	Prime3
		Type	start
		At	WepL
		Geom	CenterDummy
		
		Bhvr	:dagger01.bhvr	
		Magnet	Target03
		#LookAt	Target
		Sound throw3 100.0 100.0 .8

	End

	Event
		EName 	knife03
		Type	local
		At	Prime3
		Altpiv	1
		Geom	GEO_WepR_Thorn_1
		
	End

	Event
		EName 	knifestreaks03
		Type	local
		At	Prime3
		part	:daggerstreaksCone.part
		
	End

End


Condition
	On 	Prime3Hit

	Event
		Ename 	Prime3
		Type	Destroy
	End
End

##  Knife 4 ##

Condition
	On 	Time
	Time 	8

	Event
		EName 	knife04
		Type	Local 
		At	WepL
		Bhvr	Behaviors\FX_Daggers_Multi01.bhvr
		Geom	GEO_WepR_Thorn_1
		lifespan 10
	End

End

Condition
	On 	Time
	Time 	18

	Event
		EName	Prime4
		Type	start
		At	WepL
		Geom	CenterDummy
		
		Bhvr	:dagger01.bhvr	
		Magnet	Target04
		#LookAt	Target
#		Sound	throw3 100.0 30.0 .8

	End

	Event
		EName 	knife04
		Type	local
		At	Prime4
		Altpiv	1
		Geom	GEO_WepR_Thorn_1
		
	End

	Event
		EName 	knifestreaks04
		Type	local
		At	Prime4
		part	:daggerstreaksCone.part
		
	End

End


Condition
	On 	Prime4Hit

	Event
		Ename 	Prime4
		Type	Destroy
	End
End

##  Knife 5 ##
#
#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName 	knife05
#		Type	Local 
#		At	WepL
#		Geom	GEO_WepR_Thorn_1
#		lifespan 19
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	20
#
#	Event
#		EName	Prime5
#		Type	start
#		At	WepL
#		Geom	CenterDummy
#		
#		Bhvr	:dagger01.bhvr	
#		Magnet	Target05
#		#LookAt	Target
##		Sound	throw3 100.0 30.0 .8
#
#	End
#
#	Event
#		EName 	knife05
#		Type	local
#		At	Prime5
#		Altpiv	1
#		Geom	GEO_WepR_Thorn_1
#		
#	End
#
#	Event
#		EName 	knifestreaks05
#		Type	local
#		At	Prime5
#		part	:daggerstreaksCone.part
#		
#	End
#
#End
#
#
#Condition
#	On 	Prime5Hit
#
#	Event
#		Ename 	Prime5
#		Type	Destroy
#	End
#End
#
###  Knife 6 ##
#
#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName 	knife05
#		Type	Local 
#		At	WepL
#		Geom	GEO_WepR_Thorn_1
#		lifespan 19
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	20
#
#	Event
#		EName	Prime
#		Type	start
#		At	WepL
#		Geom	CenterDummy
#		
#		Bhvr	:dagger01.bhvr	
#		Magnet	Target06
#		#LookAt	Target
##		Sound	throw3 100.0 30.0 .8
#
#	End
#
#	Event
#		EName 	knife05
#		Type	local
#		At	Prime
#		Altpiv	1
#		Geom	GEO_WepR_Thorn_1
#		
#	End
#
#	Event
#		EName 	knifestreaks06
#		Type	local
#		At	Prime
#		part	:daggerstreaksCone.part
#		
#	End
#
#End
#
#
#Condition
#	On 	PrimeHit
#
#	Event
#		Ename 	Prime
#		Type	Destroy
#	End
#End
#
#

##################################


End				