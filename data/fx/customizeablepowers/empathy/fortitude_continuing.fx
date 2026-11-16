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
		bhvr	CustomizeablePowers\Empathy\soundFade2.bhvr
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
		
		Part1	CustomizeablePowers\Empathy\WillPower1.part
		Part2	CustomizeablePowers\Empathy\WillPowerSmall1.part
		Part3	CustomizeablePowers\Empathy\AbilityDots.part
		Part4	CustomizeablePowers\Empathy\AbilitySparklingDots.part
		Part5	CustomizeablePowers\Empathy\WillpowerContinuingRays.part
	End

End

End
##################################

