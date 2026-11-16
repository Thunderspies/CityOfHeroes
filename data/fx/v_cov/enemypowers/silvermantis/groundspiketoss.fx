#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	WepR
End

Input  
	Inpname	T_Root
End

Lifespan		200

#################################################################################

Condition
	On	Time
	Time	17

	Event	
		Type	Local
		At	origin
		Sound caltropstoss 60 60 .8
	End
End

Condition
	On 	Time
	Time 	19

	Event
		EName	Prime
		Type	start
		At	WepR
			
		Bhvr	:SilverCaltropsToss.bhvr	
		Geom	SilverGroundSpike01	
		Geom	SilverGroundSpike02	
		Geom	SilverGroundSpike03	
		Geom	SilverGroundSpike04
		
		Magnet	T_Root
		

	End
End

Condition
	On 	Time
	Time 	19
	Repeat	20
	RepeatJitter	5

	Event
		EName	secondary
		Type	Local
		At	Prime
			
		Bhvr	:SilverCaltropsToss2.bhvr
		Geom	SilverGroundSpike01	
		Geom	SilverGroundSpike02	
		Geom	SilverGroundSpike03	
		Geom	SilverGroundSpike04
		

	End

			
End

Condition
	On	PrimeHit
		
		
	Event
		Type    Destroy
		EName 	All

	End

End

End