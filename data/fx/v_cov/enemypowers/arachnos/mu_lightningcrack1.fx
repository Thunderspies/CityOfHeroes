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
		EName	BoltOffset
		Type	local
		At	Root
		bhvr	:mu_Elec_bolt_offset1.bhvr
		
		##Geom    Testing_Target
	End

	Event
		EName	aim
		Type	start
		At	BoltOffset
		bhvr	:mu_Elec_BoltAim1.bhvr
		#Geom    Testing_Target
		LookAt	Target
	End		

	Event
		EName	walker
		Type	start
		At	aim
		bhvr	:mu_Elec_BoltWalker1.bhvr
		#Geom    Testing_Target
		Magnet	Target
	End	
	

	Event
		Type	local
		At	walker
		Part1	:mu_Elec_BoltLink1.part
		Part1	:mu_Elec_BoltLink1.part
		Part1	:mu_Elec_BoltLink1.part
		Part1	:mu_Elec_BoltLink1.part
		Part1	:mu_Elec_BoltLink1.part
		Part1	:mu_Elec_BoltLink1.part
		Part1	:mu_Elec_BoltLink1.part
		Part1	:mu_Elec_BoltLink1.part
		pother	target
	End		

	Event
		Type	local
		At	walker
		Part1	:mu_Elec_BoltLinkCore1.part
		Part1	:mu_Elec_BoltLinkCore2.part
	End	

End





#Condition
#	On 	time
#	Time 	1
#	Event
#		Type	start
#		childFX	:mu_Elec_Boltball.fx
#		#lifespan 10
#	End		
#End

