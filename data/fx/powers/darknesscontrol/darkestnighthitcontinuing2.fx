#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

Input  
	InpName	chest
End    

#########################################################

condition
	On	Time
	Time	19

	Event
		Type	Local
		At	Chest
		Sound Sootthrow 50 50 .83
		End
End


Condition
	On 	Time
	Time 	0

	Event

		EName 	Hit
		Type	local 
		At	chest
		Sound Darktendrils 50 50 .73
		part1	:DamageHitMist.part
		part2	:DefenceHitMist2.part
		Part	:ChillOfNightTendril.part

	End
	
End
	

End	


			