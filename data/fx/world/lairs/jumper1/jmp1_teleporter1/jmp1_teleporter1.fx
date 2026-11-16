#########################################################
##	jmp1_teleporter
##
FxInfo

Input  
	InpName	Origin
End


Condition
	On 	Time
	Time 	0
##	Chance	.8

	Event
		EName 	Clouds
		Type	start 
		At	origin
		Part	:jmp1teleStreaks1.part
		Part	:jmp1teleBubbles.part
		Part	:jmp1tele1.part
			
	End

End

Condition
	On 	Cycle
	Time 	60
	Chance	.5

	Event
		EName 	Flashes
		Type	start 
		At	origin
		#Part	:jmp1teleStreaks1.part
		#Part	:jmp1tele2.part
		Lifespan 10
				
	End


End	


			