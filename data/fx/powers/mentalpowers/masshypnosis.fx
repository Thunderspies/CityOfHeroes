#################################################################################
##	TemperatureControl
######################################################################

FxInfo

LifeSpan	200

Input  
	Inpname	root
End

Input  
	Inpname	head
End
##########################################################################
Condition
	On 	Time
	Time 	10

	Event
		EName	nexius
		Type	start
		At	root
		
		Geom	RootToHeadDummy
		
	End

	Event
		EName	corea
		Type	start
		At	nexius
		altpiv	1
		bhvr	Behaviors/HypnoSpin.bhvr
		Geom	LocalPivot01
		Sound mental5b 80 80 .75
		
	End


	Event
		EName	coreaa
		Type	Local
		At	corea
		  Altpiv   4
		
		part1	:MindControlRings.part
		part2	:MindControlRingz.part
		
	End

End

Condition
	On 	Time
	Time 	20

Event
		EName	coreaaa
		Type	Local
		At	corea
		  Altpiv   4
		
		part2	:MindGlowAOE.part
				
	
	End

End

####################################################################################
##2
##################################################################3
Condition
	On 	Time
	Time 	21

	Event
		EName	coreb
		Type	start
		At	nexius
		altpiv	1
		bhvr	Behaviors/HypnoSpin.bhvr
		Geom	LocalPivot01
		
	End


	Event
		EName	corebb
		Type	Local
		At	coreb
		  Altpiv   4
		
		part1	:MindControlRings.part
		part2	:MindControlRingz.part
		
	End

End

Condition
	On 	Time
	Time 	26

Event
		EName	corebbb
		Type	Local
		At	coreb
		  Altpiv   4
		
		part2	:MindGlowAOE.part
				
	
	End

End


####################################################################################
##3
#########################
Condition
	On 	Time
	Time 	32

	Event
		EName	corec
		Type	start
		At	nexius
		altpiv	1
		bhvr	Behaviors/HypnoSpin.bhvr
		Geom	LocalPivot01
		
	End


	Event
		EName	corecc
		Type	Local
		At	corec
		  Altpiv   4
		
		part1	:MindControlRings.part
		part2	:MindControlRingz.part
		
	End

End

Condition
	On 	Time
	Time 	35

Event
		EName	coreccc
		Type	Local
		At	corec
		  Altpiv   4
		
		part2	:MindGlowAOE.part
				
	
	End

End

####################################################################
##4
#####################################################

Condition
	On 	Time
	Time 	43

	Event
		EName	cored
		Type	start
		At	nexius
		altpiv	1
		bhvr	Behaviors/HypnoSpin.bhvr
		Geom	LocalPivot01
		
	End


	Event
		EName	coredd
		Type	Local
		At	cored
		  Altpiv   4
		
		part1	:MindControlRings.part
		part2	:MindControlRingz.part
		
	End

End

Condition
	On 	Time
	Time 	48

Event
		EName	coreddd
		Type	Local
		At	cored
		  Altpiv   4
		
		part2	:MindGlowAOE.part
				
	
	End

End

####################################################################
##Glows
####################################################################
Condition
	On 	Time
	Time	60.25
	
	Event
		Ename 	coreaaa
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	71.25
	
	Event
		Ename 	corebbb
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	82.25
	
	Event
		Ename 	coreccc
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	93.25
	
	Event
		Ename 	coreddd
		Type	Destroy
	End
End

####################################################################
##Rings
####################################################################
Condition
	On 	Time
	Time	53
	
	Event
		Ename 	corea
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	63
	
	Event
		Ename 	coreb
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	74
	
	Event
		Ename 	corec
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	85
	
	Event
		Ename 	cored
		Type	Destroy
	End
End


End