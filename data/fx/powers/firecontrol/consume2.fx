############################################################
##	Fireweapon
###########################################################
FxInfo

Input  
	Inpname	WepR
End

Input  
	Inpname	WepL
End

Input  
	Inpname	Root
End

Condition
	On 	Time
	Time 	10

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Part1	:FireHandz.part
		Sound Firehand_loop 100.0 100.0 .55
		LifeSpan	40
	
	End


	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part1	:FireHandz.part
		Sound fireball2 100.0 100.0 .8
	
		LifeSpan	40
	End

End

Condition
	On 	Time
	Time 	10

	Event	
		Type	Local
		At 	Origin
		Sound firering 100 100 .8

	End
End

#Condition
#	On	Time
#	Time	20
	
#	Event
#		EName   RadiusFxScalex
#		Type	local
#		At	root
#		Bhvr	powers/firecontrol/FireShield.bhvr
#		Sound	Spewhiss2 66 22 .44
		
#	End

#	Event
#		EName 	Prime1
#		Type	Local 
#		At	RadiusFxScalex
#		bhvr    behaviors/powers/firecontrol/InfernoBubble6.bhvr
#		Part1	:FireShieldCloud01a.part
#		Part2	:FireShieldCloud02a.part
#		Part4	:FireShieldSparksa.part
		
#		LifeSpan	95
		
#	End
End


##############################################################

