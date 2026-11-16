
FxInfo

Lifespan 600

############################################################################################
################	Spawn two guards when statesman goes down	####################
############################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	GoalStart		######  event to establish goal direction to orient guard characters
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 0 15
			scale		5 5 5
		End

#		Geom	Testing_Target
	End

	Event
		EName	Goal			######  event to establish spawn point
		Type	Start
		At	GoalStart
		
#		Geom	Testing_Target
#		LookAt	Target
	End
End

############################################################################################
############################################################################################

Condition
	On	Time
	Time	0
	
	Event
		EName	SpawnStart		######  event to establish spawn point
		Type	Start
		At	Root
		bhvrOverride
			PositionOffset	0 0 0
		End

#		Geom	Testing_Target
		LookAt	Goal
	End
End

############################################################################################
############################################################################################

Condition
	On 	Time						
	Time 	35 #70

	Event
		EName	Spawn					
		Type	Local
		At	SpawnStart					
		bhvrOverride
			PositionOffset	0 0 -1
		End
		
		Bhvr	:B_FadeInOut_Entity.bhvr
		Anim	FX_StatesmanGuard			
		lifespan 250
	End
End

Condition
	On 	Time						
	Time 	20 #70

	Event
		EName	Spawn01					
		Type	Local
		At	SpawnStart					
		bhvrOverride
			PositionOffset	0 0 0
		End
		
		Bhvr	:B_FadeInOut_Entity.bhvr
		Anim	FX_StatesmanGuard01			
		lifespan 265
	End
	
End

End		