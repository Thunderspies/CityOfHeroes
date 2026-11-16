#########################################################
## Radiation - Enervating Field
#########################################################

FxInfo

LifeSpan 30

ClampMaxScale 1 1 1

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Orb
		Type	posit
		At	T_Root
		part1	:EnervatingFieldMist1.part
		part2	:EnervatingFieldMist2.part
		Part4	:EnervatingFieldBubbles1.Part
		Part5	:EnervatingFieldBubbles2.Part
		Part3	:EnervatingFieldRings.Part
	End
End

Condition
	On 	Time
	Time 	5

	Event
		EName 	rays
		Type	posit
		At	T_Root
		Part1	:EnervatingFieldRays1.part
	End
End

Condition
	On 	Time
	Time 	5

	Event
		EName 	rays
		Type	posit
		At	T_Chest
		Part1	:EnervatingFieldRays2.part
	End
End

#############################################################

End