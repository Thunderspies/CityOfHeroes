#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Root
End

#################################################################################


Condition
	On 	Time
	Time 	5



	Event
		
		Type	Local
		At	chest
		Sound Manyspikes 60 60 .55
	End
End




Condition
	On 	Time
	Time 	0
	Repeat 35
	RepeatJitter 5

	Event
		EName	lazer
		Type	start
		At	root
			
		Bhvr	:SilverCaltropsFadein.bhvr
		Geom	SilverGroundSpike01	
		Geom	SilverGroundSpike02	
		Geom	SilverGroundSpike03	
		Geom	SilverGroundSpike04
		
		
		
	End

		
End


End