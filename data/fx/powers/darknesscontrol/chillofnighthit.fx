#########################################################
## Darkness Control - Darkest Night
#########################################################

FxInfo

LifeSpan	100

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		ENAME   Tendrills
		Type	Local 
		At	chest
		Part1	:ChillOfNightTendril.part
		#Part2	:DarkBlastHitPuddle2.part
	End

	Event

		EName 	Hit
		Type	local 
		At	chest
		
		part1	:DamageHitMist.part
		part2	:DefenceHitMist2.part
	End
End

Condition
	On 	Time
	Time 	75

	Event
		EName 	Tendrills
		Type	Destroy
	End
End

#########################################################

End			