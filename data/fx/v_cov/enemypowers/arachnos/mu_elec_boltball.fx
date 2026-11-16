#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	Mystic
End

Input  
	Inpname	HandR
End

Input  
	Inpname	HandL
End

LifeSpan	100

########################################################


Condition
	On 	Time
	Time 	0


	Event
		EName	Offset
		Type	local
		At	Root
		bhvr	:mu_Elec_Shooter_offset1.bhvr
		##Geom    Testing_Target
	End	

	Event
		EName	BoltOffset
		Type	local
		At	Root
		bhvr	:mu_Elec_bolt_offset1.bhvr
		#Geom    Testing_Target
	End	

	Event
		EName	shooter
		Type	start 
		At	Offset
		bhvr	:mu_Elec_shooter1.bhvr
		##Geom    Testing_Target
		Lifespan 60
		magnet BoltOffset
	End

	Event
		EName	Ball
		Type	start
		At	chest
		bhvr	:mu_Elec_Ball1.bhvr
		#Geom    Testing_Target
		Lifespan 15
		magnet shooter
	End		
	
End

Condition
	On 	Time
	Time 	1
	
	
	Event
		Type	local
		At	Ball ##Offset
		Part1	:mu_Elec_reachers1.part
#		POther shooter
#		#Geom Testing_Target
#		Pmagnet BoltOffset
#		magnet BoltOffset
	End	

End
