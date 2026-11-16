#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	root
End


LifeSpan	60

############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	OrbMist
		Type	Local 
		At	root
		bhvr	behaviors/Rune_spinFastSmall.bhvr
		part1	:DarkessChillMistz.part
		part2	:DarkessChillMist2z.part
		part3	:DarkessChillTenticles.part
		
		Sound miasmaburst 100 100 .8
				
	End

End


End			