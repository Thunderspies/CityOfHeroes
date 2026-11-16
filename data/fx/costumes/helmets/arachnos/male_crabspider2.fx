#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAlpha

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Offset
		Type	Local
		At	Head
		Geom	FX_Male_CrabSpiderHelmet
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Offset
		AltPiv	1
		Part1	:CrabSpider_Glow1.part
		Part1	:CrabSpider_Glow2.part
	End

	Event
		Type	Local
		At	Offset
		AltPiv	5
		Part1	:CrabSpider_Glow1.part
		Part1	:CrabSpider_Glow2.part
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	Offset
		AltPiv	2
		Part1	:CrabSpider_Glow1.part
		Part1	:CrabSpider_Glow2.part
	End

	Event
		Type	Local
		At	Offset
		AltPiv	6
		Part1	:CrabSpider_Glow1.part
		Part1	:CrabSpider_Glow2.part
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	Offset
		AltPiv	3
		Part1	:CrabSpider_Glow1.part
		Part1	:CrabSpider_Glow2.part
	End

	Event
		Type	Local
		At	Offset
		AltPiv	7
		Part1	:CrabSpider_Glow1.part
		Part1	:CrabSpider_Glow2.part
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	Offset
		AltPiv	4
		Part1	:CrabSpider_Glow1.part
		Part1	:CrabSpider_Glow2.part
	End

	Event
		Type	Local
		At	Offset
		AltPiv	8
		Part1	:CrabSpider_Glow1.part
		Part1	:CrabSpider_Glow2.part
	End
End

#############################################################

End