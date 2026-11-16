#########################################################
##	Warburg Bomb - Nuke
##

FxInfo
	
lifespan	150

Condition

	On Time 
	Time 0
	
	Event
		Ename	Start
		Type	Start 
		Bhvr	:CloudRiser.bhvr
	End

End

Condition

	On 	Time
	Time 	30

	Event
		EName 	crime
		Type	Local
		At	Start
		
		Part	:InfernorockBits1.part
		Lifespan	100

	End

End

Condition

	On 	Time
	Time 	20

	Event
		EName 	crime
		Type	Local
		At	Start
		
		Part	:InfernorockBits2.part
		Lifespan	90

	End

End

Condition

	On 	Time
	Time 	10

	Event
		EName 	crime
		Type	Local
		At	Start
		
		Part	:InfernorockBits3.part
		Lifespan	80

	End

End

Condition

	On 	Time
	Time 	40

	Event
		EName 	crime
		Type	Local
		At	Start
		
		Part2	:Inferno.part
		Lifespan	20

	End

End

Condition

	On 	Time
	Time 	30

	Event
		EName 	crime
		Type	Local
		At	Start
		
		Part2	:Inferno1.part
		Lifespan	20

	End

End

Condition

	On 	Time
	Time 	20

	Event
		EName 	crime
		Type	Local
		At	Start
		
		Part2	:Inferno2.part
		Lifespan	20

	End

End

Condition

	On 	Time
	Time 	12

	Event
		EName 	crime
		Type	Local
		At	Start
		
		Part2	:Inferno3.part
		Lifespan	20

	End

End

Condition

	On 	Time
	Time 	5

	Event
		EName 	crime
		Type	Local
		At	Start
		
		Part2	:Inferno4.part
		Lifespan	20

	End

End

End
