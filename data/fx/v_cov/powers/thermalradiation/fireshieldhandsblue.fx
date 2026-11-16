#########################################################
##	FireAura
FxInfo

LifeSpan	70

##################################

Condition
	On	Time
	Time	0
	
	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepL
		Part1	:FireHandzBlue.part
		Part1	:FireHandzBlue.part
		Part1	:FireHandzBlue.part
		Part1	:FireHandzBlue.part
		Part1	:FireHandzBlue.part
		Sound Firehand_loop 100.0 100.0 .55
		LifeSpan	70
	
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WepR
		Sound firering 60.0 60.0 .7
		
	
	End
End


#	Event
#		EName	InitialSwordFire
#		Type	Local 
#		At	WepR
#		Part1	:FireHandz.part
#		Sound	Firehand_loop 100.0 30.0 .55
#		LifeSpan	50
#	
#	End

End
			

				