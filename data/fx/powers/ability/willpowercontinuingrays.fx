#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Head
End

########################################################

Condition
	On 	Time
	Time 	0


	Event
		Type	Local 
		At	Head
		Sound Intel_loop 30 30 .45
		bhvr	behaviors/soundFade2.bhvr
		Flags	PowerLoopingSound
		Lifespan 130
	End

End




Condition
	On 	Time
	Time 	0


	Event
		Type	Local 
		At	Head
		
		Part1	:WillPower1.part
		Part2	:WillPowerSmall1.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
		Part5	:WillpowerContinuingRays.part
	End

End

End
##################################

