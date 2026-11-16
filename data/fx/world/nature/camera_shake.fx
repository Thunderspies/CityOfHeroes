#########################################################
##	Stone_Wall
#########################################################

FxInfo

Input  
	Inpname	Root
End

#########################################################
##	Audio
#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound Rumble1_loop 100 100 .76
	End
	
End

Condition

	On Cycle
	Time	46
	Chance	1
	
	Event
			
		Type	start
		At	Root
		Sound Quake3 100 100 .64
		Sound Quake4 100 100 .76
		Sound Quake3 100 100 .68
		Sound Rumble4 100 100 .55
		Sound Quake5 100 100 .6
		LifeSpan	46
		
	End

End



#########################################################

LifeSpan	60

Condition
	On Time
	Time	1
	Chance	1
	
	Event
		EName 	cameraShake01
		Type	start
		At	Root
		bhvr	Behaviors/camera_shake_initial.bhvr
	
		LifeSpan	1
	End

End

#Condition
	#On Cycle
	#Time	20
	#Chance	1
	
	#Event
			
		#Type	start
		#At	Root
		#Bhvr	Behaviors/camera_shake_sustained.bhvr
		#LifeSpan	20
		
	#End

#End

End


