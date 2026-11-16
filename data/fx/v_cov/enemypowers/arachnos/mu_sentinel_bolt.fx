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

LifeSpan	18

########################################################

Condition
	On 	Time
	Time 	14
	


	Event
		Type	Posit 
		At	origin
		Sound lightningbolthit 100 100 .7
	End


	Event
		EName	origin   ##Bolt Offset
		Type	local
		At	Root
		bhvr	:mu_Elec_bolt_offset1.bhvr
		##Geom    Testing_Target
	End

	Event
		EName	aim
		Type	start
		At	origin
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
		Part2	:mu_Elec_BoltLink1.part
		Part3	:mu_Elec_BoltLink1.part
		Part4	:mu_Elec_BoltLink1.part
		Part3	:mu_Elec_BoltLink1.part
		Part4	:mu_Elec_BoltLink1.part
		pother	target
		lifespan 10
	End

	Event
		Type	local
		At	walker
		Part1	:mu_Elec_BoltLinkCore1.part
		Part2	:mu_Elec_BoltLinkCore2.part
		lifespan 10
	End	

End



#
#Condition
#	On 	Time
#	Time 	0
#
#
#
#
#	Event
#		EName	origin
#		Type	local
#		At	Root
#		bhvr	:mu_Elec_bolt_offset1.bhvr
#		##Geom    Testing_Target
#	End	
#
#End
#
#Condition
#	On 	time
#	Time 	6
#	Event
#		EName	asplode
#		Type	local
#		At	origin
#		bhvr	:mu_Elec_Splosion1.bhvr
#		#Geom    Testing_Target
#	End
#	Event
#		Type	local
#		At	asplode
#		Part1	:mu_Elec_Redball1.part
#	End
#End
#
#
#
#Condition
#	On 	time
#	Time 	0
#	Event
#		Type	start
#		childFX	:mu_Elec_Boltball.fx
#	End		
#	Event
#		Type	start
#		childFX	:mu_Elec_Boltball.fx
#	End		
#	Event
#		Type	start
#		childFX	:mu_Elec_Boltball.fx
#	End		
#	Event
#		Type	start
#		childFX	:mu_Elec_Boltball.fx
#	End		
#	Event
#		Type	start
#		childFX	:mu_Elec_Boltball.fx
#	End		
#	Event
#		Type	start
#		childFX	:mu_Elec_Boltball.fx
#	End		
#	Event
#		Type	start
#		childFX	:mu_Elec_Boltball.fx
#	End		
#	Event
#		Type	start
#		childFX	:mu_Elec_Boltball.fx
#	End		
#	Event
#		Type	start
#		childFX	:mu_Elec_Boltball.fx
#	End		
#	Event
#		Type	start
#		childFX	:mu_Elec_Boltball.fx
#	End		
#End
#
#Condition
#	On 	Time
#	Time 	15
#	Event
#		Type	local
#		childFX	:mu_LightningCrack1.fx
#		#Geom    Testing_Target
#	End		
#	
#End
#
#