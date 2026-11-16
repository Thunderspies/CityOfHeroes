#########################################################
## Hamidon - Beam Continuing FX
#########################################################

FxInfo

#Lifespan	180

#########################################################

Condition

	On	Time
	Time	0
	
	Event
		EName	1
		Type	Local
		At	chest
		#Geom	FX_ancestor
		Sound Glow8_loop 70 70 .82
	End

	Event
		EName	2
		Type	Local
		At	FootR
		part1	:DoT_Glow1.part
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:DoT_Glow1.part
		part1	:DoT_Flashes1.part
		part1	:DoT_Flashes2.part
		PMagnet LLEGL
	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:DoT_Glow1.part
		part1	:DoT_Flashes1.part
		part1	:DoT_Flashes2.part
		PMagnet FootL
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:DoT_Glow1.part
	End

	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:DoT_Glow1.part
		part1	:DoT_Flashes1.part
		part1	:DoT_Flashes2.part
		PMagnet LlegR
	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:DoT_Glow1.part
		part1	:DoT_Flashes1.part
		part1	:DoT_Flashes2.part
		PMagnet FootR
	End

	Event
		EName	9
		Type	Local
		At	Head
		part2	:DoT_Glow1.part
	End

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:DoT_Glow1.part
		part1	:DoT_Flashes1.part
		part1	:DoT_Flashes2.part
		PMagnet LArmL
	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:DoT_Glow1.part
		part1	:DoT_Flashes1.part
		part1	:DoT_Flashes2.part
		PMagnet WepL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	:DoT_Glow1.part
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:DoT_Glow1.part
		part1	:DoT_Flashes1.part
		part1	:DoT_Flashes2.part
		PMagnet LArmR
	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:DoT_Glow1.part
		part1	:DoT_Flashes1.part
		part1	:DoT_Flashes2.part
		PMagnet WepR
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	:DoT_Glow1.part
	End

	Event
		EName	15
		Type	Local
		At	Chest
		part1	:DoT_Flashes1.part
		part1	:DoT_Flashes2.part
		part1	:DoT_Glow1.part
		part1	:DoT_Motes.part
	End

End

#########################################################

End