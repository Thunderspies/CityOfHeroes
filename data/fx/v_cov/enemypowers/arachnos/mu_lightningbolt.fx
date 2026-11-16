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

	On 	time
	Time 	3
	
	Event
		Type	Local
	
		At	origin

	Sound elecfry 80 80 1
	end
end


Condition

	On 	time
	Time 	15
	
	Event
		Type	Local
	
		At	origin
	Sound ThunderCrack 80 80 .8
	#Sound	rumble4 80 30 .3
	end
end




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

End

Condition
	On 	time
	Time 	6
	Event
		EName	asplode
		Type	local
		At	BoltOffset
		bhvr	:mu_Elec_Splosion1.bhvr
		#Geom    Testing_Target
	End
	Event
		Type	local
		At	asplode
		Part1	:mu_Elec_Redball1.part
	End
End



Condition
	On 	time
	Time 	0
	Event
		Type	start
		childFX	:mu_Elec_Boltball.fx
	End		
	Event
		Type	start
		childFX	:mu_Elec_Boltball.fx
	End		
	Event
		Type	start
		childFX	:mu_Elec_Boltball.fx
	End		
	Event
		Type	start
		childFX	:mu_Elec_Boltball.fx
	End		
	Event
		Type	start
		childFX	:mu_Elec_Boltball.fx
	End		
	Event
		Type	start
		childFX	:mu_Elec_Boltball.fx
	End		
	Event
		Type	start
		childFX	:mu_Elec_Boltball.fx
	End		
	Event
		Type	start
		childFX	:mu_Elec_Boltball.fx
	End		
	Event
		Type	start
		childFX	:mu_Elec_Boltball.fx
	End		
	Event
		Type	start
		childFX	:mu_Elec_Boltball.fx
	End		
End

Condition
	On 	Time
	Time 	15
	Event
		Type	local
		childFX	:mu_LightningCrack1.fx
		#Geom    Testing_Target
	End		
	
End

#Condition
#	On 	Time
#	Time 	20
#
#	Event
#		Type	local
#		At	BoltOffset
#		Part1	:mu_Elec_Bolt1.part
#		POther Target
#		##Geom    Testing_Target
#	End		
#	
#End


#Condition
#	On 	Time
#	Time 	1
#	
#	
#	Event
#		Type	local
#		At	Ball ##Offset
#		Part1	:mu_Elec_reachers1.part
##		POther shooter
#		##Geom Testing_Target
#		Pmagnet shooter
#	End	
#
#End
